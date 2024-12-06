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
#include "defines.h"
#include "encoder.h"
#include "motor.h"
#include "parameter.h"

namespace mslh {
class WheelController {
   public:
    WheelController(Motor &motor, Encoder &encoder, AnalogInDMAStream &battery,
                    float32_t wheel_diameter, float32_t sampling_time)
        : _motor(motor),
          _encoder(encoder),
          _battery(battery),
          _sampling_time(sampling_time),
          _distance_per_pulse(
              wheel_diameter * PI /
              static_cast<float32_t>(_encoder.getOneRotationPulse())),
          _velocity_per_pulse(_distance_per_pulse / _sampling_time) {}

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
        _integral_velocity_error = 0.0f;
    }

    float32_t getVelocity() { return _velocity; }

    void controlWheelISR(float32_t velocity) {
        controlMeasureVelocityISR();  // ここで"_velocity"に反映されるのは、前フレームに指定した速度に対応する実速度（つまり1次遅れ）
        _velocity_error = _velocity - _preview_target_velocity;  // 前フレームの指令速度と実速度の差分を取得
        const float32_t accel = velocity - _velocity;  // 現フレームの速度にするために必要な加速度を算出

        float32_t motor_voltage = 0.0f;
        motor_voltage += controlFeedBackISR(velocity);  // 前フレームの実績をもとにしたPID制御
        motor_voltage += controlFeedForwardAccelISR(accel);  // 加速に必要なトルクベースの電圧値
        motor_voltage += controlFeedForwardVelocityISR(velocity);  // 逆起電力を打ち消す電圧値
        controlMotorISR(motor_voltage);  // duty比を計算してモータに印加
        _preview_target_velocity = velocity;
    }

   private:
    void controlMeasureVelocityISR() {
        _encoder.update();
        _velocity == _velocity_per_pulse *static_cast<float32_t>(_encoder.getDeltaPulse());
    }

    float32_t controlFeedForwardAccelISR(float32_t accel) {
        const float32_t wheel_torque = _mass * (accel / 1000) * ((_diameter / 2) / 1000);
        const float32_t motor_toruqe = wheel_torque / _gear_ratio;
        const float32_t current = motor_toruqe / _K_T;
        const float32_t torque_voltage = _resistance * current;
        return torque_voltage;
    }

    float32_t controlFeedForwardVelocityISR(float32_t velocity) {
        const float32_t reverse_voltage = (_K_E * (60.0f * velocity) * _gear_ratio) / (M_PI * _diameter);
        return reverse_voltage;
    }

    float32_t controlFeedBackISR(float32_t velocity) {
        _integral_velocity_error += _velocity_error;  // メンバ変数に頼りたくないが、積分なので仕方なく
        const float32_t p_error = _velocity_error * KP;
        const float32_t i_error = _integral_velocity_error * KI;
        const float32_t pid_error = p_error + i_error;  
        // どのモータでも一定のスケールにするために逆起電力の式をもとにPID電圧を決定（計算は重くなるので不要なら削除）
        const float32_t pid_voltage = (_K_E * (60.0f * pid_error) * _gear_ratio) / (M_PI * _diameter);
        return pid_voltage;
    }

    void controlMotorISR(float32_t voltage) {
        // バッテリ電圧を考慮したduty比算出
        const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * battery_voltage_ratio;
        float32_t duty_ratio;
        if(battery_voltage > 0.0f) duty_ratio = voltage / battery_voltage;  // バッテリ読み取り不可時の0除算を回避
        else duty_ratio = 0.0f;
        _motor.update(duty_ratio);
    }

    // float32_t _target_accel;
    float32_t _target_velocity;
    float32_t _velocity;
    float32_t _corrected_velocity;  // PID制御を考慮したフィードバック補正後の速度 accelはvelocityで出しているのでおかしくならないか？
    float32_t _velocity_error;  // フィードフォワード実行後の速度と指定速度との差分
    float32_t _integral_velocity_error;
    float32_t _preview_target_velocity;
    float32_t _mass;  // ホイール単体の質量
    float32_t _diameter;  // ホイール直径
    float32_t _gear_ratio;  // タイヤとモーターのギア比
    float32_t _K_T;  // モータのK_T これはmotorクラス側で持ったほうが良いかも
    float32_t _K_E;  // モータのK_T これはmotorクラス側で持ったほうが良いかも
    float32_t _resistance;  // モータの抵抗
    float32_t battery_voltage_ratio;  // これはバッテリのパラメータだが、そもそもバッテリクラスを作ったほうがいいのでは？
    float32_t KP;  // Pゲイン
    float32_t KI;  // Iゲイン
    Encoder &_encoder;
    Motor &_motor;
    AnalogInDMAStream &_battery;
    const float32_t _sampling_time;       //< second [s]
    const float32_t _distance_per_pulse;  //< [mm/pulse]
    const float32_t _velocity_per_pulse;  //< [mm/s]

};  // class WheelController

}  // namespace mslh

#endif  // MSLH_WHEEL_CONTROLLER_H
