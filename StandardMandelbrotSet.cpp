#include <mutex>
#include <thread>
#include "StandardMandelbrotSet.h"

int StandardMandelbrotSet::calculateEscapeIteration(std::complex<double> p) {

    if(isInMainBulbs(p)){
        return MAX_ITERATION;
    }

    std::complex<double> z(0,0);
    std::complex<double> oldZ = z;
    int pow2 = 1;

    for(int i = 0 ; i < MAX_ITERATION ; i++) {
        z = z * z + p;

        if(z == oldZ){
            //reported cycle, p not in mandelbrot
            return MAX_ITERATION;
        }

        if(norm(z) >= 4.0){
            return i;
        }

        if(i == pow2){
            oldZ = z;
            pow2 *= 2;
        }
    }
    return MAX_ITERATION;
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

    std::thread threads[NUM_OF_THREADS];

    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            tasks.push_back(std::pair<int,int>(x, y));
        }
    }

    for(int i = 0 ; i < NUM_OF_THREADS ; i++){
        threads[i] = std::thread(process, this);
    }

    for(int i = 0 ; i < NUM_OF_THREADS ; i++){
        threads[i].join();
    }
}

void StandardMandelbrotSet::generateAfterDragging(int dx, int dy){

    int newEscapeIteration[MAX_WINDOW_SIZE][MAX_WINDOW_SIZE];
    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            if(between(x - dx, 0, windowSize - 1) && between(y - dy, 0, windowSize - 1)){
                newEscapeIteration[x][y] = escapeIteration[x - dx][y - dy];
            } else {
                newEscapeIteration[x][y] = calculateEscapeIteration(getPointFromPixelCoordinates(x, y));
            }
        }
    }
    for(int x = 0 ; x < windowSize ; x++){
        for(int y = 0 ; y < windowSize ; y++){
            escapeIteration[x][y] = newEscapeIteration[x][y];
        }
    }

}

sf::Color StandardMandelbrotSet::getPixelColor(int x, int y) const{
    if(escapeIteration[x][y] == MAX_ITERATION){
        return sf::Color::Black;
    } else {
        return sf::Color::White;
    }
}