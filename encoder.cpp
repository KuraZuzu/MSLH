/*!
 * @file encoder.cpp v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.10.23
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "encoder.h"

mslh::Encoder::Encoder(TIM_HandleTypeDef &htim_x, int32_t one_rotation_pulse, bool cw)
  : _delta_pulse(0)
  , _total_pulse(0)
  , _htim_x(htim_x)
  , _offset_pulse(__HAL_TIM_GET_AUTORELOAD(&_htim_x) / 2 - 1)
  , _one_rotation_pulse(static_cast<int32_t>(one_rotation_pulse))  //< 計算高速化のため．
  , _forward_wise(cw)
{
}

void mslh::Encoder::start() const {
    HAL_TIM_Encoder_Start(&_htim_x, TIM_CHANNEL_ALL);
}

void mslh::Encoder::stop() const {
    HAL_TIM_Encoder_Stop(&_htim_x, TIM_CHANNEL_ALL);
}

void mslh::Encoder::reset() {
    _delta_pulse = 0;
    _total_pulse = 0;
    _htim_x.Instance->CNT = _offset_pulse;
}

void mslh::Encoder::update() {
    uint32_t pulse_count = __HAL_TIM_GET_COUNTER(&_htim_x);
    __HAL_TIM_SET_COUNTER(&_htim_x, _offset_pulse);

    /// _delta_pulse を更新
    _delta_pulse = static_cast<int32_t>(pulse_count - _offset_pulse);

    /// _forward_wise が true の時にカウントアップとする
    if (!_forward_wise) _delta_pulse *= -1;

    /// _total_pulse を更新
    _total_pulse += static_cast<int64_t>(_delta_pulse);
}

int32_t mslh::Encoder::getOneRotationPulse() const {
    return _one_rotation_pulse;
}