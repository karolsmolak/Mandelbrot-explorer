#include <mutex>
#include <thread>
#include "StandardMandelbrotSet.h"

int StandardMandelbrotSet::calculateEscapeIteration(std::complex<double> p) {
    if(isInMainBulbs(p)){
        return maxIteration;
    }

    std::complex<double> z(0,0);
    std::complex<double> oldZ = z;
    int pow2 = 1;

    for(int i = 0 ; i < maxIteration ; i++) {
        z = z * z + p;

        if(z == oldZ){
            //detected cycle, p not in mandelbrot
            return maxIteration;
        }

        if(norm(z) >= 4.0){
            //escaped
            return i;
        }

        if(i == pow2){
            oldZ = z;
            pow2 *= 2;
        }
    }

    return maxIteration;
}

std::vector<std::pair<int, int>> tasks;
std::mutex mtx;

void process(StandardMandelbrotSet *standardMandelbrotSet){
    while(true)
    {
        mtx.lock();

        if (tasks.empty()) {
            mtx.unlock();
            return;
        }

        std::pair<int, int> myTask = tasks[tasks.size() - 1];
        tasks.pop_back();

        mtx.unlock();

        standardMandelbrotSet->escapeIteration[myTask.first][myTask.second] = standardMandelbrotSet->calculateEscapeIteration(
                standardMandelbrotSet->getPointFromPixelCoordinates(myTask.first, myTask.second));
    }
}

void StandardMandelbrotSet::generate() {
    tasks.resize(windowSize * windowSize);

    std::thread threads[numOfThreads];

    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            tasks.push_back(std::pair<int,int>(x, y));
        }
    }

    for(int i = 0 ; i < numOfThreads ; i++){
        threads[i] = std::thread(process, this);
    }

    for(int i = 0 ; i < numOfThreads ; i++){
        threads[i].join();
    }
}

void StandardMandelbrotSet::generateAfterDragging(int dx, int dy){
    boost::multi_array<int, 2> newEscapeIteration(boost::extents[windowSize][windowSize]);

    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            if(between(x - dx, 0, windowSize - 1) && between(y - dy, 0, windowSize - 1)){
                newEscapeIteration[x][y] = escapeIteration[x - dx][y - dy];
            } else {
                newEscapeIteration[x][y] = calculateEscapeIteration(getPointFromPixelCoordinates(x, y));
            }
        }
    }

    escapeIteration = newEscapeIteration;
}

sf::Color StandardMandelbrotSet::getPixelColor(int x, int y) const{
    if(escapeIteration[x][y] == maxIteration){
        return sf::Color::Black;
    } else {
        return sf::Color::White;
    }
}