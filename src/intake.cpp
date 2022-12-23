#include "vex.h"
#include <cmath>
#include "global.h"

using namespace vex;

bool intakeSpinning = false;

void setIntake(int intakeSpeed) {
  intake.setVelocity(-1 * intakeSpeed, percent);
}

void switchIntake() { intakeSpinning = !intakeSpinning; }

void turnRoller(double target, double initialTime, bool repeat) {
  intake.spin(forward, 12, volt);
  timeDriveStraight(initialTime, target, 3);
  intake.stop();
  wait(0.3, seconds);
  while (repeat) {

    if (convertColor() == isBlue) {
      controller1.Screen.print(convertColor());
      break;
    }
    intake.spin(forward, 12, volt);
    timeDriveStraight(-0.3, target, 3);
    timeDriveStraight(0.7, target, 3);
    intake.stop();
    wait(0.3, seconds);
  }
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  intake.stop();
}