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
#include "global.h"
#include <cmath>

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

void vexcodeInit(void) {
  // Nothing to initialize
  inertial1.calibrate();
  robotDrive.setTimeout(3, seconds);
  vertical.setRotation(0, degrees);
  horizontal.setRotation(0, degrees);
  robotDrive.setDriveVelocity(100, percent);
  robotDrive.setTurnVelocity(100, percent);
  inertial1.setRotation(0, degrees);
  inertial1.setHeading(0, degrees);
  Brain.setTimer(0, seconds);
  intake.setVelocity(0, percent);
  intake.spin(forward);
  Brain.Screen.setFont(mono60);
  Brain.Screen.drawRectangle(0, 0, 500, 500, red);
  Brain.Screen.print("3142P");
  Brain.Screen.pressed(changeColor);
  extension.set(false);
  launcher.set(false);
  while (Brain.timer(seconds) < 3) {
  }
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
  vexcodeInit();
}


void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  //driveStraight(30, 0);
  //solo();
  //auton_right();
  //auton_left();
  //a_right();
  //a_left();
  skills();
  //extension.set(true);
  //while (true) {
  //  controller1.Screen.setCursor(0, 0);
  //  controller1.Screen.print(convertColor());
  //} 
  //turnRoller(0, 1);
  //while (true) {
  //  controller1.Screen.setCursor(0, 0);
  //  controller1.Screen.print(vertical.velocity(dps));
  //}
}

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