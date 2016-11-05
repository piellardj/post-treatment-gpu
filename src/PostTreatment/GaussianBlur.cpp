#include "PostTreatment/GaussianBlur.hpp"

#include <cmath>
#include <algorithm>
#include <stdexcept>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderStates.hpp>


GaussianBlur::GaussianBlur (sf::Vector2u textureSize, unsigned int kernelSize, unsigned int sizeFactor):
            _directionalBlur(sf::Vector2f(1.f,0.f), kernelSize),
            _sizeFactor(sizeFactor)
{
    _sizeFactor = std::max(_sizeFactor, 1u);

    sf::Vector2i bufferSize (textureSize.x/_sizeFactor, textureSize.y/_sizeFactor);

    if (_sizeFactor > 1) {
        if (!_scalingBuffer.create (bufferSize.x, bufferSize.y)) {
            throw std::runtime_error("Error: GaussianBlur, buffer creation failed");
        }
        _scalingBuffer.setSmooth(true);
    }
    if (!_blurBuffer.create (bufferSize.x, bufferSize.y)) {
        throw std::runtime_error("Error: GaussianBlur, buffer creation failed");
    }
}


void GaussianBlur::applyTreatment (sf::Texture const& inputTexture,
                                   sf::RenderTarget& target)
{
    if (_sizeFactor == 1) {
        _directionalBlur.setDirection(sf::Vector2f(0.f,1.f));
        _directionalBlur.applyTreatment(inputTexture, _blurBuffer);
        _blurBuffer.display();

        _directionalBlur.setDirection(sf::Vector2f(1.f,0.f));
        _directionalBlur.applyTreatment(_blurBuffer.getTexture(), target);
    } else {
        float downcaling = 1.f /  static_cast<float>(_sizeFactor);
        float upscaling = _sizeFactor;

        /* First shrink the source so that we work on less pixels with linear interpolation */
        sf::Sprite sprite (inputTexture);
        sprite.setScale(downcaling, downcaling);
        _scalingBuffer.draw (sprite, sf::RenderStates(sf::BlendNone));
        _scalingBuffer.display();

        /* Processes the blur on the small texture */
        _directionalBlur.setDirection(sf::Vector2f(0.f,1.f));
        _directionalBlur.applyTreatment(_scalingBuffer.getTexture(), _blurBuffer);
        _blurBuffer.display();

        _directionalBlur.setDirection(sf::Vector2f(1.f,0.f));
        _directionalBlur.applyTreatment(_blurBuffer.getTexture(), _scalingBuffer);
        _scalingBuffer.display();

        /* upscales the blurred texture */
        sprite.setTexture (_scalingBuffer.getTexture());
        sprite.setScale (upscaling, upscaling);
        target.draw (sprite, sf::RenderStates(sf::BlendNone));
    }
}
