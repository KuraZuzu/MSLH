//
// Created by zuzukun on 22/10/24.
//
#include "parameter.h"

float32_t mslh::machine_parameter::convertDistanceLF(u_int16_t sensor_value) {
    constexpr float32_t a = 3442.691;
    constexpr float32_t b = -8.21455;

    return a * static_cast<float32_t>(sensor_value) + b;
}

float32_t mslh::machine_parameter::convertDistanceLS(u_int16_t sensor_value) {
    constexpr float32_t a = 3384.727;
    constexpr float32_t b = -14.0955;

    return a * static_cast<float32_t>(sensor_value) + b;
}

float32_t mslh::machine_parameter::convertDistanceRF(u_int16_t sensor_value) {
    constexpr float32_t a = 3177.527;
    constexpr float32_t b = -7.92091;

    return a * static_cast<float32_t>(sensor_value) + b;
}

float32_t mslh::machine_parameter::convertDistanceRS(u_int16_t sensor_value) {
    //故障
    return sensor_value;
}