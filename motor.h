/*!
 * @file motor.h v1.0
 * @Copyright © 2019 Kazushi Kurasawa
 * @date 2019.03.06
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#ifndef MSLH_MOTOR_H
#define MSLH_MOTOR_H

#include "defines.h"
#include "digital_out.h"
#include "pwm_out.h"
#include "tim.h"

namespace mslh {

class MotorParams {
   public:
    MotorParams(): _resistance(1.0f), _kt(1.0f), _ke(1.0f) {}

    float32_t getResistance() {
        return _resistance;
    }

    float32_t getKt() {
        return _kt;
    }

    float32_t getKe() {
        return _ke;
    }

    MotorParams& setResistance(float32_t resistance) {
        this->_resistance = resistance;
        return *this;
    }

    MotorParams& setKt(float32_t kt) {
        this->_kt = kt;
        return *this;
    }

    MotorParams& setKe(float32_t ke) {
        this->_ke = ke;
        return *this;
    }

   private:
    float32_t _resistance;  // 内部抵抗
    float32_t _kt;  // [Nm/A]
    float32_t _ke;  // [V/ω)
};

/**
 * @brief
 *   This Class, the target of the control is Motor-Driver with PHASE-ENABLE
 * mode.  <br> Turns the motor at the specified PWM ratio.
 */

class Motor {
   public:
    /**
     * @note
     *   Motor(___ , ___ , ___, ___,  bool cw);  <br>
     *
     * @param
     *   bool cw: The _direction corresponds
     *   to the forward rotation of your machine.
     */
    Motor(TIM_HandleTypeDef &htim_x, uint32_t channel, GPIO_TypeDef *phase_x,
          uint16_t phase_pin, bool cw, MotorParams params);

    /**
     * @fn Start motor.
     */
    void start() const;

    /**
     * @fn Stop motor.
     */
    void stop() const;

    /**
     * @fn Specifies the PWM of the motor.
     *
     * @details
     *   {0.0 < duty_ratio} is PWM of clock wise  <br>
     *   {duty_ratio < 0.0} is PWM of counter clock wise
     *
     * @param duty_ratio {-1.0 ≦ duty_ratio ≦ 1.0}
     */
    void update(float32_t duty_ratio);

    float32_t getResistance();

    float32_t getKt();

    float32_t getKe();

   private:
    GPIO_TypeDef *_phase_x;
    const uint16_t _phase_pin;
    TIM_HandleTypeDef &_htim_x;
    const uint64_t _channel;
    const GPIO_PinState _forward_wise;
    MotorParams _params;
};

}  // namespace mslh

#endif  // MSLH_MOTOR_H
