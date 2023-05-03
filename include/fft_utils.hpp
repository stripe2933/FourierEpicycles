//
// Created by gomkyung2 on 2023/04/25.
//

#ifndef FOURIEREPICYCLES_FFT_UTILS_HPP
#define FOURIEREPICYCLES_FFT_UTILS_HPP

#include <vector>
#include <complex>
#include <span>

std::vector<std::complex<double>> dft(std::span<std::complex<double>> source);

#endif //FOURIEREPICYCLES_FFT_UTILS_HPP
