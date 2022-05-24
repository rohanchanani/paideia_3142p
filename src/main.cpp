/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

double WHEEL_RADIUS = 4;
bool prioritizeWinPoint = false;
bool intakeSpinning = false;

void setup(void) {
  tiltLift.set(false);
  tiltClamp.set(false);
  fourBarClamp.set(true);
}

void openPneumatics(void) {
  tiltLift.set(false);
  tiltClamp.set(false);
  fourBarClamp.set(false);
}

void vexcodeInit(void) {
  // Nothing to initialize
  inertial1.calibrate();
  robotDrive.setTimeout(3, seconds);
  controller1.Screen.print("PRE AUTON");
  distanceEncoder.setRotation(0, degrees);
  robotDrive.setDriveVelocity(100, percent);
  robotDrive.setTurnVelocity(100, percent);
  setup();
  inertial1.setRotation(0, degrees);
  inertial1.setHeading(0, degrees);
  Brain.setTimer(0, seconds);
  intake.setVelocity(0, percent);
  intake.spin(forward);
  while(Brain.timer(seconds) < 3) {
  }
  controller1.Screen.print("done calibrating");
}

void setIntake(int intakeSpeed) {
  intake.setVelocity(-1 * intakeSpeed, percent);
}

void switchIntake() {
  intakeSpinning = !intakeSpinning;
}

void liftBackLift() {
  tiltClamp.set(true);
}

void dropBackLift() {
  tiltClamp.set(false);
}

void grabBackLift() {
  tiltLift.set(true);
}

void releaseBackLift() {
  tiltLift.set(false);
}

void grabClamp() {
  fourBarClamp.set(true);
}

void releaseClamp() {
  fourBarClamp.set(false);
}

void fullTiltLift() {
  grabBackLift();
  wait(0.5, seconds);
  liftBackLift();
}

void fullTiltDrop() {
  dropBackLift();
  wait(0.5, seconds);
  releaseBackLift();
}

void fullTiltChange() {
  if (tiltClamp.value()) {
    fullTiltDrop();
  } else {
    fullTiltLift();
  }
}

bool isDoneDriving(double initialDistance, double travelDegrees) {
  double margin = 15;
  return std::abs(initialDistance + travelDegrees - distanceEncoder.rotation(degrees)) < margin;
}

double SCALE_FACTOR = 2;
int NEW_SCALE_FACTOR = 12;

int PID(double initialHeading, double currentError, double scaleFactor, double newScaleFactor) {
  double currentHeading = inertial1.rotation(degrees); 
  double newError = currentHeading - initialHeading;
  double errorChange = newError - currentError;
  return scaleFactor * newError + newScaleFactor * errorChange;
}

void driveStraight(double travelDistance, double targetRotation, int taperDistance = 10, int timeout = 3, bool taper = true, int maxVelocity = 12, double scaleFactor = SCALE_FACTOR, double newScaleFactor = NEW_SCALE_FACTOR, int shouldPID = 1) {
  double initialDistance = distanceEncoder.rotation(degrees);
  double travelDegrees = 360 * travelDistance / (2.75 * M_PI);
  double driveDirection = travelDistance / std::abs(travelDistance);
  double adjustment = maxVelocity;
  double adjustmentScaler = maxVelocity / (taperDistance * 360 / 2.75);
  int currentTime = Brain.timer(seconds);
  while (!isDoneDriving(initialDistance, travelDegrees) && currentTime + timeout > Brain.timer(seconds)) {
    double currentError = inertial1.rotation(degrees) - targetRotation;
    wait(10, msec);
    if (taper) {
      adjustment = adjustmentScaler * std::abs(initialDistance + travelDegrees - distanceEncoder.rotation(degrees));
      if (adjustment > maxVelocity) {
        adjustment = maxVelocity;
      }
      if (adjustment < 1) {
        adjustment = 1;
      }
    } else {
      adjustment = maxVelocity;
    }
    leftDrivetrain.spin(forward, driveDirection * (adjustment - driveDirection * PID(targetRotation, currentError, scaleFactor, newScaleFactor)), volt);
    rightDrivetrain.spin(forward, driveDirection * (adjustment + driveDirection * PID(targetRotation, currentError, scaleFactor, newScaleFactor)), volt);
  }
  rightDrivetrain.stop();
  leftDrivetrain.stop();
  robotDrive.setDriveVelocity(100, percent);
  robotDrive.setTurnVelocity(100, percent);
}

void turnHeading(int targetHeading, bool fullTurn = false) {
  double amountToTurn = (targetHeading % 360) - inertial1.heading(degrees);
  if (std::abs(amountToTurn) > 180 && !fullTurn) {
    amountToTurn = amountToTurn - (360 * std::abs(amountToTurn) / amountToTurn);
  }
  robotDrive.turnFor(right, amountToTurn, degrees);
}

void goalRush() {
  fourBar.spin(forward);
  driveStraight(50, 0, 2);
  wait(0.3, seconds);
  grabClamp();
  wait(0.2, seconds);
  fourBar.spin(reverse);
  driveStraight(-45, 0, 10);
}

void autonRight() {
  controller1.Screen.print(prioritizeWinPoint);
  robotDrive.setTimeout(3, seconds);
  if (prioritizeWinPoint) {
    driveStraight(15, 0, 2, 3);
    turnHeading(-90);
    driveStraight(-10, -90, 2, 3);
    wait(0.3, seconds);
    fullTiltLift();
    setIntake(100);
    driveStraight(15, -90, 2, 3);
    turnHeading(0);
    setIntake(0);
    fourBar.spin(forward);
    driveStraight(40, 0, 2, 3);
    grabClamp();
    wait(0.2, seconds);
    driveStraight(-50, 0, 2, 3);
    turnHeading(60);
    releaseClamp();
    wait(0.3, seconds);
    turnHeading(-42);
    driveStraight(53, -42, 2, 3);
    grabClamp();
    wait(0.2, seconds);
    driveStraight(-60, -42, 10, 3);
  } else {
    fourBar.spin(forward);
    driveStraight(43, 0, 2, 15);
    grabClamp();
    wait(0.2, seconds);
    driveStraight(-33, 0, 2, 15);
    turnHeading(60);
    releaseClamp();
    wait(0.3, seconds);
    turnHeading(-40);
    driveStraight(49, -40, 2, 3);
    grabClamp();
    wait(0.2, seconds);
    driveStraight(-56, -40, 3, 3);
    turnHeading(-105);
    driveStraight(-13, -100, 2, 3);
    fourBar.spinFor(reverse, 200, degrees, false);
    wait(0.3, seconds);
    fullTiltLift();
    setIntake(100);
    driveStraight(20, -100, 2, 3);
    setIntake(-100);
    wait(0.2, seconds);
    setIntake(100);
  }
}

void autonLeft() {
  if (prioritizeWinPoint) {
    driveStraight(-10, 0, 2);
    wait(0.3, seconds);
    fullTiltLift();
    setIntake(50);
    driveStraight(10, 0, 3);
    turnHeading(30);
    driveStraight(10, 30, 3);
    turnHeading(113);
    fullTiltDrop();
    setIntake(0);
    fourBar.spin(forward);
    driveStraight(47, 113, 2);
    grabClamp();
    driveStraight(-35, 113, 3);
  } else {
    fourBar.spin(forward);
    driveStraight(45, 0, 2, 15);
    grabClamp();
    wait(0.2, seconds);
    fourBar.spinFor(reverse, 200, degrees, false);
    driveStraight(-45, 0, 2, 15);
    fourBar.stop();
    turnHeading(-83);
    driveStraight(-15, -83, 2);
    wait(0.3, seconds);
    fullTiltLift();
    setIntake(100);
    driveStraight(20, -83, 2);
  }
}

void fullAWP() {
  driveStraight(-8, 0, 2, 1);
  wait(0.3, seconds);
  fullTiltLift();
  setIntake(10);
  wait(0.5, seconds);
  fullTiltDrop();
  setIntake(0);
  driveStraight(10, 0, 2, 2);
  turnHeading(-45);
  driveStraight(-20, -45, 2, 2);
  turnHeading(0);
  driveStraight(-69, 0, 2, 5);
  turnHeading(-13);
  driveStraight(-8, -13, 0, 2);
  fullTiltLift();
  setIntake(100);
  turnHeading(0);
  driveStraight(18, 0, 5, 2);
  turnHeading(90);
  setIntake(0);
  fourBar.spin(forward);
  driveStraight(30, 90, 2, 3);
  grabClamp();
  wait(0.2, seconds);
  driveStraight(-40, 90, 2, 3);
  turnHeading(150);
  releaseClamp();
  wait(0.3, seconds);
  turnHeading(50);
  driveStraight(56, 50, 2, 3);
  grabClamp();
  wait(0.2, seconds);
  driveStraight(-70, 45, 10, 3);
}

void originalSkills() {
  driveStraight(-8, 0, 2);
  wait(0.3, seconds);
  fullTiltLift();
  wait(0.3, seconds);
  driveStraight(10, 0, 3);
  turnHeading(30);
  driveStraight(12, 30, 3);
  turnHeading(109);
  fourBar.spin(forward);
  driveStraight(65, 109, 2);
  grabClamp();
  fourBar.spin(reverse);
  wait(0.5, seconds);
  turnHeading(142);
  driveStraight(34, 142, 3);
  turnHeading(90);
  driveStraight(8, 90, 2);
  fourBar.spinFor(forward, 2, seconds);
  releaseClamp();
  fourBar.spinFor(reverse, 0.5, seconds);
  driveStraight(-5, 90, 2);
  fourBar.spin(forward);
  turnHeading(0);
  fullTiltDrop();
  driveStraight(5, 0, 2);
  turnHeading(180);
  driveStraight(7, 180, 2);
  grabClamp();
  fourBar.spin(reverse);
  return
  turnHeading(90);
  driveStraight(5, 90, 2);
  fourBar.spinFor(forward, 0.5, seconds);
  releaseClamp();
  fourBar.spinFor(reverse, 0.5, seconds);
  turnHeading(0);
  driveStraight(80, 0, 5);
  grabClamp();
  fourBar.spin(reverse);
  wait(0.3, seconds);
  fourBar.stop();
  driveStraight(-40, 0, 3);
  turnHeading(45);
  driveStraight(-40, 45, 5);
  fullTiltLift();
  driveStraight(-40, 45, 5);
  fullTiltDrop();
  driveStraight(20, 45, 5);
  turnHeading(0);
  fourBar.spin(reverse);
  driveStraight(-30, 0, 5);
  fullTiltLift();
  driveStraight(40, 0, 5);
  turnHeading(-90);
  driveStraight(5, 270, 2);
  fourBar.spinFor(forward, 0.5, seconds);
  releaseClamp();
  driveStraight(-5, 270, 2);
  turnHeading(135);
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  openPneumatics();
  fullAWP();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  leftDrivetrain.setStopping(coast);
  rightDrivetrain.setStopping(coast);
  controller1.ButtonY.pressed(fullTiltChange);
  controller1.ButtonX.pressed(switchIntake);
  controller1.ButtonDown.pressed(grabBackLift);
  controller1.ButtonUp.pressed(releaseBackLift);
  controller1.ButtonLeft.pressed(liftBackLift);
  controller1.ButtonA.pressed(grabClamp);
  controller1.ButtonB.pressed(releaseClamp);

  // User control code here, inside the loop
  while (1) {
    
    double speedScaler = 1;
    //Drivetrain Arcade Steering
    if (abs(controller1.Axis3.position()) > 0 || abs(controller1.Axis4.position()) > 0) {
      rightDrivetrain.spin(forward, speedScaler * (controller1.Axis3.position() - controller1.Axis4.position()), percent);
      leftDrivetrain.spin(forward, speedScaler * (controller1.Axis3.position() + controller1.Axis4.position()), percent);
    }
    else {
      leftDrivetrain.spin(forward, 0, percent);
      rightDrivetrain.spin(forward, 0, percent);
    }

    fourBar.spin(forward, (int(controller1.ButtonR1.pressing()) - int(controller1.ButtonR2.pressing())) * 100, percent);
    intake.spin(forward, (2 * int(controller1.ButtonL2.pressing()) - int(intakeSpinning) - int(controller1.ButtonL1.pressing())) * 85, percent);
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  pre_auton();
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
