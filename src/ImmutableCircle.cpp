//
// Created by gomkyung2 on 2023/04/21.
//

#include <ImmutableCircle.hpp>

#include <numbers>

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <range/v3/view/linear_distribute.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/range/conversion.hpp>

ImmutableCircle::ImmutableCircle(float radius, const sf::Color &outline_color) {
    static constexpr float PI = std::numbers::pi_v<float>;

    using namespace ranges::views;
    auto vertices = linear_distribute(0.f, 2.f * PI, NUM_POINTS)
                  | transform([&](float angle_radian){
                        auto position = radius * sf::Vector2f::UnitX.rotatedBy(sf::radians(angle_radian));
                        return sf::Vertex { position, outline_color };
                  })
                  | ranges::to_vector;

    if (!buffer.create(NUM_POINTS)){
        throw std::runtime_error { "Failed to create vertex buffer object." };
    }
    if (!buffer.update(vertices.data())){
        throw std::runtime_error { "Failed to update vertex buffer object." };
    }
}

void ImmutableCircle::draw(sf::RenderTarget &target, const sf::RenderStates &states) const {
    auto transformed_state = states;
    transformed_state.transform *= getTransform();

    target.draw(buffer, transformed_state);
}
