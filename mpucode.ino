#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
int minVal=265;
int maxVal=402;
char tmp_str[7]; // temporary variable used in convert function
double x;
double y;
double z;
float gyro_sensitivity = 131.0; // Sensitivity value for ±250°/s range (default)
// Use 131.0 for ±250°/s, 65.5 for ±500°/s, 32.8 for ±1000°/s, or 16.4 for ±2000°/s

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  // print out data
  // -/+2g = 16384 LSB/g
  // -/+4g = 8192 LSB/g
  // -/+8g = 4096 LSB/g
  // -/+16g = 2048 LSB/g

  Serial.print("aX = "); Serial.print(float(accelerometer_x)/16384);
  Serial.print(" | aY = "); Serial.print(float(accelerometer_y)/16384);
  Serial.print(" | aZ = "); Serial.print(float(accelerometer_z)/16384);
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  // Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  
  // The MPU6050 has an on-chip gyroscope that can measure angular rotation over four programmable 
  // full scale ranges of ±250°/s, ±500°/s, ±1000°/s, and ±2000°/s.
  x = gyro_x / gyro_sensitivity;
  y = gyro_y / gyro_sensitivity;
  z = gyro_z / gyro_sensitivity;

  Serial.print(" | gX = "); Serial.print(x);
  Serial.print(" | gY = "); Serial.print(y);
  Serial.print(" | gZ = "); Serial.print(z);

  Serial.println();
  delay(1000);
}


// -------------------------------------------------------------------------------SCAN--------------

// #include <Wire.h>
// void setup()
// {
//   Wire.begin();
 
//   Serial.begin(9600);
//   while (!Serial);             // Leonardo: wait for serial monitor
//   Serial.println("\nI2C Scanner");
// }
 
 
// void loop()
// {
//   byte error, address;
//   int nDevices;
 
//   Serial.println("Scanning...");
 
//   nDevices = 0;
//   for(address = 1; address < 127; address++ )
//   {
//     // The i2c_scanner uses the return value of
//     // the Write.endTransmisstion to see if
//     // a device did acknowledge to the address.
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
 
//     if (error == 0)
//     {
//       Serial.print("I2C device found at address 0x");
//       if (address<16)
//         Serial.print("0");
//       Serial.print(address,HEX);
//       Serial.println("  !");
 
//       nDevices++;
//     }
//     else if (error==4)
//     {
//       Serial.print("Unknown error at address 0x");
//       if (address<16)
//         Serial.print("0");
//       Serial.println(address,HEX);
//     }    
//   }
//   if (nDevices == 0)
//     Serial.println("No I2C devices found\n");
//   else
//     Serial.println("done\n");
 
//   delay(5000);           // wait 5 seconds for next scan
// }
