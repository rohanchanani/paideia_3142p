#include "vex.h"
#include <cmath>
#include "global.h"

using namespace vex;

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
  flywheelScaler = 0.52;
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