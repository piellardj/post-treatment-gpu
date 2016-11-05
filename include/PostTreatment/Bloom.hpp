#ifndef BLOOM_HPP_INCLUDED
#define BLOOM_HPP_INCLUDED

#include "PostTreatment/PostTreatment.hpp"
#include "PostTreatment/GaussianBlur.hpp"
#include "PostTreatment/ExtractBrightParts.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Shader.hpp>

/* Bloom post-treatment.
 * Makes bright parts of the image shinier.
 *
 * The treatment works in 3 steps:
 * - extraction of the bright parts of the image
 * - blurring of the resulting image
 * - adding of the blurred bright parts to the input image.
 */
class Bloom: public PostTreatment
{
    public:
        /* Constructor.
         * textureSize: dimensions of the texture to be processed.
         * threshold: which bright parts to extract.
         * force: force of the effect
         */
         Bloom (sf::Vector2u textureSize,
              float threshold=0.8f,
              float force=1.f);

        float getThreshold() const;
        void setThreshold(float threshold);

        float getForce() const;
        void setForce(float force);

        virtual void applyTreatment (sf::Texture const& inputTexture,
                                     sf::RenderTarget& target);
    private:
        float _threshold;
        float _force;

        GaussianBlur _blur;
        ExtractBrightParts _brightPartsExtractor;

        sf::RenderTexture _lightParts;
        sf::RenderTexture _blurred;

        sf::Shader _shader;
        sf::RenderStates _renderStates;
};

#endif // BLOOM_HPP_INCLUDED
