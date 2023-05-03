//
// Created by gomkyung2 on 2023/04/23.
//

#ifndef FOURIEREPICYCLES_RINGBUFFERPOLYLINE_HPP
#define FOURIEREPICYCLES_RINGBUFFERPOLYLINE_HPP

#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>

class RingBufferPolyline : public sf::Drawable, public sf::Transformable{
public:
    RingBufferPolyline(std::size_t capacity, sf::Color line_color);

    std::size_t getCapacity() const noexcept { return vertices.size(); }

    void reset();
    void push(sf::Vector2f point);

protected:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

private:
    std::vector<sf::Vertex> vertices;
    std::size_t current_index = 0;
    bool is_first_cycle = true;
};

#endif //FOURIEREPICYCLES_RINGBUFFERPOLYLINE_HPP
