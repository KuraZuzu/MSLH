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
        , _ideal_velocity(0.0)
        , _target_velocity(0.0f)
        , _diff_velocity(0.0f)
        , _old_diff_velocity(0.0f)
        , _integral_velocity(0.0f)
        , _sampling_time(speed_sampling_time)
        , _distance_per_pulse(wheel_diameter*PI/static_cast<float32_t>(_encoder.getOneRotationPulse()))
        , _velocity_per_pulse(_distance_per_pulse / _sampling_time)
        , _complete_set_velocity_flag(true)
{
}

void mslh::WheelController::start() {
    _encoder.start();
    _motor.start();
    _motor.update(0);
    _battery.start();
}

void mslh::WheelController::stop() {
    _motor.stop();
    _encoder.stop();
}

void mslh::WheelController::reset() {
    _motor.update(0.0f);
    _velocity = 0.0f;
    _target_accel = 0.0f;
    _ideal_velocity = 0.0f;
    _target_velocity = 0.0f;
    _encoder.reset();
}


void mslh::WheelController::setVelocity(float32_t accel, float32_t velocity) {
    _complete_set_velocity_flag = false;

    // 加速度と速度の組み合わせで到達し得るもののみ代入 (accel=0 は例外)
    if((accel * (velocity - _velocity)) >= 0) {
        _target_accel = accel;
        _target_velocity = velocity;
        _init_velocity = _velocity;
        _diff_velocity = _target_velocity - _init_velocity;
    }

    _complete_set_velocity_flag = true;
}


void mslh::WheelController::orderVelocityMomentarily() {
    setVelocity(0.0f, 0.0f);
    _motor.update(0.0f);
//    setVelocity(0.0f, 0.0f);
}


void mslh::WheelController::interruptTwoFreedomDegreeControl() {

    if(_complete_set_velocity_flag) {  //加速度と速度が両方入力される間に割り込みが発生することを防ぐ

        /**
         * @note フィードバック制御
         *   PID制御を実行(現在はP制御のみ)
         */
        _old_diff_velocity = _diff_velocity;
        _diff_velocity = _ideal_velocity - _velocity; // (目標速度) - (現在速度)　逆進の際は負の値となる
        _integral_velocity += (_diff_velocity + _old_diff_velocity) * 0.5f * _sampling_time; // _diff_speed + _old_diff_speed じゃない？P制御を妨げる感じで// I制御のための積分 (台形として面積積分)
        const float32_t p_error = _diff_velocity * machine_parameter::KP_MOTOR_VOLTAGE; // ゲインをかける (逆進の際は負の値となる)
        const float32_t i_error = _integral_velocity * machine_parameter::KI_MOTOR_VOLTAGE; // ゲインをかける (逆進の際は負の値となる)
        const float32_t d_error = (_diff_velocity - _old_diff_velocity) / _sampling_time * machine_parameter::KD_MOTOR_VOLTAGE; // ゲインをかける (逆進の際は負の値となる)

        float32_t accel_ratio = (_velocity - _init_velocity) / _diff_velocity;
        if((_target_accel > 0) && (accel_ratio < 0.1f)) accel_ratio = 0.1f; // 加速時に本来の加速度の0.1倍以下の時は強制的に0.1倍に
        else if ((_target_accel < 0) && (accel_ratio > -0.1f)) accel_ratio = -0.1f; // 減速速時に本来の加速度の-0.1倍以上の時は強制的に0.1倍に
        float32_t ideal_accel = _target_accel * sin(accel_ratio * PI);
        /**
         * @note 目標速度に到達していた場合、加速度を0にする
         *   ①(加速度 > 0) && ((目標速度 - 現在速度) > 0) の際に加速度有効
         *   ②(加速度 < 0) && ((目標速度 - 現在速度) < 0) の際に加速度有効
         *   ①と②を満たさない時，加速度を無効(=0)として速度のPID制御のみ行う(現在はP制御のみ)
         */
        if ((_target_accel * (_target_velocity - _velocity)) < 0.0f) { //ここが悪さしてる
            ideal_accel = 0.0f;
            _ideal_velocity = _target_velocity;
        }

        /** 指定された加速度での理想速度算出。次回割り込み時のフィードバック制御で参照する。 */
        _ideal_velocity += (ideal_accel * _sampling_time);
        const float32_t corrected_velocity = _ideal_velocity + p_error + i_error + d_error; //PIDを足し合わせた速度算出


        /**
         * @note フィードフォワード制御
         *   加速度から必要な出力電圧を算出する(なぜかPIDのゲインによって特定の速度より上がらなくなる)
         *   ① 加速Wheelトルク = (機体質量) * (タイヤ加速度[mm/ss -> m/ss]) * (タイヤ直径 [mm -> m]) / (2 [2つのモータのため]);
         *   ② 定速度Wheel電圧 = Ke(逆起電力定数[V/rpm]) * (PIDを考慮したモータ回転数 [mm/s -> rpm]  ギア比も考慮)
         */
        // ① 加速Wheelトルク
        const float32_t wheel_torque = machine_parameter::MASS * ideal_accel * 0.001f * (machine_parameter::WHEEL_RADIUS * 0.001f) * 0.5f; //  *0.001f は[mm]->[m]変換  *0.5f は2つのモータのため
        const float32_t motor_torque = wheel_torque / machine_parameter::GEAR_RATIO; //必要モータトルク
        const float32_t current = motor_torque / machine_parameter::K_T; // モータに必要な電流
        // ② 定速度Wheel電圧
        const float32_t reverse_voltage = (machine_parameter::K_E * (60.0f * corrected_velocity) * machine_parameter::GEAR_RATIO) / (PI * machine_parameter::WHEEL_DIAMETER); // 分子の速度の単位を[mm]のままなのは、分母のタイヤ径も[mm]のため
        const float32_t voltage = (machine_parameter::RESISTANCE_MOTOR * current + reverse_voltage); // 必要電圧


        /** バッテリ電圧を考慮したduty比算出 */
        // const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * machine_parameter::BATTERY_VOLTAGE_RATIO;
        // const float32_t duty_ratio = voltage / battery_voltage;


        /** モータに電圧印加 */
        _motor.update(duty_ratio);
    }
}
