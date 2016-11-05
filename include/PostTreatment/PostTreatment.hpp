#ifndef POSTTREATMENT_HPP_INCLUDED
#define POSTTREATMENT_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


/* Abstract class of a post-treatment:
 * takes an input texture, processes it and writes the result on
 * a sf::RenderTarget (sf::RenderTexture or sf::RenderWindow) */
class PostTreatment
{
    public:
        virtual ~PostTreatment() {};

        virtual void applyTreatment (sf::Texture const& inputTexture,
                                     sf::RenderTarget& target) = 0;
};

#endif // POSTTREATMENT_HPP_INCLUDED
