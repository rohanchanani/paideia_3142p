#include "vex.h"
#include <cmath>
#include "global.h"

using namespace vex;

bool flywheelSpinning = true;
double flywheelScaler = 0.6;
double FLYWHEEL_RPM = 600;
double FLY_CONSTANT = 20; 

void flyThread() {
  while (true) {
    wait(10, msec);
    if (flywheelSpinning) {
      double flywheelConst = FLY_CONSTANT * (flywheelScaler - flywheel.velocity(rpm) / FLYWHEEL_RPM);
      flywheel.spin(forward, 12 * (flywheelScaler + flywheelConst), volt);
    }
    else {
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