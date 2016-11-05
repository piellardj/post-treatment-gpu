#include "PostTreatment/Bloom.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cmath>
#include <stdexcept>
#include <sstream>

/* sizeFactor: defines if the image should be shrinked, blurred, then upscaled */
Bloom::Bloom (sf::Vector2u bufferSize, float threshold, float force):
            _threshold (threshold),
            _force (force),
            _blur (bufferSize, 10, 2),
            _brightPartsExtractor(_threshold)
{
    if (!_lightParts.create (bufferSize.x, bufferSize.y)) {
        throw std::runtime_error("Error: Bloom, buffer creation failed");
    }
    if (!_blurred.create (bufferSize.x, bufferSize.y)) {
        throw std::runtime_error("Error: Bloom, buffer creation failed");
    }

    std::string tab("    ");
    std::stringstream fragment;
    fragment << "#version 130" << std::endl << std::endl << std::endl
             << "uniform sampler2D clearTexture;" << std::endl
             << "uniform sampler2D blurredLightsTexture;" << std::endl
             << "uniform vec2 inputSize;" << std::endl
             << "uniform float force;" << std::endl << std::endl
             << "out vec4 fragColor;" << std::endl << std::endl << std::endl
             << "void main()" << std::endl
             << "{" << std::endl
             << tab << "vec2 texCoord = gl_FragCoord.xy / inputSize;" << std::endl
             << tab << "vec4 clear = texture(clearTexture, texCoord);" << std::endl << std::endl
             << tab << "vec4 blurred = texture(blurredLightsTexture, texCoord);" << std::endl
             << tab << "fragColor = clear + force * blurred;" << std::endl
             << "}";
    std::string fragmentShader = fragment.str();

    /* Load shader */
    if (!_shader.loadFromMemory(fragmentShader, sf::Shader::Fragment))
        throw std::runtime_error("unable to load bloom fragment shader\n" + fragmentShader);

    _renderStates.shader = &_shader;
    _renderStates.blendMode = sf::BlendNone;
}

float Bloom::getThreshold() const
{
    return _threshold;
}
void Bloom::setThreshold(float threshold)
{
    _threshold = threshold;
    _brightPartsExtractor.setThreshold(threshold);
}

float Bloom::getForce() const
{
    return _force;
}
void Bloom::setForce(float force)
{
    _force = force;
}

/* Returns true if successfull, false otherwise. */
void Bloom::applyTreatment (sf::Texture const& inputTexture,
                            sf::RenderTarget& target)
{
    /* First we extract the bright parts */
    _brightPartsExtractor.applyTreatment(inputTexture, _lightParts);
    _lightParts.display();

    /* Then blur then */
    _blur.applyTreatment(_lightParts.getTexture(), _blurred);
    _blurred.display();

    /* Finally we render the input texture + the blurred bright parts */
    sf::Vector2f texSize(target.getSize().x, target.getSize().y);
    _shader.setParameter("clearTexture", inputTexture);
    _shader.setParameter("blurredLightsTexture", _blurred.getTexture());
    _shader.setParameter("inputSize", texSize);
    _shader.setParameter("force", _force);
    sf::RectangleShape square(texSize);
    target.draw (square, _renderStates);
}
