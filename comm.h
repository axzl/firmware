#ifndef COMM_H
#define COMM_H

#include <arduino.h>
#include "Adafruit_TCS34725.h"

//三色灯引脚定义
#define LED_R            9
#define LED_G           10
#define LED_B           11

//数据类型
#define BYTE_TYPE      0x01
#define SHORT_TYPE     0x02
#define LONG_TYPE      0x03
#define FLOAT_TYPE     0x04

//传感器类型
#define SENSOR         0x01
#define ACTUATOR       0x02

//传感器
#define JOYSTICK       0x01
#define RGB_C_S        0x02

//执行器
#define LED            0x01
#define CLOR3          0x02

//================================= 基础函数 =======================================

void sendDataFloat(float value);
void sendDataFloats(float * value, byte num);
void sendDataInteger(long value);
void sendDataIntegers(long * value, byte num);
void sendDataShort(uint16_t value);
void sendDataShorts(uint16_t * value, byte num);
void sendDataByte(byte value);
void sendData(byte * value, byte type, byte num);

//================================= 传感器类型 =======================================

void get_sensor(byte port, byte device, byte * data); //传感器
void run_actuator(byte port, byte device, byte * data); //执行器
void init_device();

//===================================== 传感器 ===================================

void joystick(byte port, byte * data); //摇动杆
void rgbColorSensor(byte port, byte * data); //颜色传感器

//===================================== 执行器 ===================================

void led(byte port, byte * data);
void clor3(byte port, byte * data);

#endif
