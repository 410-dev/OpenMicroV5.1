#include "vex.h"
#include <string>
#include <sstream>
#include <vector>

#include "micro/MDisplay.h"
#include "micro/MRemote.h"
#include "micro/MMotor.h"
#include "micro/MTypeConvert.h"
#include "micro/MSensor.h"
#include "micro/hardlink.h"

using namespace vex;
using namespace std;

vector<int> encLeft = {0, 0};
vector<int> encRight = {0, 0};
vector<int> encSide = {0, 0};
vector<int> times = {0, 0};
int init_time = -1;

bool actionUpdate = false;
float multiplier = 1.0;
MDisplay debug_display;

string recentActivity = "";
string FWD = "";
string LFT = "";

int condition = 0;
float a, b, c, d, e, f;
float speed = 25.0;

bool MRemote::updateAll(controller Controller, bool liveControl) {

  MTypeConvert convert;
  MSensor srutil;

  if(liveControl){
    debug_display.setValueOfLine(10, 0, "I AM LIVE");
  }
  else{
    debug_display.setValueOfLine(10, 0, "BRUH");
  }

  if (!liveControl) {
    //encLeft.at(encLeft.size() - 1) = srutil.getShaftEncoderValue(ENCODER_LEFT);
    //encRight.at(encRight.size() - 1) = srutil.getShaftEncoderValue(ENCODER_RIGHT);
    //encSide.at(encSide.size() - 1) = srutil.getShaftEncoderValue(ENCODER_SIDE);
    encLeft.push_back(srutil.getShaftEncoderValue(ENCODER_LEFT));
    encRight.push_back(srutil.getShaftEncoderValue(ENCODER_RIGHT));
    encSide.push_back(srutil.getShaftEncoderValue(ENCODER_SIDE));

    if(init_time == -1) init_time = Brain.timer(vex::timeUnits::msec);
    //times.at(times.size() - 1) = 
    times.push_back(Brain.timer(vex::timeUnits::msec) - init_time);
  }
  a=0;
  b=0;
  c=0;
  d=0;
  e=0;
  f=0;

  FWD = convert.convertToString((int) Controller.Axis3.value());
  LFT = convert.convertToString((int) Controller.Axis4.value());

  multiplier = 1.0;
  condition = 0;
  if (Controller.ButtonL1.pressing()){
    multiplier = 2.0;
  }
  if (Controller.ButtonL2.pressing()){
    multiplier = 4.0;
  }

  if (Controller.ButtonUp.pressing() || Controller.Axis3.value() > 60){
    e = 1;
    a = 100.0 * multiplier;
  }
  if (Controller.ButtonDown.pressing() || Controller.Axis3.value() < -60){
    e = 1;
    a = -100.0 * multiplier;
  }
  if (Controller.ButtonLeft.pressing() || Controller.Axis4.value() < -60 || Controller.Axis1.value() < -60){
    e = 1;
    b = 100 * multiplier;
  }
  if (Controller.ButtonRight.pressing() || Controller.Axis4.value() > 60 || Controller.Axis1.value() > 60){
    e = 1;
    b = -100 * multiplier;
  }
  if (Controller.ButtonA.pressing() || Controller.Axis2.value() > 60){
    e = 1;
    onPress_startPuller();
  }
  if (0==e){
    onRelease_wheels();
    onRelease_puller();
  }
  else{
    execute(a, b);
  }
  if (Controller.ButtonX.pressing() && Controller.ButtonL2.pressing()){
    onPress_systemTerminate();
    return 1;
  }
  return 0;
}

void MRemote::execute(int a, int b) {
  recentActivity = "REMOTE: MOVEMENT";
  WHEEL_FRONT_LEFT.spin(directionType::fwd, (a-b)/2, velocityUnits::pct);
  WHEEL_FRONT_RIGHT.spin(directionType::fwd, (a+b)/2, velocityUnits::pct);
  WHEEL_BACK_LEFT.spin(directionType::fwd, (a-b)/2, velocityUnits::pct);
  WHEEL_BACK_RIGHT.spin(directionType::fwd, (a+b)/2, velocityUnits::pct);
}

void MRemote::onRelease_wheels() {
  MMotor mtctl;
  recentActivity = "REMOTE: RELEASED";
  motor allmotors[] = {WHEEL_FRONT_LEFT,WHEEL_BACK_RIGHT,WHEEL_BACK_LEFT,WHEEL_FRONT_RIGHT};
  mtctl.stopMotors(allmotors, 4);
}

void MRemote::onPress_startPuller() {
  MMotor mtctl;
  recentActivity = "REMOTE: PULL";
  motor pullers[] = {PULL_MOTOR_1, PULL_MOTOR_2, INTAKE_LEFT, INTAKE_RIGHT};
  mtctl.runMotors(pullers, 4, directionType::fwd, speed*multiplier);
}

void MRemote::onPress_startPuller2() {
  MMotor mtctl;
  recentActivity = "REMOTE: PULL";
  motor pullers[] = {PULL_MOTOR_1, PULL_MOTOR_2, INTAKE_LEFT, INTAKE_RIGHT};
  mtctl.runMotors(pullers, 4, directionType::rev, speed*multiplier);
}

void MRemote::onRelease_puller() {
  MMotor mtctl;
  motor pullers[] = {PULL_MOTOR_1, PULL_MOTOR_2, INTAKE_LEFT, INTAKE_RIGHT};
  mtctl.stopMotors(pullers, 4);
}

void MRemote::onPress_systemTerminate() {
  MMotor mtctl;
  recentActivity = "REMOTE: STOP";
  mtctl.stopMotors(MOTOR_ALL, 8);
}