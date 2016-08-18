#pragma once
#ifndef __SERVO__H__
#define __SERVO__H__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

/*
 * Sets servos position. Accepts values as % offset from left and bottom.
 * 0.0 point is left bottom corner.
 */
class ServoController {
public:
    ServoController(const char * path);
    ~ServoController();
    void setPosition(float xPercent, float yPercent);

private:
    static const unsigned int X_MIN = 130;
    static const unsigned int X_MAX = 190;
    static const unsigned int Y_MIN = 135;
    static const unsigned int Y_MAX = 175;

    const char * path;
    FILE *fp;
};
#endif
