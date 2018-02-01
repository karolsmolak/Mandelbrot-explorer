#include <SFML/Graphics.hpp>
#include <boost/log/trivial.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "StandardMandelbrotSet.h"
#include "Buddhabrot.h"

int windowSize;
double step;
double scaleFactor;

void readConfig();
std::shared_ptr<MandelbrotSet> getMandelbrotSet(std::string_view type);

int main() {
    readConfig();

    BOOST_LOG_TRIVIAL(info) << "Starting the application...";

    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Mandelbrot set");

    std::shared_ptr<MandelbrotSet> mandelbrotSet = getMandelbrotSet("buddhabrot");

    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                BOOST_LOG_TRIVIAL(info) << "Exiting...";
                window.close();
            } else if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::S){
                    mandelbrotSet->saveToFile();
                }
            }
        }

        double scale = 1;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
            scale *= 1 - scaleFactor;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
            scale *= 1 + scaleFactor;
        }

        int dx = 0, dy = 0;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            dy += step;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            dy -= step;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            dx -= step;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            dx += step;
        }

        if(scale != 1.0){
            mandelbrotSet->scale(scale);
        }

        if(dx != 0 || dy != 0){
            mandelbrotSet->drag(dx, dy);
        }

        mandelbrotSet->draw(window, sf::RenderStates::Default);

        window.display();
    }

}

boost::property_tree::ptree pt;
double viewSize;
double x;
double y;
int numberOfThreads;

void readConfig(){
    BOOST_LOG_TRIVIAL(info) << "Reading config.ini...";
    boost::property_tree::ini_parser::read_ini("../config.ini", pt);

    windowSize = pt.get<int>("general.windowSize");
    viewSize = pt.get<double>("general.viewSize");
    x = pt.get<double>("general.x");
    y = pt.get<double>("general.y");
    numberOfThreads = pt.get<int>("general.numberOfThreads");
    step = pt.get<double>("general.step");
    scaleFactor = pt.get<double>("general.scaleFactor");
}


std::shared_ptr<MandelbrotSet> getMandelbrotSet(std::string_view type){
    BOOST_LOG_TRIVIAL(info) << "Requested mandelbrot set of type: " << type;

    if(type == "standard"){
        return std::make_shared<StandardMandelbrotSet>(windowSize, viewSize,
                                                       std::pair<double, double>(x, y),
                                                       numberOfThreads,
                                                       pt.get<int>("standard.maxIteration"));
    } else if(type == "buddhabrot"){
        return std::make_shared<Buddhabrot>(windowSize, viewSize,
                                            std::pair<double, double>(x, y),
                                            numberOfThreads,
                                            pt.get<int>("buddhabrot.maxIteration"),
                                            pt.get<int>("buddhabrot.randomSamplesPerThread"));
    }

    return nullptr;
}
