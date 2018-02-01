#ifndef MANDELBROT_BUDDHABROT_H
#define MANDELBROT_BUDDHABROT_H

#include <SFML/Graphics.hpp>
#include <boost/multi_array.hpp>
#include "MandelbrotSet.h"
#include <random>
#include <mutex>
#include <iostream>

class Buddhabrot : public MandelbrotSet {
public:
    Buddhabrot(int windowSize, double viewSize, std::pair<double, double> leftUpPointCoordinates, int numOfThreads,
               int maxIteration, int randomSamplesPerThread, int seed = -1):
            MandelbrotSet(windowSize, viewSize, leftUpPointCoordinates, numOfThreads),
            maxIteration(maxIteration), randomSamplesPerThread(randomSamplesPerThread){

        if(seed == -1){
            std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
            std::random_device rd;
            this->seed = dist(rd);
        } else {
            this->seed = seed;
        }

        numOfPointsPassingThrough.resize(boost::extents[windowSize][windowSize]);
        generate();
    }

protected:
    int maxIteration;
    int randomSamplesPerThread;

    boost::multi_array<int, 2> numOfPointsPassingThrough;
    int maximumNumOfPointsPassingThrough;
    int minimumNumOfPointsPassingThrough;

    std::mutex mtx;
    std::mt19937 mt;
    int seed;

    void updateMaxMinAfterGeneration();
    void processPoint(std::complex<double> complex);

    std::complex<double> getRandomSample();
    friend void process(Buddhabrot *);

    sf::Color getPixelColor(int x, int y) const override;
    void generate() override;
    void generateAfterDragging(int dx, int dy) override;
};


#endif //MANDELBROT_BUDDHABROT_H
