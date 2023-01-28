#include "vex.h"
#include <cmath>
#include "global.h"

using namespace vex;

void match_load() {
  wait(0.8, seconds);
  launch();
}

void a_left() { 
  intake.spin(forward, 12, volt);
  timeDriveStraight(0.3, 0, 3);
  intake.stop(); 
}

void auton_left() {
  flywheelSpinning = true;
  flywheelScaler = 0.69;
  thread t = thread(flyThread);
  a_left();
  driveStraight(-6, 0, 3, 3);
  turnHeading(-12);
  timeDriveStraight(2, -12, 0);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  launch();
  wait(0.5, seconds);
  launch();
  turnHeading(-128);
  flywheelScaler = 0.7;
  intake.spin(reverse, 12, volt);
  driveStraight(15, -128, 2, 2);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  wait(0.5, seconds);
  driveStraight(12.5, -128, 3, 3, true, 6);
  turnHeading(-31);
  timeDriveStraight(0.3, -28, 0);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  launch();
  wait(0.5, seconds);
  launch();
  wait(0.5, seconds);
  launch();
  intake.stop();
}

void a_right() {
  driveStraight(23, 0, 2);
  turnHeading(90);
  intake.spin(forward, 12, volt);
  timeDriveStraight(0.3, 90, 3);
  intake.stop(); 
}

void solo() {
  flywheelSpinning = true;
  flywheelScaler = 0.69;
  thread t = thread(flyThread);
  a_left();
  driveStraight(-6, 0, 1, 2);
  turnHeading(-130);
  intake.spin(reverse, 12, volt);
  driveStraight(9, -130, 0, 2);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  wait(0.5, seconds);
  driveStraight(13, -130, 3, 2, true, 6);
  turnHeading(-27);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  wait(0.3, seconds);
  launch();
  wait(0.5, seconds);
  launch();
  wait(0.7, seconds);
  launch();
  turnHeading(-135);
  intake.spin(reverse, 12, volt);
  driveStraight(63, -135, 4, 5, true, 10);
  //turnHeading(-68);
  //leftDrivetrain.stop();
  //rightDrivetrain.stop();
  //launch();
  //wait(0.5, seconds);
  //launch();
  //wait(0.5, seconds);
  //launch();
  intake.stop();
  flywheel.stop();
  turnHeading(-90);
  driveStraight(8, -90);
  intake.spin(forward, 12, volt);
  timeDriveStraight(1, -90, 3);
  intake.stop();
}

void auton_right() {
  flywheelScaler = 0.6;
  thread t = thread(flyThread);
  intake.spin(reverse, 12, volt);
  driveStraight(13, 0, 2, 2);
  turnHeading(70);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  launch();
  wait(0.5, seconds);
  launch();
  turnHeading(-45);
  flywheelScaler = 0.622;
  driveStraight(33, -45, 3, 3, true, 8);
  turnHeading(-123);
  driveStraight(-8, -123);
  turnHeading(-123);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  launch();
  wait(0.5, seconds);
  launch();
  wait(0.5, seconds);
  launch();
  flywheel.stop();
  intake.stop();
  turnHeading(-223);
  driveStraight(58, -223);
  turnHeading(-180);
  intake.spin(forward, 12, volt);
  timeDriveStraight(0.5, -180, 3);
  intake.stop();
}

void old_solo() {
  flywheelScaler = 0.66;
  thread t = thread(flyThread);
  a_left();
  driveStraight(-1, 0, 1, 0.5);
  turnHeading(45, false, 0.7);
  timeDriveStraight(0.5, 45, 0);
  driveStraight(-44, 45);
  turnHeading(-38, false, 1.5);
  driveStraight(-4, -38);
  turnHeading(-45);
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
  intake.spin(forward, 12, volt);
  timeDriveStraight(1.2, -90, 3);
  intake.stop();
}

void skills() {
  flywheelScaler = 0.545;
  thread t = thread(flyThread);
  //thread j = thread(intakeThread);
  turnRoller(0, 0.5, 0);
  driveStraight(-8, 0, 2);
  turnHeading(90);
  driveStraight(12, 90, 2, 2);
  turnRoller(90, 1, 0);
  driveStraight(-14, 90, 2);
  turnHeading(100);
  driveStraight(-34, 100, 3);
  turnHeading(88);
  launch();
  wait(0.8, seconds);
  launch();
  while (numDiscs()) {
    intake.spin(reverse, 12, volt);
    launch();
    wait(0.5, seconds);
  }
  intake.stop();
  turnHeading(125);
  driveStraight(21, 160);
  turnHeading(215);
  flywheelScaler = .56;
  intake.spin(reverse, 12, volt);
  driveStraight(45, 220, 3, 3, true, 8);
  turnHeading(172);
  driveStraight(-17, 172, 3, 3, true, 11.5);
  if (numDiscs()==3) {
    intake.stop();
  }
  turnHeading(174);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  launch();
  wait(0.5, seconds);
  launch();
  wait(1, seconds);
  launch();
  while (numDiscs()) {
    intake.spin(reverse, 12, volt);
    launch();
    wait(0.5, seconds);
  }
  intake.stop();
  turnHeading(165);
  intake.spin(reverse, 12, volt);
  driveStraight(22, 165, 2, 2);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  wait(0.5, seconds);
  driveStraight(17, 165, 3, 3, true, 6);
  driveStraight(-37, 165, 3);
  if (numDiscs()==3) {
    intake.stop();
  }
  turnHeading(172);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  launch();
  wait(0.5, seconds);
  launch();
  wait(1, seconds);
  launch();
  while (numDiscs()) {
    intake.spin(reverse, 12, volt);
    launch();
    wait(0.5, seconds);
  }
  intake.stop();
  turnHeading(160);
  driveStraight(15, 160, 2, 1);
  intake.spin(reverse, 12, volt);
  turnHeading(180);
  driveStraight(28, 180);
  turnRoller(180, 0.7, 0);
  driveStraight(-12, 180, 2, 1.5);
  turnHeading(270);
  driveStraight(15, 270, 2, 2);
  turnRoller(270, 1, 0);
  driveStraight(-20, 270, 2, 2);
  while (numDiscs()) {
    intake.spin(reverse, 12, volt);
    launch();
    wait(0.5, seconds);
  }
  intake.stop();
  turnHeading(65);
  intake.spin(reverse, 12, volt);
  driveStraight(9, 65, 2, 1);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  wait(0.5, seconds);
  turnHeading(65);
  driveStraight(24, 65, 3, 2.5, true, 6);
  if (numDiscs()>2) {
    intake.spin(forward, 12, volt);
    if (numDiscs()>3) {
      launch();
    }
  }
  turnHeading(305);
  driveStraight(-4, 305, 1);
  turnHeading(295);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  launch();
  wait(0.5, seconds);
  launch();
  wait(1, seconds);
  launch();
  while (numDiscs()) {
    intake.spin(reverse, 12, volt);
    launch();
    wait(0.5, seconds);
  }
  intake.stop();
  wait(0.1, seconds);
  turnHeading(370);
  intake.spin(reverse, 12, volt);
  driveStraight(15, 370, 3, 2, true, 8);
  turnHeading(405);
  while (numDiscs() > 1) {
    intake.spin(reverse, 12, volt);
    launch();
    wait(0.5, seconds);
  }
  driveStraight(30, 405, 3, 3, true, 8);
  turnHeading(352);
  driveStraight(-17, 352, 3, 3, true, 11.5);
  turnHeading(355);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  launch();
  wait(0.5, seconds);
  launch();
  wait(1, seconds);
  launch();
  driveStraight(34, 360);
  turnHeading(405);
  driveStraight(8, 405);
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  extension.set(true);
}

void match_skills() {
  flywheelSpinning = false;
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
  driveStraight(6.8, 79);
  launch();
  wait(0.8, seconds);
  launch();
  repeat(9) {
    match_load();
  }
  driveStraight(-14, 81);
  turnHeading(135);
  driveStraight(27, 135, 3, 3);
  turnHeading(225);
  intake.spin(reverse, 12, volt);
  driveStraight(53, 225, 3, 6, true, 12);
  turnHeading(180);
  launch();
  driveStraight(35, 180, 3, 6, true, 4);
  driveStraight(-12, 180);
  turnHeading(270);
  timeDriveStraight(2, 270, 4);
  intake.stop();
  driveStraight(-7, 270, 1);
  turnHeading(310);
  driveStraight(-13, 310, 2);
  turnHeading(282);
  driveStraight(-10, 282, 2);
  turnHeading(275);
  driveStraight(-12, 275, 2);
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