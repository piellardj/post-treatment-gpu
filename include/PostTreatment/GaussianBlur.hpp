#ifndef GAUSSIANBLUR_HPP_INCLUDED
#define GAUSSIANBLUR_HPP_INCLUDED

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "PostTreatment/PostTreatment.hpp"
#include "PostTreatment/DirectionalBlur.hpp"

/* Gaussian blur effect.
 * It is implemented as two orthogonal directional blurs.
 *
 * For better performance, the texture can be downscaled,
 * blurred and then upscaled. Typically, shrinking the blurred
 * texture by x2 enables x4 performance for a visually
 * equivalent result.
 *
 */
class GaussianBlur: public PostTreatment
{
    public:
        /* Constructor.
         * textureSize: the size of the texture to be blurred.
         * kernelSize: smei-width of the gaussian blur. 1 = no blur.
         * sizeFactor: downscaling factor.
         */
        GaussianBlur (sf::Vector2u textureSize,
                      unsigned int kernelSize,
                      unsigned int sizeFactor=1);

        virtual void applyTreatment (sf::Texture const& inputTexture,
                                     sf::RenderTarget& target);

    private:
        DirectionalBlur _directionalBlur;
        unsigned int _sizeFactor;

        sf::RenderTexture _scalingBuffer; //used for downscaling/upscaling
        sf::RenderTexture _blurBuffer;
};

#endif // GAUSSIANBLUR_HPP_INCLUDED
