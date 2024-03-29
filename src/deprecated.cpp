#include "vex.h"
#include <cmath>
#include "global.h"

using namespace vex;

double currentX = 0;
double currentY = 0;
double fieldOrientation = 0;

void printCoords() {
  controller1.Screen.clearScreen();
  controller1.Screen.setCursor(0, 0);
  controller1.Screen.print(currentX);
  controller1.Screen.print(", ");
  controller1.Screen.print(currentY);
}

void odom() {
  double currentVertical = vertical.rotation(degrees);
  double currentHorizontal = horizontal.rotation(degrees);
  // controller1.ButtonA.pressed(printCoords);
  while (true) {
    wait(20, msec);
    controller1.Screen.setCursor(0, 0);
    controller1.Screen.print(currentY);
    currentX += ((vertical.rotation(degrees) - currentVertical) *
                     sin(rad(inertial1.heading(degrees) - fieldOrientation)) +
                 (horizontal.rotation(degrees) - currentHorizontal) *
                     cos(rad(inertial1.rotation(degrees) - fieldOrientation))) *
                2.75 * M_PI / 360;
    currentY += ((vertical.rotation(degrees) - currentVertical) *
                     cos(rad(inertial1.heading(degrees) - fieldOrientation)) -
                 (horizontal.rotation(degrees) - currentHorizontal) *
                     sin(rad(inertial1.rotation(degrees) - fieldOrientation))) *
                2.75 * M_PI / 360;
    currentVertical = vertical.rotation(degrees);
    currentHorizontal = horizontal.rotation(degrees);
  }
}

void oldTurnRoller(double timeout) {
  double startTime = Brain.timer(seconds);
  bool initialColor = convertColor();
  intake.spin(reverse, 12, volt);
  if (initialColor == targetColor) {
    while (convertColor() == initialColor) {
      if (Brain.timer(seconds) - startTime > timeout) {
        return;
      }
    }
    double firstTime = Brain.timer(seconds);
    while (convertColor() != initialColor) {
      if (Brain.timer(seconds) - startTime > timeout) {
        return;
      }
    }
    double spinTime = Brain.timer(seconds) - firstTime;
    wait(spinTime / 2, seconds);
    intake.stop();
  } else {
    while (convertColor() == initialColor) {
      if (Brain.timer(seconds) - startTime > timeout) {
        return;
      }
    }
    double firstTime = Brain.timer(seconds);
    while (convertColor() != initialColor) {
      if (Brain.timer(seconds) - startTime > timeout) {
        return;
      }
    }
    double spinTime = Brain.timer(seconds) - firstTime;
    while (convertColor() == initialColor) {
      if (Brain.timer(seconds) - startTime > timeout) {
        return;
      }
    }
    wait(spinTime / 2, seconds);
    intake.stop();
  }
}