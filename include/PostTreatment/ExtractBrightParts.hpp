#ifndef EXTRACTBRIGHTPARTS_HPP_INCLUDED
#define EXTRACTBRIGHTPARTS_HPP_INCLUDED

#include "PostTreatment/PostTreatment.hpp"

#include <SFML/Graphics/Shader.hpp>


/* Extracts bright parts of an image.
 * Only the perceived brightness is took into account:
 * perceived brightness = 0.299*red + 0.587*green + 0.114*blue.
 *
 * The threshold value, bewteen 0 and 1, tells which parts are kept.
 *
 * Sets dark parts to pure black.
 */
class ExtractBrightParts: public PostTreatment
{
    public:
        ExtractBrightParts (float threshold);
        virtual ~ExtractBrightParts() {};

        float getThreshold () const;
        void setThreshold (float threshold);

        virtual void applyTreatment (sf::Texture const& inputTexture,
                                     sf::RenderTarget& target);

    private:
        float _threshold;
        sf::Shader _shader;

        sf::RenderStates _renderStates;
};

#endif // EXTRACTBRIGHTPARTS_HPP_INCLUDED
