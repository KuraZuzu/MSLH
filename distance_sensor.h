/*!
 * @file distance_sensor.h v1.0
 * @Copyright © 2021 Kazushi Kurasawa
 * @date 2021.03.03
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef ZUZUHALFTPPMOD1_DISTANCE_SENSOR_H
#define ZUZUHALFTPPMOD1_DISTANCE_SENSOR_H

#include "pwm_out.h"
#include "analog_in_dma_stream.h"
#include "digital_out.h"

/**
 * @brief
 *   This is a distance sensor class using Morinaga-type light emitting circuit.  <br>
 *   森永式発光回路を用いた距離センサクラスです．
 */
class DistanceSensor {

public:

    /**
     * @param phtr is Photo-Transistor adc handler.
     */
    DistanceSensor(PWMOut led, AnalogInDMAStream phtr)
    : _led(led)
    , _phtr(phtr)
    , _max_value(0)
    , _current_value(0)
    , _previous_value(0)
    , _not_higher_count(0) {

        _led.stop();  //< 明示的にLEDストップ
        measureOffset();
        write(1000);  //< Default setting is 1k[Hz].
    }

    void measureOffset() {
        _phtr.start();
        HAL_Delay(1000);  //< Delay for the ADC to stabilize at startup.
        uint64_t average_value = 0;
        for (int i = 0; i < 10; ++i) {
            average_value += _phtr.read();
        }
        average_value /= 10;
        _offset_value = static_cast<uint16_t>(average_value);
    }

    /**
     * @fn Reset phtr value.
     */
    void reset() {
        _max_value = 0;
        _current_value = 0;
        _previous_value = 0;
        _not_higher_count = 0;
    }

    void star() {
        _led.start();
    }

    void write(uint32_t led_hz) {
        _led.period(led_hz);
        _led = 0.5;  //< Set duty ratio.
    }

    uint16_t read() {
        return _max_value - _offset_value;
    }


private:

    // これを適宜どうやって呼び出すか。
    void measureValue() {
        _previous_value = _current_value;
        _current_value = _phtr.read();
        int32_t diff_value = static_cast<int32_t >(_current_value) - _previous_value;
        if(_max_value < _current_value) _max_value = _current_value;
        else _not_higher_count++;
    }

    uint16_t getDistance_mm() {
        return convert_12bit_to_mm(_phtr.read());
    }

    uint16_t convert_12bit_to_mm(uint16_t value) {
        value = 0; //ここで距離変換の数式わちゃわちゃ。
        return value;
    }

    PWMOut _led;
    AnalogInDMAStream _phtr;
    uint16_t _max_value;
    uint16_t _current_value;
    uint16_t _previous_value;
    uint16_t _offset_value;  //< 外乱光のオフセット
    uint16_t _not_higher_count;  //< _max_value が更新されない回数。
};


#endif //ZUZUHALFTPPMOD1_DISTANCE_SENSOR_H
