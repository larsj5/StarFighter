//
// Created by Lars Jensen on 4/19/22.
//

#ifndef LARGE_QUAD_H
#define LARGE_QUAD_H
//struct color {
//    double red;
//    double green;
//    double blue;
//};

#include "shape.h"

struct point {
    int x;
    int y;
};

class Quad {
protected:
    color fill;
    point center;
    unsigned int width;
    unsigned int height;

public:
    Quad();
    Quad(color fill, point center, unsigned int width, unsigned int height);

    int getCenterX() const;
    int getLeftX() const;
    int getRightX() const;
    int getCenterY() const;
    int getTopY() const;
    int getBottomY() const;
    point getCenter() const;

    double getRed() const;
    double getGreen() const;
    double getBlue() const;
    color getFill() const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void setColor(double red, double green, double blue);
    void setColor(color fill);
    void move(int deltaX, int deltaY);
    void resize(unsigned int width, unsigned int height);

    virtual void draw() const;
};
#endif //LARGE_QUAD_H
