//
// Created by Alanbly on 4/16/2025.
//

#include "Coin.h"

#include <cmath>
#include <ctime>

const double MIN_ROTATION_RAD = 0.1;
const double MIN_SPEED_RPMS = 0.01;
const double MAX_SPEED_RPMS = 100.0;
const double MAX_SIZE = 1.0;
const double FOCAL_DEPTH = 50.0;

static long now_ms() {
    struct timespec res{};
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000 * res.tv_sec + (long) (res.tv_nsec / 1e6);
}

static double getDepth(double diameter) {
    return FOCAL_DEPTH * (1 - diameter / MAX_SIZE);
}

static double getRotation(double diameter, double x, double y) {
    auto depth = getDepth(diameter);
    return atan(x / depth);
}

bool Coin::isSpinning() const {
    return abs(speed_rad_per_ms) > MIN_SPEED_RPMS;
}

bool Coin::isOffset() const {
    return abs(rotation_rad) > MIN_ROTATION_RAD;
}

void Coin::point(double x, double y) {
    rotation_rad = getRotation(diameter, x, y);
}

void Coin::setSpeed(double speed_rpms) {
    speed_rad_per_ms = fmax(-MAX_SPEED_RPMS, fmin(MAX_SPEED_RPMS, speed_rpms));

    if(isSpinning()) {
        return;
    }

    if (isOffset()) {
        speed_rad_per_ms = (rotation_rad / abs(rotation_rad)) * -MIN_SPEED_RPMS;
    } else {
        speed_rad_per_ms = 0.0;
    }
}


void Coin::press(double x, double y) {
    xStart = x;
    yStart = y;
    startTime_ms = now_ms();

    if (!isSpinning()) {
        point(x, y);
    }
}

void Coin::swipe(double x, double y) {
    if (!isSpinning()) {
        point(x, y);
    }
}

void Coin::release(double x, double y) {
    auto arc_rad = getRotation(diameter, x, y) - getRotation(diameter, xStart, yStart);
    auto time_ms = now_ms() - startTime_ms;

    auto speed_rpms = arc_rad / time_ms * damping;
    if (!isSpinning()) {
        point(x, y);
        speed_rad_per_ms = fmax(-MAX_SPEED_RPMS, fmin(MAX_SPEED_RPMS, speed_rpms));
    } else {
        speed_rad_per_ms = fmin(MAX_SPEED_RPMS, speed_rad_per_ms + speed_rpms * damping);
    }
}

void Coin::decay(double rate) {
    setSpeed(speed_rad_per_ms * (1 - rate));
}

void Coin::tick(long period_ms) {
    if (isSpinning() || isOffset()) {
        rotation_rad += speed_rad_per_ms * period_ms;
    } else {
        rotation_rad = 0.0;
        speed_rad_per_ms = 0.0;
    }
}
