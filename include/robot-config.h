using namespace vex;

extern brain Brain;
extern controller controller1;
extern motor LBMotor;
extern motor LMMotor;
extern motor LFMotor;
extern motor RMMotor;
extern motor RBMotor;
extern motor RFMotor;
extern motor flywheel;
extern motor intake;
extern motor_group leftDrivetrain;
extern motor_group rightDrivetrain;
extern digital_out launcher;
extern digital_out extension;
extern encoder vertical;
extern encoder horizontal;
extern inertial inertial1;
extern drivetrain robotDrive;
extern optical optical1;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
