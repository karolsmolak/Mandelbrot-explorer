#include <iostream>
#include <thread>
#include <random>
#include <boost/log/trivial.hpp>
#include "Buddhabrot.h"

std::random_device rd;
std::mt19937 mt(rd());

double getRandomDoubleFromRange(double minDouble, double maxDouble);

void process(Buddhabrot *buddhabrot) {
    for (int i = 0; i < buddhabrot->numberOfRandomPointsPerThread; i++) {
        std::complex<double> p(getRandomDoubleFromRange(-2.0, 2.0),
                               getRandomDoubleFromRange(-2.0, 2.0));
        buddhabrot->processPoint(p);
    }
}

void Buddhabrot::processPoint(std::complex<double> p){

    if(isInMainBulbs(p)){
        return;
    }

    std::complex<double> z(0,0);
    std::complex<double> oldZ = z;

    int pow2 = 1;

    std::vector<std::pair<int, int> > visited;
    for(int i = 0 ; i < MAX_ITERATION ; i++) {
        z = z * z + p;

        if(z == oldZ){
            //detected cycle, in mandelbrot set
            return;
        }

        std::pair<int, int> pixelCoordinates = getPixelCoordinatesFromPoint(z);
        if(between(pixelCoordinates.first, 0, windowSize - 1) &&
                between(pixelCoordinates.second, 0, windowSize - 1)){
            visited.push_back(pixelCoordinates);
        }

        if(norm(z) >= 4.0){
            for(auto pixelCoordinates : visited){
                //not thread safe, but 99,9% safe
                numOfPointsPassingThrough[pixelCoordinates.first][pixelCoordinates.second]++;
                numOfPointsPassingThrough[pixelCoordinates.first][windowSize - 1 - pixelCoordinates.second]++;
            }
            return;
        }

        if(i == pow2){
            oldZ = z;
            pow2 *= 2;
        }
    }
}

void Buddhabrot::generate() {

    auto start = std::chrono::system_clock::now();

    BOOST_LOG_TRIVIAL(info) << "Started generating buddhabrot...";

    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            numOfPointsPassingThrough[x][y] = 0;
        }
    }

    std::thread threads[NUM_OF_THREADS];

    for(int i = 0 ; i < NUM_OF_THREADS ; i++){
        threads[i] = std::thread(process, this);
    }

    for(int i = 0 ; i < NUM_OF_THREADS ; i++){
        threads[i].join();
    }

    minimumNumOfPointsPassingThrough = numberOfRandomPointsPerThread * NUM_OF_THREADS;
    maximumNumOfPointsPassingThrough = 0;

    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            if(numOfPointsPassingThrough[x][y] < minimumNumOfPointsPassingThrough){
                minimumNumOfPointsPassingThrough = numOfPointsPassingThrough[x][y];
            }
            if(numOfPointsPassingThrough[x][y] > maximumNumOfPointsPassingThrough){
                maximumNumOfPointsPassingThrough = numOfPointsPassingThrough[x][y];
            }

        }
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    BOOST_LOG_TRIVIAL(info) << "Finished, elapsed time: " << elapsed_seconds.count() << "s\n";

}

sf::Color Buddhabrot::getPixelColor(int x, int y) const {
    double factor =  ((double)numOfPointsPassingThrough[x][y] - minimumNumOfPointsPassingThrough) /
                    ((double)maximumNumOfPointsPassingThrough - minimumNumOfPointsPassingThrough);

    factor = pow(factor, 0.4);
    int scale = factor * 255.0;

    return sf::Color(scale, scale, scale);
}

double getRandomDoubleFromRange(double minDouble, double maxDouble)
{
    std::uniform_real_distribution<double> unif(minDouble, maxDouble);
    return unif(mt);
}

void Buddhabrot::generateAfterDragging(int dx, int dy) {
    generate();
}
