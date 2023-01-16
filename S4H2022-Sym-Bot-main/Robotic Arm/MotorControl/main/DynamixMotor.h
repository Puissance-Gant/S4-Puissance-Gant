#ifndef DYNAMIXMOTOR_H
#define DYNAMIXMOTOR_H

#include <DynamixelWorkbench.h>
#include "Motor.h"

#if defined(__OPENCM904__)
#define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
#define DEVICE_NAME ""
#endif

// Documentation on Dynamixel Motor
// https://emanual.robotis.com/docs/en/software/dynamixel/dynamixel_workbench/#opencr-and-opencm-tutorials
// https://emanual.robotis.com/docs/en/dxl/x/xl430-w250/#control-table-description
// https://emanual.robotis.com/docs/en/dxl/x/xm430-w350/#control-table-description
// Dynamixel Item
// https://github.com/ROBOTIS-GIT/dynamixel-workbench/blob/09e31a8649a7d3b89d76b6bdd2df3407c1622a5c/dynamixel_workbench_toolbox/src/dynamixel_workbench_toolbox/dynamixel_item.cpp

// Definition of the motor class for dynamixel motors
class DynamixMotor: public Motor
{
private:
    const char *log;
    bool cmdSucceeded;

    uint8_t id;
    uint16_t model_number;
    uint16_t baud           = 57600;        // DEFAULT DYNAMIXEL BAUD (57600)
    uint16_t velocity       = 64;           // DEFAULT VELOCITY MOTOR (64)
    uint16_t accelaration   = 16;           // DEFAULT ACCELERATION MOTOR (16)

    uint16_t step           = 4;            // DEFAULT STEP MOTOR (4)
    int16_t position;
    bool reversed;

public:
    DynamixMotor() = delete;                // DISABLE DEFAULT CONSTR
    DynamixMotor(uint8_t, uint16_t, uint16_t, uint16_t, uint16_t, bool);
    ~DynamixMotor() = default;

    uint16_t get_step() { return step; }
    void set_step(uint16_t step) { this->step = step; }

    void go_to(int16_t pos);
    void go_to_degrees(int16_t pos);
    void go_to_home();
    void go_forward();
    void go_backward();

    int16_t get_position();
    // DONT USE IN SYM-BOT PROJECT
    int16_t get_home_offset();
    void setHomingOffset(int16_t pos);
};

#endif
