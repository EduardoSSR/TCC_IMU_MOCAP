//#include <I2Cdev.h>
//#include <MPU6050.h>

// Programa para leitura do Módulo MPU 6050
// Autor: Eduardo-ssr

//Carrega a biblioteca Wire
#include<Wire.h>
 
//Endereco I2C do MPU6050
const int MPU=0x68;  
const int aPin = 2;
const int bPin = 3;
const int cPin = 4;
//Variaveis para armazenar valores dos sensores
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
void setup()
{
  Serial.begin(115200);
  //Serial.print("Digite o comando.(0,1,2 ou 3)\n");
  delay(1000);
  pinMode(aPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(cPin, OUTPUT);
  
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
      
  //Inicializa o MPU-6050 A
  digitalWrite(aPin, LOW);
  digitalWrite(bPin, HIGH);
  digitalWrite(cPin, HIGH);
  Wire.write(3); 
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  digitalWrite(aPin, HIGH);
  digitalWrite(bPin, LOW);
  digitalWrite(cPin, HIGH);
  Wire.write(3); 
  Wire.endTransmission(true);


  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  digitalWrite(aPin, HIGH);
  digitalWrite(bPin, HIGH);
  digitalWrite(cPin, LOW);
  Wire.write(3); 
  Wire.endTransmission(true);

  
}
int t1 = 0, t2;
void loop()
{ 
  t1= t1 - micros();
  for(char i=1;i<=3;i++){
    int symbol;
    switch (i)
    {
      case 1:
        symbol = 1;
        digitalWrite(aPin, LOW);
        digitalWrite(bPin, HIGH);
        digitalWrite(cPin, HIGH);
  
      break;
      
      case 2:
        symbol = 2;
        digitalWrite(aPin, HIGH);
        digitalWrite(bPin, LOW);
        digitalWrite(cPin, HIGH);

      break;
      case 3:
        symbol = 3;
        digitalWrite(aPin, HIGH);
        digitalWrite(bPin, HIGH);
        digitalWrite(cPin, LOW);
        
      break;
      default:
      
      break;
        
    }
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
     
      //Solicita os dados do sensor
      Wire.requestFrom(MPU,14,true);  
     
      //Armazena o valor dos sensores nas variaveis correspondentes
      AcX=(uint16_t)Wire.read()<<8;
      AcX|=Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
      AcY=(uint16_t)Wire.read()<<8;
      AcY|=Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
      AcZ=(uint16_t)Wire.read()<<8;
      AcZ|=Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
      Tmp=(uint16_t)Wire.read()<<8;
      Tmp|=Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
      GyX=(uint16_t)Wire.read()<<8;
      GyX|=Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
      GyY=(uint16_t)Wire.read()<<8;
      GyY|=Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
      GyZ=(uint16_t)Wire.read()<<8;
      GyZ|=Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    
    
      Serial.print(symbol);
      Serial.print("\t");

      
      //Serial.print(t1);
      //Serial.print("\t");
      
      //Envia valor X do acelerometro para a serial
      Serial.print(AcX);
      Serial.print("\t");
      
      //Envia valor Y do acelerometro para a serial 
      Serial.print(AcY);  
      Serial.print("\t");
      
      //Envia valor Z do acelerometro para a serial
      Serial.print(AcZ);
      Serial.print("\t");
      
      //Envia valor da temperatura para a serial
      //Calcula a temperatura em graus Celsius
      Serial.print(Tmp);//(Tmp/340.00+36.53)
      Serial.print("\t");
      
      //Envia valor X do giroscopio para a serial 
      Serial.print(GyX);
      Serial.print("\t");
      
      //Envia valor Y do giroscopio para a serial   
      Serial.print(GyY);
      Serial.print("\t");
      
      //Envia valor Z do giroscopio para a serial 
      Serial.println(GyZ);
       //delay(100);
  }
}

