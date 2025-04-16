//
// Created by Alanbly on 4/16/2025.
//

#ifndef PLATE_SPINNING_COIN_H
#define PLATE_SPINNING_COIN_H


class Coin {
    double xCenter;
    double yCenter;
    double diameter;
    double damping;

    double rotation_rad; // 0 is front, rotation clockwise is positive
    double speed_rad_per_ms;

    double xStart;
    double yStart;
    long startTime_ms;

    void point(double x, double y);
    void setSpeed(double speed_rpms);

public:
    void press(double x, double y);
    void swipe(double x, double y);
    void release(double x, double y);
    void decay(double rate);
    void tick(long period);
    [[nodiscard]] bool isSpinning() const;
    [[nodiscard]] bool isOffset() const;

};


#endif //PLATE_SPINNING_COIN_H
