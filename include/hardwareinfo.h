#include "vex.h"

using namespace vex;

// Motors
// vex::motor LeftBack (vex::PORT3, vex::gearSetting::ratio18_1, false);
// vex::motor RightFront (vex::PORT4, vex::gearSetting::ratio18_1, true);
// vex::motor LeftFront (vex::PORT1, vex::gearSetting::ratio18_1, false);
// vex::motor RightBack (vex::PORT2, vex::gearSetting::ratio18_1, true);
// vex::motor Intake1 (vex::PORT9, vex::gearSetting::ratio18_1, false);
// vex::motor Intake2 (vex::PORT7, vex::gearSetting::ratio18_1, true);
// vex::motor Arm (vex::PORT8, vex::gearSetting::ratio18_1, false);
// vex::motor holdUp (vex::PORT5, vex::gearSetting::ratio18_1, false);
motor TEST (vex::PORT1, vex::gearSetting::ratio18_1, false); // Param: (Port, GearSetting, Reverse);




motor WHEEL_FBMOVEMENT[] = {TEST};
motor WHEEL_ROTATE_LEFTSET[] = {};
motor WHEEL_ROTATE_RIGHTSET[] = {};
motor MOTOR_PULL_BALL[] = {};

// Sensors
triport threeWirePort(PORT22);
encoder testShaftEncoder = encoder(threeWirePort.A);
vision testVisionSensor = vision(11);