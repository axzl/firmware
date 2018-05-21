#include "comm.h"

//================================= 全局变量 =======================================

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); //RGB颜色传感器

//================================= 基础函数 =======================================
union {
  byte byteval[4];
  float floatval;
  long integer;
} val_32;

union{
  byte byteval[2];
  uint16_t integer;
}val_16;

void sendDataFloat(float value) { //发送一个浮点数
  val_32.floatval = value;
  sendData(val_32.byteval, FLOAT_TYPE, 4);
}

void sendDataFloats(float * value, byte num) { //发送浮点数
  byte * temp = (byte *)malloc(4 * num);
  for(int i = 0; i < num; i++){
     val_32.floatval = value[i];
     temp[0 + i * 4] = val_32.byteval[0];
     temp[1 + i * 4] = val_32.byteval[1];
     temp[2 + i * 4] = val_32.byteval[2];
     temp[3 + i * 4] = val_32.byteval[3];
  }
  sendData(temp, FLOAT_TYPE, 4 * num);
}

void sendDataInteger(long value) { //发送一个长整数
  val_32.integer = value;
  sendData(val_32.byteval, LONG_TYPE, 4);
}

void sendDataIntegers(long * value, byte num) { //发送长整数
  byte * temp = (byte *)malloc(4 * num);
  for(int i = 0; i < num; i++){
     val_32.integer = value[i];
     temp[0 + i * 4] = val_32.byteval[0];
     temp[1 + i * 4] = val_32.byteval[1];
     temp[2 + i * 4] = val_32.byteval[2];
     temp[3 + i * 4] = val_32.byteval[3];
  }
  sendData(temp, LONG_TYPE, 4 * num);
}

void sendDataShort(uint16_t value) { //发送1个短整数
  val_16.integer = value;
  sendData(val_16.byteval, SHORT_TYPE, 2);
}

void sendDataShorts(uint16_t * value, byte num) { //发送短整数
  byte * temp = (byte *)malloc(2 * num);
  for(int i = 0; i < num; i++){
     val_16.integer = value[i];
     temp[0 + i * 2] = val_16.byteval[0];
     temp[1 + i * 2] = val_16.byteval[1];
  }
  sendData(temp, SHORT_TYPE, 2 * num);
}

void sendDataByte(byte value) { //发送1个byte
  byte val[1] = {value};
  sendData(val, BYTE_TYPE, 1);
}

void sendData(byte * value, byte type, byte num) { //发送数据
  Serial.write(0xff);
  Serial.write(0x55);
  Serial.write(0x00);
  Serial.write(type);
  for(int i = 0; i < num; i++){
    Serial.write(value[i]);
  }
  Serial.write(0x0d);
  Serial.write(0x0a);
}

//================================= 传感器类型 =======================================

void get_sensor(byte port, byte device, byte * data) { //传感器
  switch (device) {
    case JOYSTICK: //0x01
      joystick(port, data); //摇动杆
      break;
    case RGB_C_S:
      rgbColorSensor(port, data); // 颜色传感器
      break;
    default:
      break;
  }
}

void run_actuator(byte port, byte device, byte * data) { //执行器
  switch (device) {
    case LED: //0x01
      led(port, data); //LED
      break;
    case CLOR3://0x02
      clor3(port, data); //三色灯
      break;
    default:
      break;
  }
}

void init_device() { //设备初始化
  //============ 颜色传感器 ============
  if (tcs.begin()) {
    Serial.write(RGB_C_S);
  } else {
    Serial.write(RGB_C_S);
    Serial.write(0x00);
    while (1); // halt!
  }
  //============ RGB三色灯 ============
  pinMode(LED_R, OUTPUT);  
  pinMode(LED_G, OUTPUT);  
  pinMode(LED_B, OUTPUT);
  
}

//=====================================传感器===================================

void joystick(byte port, byte * data) { //摇动杆
  float value = analogRead(A0);
  sendDataFloat(value);
}

void rgbColorSensor(byte port, byte * data){ //颜色传感器
  uint16_t crgb[4];
  tcs.setInterrupt(false); //turn on LED
  delay(60); //takes 50ms to read 
  tcs.getRawData(crgb, crgb+1, crgb+2, crgb+3);
  tcs.setInterrupt(true); //turn off LED
  sendDataShorts(crgb,4);
  //  Serial.print("C:\t"); Serial.print(crgb[0]);
  //  Serial.print("\tR:\t"); Serial.print(crgb[1]);
  //  Serial.print("\tG:\t"); Serial.print(crgb[2]);
  //  Serial.print("\tB:\t"); Serial.print(crgb[3]);
}

//=====================================执行器===================================

void led(byte port, byte * data) { //LED
  pinMode(port, OUTPUT); //LED
  byte echo = 2;
  if (data[0] == 0) { //关灯
    digitalWrite(port, LOW);
    echo = 0;
  }
  else if (data[0] == 1) { //开灯
    digitalWrite(port, HIGH);
    echo = 1;
  }
  sendDataByte(echo);
}

void clor3(byte port, byte * data){
  analogWrite(LED_R, data[0]>>2); //R
  analogWrite(LED_G, data[1]>>2); //G
  analogWrite(LED_B, data[2]>>2); //B
  sendDataByte(0); //OK
}

