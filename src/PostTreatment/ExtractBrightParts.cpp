#include "PostTreatment/ExtractBrightParts.hpp"

#include <stdexcept>
#include <sstream>

#include <SFML/Graphics/RectangleShape.hpp>

ExtractBrightParts::ExtractBrightParts (float threshold):
            _threshold (std::min(1.f, std::max(threshold, 0.f)))
{
    std::string tab("    ");
    std::stringstream fragment;
    fragment << "#version 130" << std::endl << std::endl << std::endl
             << "uniform sampler2D inputTexture;" << std::endl
             << "uniform vec2 inputSize;" << std::endl
             << "uniform float threshold;" << std::endl << std::endl
             << "out vec4 fragColor;" << std::endl << std::endl << std::endl
             << "void main()" << std::endl
             << "{" << std::endl
             << tab << "vec3 color = texture(inputTexture, gl_FragCoord.xy / inputSize).rgb;" << std::endl
             << tab << "float perceivedBrightness = dot(vec3(0.299, 0.587, 0.114), color.rgb);" << std::endl
             << tab << "fragColor.rgb = color * step(threshold, perceivedBrightness);" << std::endl
             << tab << "fragColor.a = 1.0;" << std::endl
             << "}";
    std::string fragmentShader = fragment.str();

    /* Load shader */
    if (!_shader.loadFromMemory(fragmentShader, sf::Shader::Fragment))
        throw std::runtime_error("unable to load ExtractBrightParts fragment shader\n" + fragmentShader);

    _renderStates.shader = &_shader;
    _renderStates.blendMode = sf::BlendNone;
}

float ExtractBrightParts::getThreshold () const
{
    return _threshold;
}

void ExtractBrightParts::setThreshold (float threshold)
{
    _threshold = std::min(1.f, std::max(threshold, 0.f));
}

void ExtractBrightParts::applyTreatment (sf::Texture const& inputTexture,
                                         sf::RenderTarget& target)
{
    sf::Vector2f texSize(target.getSize().x, target.getSize().y);
    _shader.setParameter("inputTexture", inputTexture);
    _shader.setParameter("inputSize", texSize);
    _shader.setParameter("threshold", _threshold);

    sf::RectangleShape square(texSize);
    target.draw (square, _renderStates);
}
