//
// Created by gomkyung2 on 2023/04/23.
//

#include <RingBufferPolyline.hpp>

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

RingBufferPolyline::RingBufferPolyline(std::size_t capacity, sf::Color line_color) : vertices(capacity + 1, sf::Vertex { {}, line_color }) {

}

void RingBufferPolyline::push(sf::Vector2f point) {
    if (current_index == vertices.size() - 1){
        current_index = 0;
        is_first_cycle = false;
    }

    vertices[current_index].position = point;
    if (!is_first_cycle && current_index == 0){
        vertices.back().position = point;
    }

    ++current_index;
}

void RingBufferPolyline::draw(sf::RenderTarget &target, const sf::RenderStates &states) const {
    auto current_state = states;
    current_state.transform *= getTransform();

    target.draw(vertices.data(), current_index, sf::PrimitiveType::LineStrip, current_state);
    if (!is_first_cycle){
        target.draw(&vertices[current_index], vertices.size() - current_index, sf::PrimitiveType::LineStrip, current_state);
    }
}

void RingBufferPolyline::reset() {
    current_index = 0;
    is_first_cycle = true;
}
