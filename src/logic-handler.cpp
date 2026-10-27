#include "logic-handler.h"

/**
 * Logic Handler Initialiser
 * 
 * Sets up the Logic Handler's variables, including setting up
 * the current state to Idle, setting the IR Sensor class, the 
 * Stepper Motor class, setting the bulk-packing limit to 0 and
 * setting the last-hold tracker to 0 milliseconds.
 */
LogicHandler::LogicHandler(Sensor& s, StepperMotor& m) :
    currentState(LogicState::IDLE),
    sensor(s),
    motor(m),
    _limit(0),
    _lastHoldMs(0) {}

/**
 * Logic Handler Begin Function
 * 
 * Used in main.cpp's setup function, this function is used for
 * executing the IR sensor and stepper motor's begin functions, 
 * and to home the stepper motor.
 */
void LogicHandler::begin() {
    currentState = LogicState::IDLE;
    sensor.begin();
    motor.begin();
    // TODO: handle analog read on second core or migrate to first core
    // home();
}

/**
 * Logic Handler Home Function
 * 
 * Used to home the stepper motor.
 */
void LogicHandler::home() {
    // Update the State to Homing
    currentState = LogicState::HOMING;
    motor.home();
}

/**
 * Logic Handler Hold Function
 * 
 * Used to drive the stepper motor to the hold position.
 */
void LogicHandler::hold() {
    // Update the State to Holding
    currentState = LogicState::HOLDING;
    motor.moveTo(_holdPosition);
}

/**
 * Logic Handler Release Function
 * 
 * Used to drive the stepper motor to the release position.
 */
void LogicHandler::release() {
    // Update the State to Releasing
    currentState = LogicState::RELEASING;
    motor.moveTo(_releasePosition);
}

/**
 * Logic Handler Update Function
 * 
 * Used in the Logic Handling Task assigned to Core 0 of
 * the ESP32. Returns true if the motor is being driven.
 * 
 * @returns true if motor is being driven, false otherwise
 */
bool LogicHandler::update() {
    // Execute IR Sensor and Stepper Motor Update Functions
    sensor.update();
    motor.update();
    bool motorBusy = motor.isBusy();
    unsigned long now;

    // Logic Functionality changes depending on Current State
    switch (currentState) {
        // Handle Homing State
        case LogicState::HOMING:
            // When homing is complete, set state to Idle
            if (!motorBusy) {
                currentState = LogicState::IDLE;
            }
            break;
        // Handle Counting State
        case LogicState::COUNTING:
            // When Count reaches Bulk-Packing Limit, drive
            // stepper motor to Hold Position
            if (sensor.getCount() >= _limit) {
                _lastHoldMs = 0;
                hold();
            }
            break;
        // Handle Holding State
        case LogicState::HOLDING:
            // Check how long stepper motor has been holding,
            // drive to release position when held for x-time
            now = millis();
            if (now - _lastHoldMs >= _holdTime) {
                sensor.reset();
                release();
            }
            break;
        // Handle Releasing State
        case LogicState::RELEASING:
            // When releasing is complete, resume counting state
            if (!motorBusy) {
                currentState = LogicState::COUNTING;
            }
            break;
        // Handle Manula State
        case LogicState::MANUAL:
            motor.runSpeed();
            break;
        // todo: implement error state
    }
    return motorBusy;
}

void LogicHandler::setState(LogicState state) { currentState = state; }

void LogicHandler::setManual() { motor.enable(); }

void LogicHandler::setAuto() { motor.disable(); }

void LogicHandler::setMotorSpeed(float speed) { motor.setSpeed(speed); }

/**
 * Logic Handler Set Limit Function
 * 
 * Used to set the bulk-packing limit.
 * 
 * @param newLimit new bulk-packing limit
 */
void LogicHandler::setLimit(int newLimit) { _limit = newLimit; }

/**
 * Logic Handler Get Current Count Function
 * 
 * Used to return the current count from the IR Sensor.
 * 
 * @return current count from IR Sensor
 */
int LogicHandler::getCurrentCount() { return sensor.getCount(); }

/**
 * Logic Handler Set Hold Position Function
 * 
 * Used to set the hold position for the stepper motor.
 * 
 * @param pos stepper motor hold position (steps)
 */
void LogicHandler::setHoldPosition(long pos) { _holdPosition = pos; }

/**
 * Logic Handler Set Release Position Function
 * 
 * Used to set the release position for the stepper motor.
 * 
 * @param pos stepper motor release position (steps)
 */
void LogicHandler::setReleasePosition(long pos) { _releasePosition = pos; }

/**
 * Logic Handler Set Hold Time Function
 * 
 * Used to set the hold time for the stepper motor.
 * 
 * @param time stepper motor hold time (milliseconds)
 */
void LogicHandler::setHoldTime(unsigned long time) { _holdTime = time; }

/**
 * Logic Handler Set Motor Pulse Width Function
 * 
 * Used to set the stepper motor pulse width.
 * 
 * @param pw stepper motor pulse width (microseconds)
 */
void LogicHandler::setMotorPulseWidth(int pw) { motor.setPulseWidth(pw); }

/**
 * Logic Handler Set Motor Max Speed Function
 * 
 * Used to set the stepper motor's maximum speed.
 * 
 * @param s stepper motor maximum speed (steps/second)
 */
void LogicHandler::setMotorMaxSpeed(float s) { motor.setMaxSpeed(s); }

/**
 * Logic Handler Set Motor Acceleration Function
 * 
 * Used to set the stepper motor's acceleration.
 * 
 * @param a stepper motor acceleration (steps/second^2)
 */
void LogicHandler::setMotorAcceleration(float a) { motor.setAcceleration(a); }

/**
 * Logic Handler Set Hall Sensitivity Function
 * 
 * Used to set the sensitivity of the homing hall-effect sensor.
 * 
 * @param s sensitivity of hall-effect sensor
 */
void LogicHandler::setHallSensitivity(int s) { motor.setHallSensorSensitivity(s); }