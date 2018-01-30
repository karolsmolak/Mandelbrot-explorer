#ifndef MANDELBROT_MANDELBROTSET_H
#define MANDELBROT_MANDELBROTSET_H

#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <complex>
#include "MandelbrotSet.h"

class StandardMandelbrotSet : public MandelbrotSet {
    int escapeIteration[MAX_WINDOW_SIZE][MAX_WINDOW_SIZE];

public:
    void generate() override;
    void generateAfterDragging(int dx, int dy) override;

    StandardMandelbrotSet(int windowSize, double viewSize, std::pair<double, double> leftUpPointCoordinates):
            MandelbrotSet(windowSize, viewSize, leftUpPointCoordinates){
        generate();
    }

protected:
    int calculateEscapeIteration(std::complex<double> p);
    friend void process(StandardMandelbrotSet *);
    sf::Color getPixelColor(int x, int y) const override;

};
#endif //MANDELBROT_MANDELBROTSET_H
