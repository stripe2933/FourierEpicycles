//
// Created by gomkyung2 on 2023/04/25.
//

#include <fft_utils.hpp>

#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/rotate.hpp>
#include <fftw3.h>

std::vector<std::complex<double>> dft(std::span<std::complex<double>> source) {
    std::vector<std::complex<double>> out(source.size());
    auto plan = fftw_plan_dft_1d(static_cast<int>(source.size()),
                                 reinterpret_cast<fftw_complex*>(source.data()),
                                 reinterpret_cast<fftw_complex*>(out.data()),
                                 FFTW_FORWARD,
                                 FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    // normalize
    ranges::for_each(out, [n = source.size()](auto &z) { z /= n; });

    // shift (equivalent to np.fft.fftshift)
    ranges::rotate(out, std::next(out.begin(), static_cast<int>(out.size() + 1) / 2));

    return out;
}
