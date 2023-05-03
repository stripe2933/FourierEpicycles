//
// Created by gomkyung2 on 2023/04/24.
//

#ifndef FOURIEREPICYCLES_APP_HPP
#define FOURIEREPICYCLES_APP_HPP

#include <span>

#include <SFML/Graphics.hpp>

#include <FourierCoefficient.hpp>
#include <ImmutableCircle.hpp>
#include <RingBufferPolyline.hpp>

class App : sf::RenderWindow {
public:
    App();

    void run();

private:
    static constexpr std::size_t NUM_OUTPUT_VERTEX = 1000;
    static constexpr std::size_t FPS_METER_CAPACITY = 200;

    sf::View view { { 0, 0 }, { 20, 20 } };

    // Manipulative parameters.
    struct {
        float speed = 0.1f;
        int num_epicycles = 0;
    } drawing_parameters;
    struct {
        bool epicycles = true;
        bool polyline = true;
        bool output_signal = true;
    } visibility_parameters;

    // Calculated parameters.
    std::vector<FourierCoefficient> coefficients;
    float average_fps = 0.f;

    std::vector<ImmutableCircle> epicycles;
    std::vector<sf::Vertex> polyline;
    RingBufferPolyline output_signal;

    static std::vector<std::complex<double>> readSignal(std::istream &&stream);
    static std::vector<FourierCoefficient> calculateFourierCoefficients(std::span<std::complex<double>> signal);

    void setupSignal(std::istream &&data);
    void updateImGui(sf::Time elapsed);
    void updateEpicycles(sf::Time elapsed);

    void drawEpicycles();
};

#endif //FOURIEREPICYCLES_APP_HPP
