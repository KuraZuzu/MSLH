#ifndef BATTERY_H
#define BATTERY_H

#include "analog_in_dma_stream.h"
#include "arm_math.h"

namespace mslh {
class Battery {
   public:
    Battery(AnalogInDMAStream& battery, float32_t adc_ratio)
        : _battery(battery), _adc_ratio(adc_ratio) {}

    void init() { 
        _battery.init(); 
    }

    inline float32_t readVoltage() {
        return 3.3f * static_cast<float32_t>(_battery.read()) / 0x0FFF / _adc_ratio;
    }

   private:
    AnalogInDMAStream _battery;
    float32_t _adc_ratio;  // 0 ~ 1.0（バッテリ電圧を監視するADC分圧比率）
};

}  // namespace mslh

#endif /* BATTERY_H */
