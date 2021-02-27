/**
 * 2019/10/23
 * @author KuraZuzu
 * */

#include "encoder.h"

Encoder::Encoder(TIM_HandleTypeDef* htim_x, uint16_t one_rotation_pulse, bool cw_wise)
:_pulse_count(_offset_pulse_count),
_integral_count(0),
_htim_x(htim_x),
_one_rotation_pulse(one_rotation_pulse),
_rotation_count(0),
_forward_wise(cw_wise) {
    stop();
    reset_integral();
}

void Encoder::start() {
    _htim_x->Instance->CNT = _offset_pulse_count;
    HAL_TIM_Encoder_Start(_htim_x, TIM_CHANNEL_ALL);
}

void Encoder::stop() {
    HAL_TIM_Encoder_Stop(_htim_x, TIM_CHANNEL_ALL);
}

int32_t Encoder::get_delta_pulse() {

    /** _forward_wise が true の時にカウントアップとする． */
    _pulse_count = (_htim_x->Instance->CNT);
    _htim_x->Instance->CNT = _offset_pulse_count;

    int64_t delta_pulse = _pulse_count - _offset_pulse_count;
    if (!_forward_wise) delta_pulse *= -1;

    _integral_count += delta_pulse;
    update_rotation_count(_integral_count);

    return delta_pulse;
}

int64_t Encoder::get_total_pulse() {
    return _one_rotation_pulse*_rotation_count + _integral_count;
}

void Encoder::reset_integral() {
    _integral_count = 0;
}
