#include "vex.h"
#include <cmath>
#include "global.h"

using namespace vex;

bool flywheelSpinning = true;
double flywheelScaler = 0.6;
double FLYWHEEL_RPM = 600;

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