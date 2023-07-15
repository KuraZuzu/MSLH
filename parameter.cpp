//
// Created by zuzukun on 22/10/24.
//
#include "parameter.h"

float32_t mslh::machine_parameter::convertDistanceLF(u_int16_t sensor_value) {
    constexpr float32_t a = -0.103481402;
    constexpr float32_t b = 353.764886;

    return (a * static_cast<float32_t>(sensor_value)) + b;
}

float32_t mslh::machine_parameter::convertDistanceLS(u_int16_t sensor_value) {
    constexpr float32_t a = -0.068778907;
    constexpr float32_t b = 233.085245;

    return (a * static_cast<float32_t>(sensor_value)) + b;
}

float32_t mslh::machine_parameter::convertDistanceRS(u_int16_t sensor_value) {
    constexpr float32_t a = -0.07387189436;
    constexpr float32_t b = 237.6686216;

    return (a * static_cast<float32_t>(sensor_value)) + b;
}

float32_t mslh::machine_parameter::convertDistanceRF(u_int16_t sensor_value) {
    //故障
    return sensor_value;
}
