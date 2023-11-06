/*
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** SD CS - pin 5
 ** SDA - A4
 ** SCL - A5
*/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <VescUart.h>
#include <SoftwareSerial.h>
//#include <SparkFunLSM9DS1.h>
#include <mcp2515.h>

bool rtcstate = false;

VescUart UART;
//LSM9DS1 imu;

SoftwareSerial mySerial(7, 2);
SoftwareSerial VESCSerial(9,10);
MCP2515 mcp2515(4);

const int chipSelect = 5;         //SD chip select

File dataFile;

String NOWTIME = "";
//float t2, t1, dt;
//float accX,accY,accZ,gyrX,gyrY,gyrZ;
//float pitch,roll,yaw;

//float rpm; // Vesc RPM
//float inpVoltage; // Input Voltage
//float ampHours; // AmpHours
//float tachometerAbs; // Tachometers
String dataString;

//float ANGLE_DEAD_ZONE = 0.1;

//int x, y, z;
//int rows = 0;
//int linelimit = 20;


// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
//#define GPSECHO  true

void setup() {
  //imu.settings.gyro.enabled = true;  // Enable the gyro
  //imu.settings.gyro.scale = 500;
  //imu.settings.accel.enabled = true; // Enable accelerometer
  //imu.settings.accel.scale = 8;
  //imu.settings.mag.enabled = true; // Enable magnetometer
  //imu.settings.mag.scale = 12;
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  //--------------------------CARD SETUP---------------------------------------------------------------------------------------
  //pinMode(10, OUTPUT); // pin 10 should be OUTPUT - despite us not using it - or else it interferes with writing process
 
  // VESC UART Serial Ports
  SoftwareSerial VescSerial(9, 10); // RX, TX
  VESCSerial.begin(19200); 

  Serial.begin(115200);
  Wire.begin();

  //if (imu.begin() == false) // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
  //{
    //Serial.println("Failed to communicate with LSM9DS1.");
    //while (1);
  //}

  while (!Serial) {;}

  /** Define which ports to use as UART */
  UART.setSerialPort(&VESCSerial);

  for (int i;i<10;i++)
  {
    if (!SD.begin(chipSelect))
    {
      delay(200);
    }
    else
    {
      break;
    }
  }
  
  String dataString = "TEST";
  //File dataFile = SD.open("datalog.txt", FILE_WRITE);
  //dataFile.println("RTC_time#RTC_millis#BMS_ampere#BMS_voltage#BMS_charge#BMS_temperature#VESC_rpm#VESC_inpvoltage#VESC_watt#VESC_tempMosfet#VESC_motortemp#VESC_motorcurr#VESC_batterycurr#VESC_dutyCycle#VESC_error");                  
  //dataFile.close();

  delay(1000); //keep in mind that RTC might not connect

}
uint32_t timer = millis();

//Probably won't use wai4key: if we use it, we should send the arduino a signal(pressing 0) to start writing
struct can_frame canMsg;
void loop() {
  
  if (millis() - timer > 100) {
    timer = millis(); // reset the timer
    //t1 = t2;
    //t2 = millis();
    //dt = (t2-t1)/1000;

    String dataString = "TEST";
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    

    if(rtcstate){ 
      Serial.print("#");
      Serial.print(millis());
      Serial.print("#");
      if (dataFile) {
        dataFile.print("#");
        dataFile.print(millis());
        dataFile.print("#");
      }
    }
    else
    {
      Serial.print(" #");
      Serial.print(millis());
      Serial.print("#");
      //Serial.print("Failed to get data!");
      if (dataFile) {
        dataFile.print(" #");
        dataFile.print(millis());
        dataFile.print("#");
      }
    }
    /*if (imu.accelAvailable())
    {
      if (imu.gyroAvailable())
      {
        imu.readAccel();
        imu.readGyro();
        accX = imu.calcAccel(imu.ax)*9.8066;
        accY = imu.calcAccel(imu.ay)*9.8066;
        accZ = imu.calcAccel(imu.az)*9.8066;
        gyrX = imu.calcGyro(imu.gx);
        gyrY = imu.calcGyro(imu.gy);
        gyrZ = imu.calcGyro(imu.gz);

        //Trigonometry to calculate pitch and roll by the gravity vector acting on the X,Y,Z axes
        float acc_angle_X = (atan(accY / sqrt(pow(accX, 2) + pow(accZ, 2))) * 180 / PI); 
        float acc_angle_Y = (atan(-1*accX / sqrt(pow(accY, 2) + pow(accZ, 2))) * 180 / PI);

        //Integration for orientation, correcting for drift by implementing a dead zone to ignore noise
        if (abs(gyrX)>ANGLE_DEAD_ZONE) pitch = pitch + dt*(gyrX);
        if (abs(gyrY)>ANGLE_DEAD_ZONE) roll = roll + dt*(gyrY);
        if (abs(gyrZ)>ANGLE_DEAD_ZONE) yaw = yaw + dt*(gyrZ);

        //wannabe sensor fusion
        pitch = 0.9*pitch+0.1*acc_angle_X;
        roll = 0.9*roll+0.1*acc_angle_Y;

        Serial.print(pitch);
        Serial.print("#");
        Serial.print(roll);
        Serial.print("#");
        Serial.print(yaw);
        Serial.print("#");
        if (dataFile) {
          dataFile.print(pitch);
          dataFile.print("#");
          dataFile.print(roll);
          dataFile.print("#");
          dataFile.print(yaw);
          dataFile.print("#");
        }
      }
    }
    else
    {
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      //Serial.print("Failed to get data!");
      if (dataFile) {
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
      }
    }*/
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      int64_t ampHour = ((int64_t)canMsg.data[0] << 8) | ((int64_t)canMsg.data[1]);
      Serial.print((int32_t)ampHour*0.1);
      Serial.print("#");
      int64_t volt = ((int64_t)canMsg.data[2] << 8) | ((int64_t)canMsg.data[3]);
      Serial.print((int32_t)volt*0.1);
      Serial.print("#");
      int charge = canMsg.data[4];
      Serial.print(0.5*charge);
      Serial.print("#");
      int avgtemp = canMsg.data[5];
      Serial.print(avgtemp);
      Serial.print("#");
      if (dataFile) {
        dataFile.print((int32_t)ampHour*0.1);
        dataFile.print("#");
        dataFile.print((int32_t)volt*0.1);
        dataFile.print("#");
        dataFile.print(0.5*charge);
        dataFile.print("#");
        dataFile.print(avgtemp);
        dataFile.print("#");
      }
      
    }
    else {
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      if (dataFile){
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");

      }

    }
    if ( UART.getVescValues() ) {
      Serial.print(UART.data.rpm);
      Serial.print("#");
      Serial.print(UART.data.inpVoltage);
      Serial.print("#");
      Serial.print(UART.data.wattHours);
      Serial.print("#");
      Serial.print(UART.data.tempMosfet);
      Serial.print("#");
      Serial.print(UART.data.tempMotor);
      Serial.print("#");
      Serial.print(UART.data.avgMotorCurrent);
      Serial.print("#");
      Serial.print(UART.data.avgInputCurrent);
      Serial.print("#");
      Serial.print(UART.data.dutyCycleNow);
      Serial.print("#");
      Serial.print(UART.data.error);

      
      if (dataFile) {
      dataFile.print(UART.data.rpm);
      dataFile.print("#");
      dataFile.print(UART.data.inpVoltage);
      dataFile.print("#");
      dataFile.print(UART.data.wattHours);
      dataFile.print("#");
      dataFile.print(UART.data.tempMosfet);
      dataFile.print("#");
      dataFile.print(UART.data.tempMotor);
      dataFile.print("#");
      dataFile.print(UART.data.avgMotorCurrent);
      dataFile.print("#");
      dataFile.print(UART.data.avgInputCurrent);
      dataFile.print("#");
      dataFile.print(UART.data.dutyCycleNow);
      dataFile.print("#");
      dataFile.print(UART.data.error);
      }
    }
    else
    {
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      Serial.print(" #");
      //Serial.print("Failed to get data!");
      if (dataFile) {
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
        dataFile.print(" #");
      }
    }
    Serial.println();
    if (dataFile) {
      dataFile.println();
      dataFile.close();
    }
  }
}
