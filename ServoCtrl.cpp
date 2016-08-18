#include "ServoCtrl.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

ServoController::ServoController(const char * path){
    this->path = path;
    fp = fopen("/dev/servoblaster", "w");
    if (fp == NULL) {
        cerr << "Error opening " << path << endl;
        exit(0);
    }
    cout << "servo controller start success" << endl;
};

ServoController::~ServoController() {
    if (fp != NULL) {
        fclose(fp);
    }
}

void ServoController::setPosition(float xPercent, float yPercent) {
    if (xPercent < 0 || xPercent > 1.0 || yPercent < 0 || yPercent > 1.0) {
        cerr << "invald servo coordinates requested: " << xPercent << ":" << yPercent << endl;
        return;
    }

    cout << "setting servo pos to " << xPercent << ":" << yPercent << endl;
    unsigned int x = (unsigned int)(X_MIN + (X_MAX - X_MIN) * xPercent);
    unsigned int y = (unsigned int)(Y_MAX - (Y_MAX - Y_MIN) * yPercent);
    printf("0=%d 1=%d\n", x, y);
    fprintf(fp, "0=%d\n", x);
    fflush(fp);
    fprintf(fp, "1=%d\n", y);
    fflush(fp);
}
