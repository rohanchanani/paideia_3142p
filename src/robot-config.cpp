#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;
controller controller1 = controller();
bool reversal = false;
motor RBMotor = motor(PORT11, gearSetting::ratio6_1, reversal);
motor RMMotor = motor(PORT20, gearSetting::ratio6_1, reversal);
motor RFMotor = motor(PORT12, gearSetting::ratio6_1, reversal);
motor LBMotor = motor(PORT14, gearSetting::ratio6_1, !reversal);
motor LMMotor = motor(PORT15, gearSetting::ratio6_1, !reversal);
motor LFMotor = motor(PORT16, gearSetting::ratio6_1, !reversal);
motor flywheel = motor(PORT7, gearSetting::ratio6_1, false);
motor intake = motor(PORT18, gearSetting::ratio6_1, true);
motor_group leftDrivetrain = motor_group(LBMotor, LMMotor, LFMotor);
motor_group rightDrivetrain = motor_group(RBMotor, RMMotor, RFMotor);
drivetrain robotDrive = drivetrain(leftDrivetrain, rightDrivetrain, 260, 317.5, 284.5, distanceUnits::mm, 1.15); 
digital_out launcher = digital_out(Brain.ThreeWirePort.B);
digital_out extension = digital_out(Brain.ThreeWirePort.C);
encoder vertical = encoder(Brain.ThreeWirePort.G);
encoder horizontal = encoder(Brain.ThreeWirePort.E);
inertial inertial1 = inertial(PORT10);
optical optical1 = optical(PORT18);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
