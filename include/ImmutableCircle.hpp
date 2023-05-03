//
// Created by gomkyung2 on 2023/04/21.
//

#ifndef FOURIEREPICYCLES_IMMUTABLECIRCLE_HPP
#define FOURIEREPICYCLES_IMMUTABLECIRCLE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>

class ImmutableCircle : public sf::Drawable, public sf::Transformable {
public:
    explicit ImmutableCircle(float radius, const sf::Color &outline_color);

protected:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

private:
    static constexpr std::size_t NUM_POINTS = 64;

    sf::VertexBuffer buffer { sf::PrimitiveType::LineStrip, sf::VertexBuffer::Usage::Static };
};

#endif //FOURIEREPICYCLES_IMMUTABLECIRCLE_HPP
