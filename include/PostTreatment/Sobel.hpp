#ifndef SOBEL_HPP_INCLUDED
#define SOBEL_HPP_INCLUDED

#include "PostTreatment/PostTreatment.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Vector2.hpp>


/* Sobel filter
 */
class Sobel: public PostTreatment
{
    public:
        Sobel ();
        virtual ~Sobel() {};

        virtual void applyTreatment (sf::Texture const& inputTexture,
                                     sf::RenderTarget& target);

    private:
        sf::Shader _shader;

        sf::RenderStates _renderStates;
};

#endif // SOBEL_HPP_INCLUDED
