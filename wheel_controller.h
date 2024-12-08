/*!
 * @file motor_control.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.12
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_WHEEL_CONTROLER_H
#define MSLH_WHEEL_CONTROLER_H

#include <cmath>

#include "analog_in_dma_stream.h"
#include "arm_math.h"
#include "battery.h"
#include "defines.h"
#include "encoder.h"
#include "motor.h"
#include "parameter.h"

namespace mslh {

class WheelParams {
   public:
    WheelParams()
        : _mass(0.0f), _diameter(0.0f), _kp(0.0f), _ki(0.0f), _kd(0.0f) {};

    float32_t getMass() { return _mass; }

    float32_t getDiameter() { return _diameter; }

    float32_t getGearRatio() { return _gear_ratio; }

    float32_t getKp() { return _kp; }

    float32_t getKi() { return _ki; }

    float32_t getKd() { return _kd; }

    WheelParams &setMass(float32_t mass) {
        this->_mass = mass;
        return *this;
    }

    WheelParams &setDiameter(float32_t diameter) {
        this->_diameter = diameter;
        return *this;
    }

    WheelParams &setGearRatio(float32_t gear_ratio) {
        this->_gear_ratio = gear_ratio;
        return *this;
    }

    WheelParams &setKp(float32_t kp) {
        this->_kp = kp;
        return *this;
    }

    WheelParams &setKi(float32_t ki) {
        this->_ki = ki;
        return *this;
    }

    WheelParams &setKd(float32_t kd) {
        this->_kd = kd;
        return *this;
    }

   private:
    float32_t _mass;
    float32_t _diameter;
    float32_t _gear_ratio;
    float32_t _kp;  // Pゲイン
    float32_t _ki;  // Iゲイン
    float32_t _kd;  // Dゲイン
};

class WheelController {
   public:
    WheelController(Motor &motor, Encoder &encoder, Battery &battery,
                    WheelParams wheel_params, float32_t dt)
        : _motor(motor),
          _encoder(encoder),
          _battery(battery),
          _dt(dt),
          _wheel_params(wheel_params),
          _distance_per_pulse(
              _wheel_params.getDiameter() * PI /
              static_cast<float32_t>(_encoder.getOneRotationPulse())),
          _velocity_per_pulse(_distance_per_pulse / _dt),
          _target_velocity(0.0f),
          _velocity(0.0f),
          _corrected_velocity(0.0f),
          _velocity_error(0.0f),
          _integral_velocity_error(0.0f),
          _preview_target_velocity(0.0f)
           {}

    void init() {
        _battery.init();
        _encoder.reset();
    }

    void start() {
        _encoder.start();
        _motor.start();
        _motor.update(0.0f);
    }

    void stop() {
        _motor.stop();
        _encoder.stop();
    }

    void reset() {
        _encoder.reset();
        _target_velocity = 0.0f;
        _velocity = 0.0f;
        _corrected_velocity = 0.0f;
        _velocity_error = 0.0f;
        _integral_velocity_error = 0.0f;
        _preview_target_velocity = 0.0f;
    }

    float32_t getVelocity() { return _velocity; }

    void controlWheelISR(float32_t velocity) {
        // ここで"_velocity"に反映されるのは、前フレームに指定した速度に対応する実速度（つまり1次遅れ）
        controlMeasureVelocityISR();
        // 前フレームの指令速度と実速度の差分を取得
        _velocity_error = _velocity - _preview_target_velocity;
        // 現フレームの速度にするために必要な加速度を算出
        const float32_t accel = velocity - _velocity;
        float32_t motor_voltage = 0.0f;
        // 前フレームの実績をもとにしたPID制御
        motor_voltage += controlFeedBackISR(velocity);
        // 加速に必要なトルクベースの電圧値
        motor_voltage += controlFeedForwardAccelISR(accel);
        // 逆起電力を打ち消す電圧値
        motor_voltage += controlFeedForwardVelocityISR(velocity);
        // duty比を計算してモータに印加
        controlMotorISR(motor_voltage);
        // 次フレーム更新直前のPID制御のための速度保存
        _preview_target_velocity = velocity;
    }

   private:
    void controlMeasureVelocityISR() {
        _encoder.update();
        _velocity = _velocity_per_pulse *static_cast<float32_t>(
                         _encoder.getDeltaPulse());
    }

    float32_t controlFeedForwardAccelISR(float32_t accel) {
        const float32_t wheel_torque =
            _wheel_params.getMass() * (accel / 1000) *
            ((_wheel_params.getDiameter() / 2) / 1000);
        const float32_t motor_toruqe =
            wheel_torque / _wheel_params.getGearRatio();
        const float32_t current = motor_toruqe / _motor.getKt();
        const float32_t torque_voltage = _motor.getResistance() * current;
        return torque_voltage;
    }

    float32_t controlFeedForwardVelocityISR(float32_t velocity) {
        const float32_t reverse_voltage =
            (_motor.getKe() * (60.0f * velocity) * _wheel_params.getGearRatio()) /
            (M_PI * _wheel_params.getDiameter());
        return reverse_voltage;
    }

    float32_t controlFeedBackISR(float32_t velocity) {
        // メンバ変数に頼りたくないが、積分なので仕方なく
        _integral_velocity_error += _velocity_error;
        const float32_t p_error = _velocity_error * _wheel_params.getKp();
        const float32_t i_error =
            _integral_velocity_error * _wheel_params.getKi();
        const float32_t pid_error = p_error + i_error;
        // どのモータでも一定のスケールにするために逆起電力の式をもとにPID電圧を決定（計算は重くなるので不要なら削除）
        const float32_t pid_voltage =
            (_motor.getKe() * (60.0f * pid_error) * _wheel_params.getGearRatio()) /
            (M_PI * _wheel_params.getDiameter());
        return pid_voltage;
    }

    void controlMotorISR(float32_t voltage) {
        // バッテリ電圧を考慮したduty比算出
        float32_t duty_ratio;
        float32_t battery_voltage = _battery.readVoltage();
        if (battery_voltage > 0.0f) duty_ratio = voltage / battery_voltage;
        else duty_ratio = 0.0f;
        // printf("%f\r\n", duty_ratio);
        _motor.update(duty_ratio);
    }

    float32_t _target_velocity;
    float32_t _velocity;
    float32_t _corrected_velocity;  // PID制御を考慮したフィードバック補正後の速度
    float32_t _velocity_error;  // フィードフォワード実行後の速度と指定速度との差分
    float32_t _integral_velocity_error;
    float32_t _preview_target_velocity;
    WheelParams _wheel_params;
    Encoder &_encoder;
    Motor &_motor;
    Battery &_battery;
    const float32_t _dt;       //< second [s]
    const float32_t _distance_per_pulse;  //< [mm/pulse]
    const float32_t _velocity_per_pulse;  //< [mm/s]

};  // class WheelController

}  // namespace mslh

#endif  // MSLH_WHEEL_CONTROLLER_H
