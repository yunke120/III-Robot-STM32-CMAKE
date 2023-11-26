
#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include <stdint.h>

#define PROTOCOL_MSG_LEN 12

#define FrameHeader 0xAA
#define FrameTail 0xDD

struct UInt16Parts {
    uint8_t lowByte;
    uint8_t highByte;
};

union UInt16Union {
    short value;
    struct UInt16Parts parts;
};


typedef enum
{
	DEVNone = 0x00,
	Robot,
	PTZ,
	RoboticArm,
	Battery,
	IMU,
	Sensor,
} eDEVICE;

/* command send prase */
typedef enum
{
	S_Reserved,
	S_RobotVelocity,
	S_RobotIMU,
	S_RobotBatVoltage,
	S_RobotGetMQ2,
}eSRobot;

typedef enum
{
    R_RobotVelocity = 0x20,
	HandShake = 0xFF,
} eRRobot;

typedef enum
{
	GetVoltage = 0x01,
} eBattery;

typedef enum
{
	AngularVelocity = 0x01, 
	Posture,				
	Quaternion,				
}eIMU;

typedef enum
{
	GetMQ2 = 0x01,
}eSensor;

#endif /* __PROTOCOL_H */
