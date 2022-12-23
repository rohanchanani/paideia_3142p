#include "vex.h"
#include <cmath>
#include "global.h"

using namespace vex;

void match_load() {
  wait(0.8, seconds);
  launch();
}

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
}

void old_skills() {
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