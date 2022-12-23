#include "vex.h"
#include <cmath>
#include "global.h"

using namespace vex;

double WHEEL_RADIUS = 4;
double rad(double degrees) { return degrees * M_PI / 180; }

double calcRotation() {
  return (LBMotor.rotation(degrees)+LMMotor.rotation(degrees)+LFMotor.rotation(degrees)+RBMotor.rotation(degrees)+RMMotor.rotation(degrees)+RFMotor.rotation(degrees)) / 10;
}

bool isDoneDriving(double travelDegrees) {
  double margin = 5;
  return std::abs(travelDegrees -
                  calcRotation()) < margin;
}

double SCALE_FACTOR = 2;
double NEW_SCALE_FACTOR = 10;

double PID(double initialHeading, double currentError, double scaleFactor,
           double newScaleFactor) {
  double currentHeading = inertial1.rotation(degrees);
  double newError = currentHeading - initialHeading;
  double errorChange = newError - currentError;
  return scaleFactor * newError + newScaleFactor * errorChange;
}

void timeDriveStraight(double timeAmount, double targetRotation, double driveVelocity, double scaleFactor, double newScaleFactor) {
  double startTime = Brain.timer(seconds);
  double driveDirection = timeAmount / std::abs(timeAmount);
  while (Brain.timer(seconds) - startTime < std::abs(timeAmount)) {
    double currentError = inertial1.rotation(degrees) - targetRotation;
    wait(10, msec);
    leftDrivetrain.spin(
          forward,
          driveDirection * (driveVelocity - PID(targetRotation, currentError,
                                                scaleFactor, newScaleFactor)),
          volt);
      rightDrivetrain.spin(
          forward,
          driveDirection * (driveVelocity + PID(targetRotation, currentError,
                                                scaleFactor, newScaleFactor)),
          volt);
  }
  leftDrivetrain.stop(); 
  rightDrivetrain.stop(); 
}
void driveStraight(double travelDistance, double targetRotation,
                   double taperDistance, double timeout, bool taper,
                   double maxVelocity, double minVelocity,
                   double scaleFactor,
                   double newScaleFactor,
                   int shouldPID) {
  LBMotor.setRotation(0, degrees);
  LMMotor.setRotation(0, degrees);
  LFMotor.setRotation(0, degrees);
  RFMotor.setRotation(0, degrees);
  RMMotor.setRotation(0, degrees);
  RBMotor.setRotation(0, degrees);
  double travelDegrees = 360 * travelDistance / (2.75 * M_PI);
  double adjustment = maxVelocity;
  double adjustmentScaler = maxVelocity / (taperDistance * 360 / 2.75);
  int currentTime = Brain.timer(seconds);
  double driveDirection = travelDistance / std::abs(travelDistance);
  while (!isDoneDriving(travelDegrees) &&
         currentTime + timeout > Brain.timer(seconds)) {
    double currentError = inertial1.rotation(degrees) - targetRotation;
    wait(10, msec);
    if (taper) {
      adjustment = driveDirection * adjustmentScaler * (travelDegrees -
                                       calcRotation());
      if (std::abs(adjustment) > maxVelocity) {
        adjustment = maxVelocity;
      }
      if (std::abs(adjustment) < minVelocity) {
        adjustment = minVelocity;
      }
    } else {
      adjustment = maxVelocity;
    }
    // controller1.Screen.setCursor(0, 0);
    // controller1.Screen.print(vertical.rotation(degrees));
    leftDrivetrain.spin(
        forward,
        driveDirection * adjustment - PID(targetRotation, currentError,
                                               scaleFactor, newScaleFactor),
        volt);
    rightDrivetrain.spin(
        forward,
        driveDirection * adjustment + PID(targetRotation, currentError,
                                               scaleFactor, newScaleFactor),
        volt);
  }
  // rightDrivetrain.stop();
  // leftDrivetrain.stop();
  robotDrive.setDriveVelocity(100, percent);
  robotDrive.setTurnVelocity(100, percent);
}

void turnHeading(int targetRotation, bool fullTurn, double timeout) {
  while (std::abs(inertial1.rotation()-targetRotation)>30) {
    double dir = std::abs(inertial1.rotation()-targetRotation) / (inertial1.rotation()-targetRotation);
    leftDrivetrain.spin(forward, dir*-12, volt);
    rightDrivetrain.spin(forward, dir*12, volt);
  }
  double currentError = inertial1.rotation(degrees) - targetRotation;
  double initialTime = Brain.timer(seconds);
  while (Brain.timer(seconds) - initialTime < timeout && std::abs(currentError) >5 | std::abs(PID(targetRotation, currentError,
                                               SCALE_FACTOR/2, NEW_SCALE_FACTOR/2)) > 1.5) {
    currentError = inertial1.rotation(degrees) - targetRotation;
    wait(10, msec);
    // controller1.Screen.setCursor(0, 0);
    // controller1.Screen.print(vertical.rotation(degrees));
    leftDrivetrain.spin(
        forward,
        - PID(targetRotation, currentError,
                                               SCALE_FACTOR/2, NEW_SCALE_FACTOR/2),
        volt);
    rightDrivetrain.spin(
        forward,
        PID(targetRotation, currentError,
                                               SCALE_FACTOR/2, NEW_SCALE_FACTOR/2),
        volt);
  }
  //rightDrivetrain.stop();
  //leftDrivetrain.stop();
  //robotDrive.setDriveVelocity(100, percent);
  //robotDrive.setTurnVelocity(100, percent);
  controller1.Screen.print(inertial1.rotation());
}