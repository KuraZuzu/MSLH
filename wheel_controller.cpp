/*!
 * @file motor_control.cpp v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.12
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "wheel_controller.h"

mslh::WheelController::WheelController(Motor &motor, Encoder &encoder, AnalogInDMAStream &battery, float32_t wheel_diameter, float32_t sampling_time)
    : _motor(motor)
    , _encoder(encoder)
    , _battery(battery)
    , _target_accel(0.0f)
    , _velocity(0.0f)
    , _init_velocity(0.0f)
    , _ideal_velocity(0.0f)
    , _target_velocity(0.0f)
    , _integral_diff_velocity(0.0f)
    , _duty_ratio(0.0f)
    , _sampling_time(sampling_time)
    , _distance_per_pulse(wheel_diameter * PI / static_cast<float32_t>(_encoder.getOneRotationPulse()))
    , _velocity_per_pulse(_distance_per_pulse / _sampling_time)
    {}

void mslh::WheelController::start()
{
    _battery.init();  // ここがHAL_TIM_BASE_INITより先に動いていないとバッテリ計測でinfになって止まる。
    _encoder.start();
    _motor.start();
    _motor.update(0.0f);
    _encoder.reset();
}

void mslh::WheelController::stop()
{
    _motor.stop();
    _encoder.stop();
}

void mslh::WheelController::reset()
{
    _motor.update(0.0f);
    _velocity = 0.0f;
    _target_accel = 0.0f;
    _ideal_velocity = 0.0f;
    _target_velocity = 0.0f;
    _integral_diff_velocity = 0.0f;
    _init_velocity = 0.0f;
    _duty_ratio = 0.0f;
    _encoder.reset();
}

void mslh::WheelController::orderVelocityMomentarily()
{
    setVelocity(0.0f, 0.0f);
    _motor.update(0.0f);
}

void mslh::WheelController::setVelocity(float32_t accel, float32_t velocity)
{
    _target_accel = accel;
    _init_velocity = _velocity;
    _target_velocity = velocity;
}

void mslh::WheelController::interrupt2DoFControll()
{

    /**
     * @note フィードバック制御
     *   FF制御より先に呼び出すが、初回は理想加速度が考慮されていないのでFBは２回目以降から実行される。
     *   PID制御を実行
     */
    const float32_t diff_velocity = _ideal_velocity - _velocity;
    _integral_diff_velocity += diff_velocity;                                                // (目標速度) - (現在速度)　逆進の際は負の値となる
    const float32_t p_error = diff_velocity * machine_parameter::KP_MOTOR_VOLTAGE;           // ゲインをかける (逆進の際は負の値となる)
    const float32_t i_error = _integral_diff_velocity * machine_parameter::KI_MOTOR_VOLTAGE; // ゲインをかける (逆進の際は負の値となる)

    // 現在速度が目標を超えていた場合に加速度を"0"にして次フレームでの理想速度に加算されないようにする（正負の両方に対応）
    // これは先にMeasureSpeedを呼び出さないといけないと思う
    float32_t accel = _target_accel;
    if(_target_velocity - _init_velocity >= 0) {
        if(_target_velocity <= _velocity) {
            accel = 0;
            _ideal_velocity = _target_velocity;
        }

    } else {
        if(_target_velocity >= _velocity) {
            accel = 0;
            _ideal_velocity = _target_velocity;
        }
    }

    _ideal_velocity += (accel * _sampling_time); // 指定された加速度での理想速度算出

    // PIDをモータ印加電圧に反映
    const float32_t corrected_velocity = _ideal_velocity + p_error + i_error;  // これ前進と逆進で逆起電力変わっちゃうのでは？いや、PWMを-から+で扱ってるからいいのか？

    /**
     * @note フィードフォワード制御
     *   加速度から必要な出力電圧を算出する(なぜかPIDのゲインによって特定の速度より上がらなくなる)
     *   ① 加速Wheelトルク = (機体質量) * (タイヤ加速度[mm/ss -> m/ss]) * (タイヤ直径 [mm -> m]) / (2 [2つのモータのため]);
     *   ② 定速度Wheel電圧 = Ke(逆起電力定数[V/rpm]) * (PIDを考慮したモータ回転数 [mm/s -> rpm]  ギア比も考慮)
     */
    // ① 加速Wheelトルク
    const float32_t wheel_torque = machine_parameter::MASS * accel * 0.001f * (machine_parameter::WHEEL_RADIUS * 0.001f) * 0.5f; //  *0.001f は[mm]->[m]変換  *0.5f は2つのモータのため
    const float32_t motor_torque = wheel_torque / machine_parameter::GEAR_RATIO;                                                         // 必要モータトルク
    const float32_t current = motor_torque / machine_parameter::K_T;                                                                     // モータに必要な電流

    // ② 定速度Wheel電圧
    const float32_t reverse_voltage = (machine_parameter::K_E * (60.0f * corrected_velocity) * machine_parameter::GEAR_RATIO) / (PI * machine_parameter::WHEEL_DIAMETER); // 分子の速度の単位を[mm]のままなのは、分母のタイヤ径も[mm]のため
    const float32_t voltage = (machine_parameter::RESISTANCE_MOTOR * current + reverse_voltage);                                                                          // 必要電圧

    /** バッテリ電圧を考慮したduty比算出 */
    const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * machine_parameter::BATTERY_VOLTAGE_RATIO;
    if(battery_voltage > 0.0f) _duty_ratio = voltage / battery_voltage;
    else _duty_ratio = 0.0f;

    // /** モータに電圧印加 */
    _motor.update(_duty_ratio);
}
