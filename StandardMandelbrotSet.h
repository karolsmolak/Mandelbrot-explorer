#ifndef MANDELBROT_MANDELBROTSET_H
#define MANDELBROT_MANDELBROTSET_H

#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <complex>
#include <boost/multi_array.hpp>
#include "MandelbrotSet.h"

class StandardMandelbrotSet : public MandelbrotSet {
public:
    void generate() override;
    void generateAfterDragging(int dx, int dy) override;

    StandardMandelbrotSet(int windowSize, double viewSize, std::pair<double, double> leftUpPointCoordinates, int numOfThreads, int maxIteration):
            MandelbrotSet(windowSize, viewSize, leftUpPointCoordinates, numOfThreads), maxIteration(maxIteration){
        escapeIteration.resize(boost::extents[windowSize][windowSize]);
        generate();
    }

protected:
    boost::multi_array<int, 2> escapeIteration;
    int maxIteration;

    int calculateEscapeIteration(std::complex<double> p);
    friend void process(StandardMandelbrotSet *);
    sf::Color getPixelColor(int x, int y) const override;

};
#endif //MANDELBROT_MANDELBROTSET_H
