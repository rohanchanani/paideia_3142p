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
bool intakeSpinning = false;
bool flywheelSpinning = true;
double flywheelScaler = 0.6;
bool isBlue = false;
bool isRight = false;
double FLYWHEEL_RPM = 600;
double rad(double degrees) { return degrees * M_PI / 180; }

void flyThread() {
  while (true) {
    if (flywheelSpinning) {
      if (flywheel.velocity(rpm) / FLYWHEEL_RPM < flywheelScaler) {
        controller1.Screen.setCursor(0, 0);
        controller1.Screen.clearScreen();
        controller1.Screen.print(flywheelScaler);
        flywheel.spin(forward, 100, percent);
    }
      else {
        flywheel.spin(forward, 0, percent);
      }
    } else {
      flywheel.stop();
    }
  }
}

void changeColor() {
  if (isBlue) {
    Brain.Screen.drawRectangle(0, 0, 500, 500, red);
    Brain.Screen.print("3142P");
    isBlue = false;
  } else {
    isBlue = true;
    Brain.Screen.drawRectangle(0, 0, 500, 500, blue);
    Brain.Screen.print("3142P");
  }
}

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

int extensionVal = 0;
bool extended = true;
void updateExtension() {
  if (extensionVal < 4) {
    extensionVal++;
  } else {
    extension.set(true);
  }
}

void setIntake(int intakeSpeed) {
  intake.setVelocity(-1 * intakeSpeed, percent);
}

void switchIntake() { intakeSpinning = !intakeSpinning; }

void switchFlywheel() { flywheelSpinning = !flywheelSpinning; }

void switchPneumatic() {
  if (launcher.value()) {
    launcher.set(false);
  } else {
    launcher.set(true);
  }
}

void launch() {
  controller1.Screen.print("Launching");
  switchPneumatic();
  wait(.24, sec);
  switchPneumatic();
  if (!launcher.value()) {
    wait(.24, sec);
    switchPneumatic();
  }
}

void decrementFlywheel() {
  if (flywheelScaler > 0) {
    flywheelScaler -= 0.01;
    controller1.Screen.setCursor(4, 1);
    controller1.Screen.print(100 * flywheelScaler);
  }
}

void incrementFlywheel() {
  if (flywheelScaler < 1) {
    flywheelScaler += 0.01;
    controller1.Screen.setCursor(4, 1);
    controller1.Screen.print(100 * flywheelScaler);
  }
}

double calcRotation() {
  return (LBMotor.rotation(degrees)+LMMotor.rotation(degrees)+LFMotor.rotation(degrees)+RBMotor.rotation(degrees)+RMMotor.rotation(degrees)+RFMotor.rotation(degrees)) / 10;
}

bool isDoneDriving(double travelDegrees) {
  double margin = 5;
  return std::abs(travelDegrees -
                  calcRotation()) < margin;
}

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

double SCALE_FACTOR = 2;
double NEW_SCALE_FACTOR = 10;

double PID(double initialHeading, double currentError, double scaleFactor,
           double newScaleFactor) {
  double currentHeading = inertial1.rotation(degrees);
  double newError = currentHeading - initialHeading;
  double errorChange = newError - currentError;
  return scaleFactor * newError + newScaleFactor * errorChange;
}

void timeDriveStraight(double timeAmount, double targetRotation, double driveVelocity, double scaleFactor=SCALE_FACTOR, double newScaleFactor = NEW_SCALE_FACTOR) {
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
                   double taperDistance = 3, double timeout = 3, bool taper = true,
                   double maxVelocity = 12, double minVelocity = 0,
                   double scaleFactor = SCALE_FACTOR,
                   double newScaleFactor = NEW_SCALE_FACTOR,
                   int shouldPID = 1) {
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

bool convertColor() {
  if (optical1.hue() > 150 && optical1.hue() < 300) {
    return true;
  } else {
    return false;
  }
}

void oldTurnRoller(double timeout = 10) {
  double startTime = Brain.timer(seconds);
  bool initialColor = convertColor();
  intake.spin(reverse, 20, percent);
  if (initialColor == isBlue) {
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

void turnHeading(int targetRotation, bool fullTurn = false, double timeout=2) {
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

void turnRoller(double target = 0, double initialTime = 1, bool repeat=true) {
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

void match_load() {
  wait(0.8, seconds);
  launch();
}
void skills() {
  //flywheelSpinning = false;
  flywheelScaler = 0.54;
  thread t = thread(flyThread);
  turnRoller(0, 1.5, false); 
  driveStraight(-9, 0, 2);
  turnHeading(90);
  turnRoller(90, 2.7, false);
  driveStraight(-12, 90, 2, 2);
  turnHeading(130);
  driveStraight(-12, 130, 3, 2);
  turnHeading(100);
  driveStraight(-24, 100, 3, 2);
  turnHeading(79);
  driveStraight(7.7, 79);
  launch();
  wait(0.8, seconds);
  launch();
  repeat(17) {
    match_load();
  }
  driveStraight(-7, 79);
  turnHeading(120);
  driveStraight(35, 120);
  turnHeading(45);
  extension.set(true);
  return;
  //return;
  driveStraight(-14, 81);
  turnHeading(135);
  driveStraight(27, 135, 3, 3);
  turnHeading(225);
  intake.spin(reverse, 12, volt);
  driveStraight(60, 225, 3, 6, true, 4);
  launch();
  driveStraight(30, 225, 3, 6, true, 4);
  turnHeading(162);
  driveStraight(-27, 162);
  turnHeading(180);
  intake.stop();
  launch();
  wait(0.5, seconds);
  launch();
  wait(0.5, seconds);
  launch();
  turnHeading(171);
  intake.spin(reverse, 12, volt);
  driveStraight(49, 171, 3, 6, true, 8);
  turnHeading(225);
  extension.set(true);
  launch();
  wait(0.7, seconds);
  launch();
  return;
  driveStraight(-5, 171);
  turnHeading(270);
  turnRoller(270, 2.5, false);
  driveStraight(-16, 270);
  turnHeading(180);
  turnRoller(180, 2, false);
  driveStraight(-20, 180);
  turnHeading(225);
  extension.set(true);
  return;


  turnHeading(315);
  driveStraight(-13, 315);
  //driveStraight(10, 315);
  turnHeading(180);
  intake.spin(forward, 12, volt);
  driveStraight(15, 180);
  turnRoller(180, 2, false);
  driveStraight(-12, 180, 2);
  turnHeading(270);
  intake.spin(reverse, 12, volt);
  driveStraight(15, 270, 3, 2, true, 5);
  timeDriveStraight(1.5, 270, 3);
  driveStraight(-12, 270);
  turnHeading(310);
  driveStraight(-10, 310, 3);
  turnHeading(282);
  driveStraight(-33, 282);
  turnHeading(260);
  driveStraight(10, 260);
  launch();
  wait(0.8, seconds);
  launch();
  driveStraight(-5, 260);
  turnHeading(282);
  driveStraight(40, 282);
  turnHeading(225);
  extension.set(true);
  return;


  turnHeading(162);
  driveStraight(-27, 162);
  turnHeading(180);
  intake.stop();
  launch();
  wait(0.5, seconds);
  launch();
  wait(0.5, seconds);
  launch();
  turnHeading(171);
  intake.spin(reverse, 12, volt);
  driveStraight(49, 171, 3, 6, true, 4);
  driveStraight(-5, 171);
  turnHeading(270);
  turnRoller(270, 2.5, false);
  driveStraight(-16, 270);
  turnHeading(180);
  turnRoller(180, 2, false);
  driveStraight(-9, 180);
  turnHeading(130+180);
  driveStraight(-14, 130+180, 3);
  turnHeading(100+180);
  driveStraight(-24, 100+180);
  turnHeading(81+180);
  driveStraight(8, 81+180);
  launch();
  driveStraight(-14, 81+180);
  turnHeading(135+180);
  driveStraight(29, 135+180, 3, 3);
  turnHeading(225+180);
  intake.spin(reverse, 12, volt);
  driveStraight(62, 225+180, 3, 6, true, 4);
  turnHeading(162+180);
  driveStraight(-27, 162+180);
  turnHeading(180+180);
  intake.stop();
  launch();
  wait(0.5, seconds);
  launch();
  wait(0.5, seconds);
  launch();
  return;
  driveStraight(40, 180);
  turnHeading(90);
  intake.setVelocity(100, percent);
  intake.spinFor(reverse, 1, seconds);
  turnHeading(270);
  turnRoller(270, 1.5, false);
  driveStraight(-12, 270);
  turnHeading(180);
  turnRoller(180, 2.5, false);
  driveStraight(-19, 180);
  turnHeading(225);
  driveStraight(12, 225);
  extension.set(true);
  return;
  turnHeading(120, false, 1);
  intake.spin(reverse, 12, volt);
  driveStraight(40, 120, 3, 4, true, 6);
  intake.stop();
  driveStraight(-40, 120);
  turnHeading(105);
  launch();
  wait(0.5, seconds);
  launch();
  wait(0.5, seconds);
  launch();
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

void a_left() { turnRoller(0, 0.2, false); }

void auton_left() {
  flywheelScaler = 0.616;
  thread t = thread(flyThread);
  a_left();
  driveStraight(-1, 0, 1, 0.5);
  turnHeading(45, false, 0.7);
  timeDriveStraight(0.5, 45, 0);
  driveStraight(-44, 45);
  turnHeading(-38, false, 1.5);
  driveStraight(-4, -38);
  turnHeading(-45);
  timeDriveStraight(0.5, -45, 0);
  launch();
  wait(1, seconds);
  launch();
  flywheel.stop();
}

void a_right() {
  driveStraight(23, 0, 2);
  turnHeading(90);
  turnRoller(90, 1, false);
}

void solo() {
  flywheelScaler = 0.618;
  thread t = thread(flyThread);
  a_left();
  driveStraight(-1, 0, 1, 0.5);
  turnHeading(45, false, 0.7);
  timeDriveStraight(0.5, 45, 0);
  driveStraight(-44, 45);
  turnHeading(-38, false, 1.5);
  driveStraight(-4, -38);
  turnHeading(-43);
  timeDriveStraight(0.5, -43, 0);
  launch();
  wait(1, seconds);
  launch();
  flywheel.stop();
  turnHeading(-135, false, 1);
  driveStraight(44, -135);
  turnHeading(-150, false, 0.5);
  driveStraight(5, -150, 3, 1.7);
  turnHeading(-90, false, 1);
  turnRoller(-100, 1);
}

void autonomous(void) {
  // controller1.Screen.print(isRight);
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  //driveStraight(30, 0);
  //solo();
  auton_left();
  //a_right();
  //a_left();
  //skills();
  //extension.set(true);
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
  // thread thread1 = thread(odom);
  leftDrivetrain.setStopping(coast);
  rightDrivetrain.setStopping(coast);
  controller1.ButtonX.pressed(switchIntake);
  controller1.ButtonY.pressed(switchFlywheel);
  controller1.ButtonR2.pressed(launch);
  controller1.ButtonDown.pressed(decrementFlywheel);
  controller1.ButtonUp.pressed(incrementFlywheel);
  controller1.ButtonLeft.pressed(updateExtension);
  Brain.Screen.setFillColor("blue");
  flywheelScaler = 0.51;
  thread t = thread(flyThread);
  // User control code here, inside the loop
  while (1) {

    if (int(round(Brain.timer(seconds))) % 7 == 0) {
      extensionVal = 0;
    }

    if (controller1.ButtonDown.pressing()) {
      decrementFlywheel();
    }

    if (controller1.ButtonUp.pressing()) {
      incrementFlywheel();
    }

    double speedScaler = 1;
    // Drivetrain Arcade Steering
    if (abs(controller1.Axis3.position()) > 0 ||
        abs(controller1.Axis4.position()) > 0) {
      leftDrivetrain.spin(forward,
                          speedScaler * (controller1.Axis3.position() +
                                          controller1.Axis4.position()),
                          percent);
      rightDrivetrain.spin(forward,
                           speedScaler * (controller1.Axis3.position() -
                                           controller1.Axis4.position()),
                           percent);
    } else {
      leftDrivetrain.spin(forward, 0, percent);
      rightDrivetrain.spin(forward, 0, percent);
    }
    // controller1.Screen.setCursor(4, 6);
    // controller1.Screen.print(flywheel.temperature());
    intake.spin(forward,
                (2 * int(controller1.ButtonL2.pressing()) -
                 int(intakeSpinning) - int(controller1.ButtonL1.pressing())) *
                    85,
                percent);
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