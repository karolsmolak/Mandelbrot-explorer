#ifndef MANDELBROT_BUDDHABROT_H
#define MANDELBROT_BUDDHABROT_H

#include <SFML/Graphics.hpp>
#include <mutex>
#include "MandelbrotSet.h"

class Buddhabrot : public MandelbrotSet {
    const int numberOfRandomPointsPerThread = 1000000;

    int numOfPointsPassingThrough[MAX_WINDOW_SIZE][MAX_WINDOW_SIZE];
    int maximumNumOfPointsPassingThrough;
    int minimumNumOfPointsPassingThrough;

    void processPoint(std::complex<double> complex);
    sf::Color getPixelColor(int x, int y) const override;

    friend void process(Buddhabrot *);
    void generate() override;
    void generateAfterDragging(int dx, int dy) override;
public:

    Buddhabrot(int windowSize, double viewSize, std::pair<double, double> leftUpPointCoordinates):
            MandelbrotSet(windowSize, viewSize, leftUpPointCoordinates){
        generate();
    }

};


#endif //MANDELBROT_BUDDHABROT_H
