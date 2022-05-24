using namespace vex;

extern brain Brain;
extern controller controller1;
extern motor LBMotor;
extern motor LMMotor;
extern motor LFMotor;
extern motor RMMotor;
extern motor RBMotor;
extern motor RFMotor;
extern motor fourBar;
extern motor intake;
extern motor_group leftDrivetrain;
extern motor_group rightDrivetrain;
extern digital_out tiltClamp;
extern digital_out tiltLift;
extern digital_out fourBarClamp;
extern encoder distanceEncoder;
extern inertial inertial1;
extern drivetrain robotDrive;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
