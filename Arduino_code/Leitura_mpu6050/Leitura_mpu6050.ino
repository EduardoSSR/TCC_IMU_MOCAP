// Programa para leitura do Módulo MPU 6050
// Autor: Eduardo-ssr

//Carrega a biblioteca Wire
#include<Wire.h>
 
//Endereco I2C do MPU6050
const int MPU=0x68;  

//Variaveis para armazenar valores dos sensores
double AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup()
{
  Serial.begin(115200);
  
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  
  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
  
}
void loop()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
 
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true);  
 
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX=Wire.read()<<8|Wire.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
 
  
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
  //Serial.print("\t");
  //Serial.println(" ");
    
}

