/**
 * 2019/10/23
 * @author KuraZuzu
 * */

#include "encoder.h"

Encoder::Encoder(TIM_HandleTypeDef* htim_x, bool cw_wise) {
    _htim_x = htim_x;
    _forward_wise = cw_wise;
    _pulse_count = _offset_pulse_count;
}

void Encoder::start() {
    _htim_x->Instance->CNT = _offset_pulse_count;
    HAL_TIM_Encoder_Start(_htim_x, TIM_CHANNEL_ALL);
}

void Encoder::stop() {
    HAL_TIM_Encoder_Stop(_htim_x, TIM_CHANNEL_ALL);
}

int64_t Encoder::get_delta_pulse() {
    _pulse_count = (_htim_x->Instance->CNT);
    _htim_x->Instance->CNT = _offset_pulse_count;

    int32_t delta_pulse = _pulse_count - _offset_pulse_count;

    if (!_forward_wise) delta_pulse = -delta_pulse;

    _integral_count += delta_pulse;
    return delta_pulse;
}

int16_t Encoder::get_integral_pulse() {
    return _integral_count;
}

void Encoder::reset_integral() {
    _integral_count = 0;
}
