#ifndef BATTERY_H
#define BATTERY_H

#include "analog_in_dma_stream.h"
#include "arm_math.h"

namespace mslh {
class Battery {
   public:
    Battery(AnalogInDMAStream& analogin_dma_stream, float32_t adc_ratio)
        : _analogin_dma_stream(analogin_dma_stream), _adc_ratio(adc_ratio) {}

    void init() { 
        _analogin_dma_stream.init(); 
    }

    inline float32_t readVoltage() {
        return 3.3f * static_cast<float32_t>(_analogin_dma_stream.read()) / 0x0FFF / _adc_ratio;
    }

   private:
    AnalogInDMAStream _analogin_dma_stream;
    float32_t _adc_ratio;  // 0 ~ 1.0（バッテリ電圧を監視するADC分圧比率）
};

}  // namespace mslh

#endif /* BATTERY_H */
