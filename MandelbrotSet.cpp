#include <SFML/Graphics.hpp>
#include <boost/log/trivial.hpp>
#include "MandelbrotSet.h"

void MandelbrotSet::saveToFile() {
    sf::Image img;
    img.create(windowSize, windowSize);

    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            img.setPixel(x, y, getPixelColor(x, y));
        }
    }

    if(img.saveToFile("screenshot.jpg")){
        BOOST_LOG_TRIVIAL(info) << "saved to screenshot.jpg";
    } else {
        BOOST_LOG_TRIVIAL(error) << "unable to save the file";
    }

}

void MandelbrotSet::scale(double factor) {
    leftUpPointCoordinates.first += viewSize * (1.0 - factor) / 2.0;
    leftUpPointCoordinates.second -= viewSize * (1.0 - factor) / 2.0;
    viewSize *= factor;
    generate();
}

void MandelbrotSet::drag(int dx, int dy){
    leftUpPointCoordinates.first += dx * viewSize / windowSize;
    leftUpPointCoordinates.second += dy * viewSize / windowSize;
    generateAfterDragging(-dx, dy);
}

void MandelbrotSet::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    sf::Image img;
    img.create(windowSize, windowSize);

    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            img.setPixel(x, y, getPixelColor(x, y));
        }
    }

    sf::Texture texture;
    texture.loadFromImage(img);
    sf::Sprite sprite;
    sprite.setTexture(texture);

    target.draw(sprite, states);
}

std::complex<double> MandelbrotSet::getPointFromPixelCoordinates(int x, int y) const{
    double step = viewSize / windowSize;
    return std::complex<double>(x * step + leftUpPointCoordinates.first,
                                -y * step + leftUpPointCoordinates.second);
}

bool MandelbrotSet::isInMainBulbs(std::complex<double> p){
    if(256.0 * pow(norm(p), 2) - 96.0 * norm(p) + 32.0 * p.real() - 3.0 < 0 ||
       16.0 * norm(p + std::complex<double>(1.0, 0)) - 1 < 0){
        return true;
    } else {
        return false;
    }
}

std::pair<int, int> MandelbrotSet::getPixelCoordinatesFromPoint(std::complex<double> p) {
    double pixelSize = viewSize / windowSize;
    return std::pair<int, int>((p.real() - leftUpPointCoordinates.first) / pixelSize, (leftUpPointCoordinates.second - p.imag()) / pixelSize);
}