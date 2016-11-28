#include "PostTreatment/Sobel.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include <sstream>


Sobel::Sobel ()
{
    std::string tab("    ");
    std::stringstream fragment;
    fragment << "#version 130" << std::endl << std::endl << std::endl
             << "uniform sampler2D inputTexture;" << std::endl
             << "uniform vec2 inputSize;" << std::endl
             << "uniform vec2 dstSize;" << std::endl << std::endl
             << "uniform float threshold=5.0;" << std::endl << std::endl
             << "out vec4 fragColor;" << std::endl << std::endl << std::endl
             << "void main()" << std::endl
             << "{" << std::endl
             << tab << "vec2 pixelSize = 1 / inputSize;" << std::endl
             << tab << "vec2 texCoord = gl_FragCoord.xy / dstSize;" << std::endl
             << tab << "vec4 gradX = 1*texture(inputTexture, texCoord + pixelSize) + " << std::endl
             << tab << "             2*texture(inputTexture, texCoord + vec2(pixelSize.x, 0)) + " << std::endl
             << tab << "             1*texture(inputTexture, texCoord + vec2(pixelSize.x, -pixelSize.y)) - " << std::endl
             << tab << "             1*texture(inputTexture, texCoord + vec2(-pixelSize.x, pixelSize.y)) - " << std::endl
             << tab << "             2*texture(inputTexture, texCoord + vec2(-pixelSize.x, 0)) - " << std::endl
             << tab << "             1*texture(inputTexture, texCoord + vec2(-pixelSize.x, -pixelSize.y));" << std::endl << std::endl
             << tab << "vec4 gradY = 1*texture(inputTexture, texCoord + vec2(pixelSize.x,-pixelSize.y)) + " << std::endl
             << tab << "             2*texture(inputTexture, texCoord + vec2(0, -pixelSize.y)) + " << std::endl
             << tab << "             1*texture(inputTexture, texCoord + vec2(-pixelSize.x, -pixelSize.y)) - " << std::endl
             << tab << "             1*texture(inputTexture, texCoord + pixelSize) - " << std::endl
             << tab << "             2*texture(inputTexture, texCoord + vec2(0, pixelSize.y)) - " << std::endl
             << tab << "             1*texture(inputTexture, texCoord + vec2(-pixelSize.x, pixelSize.y));" << std::endl << std::endl
             << tab << "float grad = length(dot(gradX,gradX) + dot(gradY,gradY));" << std::endl
             << tab << "fragColor = step(threshold,grad) * vec4(1);//vec4(grad, grad, grad, 1);" << std::endl
             << "}";

   std::string fragmentShader = fragment.str();

    /* Load shader */
    if (!_shader.loadFromMemory(fragmentShader, sf::Shader::Fragment))
        throw std::runtime_error("unable to load directional Sobel fragment shader\n" + fragmentShader);

    _renderStates.shader = &_shader;
    _renderStates.blendMode = sf::BlendNone;
}

void Sobel::applyTreatment (sf::Texture const& inputTexture,
                            sf::RenderTarget& target)
{
    sf::Vector2f texSize(inputTexture.getSize().x, inputTexture.getSize().y);
    _shader.setParameter("inputTexture", inputTexture);
    _shader.setParameter("inputSize", texSize);
    _shader.setParameter("dstSize", sf::Vector2f(target.getSize().x, target.getSize().y));
    sf::RectangleShape square(sf::Vector2f(target.getSize().x, target.getSize().y));
    target.draw (square, _renderStates);
}
