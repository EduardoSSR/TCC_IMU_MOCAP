#include <I2Cdev.h>
#include "MPU6050.h"
#include "Wire.h"

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t temp; 

#define OUTPUT_READABLE_ACCELGYRO

#define MPU6050_ACCEL_FS_2 0x00
#define MPU6050_ACCEL_FS_4 0x01
#define MPU6050_ACCEL_FS_8 0x02
#define MPU6050_ACCEL_FS_16 0x03

#define MPU6050_GYRO_FS_250 0x00
#define MPU6050_GYRO_FS_500 0x01
#define MPU6050_GYRO_FS_1000 0x02
#define MPU6050_GYRO_FS_2000 0x03

#define LED_PIN 13
bool blinkState = false;

char command;
int test = 0;
int c = 0;

void setup() {
    Serial.begin(115200);
    Serial.print("Digite o comando.(0,1,2 ou 3)\n");
    delay(1000);

do{
   if (Serial.available()){
    
    command = Serial.read();

    if(command == '0'){  
      Serial.print("Comando 0 recebido.\n");
      c = 0;
      test = 1;
      delay(2);
      
    }
    else if(command == '1'){
      Serial.print("Comando 1 recebido.\n");    
      c = 1;
      test = 1;
      delay(2); 
     }
     else if (command == '2'){
      Serial.print("Comando 2 recebido.\n");    
      c = 2;
      test = 1;
      delay(2);
     }
     else if (command == '3'){
      Serial.print("Comando 3 recebido.\n");   
      c = 3;
      test = 1;
      delay(2);
     }
     else{
     Serial.print("Comando invalido. Tente outro.\n"); 
     delay(1000);
     test = 0;
     }
    }
  }while(test == 0);
  
        Wire.begin();
    accelgyro.initialize();
    if(c == 0){
    int setFullScaleGyroRange(MPU6050_GYRO_FS_250);  
    int setFullScaleAccelRange(MPU6050_ACCEL_FS_2);  
    }
    else if(c == 1){
    int setFullScaleGyroRange(MPU6050_GYRO_FS_500);  
    int setFullScaleAccelRange(MPU6050_ACCEL_FS_4);  
    }
      
    else if(c == 2){
    int setFullScaleGyroRange(MPU6050_GYRO_FS_1000);  
    int setFullScaleAccelRange(MPU6050_ACCEL_FS_8);    
      }
      
    else if(c == 3){
    int setFullScaleGyroRange(MPU6050_GYRO_FS_2000);  
    int setFullScaleAccelRange(MPU6050_ACCEL_FS_16);  
      }
      
}

unsigned long timer;
void loop() {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    temp = accelgyro.getTemperature();
     //if((millis() - timer) > 0.25) // 4000Hz
    //{
      //timer = millis();
      //Serial.print(timer);Serial.print("\t");
      // int start=millis();Serial.print(start);Serial.print("\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(temp); Serial.print("\t");///340+36.53
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
    //blinkState = !blinkState;
    //digitalWrite(LED_PIN, blinkState);
 //}
}
