/**
 * 2019/10/23
 * @author KuraZuzu
 * */

#include "encoder.h"

Encoder::Encoder(TIM_HandleTypeDef* htim_x, uint16_t one_rotation_pulse, bool cw_wise)
: _pulse_count(_offset_pulse),
  _integral_pulse(0),
  _htim_x(htim_x),
  _one_rotation_pulse(one_rotation_pulse),
  _rotation_count(0),
  _delta_pulse(0),
  _forward_wise(cw_wise) {
    stop();
}

void Encoder::start() {
    _htim_x->Instance->CNT = _offset_pulse;
    HAL_TIM_Encoder_Start(_htim_x, TIM_CHANNEL_ALL);
}

void Encoder::stop() {
    HAL_TIM_Encoder_Stop(_htim_x, TIM_CHANNEL_ALL);
}

void Encoder::reset() {
    _pulse_count = _offset_pulse;
    _integral_pulse = 0;
    _rotation_count = 0;
    _delta_pulse = 0;
}


int64_t Encoder::get_delta_pulse() {
    update_delta_pulse();
    return _delta_pulse;
}

int64_t Encoder::get_rotation_count() {
    update_encoder();
    return _rotation_count;
}

int64_t Encoder::get_total_pulse() {
    update_encoder();
    return _one_rotation_pulse*_rotation_count + _integral_pulse;
}

void Encoder::update_encoder() {
    get_delta_pulse();
    update_rotation_count();
}

void Encoder::update_delta_pulse() {
    _pulse_count = (_htim_x->Instance->CNT);
    _htim_x->Instance->CNT = _offset_pulse;

    _delta_pulse = _pulse_count - _offset_pulse;

    /**_delta_pulse を更新
     * _forward_wise が true の時にカウントアップとする． */
    if (!_forward_wise) _delta_pulse *= -1;

    /** _itegral_pulse を更新 */
    _integral_pulse += _delta_pulse;
}

void Encoder::update_rotation_count() {
    _rotation_count = _integral_pulse / _one_rotation_pulse;
    _integral_pulse %= _one_rotation_pulse;
}
