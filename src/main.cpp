#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

#include <GL/glew.h>

#include "PostTreatment/DirectionalBlur.hpp"
#include "PostTreatment/GaussianBlur.hpp"
#include "PostTreatment/Bloom.hpp"
#include "PostTreatment/ExtractBrightParts.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(512, 512), "Particles", sf::Style::Default);
    std::cout << "openGL version:" << window.getSettings().majorVersion << "." << window.getSettings().minorVersion << std::endl << std::endl;
    if (window.getSettings().majorVersion < 3) {
        std::cerr << "This program requires OpenGL 3+." << std::endl << std::endl;
        return EXIT_FAILURE;
    }
    //window.setVerticalSyncEnabled(true);

    DirectionalBlur dirBlur(sf::Vector2f(0,1), 16);

    GaussianBlur blur(window.getSize(), 4, 4);

    Bloom bloom (window.getSize(), 0.6, 0.5f);

    ExtractBrightParts brightPartsExtractor(0.6f);


    /* Loading of the image */
    sf::Texture inputTexture;
    if (!inputTexture.loadFromFile("rc/blur/original.png"))
        throw std::runtime_error("unable to open pic");

    float total = 0.f;
    int loops = 0;
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                break;
                default:
                    break;
            }
        }

        window.clear(sf::Color::Green);
        //dirBlur.applyTreatment(inputTexture, window);
        //blur.applyTreatment(inputTexture, window);
        //brightPartsExtractor.applyTreatment(inputTexture, window);
        bloom.applyTreatment(inputTexture, window);
        window.display();

       // std::cout << 1.f / clock.getElapsedTime().asSeconds() << std::endl;
        ++loops;
    }

    std::cout << "average fps: " << static_cast<float>(loops) / clock.getElapsedTime().asSeconds() << std::endl;

    return EXIT_SUCCESS;
}
