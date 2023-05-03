//
// Created by gomkyung2 on 2023/04/24.
//

#include <App.hpp>

#include <fstream>
#include <list>
#include <numbers>

#include <imgui.h>
#include <imgui-SFML.h>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/partial_sum.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/action/sort.hpp>
#include <nfd.h>

#include <fft_utils.hpp>

App::App() : sf::RenderWindow { sf::VideoMode { { 640, 640 } }, "Fourier Epicycles" },
             output_signal { NUM_OUTPUT_VERTEX, sf::Color::Blue }{
    setVerticalSyncEnabled(true);

    // Initialize ImGui.
    bool success = ImGui::SFML::Init(*this);
    if (!success){
        throw std::runtime_error { "Failed to init ImGui context." };
    }

    // Normalize the viewport to [-10, 10] x [-10, 10].
    view.setViewport({ { 0, 0 } , { 1, 1 } });
}

void App::run() {
    sf::Clock clock;
    std::list<float> fps_meter;
    float average_time_step = 0.f;

    // Parameters.
    while (isOpen()){
        // Process window events.
        for (sf::Event event {}; pollEvent(event);){
            ImGui::SFML::ProcessEvent(*this, event);

            switch (event.type){
                case sf::Event::Closed:
                    close();
                    break;

                case sf::Event::MouseWheelScrolled:{
                    auto center = mapPixelToCoords({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, view);
                    auto factor = std::pow(1.01f, event.mouseWheelScroll.delta);

                    view.zoom(factor);
                    view.move((1.f - factor) * center);
                }

                default:
                    break;
            }
        }

        auto elapsed = clock.restart();

        // Calculate fps.
        {
            auto elapsed_seconds = elapsed.asSeconds();
            fps_meter.push_back(elapsed_seconds);
            if (fps_meter.size() > FPS_METER_CAPACITY) {
                average_time_step += (elapsed_seconds - fps_meter.front()) / FPS_METER_CAPACITY;
                fps_meter.pop_front();
            } else {
                average_time_step += (elapsed_seconds - average_time_step) / static_cast<float>(fps_meter.size());
            }
            average_fps = 1.f / average_time_step;
        }

        if (!coefficients.empty()){
            updateEpicycles(elapsed);
        }
        updateImGui(elapsed);

        clear();
        drawEpicycles();
        ImGui::SFML::Render(*this);
        display();
    }

    ImGui::SFML::Shutdown();
}

std::vector<std::complex<double>> App::readSignal(std::istream &&stream) {
    std::vector<std::complex<double>> result;

    std::string line;
    std::complex<double> parsed;
    while (std::getline(stream, line)){
        std::istringstream iss { line };
        iss >> parsed;

        result.push_back(parsed);
    }

    return result;
}

void App::updateEpicycles(sf::Time elapsed) {
    static float total_elapsed_time = 0.f;

    auto elapsed_time = drawing_parameters.speed * elapsed.asSeconds();
    total_elapsed_time += elapsed_time;

    // Adjust each epicycles' center position using cumulative sum.
    using namespace ranges::views;
    auto circle_centers = coefficients
                          | take(drawing_parameters.num_epicycles)
                          | transform([=](const FourierCoefficient &cycle) {
                                return std::polar(cycle.radius, cycle.angular_velocity * total_elapsed_time + cycle.phase);
                            })
                          | partial_sum
                          | transform([](auto z) { return sf::Vector2f { z.real(), z.imag() }; });

    for (auto [i, center] : circle_centers | enumerate){
        epicycles[i + 1].setPosition(center);
        polyline[i + 1].position = center;
    }

    output_signal.push(polyline[drawing_parameters.num_epicycles].position);
}

std::vector<FourierCoefficient> App::calculateFourierCoefficients(std::span<std::complex<double>> signal){
    static constexpr float PI = std::numbers::pi_v<float>;

    auto dft_result = dft(signal);
    int n = static_cast<int>(signal.size());

    // Resize and re-calculate the coefficients.
    using namespace ranges::views;
    auto coefficients = zip(iota(-(n / 2)), dft_result)
         | transform([](auto &&kx){
             auto [k, x] = kx;

             auto r = static_cast<float>(std::abs(x));
             auto w = 2.f * PI * static_cast<float>(k);
             auto phi = static_cast<float>(std::arg(x));

             return FourierCoefficient { r, w, phi };
           })
         | ranges::to_vector
         | ranges::actions::sort(std::greater<>{}, &FourierCoefficient::radius);

    return coefficients;
}

void App::updateImGui(sf::Time elapsed){
    ImGui::SFML::Update(*this, elapsed);

    ImGui::Begin("Inspector");

    {
        ImGui::SeparatorText("Rendering info");
        ImGui::Text("FPS: %d", static_cast<int>(average_fps));
        auto [width, height] = view.getSize();
        auto [left, top] = view.getCenter() - 0.5f * view.getSize();
        ImGui::Text("Region: [%.1f, %.1f]x[%.1f, %.1f]", left, top, width, height);
    }

    {
        ImGui::SeparatorText("Input signal");
        if (ImGui::Button("Load...")) {
            NFD_Init();

            nfdchar_t *filename = nullptr;
            nfdresult_t result = NFD_OpenDialog(&filename, nullptr, 0, nullptr);
            if (result == NFD_OKAY) {
                setupSignal(std::ifstream { filename });
                delete filename;
            } else if (result == NFD_CANCEL) {
                // Do nothing.
            } else {
                throw std::runtime_error { std::string { NFD_GetError() }};
            }
        }
        if (!coefficients.empty()) {
            ImGui::Text("# of signal points: %zu", coefficients.size());
        }
    }

    if (!coefficients.empty()) {
        ImGui::SeparatorText("Epicycles manipulation");

        if (ImGui::SliderInt("# of Epicycles", &drawing_parameters.num_epicycles, 1, static_cast<int>(coefficients.size()))) {
            output_signal.reset();
        }
        ImGui::SliderFloat("Speed", &drawing_parameters.speed, 1e-2f, 0.5f);

        ImGui::Checkbox("Epicycles", &visibility_parameters.epicycles);
        ImGui::Checkbox("Polyline", &visibility_parameters.polyline);
        ImGui::Checkbox("Output", &visibility_parameters.output_signal);
    }

    ImGui::End();
}

void App::drawEpicycles() {
    if (coefficients.empty()){
        // No things to draw.
        return;
    }

    setView(view);
    using ranges::views::take;

    if (visibility_parameters.epicycles){
        for (const auto &cycle : epicycles | take(drawing_parameters.num_epicycles)){
            draw(cycle);
        }
    }
    if (visibility_parameters.polyline){
        draw(polyline.data(), drawing_parameters.num_epicycles + 1, sf::PrimitiveType::LineStrip);
    }
    if (visibility_parameters.output_signal){
        draw(output_signal);
    }

    setView(getDefaultView());
}

void App::setupSignal(std::istream &&data) {
    auto signal = readSignal(std::move(data));
    coefficients = calculateFourierCoefficients(signal);

    // Initialize drawable epicycles with corresponding epicycles.
    using namespace ranges::views;
    drawing_parameters.num_epicycles = static_cast<int>(signal.size());
    epicycles = coefficients
                | take(drawing_parameters.num_epicycles)
                | transform([](const auto &coeff) { return ImmutableCircle { coeff.radius, sf::Color::White }; })
                | ranges::to_vector;
    polyline = std::vector(drawing_parameters.num_epicycles + 1, sf::Vertex {{}, sf::Color::Red });
    output_signal.reset();
}