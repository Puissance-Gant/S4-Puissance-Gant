#include "DynamixMotor.h"

DynamixelWorkbench dxl_wb;  // create DynamixelWorkbench object to control dynamixelS

/**
 * This function initialises a DynamixMotor object
 * 
 * @param id contains the ID of dynamixel motor
 * @param zero the position of motor corresponding to its zero (STEP)
 * @param home the position of motor corresponding to home (STEP)
 * @param max_limmit the position of motor corresponding to its max value (STEP)
 * @param speed the speed of motor corresponding to its velocity
 * @param reverse indicates if motor has to turn the other side
*/
DynamixMotor::DynamixMotor(uint8_t id, uint16_t zero, uint16_t home, uint16_t max_limit, uint16_t speed, bool reverse) {
    this->id = id;

    this->zero      = zero;
    this->home      = home;
    this->max_limit = max_limit;
    this->velocity  = speed;
    this->step      = speed >> 2;
    this->reversed  = reverse;

    cmdSucceeded = dxl_wb.init(DEVICE_NAME, baud);
    cmdSucceeded = dxl_wb.ping((int8_t)id, &model_number, &log);

    if (reverse)
        cmdSucceeded = dxl_wb.setReverseDirection(id, &log);
    else
        cmdSucceeded = dxl_wb.setNormalDirection(id, &log);

    cmdSucceeded = dxl_wb.jointMode(id, this->velocity, 0, &log);    
    cmdSucceeded = dxl_wb.ledOn((uint8_t)id, &log);

    /* Unit	0.229 [rev/min]	Sets velocity of the Profile
       * Unit	1 [msec]	Sets the time span for the Profile
       * Range	0 ~ 32767	‘0’ represents an infinite velocit */
    cmdSucceeded = dxl_wb.itemWrite(id, "Profile_Velocity", this->velocity, &log);
    /* Unit	214.577 [rev/min2]	Sets acceleration of the Profile
       * Unit	1 [msec]	Sets accelerating time of the Profile
       * Range	0 ~ 32767	‘0’ represents an infinite acceleration */
    cmdSucceeded = dxl_wb.itemWrite(id, "Profile_Acceleration", this->accelaration, &log);

    // TO ADD FOR MORE SECURITY
    // cmdSucceeded = dxl_wb.itemWrite(id, "Max_Position_Limit", max_limit, &log);
    // cmdSucceeded = dxl_wb.itemWrite(id, "Min_Position_Limit", zero, &log);
}

void DynamixMotor::go_to(int16_t pos) {
    if (pos < zero)
        pos = zero;
    if (pos > max_limit)
        pos = max_limit;
    position = pos;
    dxl_wb.goalPosition(id, (int32_t)pos, &log);
};

// CONVERT STEP TO DEGREES := 4096 STEP / 360 DEGREES = 11,3778 STEP BY DEGREES
void DynamixMotor::go_to_degrees(int16_t pos) { this->go_to((pos * 11.3778) + zero); }
void DynamixMotor::go_to_home() { this->go_to(home); }

void DynamixMotor::go_forward() {
    int32_t pos = position;
    // int32_t pos = get_position();        // DON'T WORK WITH GRIPPER
    pos -= step;
    go_to(pos);
};

void DynamixMotor::go_backward() {
    int32_t pos = position;
    // int32_t pos = get_position();        // DON'T WORK WITH GRIPPER
    pos += step;
    go_to(pos);
}

int16_t DynamixMotor::get_position() {
    int32_t data = 0;
    cmdSucceeded = dxl_wb.itemRead(id, "Present_Position", &data, &log);
    return data;
}

// DONT USE IN SYM-BOT PROJECT
int16_t DynamixMotor::get_home_offset() {
    int32_t data = 0;
    cmdSucceeded = dxl_wb.itemRead(id, "Homing_Offset", &data, &log);
    return data;
}

void DynamixMotor::setHomingOffset(int16_t pos) {
    cmdSucceeded = dxl_wb.itemWrite(id, "Homing_Offset", pos, &log);
}
