/*!
 * @file encoder.cpp v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.10.23
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "encoder.h"

Encoder::Encoder(TIM_HandleTypeDef &htim_x, uint16_t one_rotation_pulse, bool cw)
: _offset_pulse(65536/2 - 1),
  _total_pulse(0),
  _htim_x(htim_x),
  _one_rotation_pulse(static_cast<int32_t>(one_rotation_pulse)),  //< 計算高速化のため．
  _rotation_count(0),
  _delta_pulse(0),
  _forward_wise(cw)
{
}

void Encoder::start() {
    HAL_TIM_Encoder_Start(&_htim_x, TIM_CHANNEL_ALL);
}

void Encoder::stop() {
    HAL_TIM_Encoder_Stop(&_htim_x, TIM_CHANNEL_ALL);
}

void Encoder::reset() {
    _total_pulse = 0;
    _rotation_count = 0;
    _delta_pulse = 0;
    _htim_x.Instance->CNT = _offset_pulse;
}

void Encoder::update() {
    update_pulse();
    update_rotation_count();
}


void Encoder::update_pulse() {
    auto pulse_count = _htim_x.Instance->CNT;
    _htim_x.Instance->CNT = _offset_pulse;

    // _delta_pulse を更新
    _delta_pulse = static_cast<int32_t>(pulse_count - _offset_pulse);

    // _forward_wise が true の時にカウントアップとする
    if (!_forward_wise) _delta_pulse *= -1;

    // _total_pulse を更新
    _total_pulse += static_cast<int64_t>(_delta_pulse);
}

void Encoder::update_rotation_count() {
    _rotation_count += static_cast<int32_t>(_total_pulse / _one_rotation_pulse);
    // _surplus_pulse = _total_pulse % _one_rotation_pulse;  //< 余剰カウントが必要になったら実装(計算コストが増える)
}
