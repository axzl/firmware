#include "comm.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  init_device();
  Serial.flush();
  Serial.print("OK");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50); //等待读取完成
  int buff_num = Serial.available();
  if (buff_num <= 0) {
    return;
  }
  byte * buff = (byte *)malloc(buff_num);
  for (int i = 0; i < buff_num; i++) {
    buff[i] = Serial.read();
  }
  byte begin_one = buff[0]; //起始位1
  byte begin_two = buff[1]; //起始位2
  byte action = buff[2]; //传感器类型
  byte port = buff[3]; //设备端口
  byte device = buff[4]; //设备号
  byte * data = buff + 5; //数据
  if (begin_one != 0xff || begin_two != 0x55) {
    return;
  }
  switch (action) {
    case SENSOR:  //传感器
      get_sensor(port, device, data);
      break;
    case ACTUATOR: //执行器
      run_actuator(port, device, data);
      break;
    default:
      break;
  }
  Serial.flush();
}

