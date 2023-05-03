//
// Created by gomkyung2 on 2023/04/21.
//

#ifndef FOURIEREPICYCLES_FOURIERCOEFFICIENT_HPP
#define FOURIEREPICYCLES_FOURIERCOEFFICIENT_HPP

#include <complex>

struct FourierCoefficient{
    float radius;
    float angular_velocity;
    float phase;

    constexpr FourierCoefficient(float radius, float angular_velocity, float phase) noexcept : radius(radius), angular_velocity(angular_velocity), phase(phase) { }
};

#endif //FOURIEREPICYCLES_FOURIERCOEFFICIENT_HPP
