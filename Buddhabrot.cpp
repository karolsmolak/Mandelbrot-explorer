#include <thread>
#include <random>
#include <mutex>
#include <boost/log/trivial.hpp>
#include "Buddhabrot.h"

void process(Buddhabrot *buddhabrot) {
    for (int i = 0; i < buddhabrot->randomSamplesPerThread; i++) {
        std::complex<double> p = buddhabrot->getRandomSample();
        buddhabrot->processPoint(p);
    }
}

void Buddhabrot::processPoint(std::complex<double> p){
    if(isInMainBulbs(p)){
        return;
    }

    std::complex<double> z(0,0);
    std::complex<double> oldZ = z;

    std::vector<std::pair<int, int> > visited;
    for(int i = 0 ; i < maxIteration ; i++) {
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

        pixelCoordinates = getPixelCoordinatesFromPoint(std::conj(z));
        if(between(pixelCoordinates.first, 0, windowSize - 1) &&
           between(pixelCoordinates.second, 0, windowSize - 1)){
            visited.push_back(pixelCoordinates);
        }

        if(norm(z) >= 4.0){
            for(auto pixelCoordinates : visited){
                //not thread safe, but 99,9% safe
                numOfPointsPassingThrough[pixelCoordinates.first][pixelCoordinates.second]++;
            }
            return;
        }

        //check if i is a power of 2
        if(!(i & (i - 1))){
            oldZ = z;
        }
    }
}

void Buddhabrot::generate() {
    mt.seed(this->seed);

    auto start = std::chrono::system_clock::now();

    BOOST_LOG_TRIVIAL(info) << "Started generating buddhabrot...";
    BOOST_LOG_TRIVIAL(info) << "Area: [" << leftUpPointCoordinates.first << ", " << leftUpPointCoordinates.first + viewSize << "] x ["
                            << leftUpPointCoordinates.second - viewSize << ", " << leftUpPointCoordinates.second << "]";
    BOOST_LOG_TRIVIAL(info) << "Seed: " << seed;
    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            numOfPointsPassingThrough[x][y] = 0;
        }
    }

    std::thread threads[numOfThreads];

    for(int i = 0 ; i < numOfThreads ; i++){
        threads[i] = std::thread(process, this);
    }

    for(int i = 0 ; i < numOfThreads ; i++){
        threads[i].join();
    }

    updateMaxMinAfterGeneration();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    BOOST_LOG_TRIVIAL(info) << "Finished, elapsed time: " << elapsed_seconds.count() << "s";
}

sf::Color Buddhabrot::getPixelColor(int x, int y) const {
    double factor =  (double)(numOfPointsPassingThrough[x][y] - minimumNumOfPointsPassingThrough) /
                    (maximumNumOfPointsPassingThrough - minimumNumOfPointsPassingThrough);

    factor = pow(factor, 0.4);
    int scale = factor * 255.0;

    return sf::Color(scale, scale, scale);
}

void Buddhabrot::generateAfterDragging(int dx, int dy) {
    generate();
}

void Buddhabrot::updateMaxMinAfterGeneration() {
    minimumNumOfPointsPassingThrough = randomSamplesPerThread * numOfThreads;
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
}

std::complex<double> Buddhabrot::getRandomSample() {
    std::unique_lock<std::mutex> lck(mtx);

    std::uniform_real_distribution<double> unif(-2.0, 2.0);
    return std::complex<double>(unif(mt), unif(mt));
}
