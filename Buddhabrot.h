#ifndef MANDELBROT_BUDDHABROT_H
#define MANDELBROT_BUDDHABROT_H

#include <SFML/Graphics.hpp>
#include <boost/multi_array.hpp>
#include "MandelbrotSet.h"

class Buddhabrot : public MandelbrotSet {
public:
    Buddhabrot(int windowSize, double viewSize, std::pair<double, double> leftUpPointCoordinates, int numOfThreads,
               int maxIteration, int randomSamplesPerThread):
            MandelbrotSet(windowSize, viewSize, leftUpPointCoordinates, numOfThreads),
            maxIteration(maxIteration), randomSamplesPerThread(randomSamplesPerThread){

        numOfPointsPassingThrough.resize(boost::extents[windowSize][windowSize]);
        generate();
    }

protected:
    int maxIteration;
    int randomSamplesPerThread;

    boost::multi_array<int, 2> numOfPointsPassingThrough;
    int maximumNumOfPointsPassingThrough;
    int minimumNumOfPointsPassingThrough;

    void updateMaxMinAfterGeneration();
    void processPoint(std::complex<double> complex);
    sf::Color getPixelColor(int x, int y) const override;

    friend void process(Buddhabrot *);
    void generate() override;
    void generateAfterDragging(int dx, int dy) override;
};


#endif //MANDELBROT_BUDDHABROT_H
