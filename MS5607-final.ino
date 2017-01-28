#include <math.h>

#include <Wire.h>
#include <MS5xxx.h>

unsigned int sPres = 1013.25; // 当日の気圧

MS5xxx sensor(&Wire);

void setup() {
  Serial.begin(9600);
  if (sensor.connect() > 0) {
    Serial.println("Error connecting...");
    delay(500);
    setup(); // 接続再試行
  }
}

void loop() {
  sensor.ReadProm(); // 通信
  sensor.Readout(); // データ更新

  float temp = sensor.GetTemp() / 100;
  float pres = sensor.GetPres() / 100;

  Serial.print("Temperature [C]: ");
  Serial.println(temp);
  Serial.print("Pressure [hPa]: ");
  Serial.println(pres);
  Serial.print("Height [m]: ");
  Serial.println(getHeight(temp, pres, sPres));
  Serial.print("Height(Standard air) [m]: ");
  Serial.println(getHeight(temp, pres));
  Serial.println("---");

  delay(5000); // あんま通信されてもうるさいので5秒ごとに更新
}

float getHeight(float temperature, float pressure) {
  return getHeight(temperature, pressure, 1013.25);
}
 
float getHeight(float temperature, float pressure, float sPressure) {
  // http://keisan.casio.jp/exec/system/1257609530 と そのコメント13 より
  // P0: 海面気圧, P: 気圧, T: 気温(摂氏) 
  // h = (1013.25/P0)^(1/5.256) * ((P0/P)^(1/5.256)-1) * (T+273.15) / 0.0065

  return pow(1013.25/sPressure, 1/5.256) * ( pow(sPressure/pressure, 1/5.256) - 1.0 ) * (temperature + 273.15) / 0.0065;
}
