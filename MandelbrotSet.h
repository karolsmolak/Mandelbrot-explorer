#ifndef MANDELBROT_MandelbrotSet_H
#define MANDELBROT_MandelbrotSet_H

#include <complex>

class MandelbrotSet : public sf::Drawable {
public:
    void drag(int dx, int dy);
    void scale(double factor);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void saveToFile();

    MandelbrotSet(int windowSize, double viewSize, std::pair<double, double> leftUpPointCoordinates, int numOfThreads):
            windowSize(windowSize), viewSize(viewSize), leftUpPointCoordinates(leftUpPointCoordinates), numOfThreads(numOfThreads){}

protected:

    bool between(int x, int a, int b){
        return x >= a && x <= b;
    }

    int windowSize;
    double viewSize;

    std::pair<double, double> leftUpPointCoordinates;

    int numOfThreads;

    std::pair<int, int> getPixelCoordinatesFromPoint(std::complex<double> p);

    bool isInMainBulbs(std::complex<double> p);
    std::complex<double> getPointFromPixelCoordinates(int x, int y) const;
    virtual sf::Color getPixelColor(int x, int y) const = 0;

    virtual void generate() = 0;
    virtual void generateAfterDragging(int dx, int dy) = 0;
};


#endif //MANDELBROT_MandelbrotSet_H
