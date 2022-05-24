#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller controller1 = controller();
bool reversal = true;
motor LBMotor = motor(PORT11, gearSetting::ratio6_1, reversal);
motor LMMotor = motor(PORT12, gearSetting::ratio6_1, !reversal);
motor LFMotor = motor(PORT13, gearSetting::ratio6_1, reversal);
motor RBMotor = motor(PORT14, gearSetting::ratio6_1, !reversal);
motor RMMotor = motor(PORT15, gearSetting::ratio6_1, reversal);
motor RFMotor = motor(PORT16, gearSetting::ratio6_1, !reversal);
motor fourBar = motor(PORT7, gearSetting::ratio36_1, true);
motor intake = motor(PORT8, gearSetting::ratio6_1, false);
motor_group leftDrivetrain = motor_group(LBMotor, LMMotor, LFMotor);
motor_group rightDrivetrain = motor_group(RBMotor, RMMotor, RFMotor);
drivetrain robotDrive = drivetrain(leftDrivetrain, rightDrivetrain, 320, 343, 254, distanceUnits::mm, 0.66666667);
digital_out tiltClamp = digital_out(Brain.ThreeWirePort.A);
digital_out tiltLift = digital_out(Brain.ThreeWirePort.B);
digital_out fourBarClamp = digital_out(Brain.ThreeWirePort.E);
encoder distanceEncoder = encoder(Brain.ThreeWirePort.G);
inertial inertial1 = inertial(9);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
