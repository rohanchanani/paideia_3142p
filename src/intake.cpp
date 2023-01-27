#include "vex.h"
#include <cmath>
#include "global.h"

using namespace vex;

bool intakeSpinning = false;
double intakeScaler = 0;

int numDiscs() {
  if (distance1.objectDistance(mm) < 40) {
    return 4;
  } else if (distance1.objectDistance(mm) < 60) {
    return 3;
  } else if (distance1.objectDistance(mm) < 86) {
    return 2;
  } else if (distance1.objectDistance(mm) < 110) {
    return 1;
  } else {
    return 0;
  }
}

bool targetColor = false;

void changeColor() {
  if (targetColor) {
    Brain.Screen.drawRectangle(0, 0, 500, 500, red);
    Brain.Screen.print("3142P");
    targetColor = false;
  } else {
    targetColor = true;
    Brain.Screen.drawRectangle(0, 0, 500, 500, blue);
    Brain.Screen.print("3142P");
  }
}

//true means blue, false means red
bool convertColor() {
  if (optical1.hue() > 50 && optical1.hue() < 300) {
    return true;
  } else {
    return false;
  }
}

void intakeThread() {
  bool seen = false;
  double firstTime;
  double discDistance = 60;
  while(true) {
    if (distance1.objectDistance(mm) < discDistance && !seen) {
      seen = true;
      firstTime = Brain.timer(seconds); 
    }
    if (seen) {
      if (distance1.objectDistance(mm) > discDistance) {
        seen = false;
        intakeScaler = 0;
      } else {
        if (Brain.timer(seconds) - firstTime >0.3) {
          intakeScaler = 1;
        }
      }
    } else {
      intakeScaler = 0;
    }
  wait(10, msec);
  }
}

void setIntake(int intakeSpeed) {
  intake.setVelocity(-1 * intakeSpeed, percent);
}

void switchIntake() { intakeSpinning = !intakeSpinning; }

void turnRoller(double target, double initialTime, double timeout, bool repeat) {
  intake.spin(reverse, 12, volt);
  double firstTime = Brain.timer(seconds);
  while((vertical.velocity(dps) > 15 && Brain.timer(seconds) - firstTime <initialTime) || Brain.timer(seconds) - firstTime < 0.3) {
    double currentError = inertial1.rotation(degrees) - target;
    wait(10, msec);
    leftDrivetrain.spin(forward,(3 - PID(target, currentError,SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
    rightDrivetrain.spin(forward, (3 + PID(target, currentError, SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
  }
  double beginning = Brain.timer(seconds);
  while(convertColor()!=targetColor && Brain.timer(seconds) - beginning <timeout) {
    double currentError = inertial1.rotation(degrees) - target;
    wait(10, msec);
    leftDrivetrain.spin(forward,(1 - PID(target, currentError,SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
    rightDrivetrain.spin(forward, (1 + PID(target, currentError, SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
  }
  while (convertColor()==targetColor && Brain.timer(seconds) - beginning <timeout) {
    double currentError = inertial1.rotation(degrees) - target;
    wait(10, msec);
    leftDrivetrain.spin(forward,(1 - PID(target, currentError,SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
    rightDrivetrain.spin(forward, (1 + PID(target, currentError, SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
  }
  double startTime = Brain.timer(seconds);
  while (convertColor()!=targetColor && Brain.timer(seconds) - beginning <timeout) {
    double currentError = inertial1.rotation(degrees) - target;
    wait(10, msec);
    leftDrivetrain.spin(forward,(1 - PID(target, currentError,SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
    rightDrivetrain.spin(forward, (1 + PID(target, currentError, SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
  }
  double totalTime = Brain.timer(seconds) - startTime;
  double newStart = Brain.timer(seconds);
  intake.spin(forward, 10, volt);
  while (Brain.timer(seconds) - newStart < totalTime * 0.85  && Brain.timer(seconds) - beginning <timeout) {
    double currentError = inertial1.rotation(degrees) - target;
    wait(10, msec);
    leftDrivetrain.spin(forward,(1 - PID(target, currentError,SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
    rightDrivetrain.spin(forward, (1 + PID(target, currentError, SCALE_FACTOR, NEW_SCALE_FACTOR)), volt);
  }
  intake.stop();
  return;  
}