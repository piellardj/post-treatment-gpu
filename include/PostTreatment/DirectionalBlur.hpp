#ifndef DIRECTIONALBLUR_HPP_INCLUDED
#define DIRECTIONALBLUR_HPP_INCLUDED


#include "PostTreatment/PostTreatment.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Vector2.hpp>


/* Directional gaussian blur.
 * Two parameters:
 * - kernelSize: force of the blur, 1 for no blur, n for a size 2*n blur
 * - direction: step of the blur. If normalized, the blur will be smooth.
 *              otherwise, neightbouring pixels might be "missed", resulting
 *              in a pixelated effect.
 */
class DirectionalBlur: public PostTreatment
{
    public:
        DirectionalBlur (sf::Vector2f direction,
                         unsigned int kernelSize);
        virtual ~DirectionalBlur() {};

        void setDirection (sf::Vector2f const& direction);

        virtual void applyTreatment (sf::Texture const& inputTexture,
                                     sf::RenderTarget& target);

    private:
        sf::Vector2f _direction;
        sf::Shader _shader;

        sf::RenderStates _renderStates;
};

#endif // DIRECTIONALBLUR_HPP_INCLUDED
