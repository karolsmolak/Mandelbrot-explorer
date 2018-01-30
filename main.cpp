#include <iostream>
#include <SFML/Graphics.hpp>
#include <complex>
#include <thread>
#include <boost/log/trivial.hpp>
#include "StandardMandelbrotSet.h"
#include "Buddhabrot.h"

using namespace std::complex_literals;

const int WINDOW_SIZE = 1000;

int main() {

    BOOST_LOG_TRIVIAL(info) << "Starting the application...";

    Buddhabrot mandelbrotSet(WINDOW_SIZE, 2.5, std::pair<double, double>(-1.75, 1.25));

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Mandelbrot set");

    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                BOOST_LOG_TRIVIAL(info) << "Exiting...";
                window.close();
            } else if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::S){
                    mandelbrotSet.saveToFile();
                }
            }
        }

        double scale = 1;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
            scale *= 0.90;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
            scale *= 1.10;
        }

        int dx = 0, dy = 0;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            dy += 10;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            dy -= 10;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            dx -= 10;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            dx += 10;
        }

        if(scale < 0.99 || scale > 1.01){
            mandelbrotSet.scale(scale);
        }

        if(dx != 0 || dy != 0){
            mandelbrotSet.drag(dx, dy);
        }

        window.clear(sf::Color::White);
        window.draw(mandelbrotSet);
        window.display();
    }

}