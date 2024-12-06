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
          _duty_ratio(0.0f),
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
        controlMeasureVelocityISR();  // ここで"_velocity"に反映されるのは、1フレーム前に指定した速度に対応する実速度
        _velocity_error = _velocity - _preview_target_velocity;  // 停止時からの初回運転フレームは0になる
        const float32_t accel = velocity - _velocity;  // 今回のフレームの速度にするために必要な加速度を算出

        controlFeedBackISR(velocity);  //返り値でvoltageの加算分を返したほうがいいかも
        controlFeedForwardISR(accel);  //返り値でvoltageの加算分を返したほうがいいかも
        _motor.update(_duty_ratio);
        _preview_target_velocity = velocity;
    }

   private:
    void controlMeasureVelocityISR() {
        _encoder.update();
        _velocity == _velocity_per_pulse *static_cast<float32_t>(
                         _encoder.getDeltaPulse());
    }

    void controlFeedForwardISR(float32_t accel) {
        // 加速
        const float32_t wheel_torque = _mass * (accel / 1000) * ((_diameter / 2) / 1000);
        const float32_t motor_toruqe = wheel_torque / _gear_ratio;
        const float32_t current = motor_toruqe / _K_T;
        // 定速
        const float32_t reverse_voltage = (_K_E * (60.0f * _corrected_velocity) * _gear_ratio) / (M_PI * _diameter);
        // 電圧
        const float32_t voltage = (_resistance * current) + reverse_voltage;
        // バッテリ電圧を考慮したduty比算出
        const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * battery_voltage_ratio;
        if(battery_voltage > 0.0f) _duty_ratio = voltage / battery_voltage;
        else _duty_ratio = 0.0f;
    }

    void controlFeedBackISR(float32_t velocity) {
        _integral_velocity_error += _velocity_error;
        const float32_t p_error = _velocity_error * KP;
        const float32_t i_error = _integral_velocity_error * KI;
        _corrected_velocity = velocity + p_error + i_error;  // 停止時からの初回速度司令時は問題ない（速度errorは0だし、仮に値が入っても1フレーム分で極小）
    }

    // float32_t _target_accel;
    float32_t _target_velocity;
    float32_t _velocity;
    float32_t _corrected_velocity;  // PID制御を考慮したフィードバック補正後の速度 accelはvelocityで出しているのでおかしくならないか？
    float32_t _velocity_error;  // フィードフォワード実行後の速度と指定速度との差分
    float32_t _integral_velocity_error;
    float32_t _preview_target_velocity;
    float32_t _duty_ratio;
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
