/*!
* @file motor_control.cpp v1.0
* @Copyright © 2021 Kazushi Kurasawa
* @date 2021.03.12
*
* Released under the MIT license.
* see https://opensource.org/licenses/MIT
*/

#include "wheel.h"

mslh::Wheel::Wheel(Motor &motor, Encoder &encoder, AnalogInDMAStream &battery, float32_t wheel_diameter, float32_t speed_sampling_time)
        : _motor(motor)
        , _encoder(encoder)
        , _battery(battery)
        , _target_accel(0.0f)
        , _speed(0.0f)
        , _ideal_speed(0.0)
        , _target_speed(0.0f)
        , _diff_speed(0.0f)
        , _old_diff_speed(0.0f)
        , _integral_speed(0.0f)
        , _speed_sampling_time(speed_sampling_time)
        , _distance_per_pulse(wheel_diameter*PI/static_cast<float32_t>(_encoder.getOneRotationPulse()))
        , _speed_per_pulse(_distance_per_pulse / _speed_sampling_time)
        , _complete_set_speed_flag(true)
{
}

void mslh::Wheel::start() {
    _encoder.start();
    _motor.start();
    _motor.update(0);
    _battery.start();
}

void mslh::Wheel::stop() {
    _motor.stop();
    _encoder.stop();
}

void mslh::Wheel::reset() {
    _motor.update(0.0f);
    _speed = 0.0f;
    _target_accel = 0.0f;
    _ideal_speed = 0.0f;
    _target_speed = 0.0f;
    _encoder.reset();
}


void mslh::Wheel::setSpeed(float32_t accel, float32_t speed) {
    _complete_set_speed_flag = false;

    // 加速度と速度の組み合わせで到達し得るもののみ代入 (accel=0 は例外)
    if((accel * (speed - _speed)) >= 0) {

        /** 定速運動に達していない状況で加減速の符号が入れ替わると急激な変化が起こるのを抑制 */
//        if((accel * _target_accel) < 0) _ideal_speed = _speed;  //< 加速度分の数値も現在の速度として逆起電力の式に加算
        _target_accel = accel;
        _target_speed = speed;
        _init_speed = _speed;
        _non_liner_range_speed = (_target_speed - _init_speed) / 10.0f;  //現在速度と目標速度の差の"1/10"倍を非線形加速範囲とする
        if(_non_liner_range_speed == 0.0f) _non_liner_range_speed = 1.0f; //後に"0"による除算が起こらないように処理 "1.0"の数値に意味はない
    }

    _complete_set_speed_flag = true;
}


void mslh::Wheel::orderSpeedMomentarily() {
    setSpeed(0.0f, 0.0f);
    _motor.update(0.0f);
//    setSpeed(0.0f, 0.0f);
}


void mslh::Wheel::interruptTwoFreedomDegreeControl() {

    if(_complete_set_speed_flag) {  //加速度と速度が両方入力される間に割り込みが発生することを防ぐ

        /**
         * @note フィードバック制御
         *   PID制御を実行(現在はP制御のみ)
         */
        _old_diff_speed = _diff_speed;
        _diff_speed = _ideal_speed - _speed; // (目標速度) - (現在速度)　逆進の際は負の値となる
        _integral_speed += (_diff_speed + _old_diff_speed) * 0.5f * _speed_sampling_time; // _diff_speed + _old_diff_speed じゃない？P制御を妨げる感じで// I制御のための積分 (台形として面積積分)
        const float32_t p_error = _diff_speed * machine_parameter::KP_MOTOR_VOLTAGE; // ゲインをかける (逆進の際は負の値となる)
        const float32_t i_error = _integral_speed * machine_parameter::KI_MOTOR_VOLTAGE; // ゲインをかける (逆進の際は負の値となる)
        const float32_t d_error = (_diff_speed - _old_diff_speed) / _speed_sampling_time * machine_parameter::KD_MOTOR_VOLTAGE; // ゲインをかける (逆進の際は負の値となる)


        float32_t accel_ratio = 1.0f;

        /// ここで加減速の始点と終点付近"(目標ー現在)速度/10"の範囲(setSpeedで範囲定義)で行う
        /// ただし、この条件分岐では減速の時に負の値に対応できない場合があるので改良必須（バグが確実にある）
        if(_speed < (_init_speed + _non_liner_range_speed)) {
            accel_ratio = (_speed - _init_speed) / _non_liner_range_speed; //0で除算しないように注意。

        }else if((_target_speed - _non_liner_range_speed) < _speed) {
            accel_ratio = (_target_speed - _speed) / _non_liner_range_speed; //0で除算しないように注意。
        }
        ///

        if(accel_ratio < 0.1f) accel_ratio = 0.2f;
        if(accel_ratio > 1.0f) accel_ratio = 1.0f;
        ///


        /**
         * @note 目標速度を超えていた場合、加速度を0にする
         *   ①(加速度 > 0) && ((目標速度 - 現在速度) > 0) の際に加速度有効
         *   ②(加速度 < 0) && ((目標速度 - 現在速度) < 0) の際に加速度有効
         *   ①と②を満たさない時，加速度を無効(=0)として速度のPID制御のみ行う(現在はP制御のみ)
         */
        if ((_target_accel * (_target_speed - _speed)) < 0.0f) { //ここが悪さしてる
            accel_ratio = 0.0f;
            _ideal_speed = _target_speed;
        }
        const float32_t ideal_accel = accel_ratio * _target_accel;

        /** 指定された加速度での理想速度算出。次回割り込み時のフィードバック制御で参照する。 */
        _ideal_speed += (ideal_accel * _speed_sampling_time);
        const float32_t corrected_speed = _ideal_speed + p_error + i_error + d_error; //PIDを足し合わせた速度算出


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
        const float32_t reverse_voltage = (machine_parameter::K_E * (60.0f * corrected_speed) * machine_parameter::GEAR_RATIO) / (PI * machine_parameter::WHEEL_DIAMETER); // 分子の速度の単位を[mm]のままなのは、分母のタイヤ径も[mm]のため
        const float32_t voltage = (machine_parameter::RESISTANCE_MOTOR * current + reverse_voltage); // 必要電圧


        /** バッテリ電圧を考慮したduty比算出 */
        const float32_t battery_voltage = 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF * machine_parameter::BATTERY_VOLTAGE_RATIO;
        const float32_t duty_ratio = voltage / battery_voltage;


        /** モータに電圧印加 */
        _motor.update(duty_ratio);
    }
}
