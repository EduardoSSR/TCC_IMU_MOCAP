 /* SENSOR DE MOVIMENTO - MPU6050
 *  
 *  AUTOR: Eduardo S. Sales Rodrigues 
 *  
 *  Graduando em Engenharia Eletrônica
 *  
 *  eduardo-ssr@aluno.unb.br
 *  
 *  
 *  
 ************************************************************************/

#include <Wire.h>

uint8_t i2c_data[14];

double accX, accY, accZ;
double tmp;
double gyroX, gyroY, gyroZ;

int mpuA = 2;
int mpuB = 3;
int mpuC = 4;

uint32_t timer;

int n = 1;
/************************************************************************
 * Setup Inicial com as Configurações para i2C.
 ************************************************************************/

void mpu_begin(){

  
  i2c_data[0] = 0;      /* 0x19 - Taxa de amostragem  8kHz/(7 + 1) = 1000Hz */
  i2c_data[1] = 0x00;   /* 0x1A - Desabilitar FSYNC, Configurar o Filtro de ACC 260Hz, Configurar Filtro de Gyro 256Hz, Amostragem de 8Khz */
  i2c_data[2] = 0x00;   /* 0x1B - Configurar o fundo de escala do Gyro ±250deg/s - Faixa */
  i2c_data[3] = 0x00;   /* 0x1C - Configurar o fundo de escala do Acelerômetro para ±2g - Faixa */

  while(i2cWrite(0x19, i2c_data, 4, false));

  /* PLL tenha como referência o gyro de eixo X, Desabilitando Sleep Mode */
  while(i2cWrite(0x6B, 0x01, true));

  /* */
  while(i2cRead(0x75, i2c_data, 1));

  if(i2c_data[0] != 0x68 && i2c_data[0] != 0x69){
    Serial.print("Erro. Placa desconhecida\n");
    while(1){
      Serial.print("Erro. Conecte a MPU6050 no barramento i2c\n");
    }
  }

  }

void setup() {

  /* Inicializando a Serial para exibir mensagens de Debug */
  Serial.begin(115200);
  
  //Configura o pino AD0 dos Sensores  
  pinMode(mpuA,OUTPUT);
  pinMode(mpuB,OUTPUT);
  pinMode(mpuC,OUTPUT);

  digitalWrite(mpuA, HIGH);
  digitalWrite(mpuB, HIGH);
  digitalWrite(mpuC, HIGH);
  
  /* Inicializando o Barramento i2c para comunicação com os 3 MPU6050's */
  Wire.begin();

  digitalWrite(mpuA, LOW);
  digitalWrite(mpuB, HIGH);
  digitalWrite(mpuC, HIGH);
  mpu_begin();
  
  digitalWrite(mpuA, HIGH);
  digitalWrite(mpuB, LOW);
  digitalWrite(mpuC, HIGH);
  mpu_begin();

  digitalWrite(mpuA, HIGH);
  digitalWrite(mpuB, HIGH);
  digitalWrite(mpuC, LOW);
  mpu_begin();
  
  /* Tempo de estabilização do Sensor MPU6050 */
  delay(100);

}

void loop() {
 for(n=1;n<4;n++){ 
  if(n==1){
    digitalWrite(mpuA, LOW);
    digitalWrite(mpuB, HIGH);
    digitalWrite(mpuC, HIGH);
    }
  else if(n==2){
    digitalWrite(mpuA, HIGH);
    digitalWrite(mpuB, LOW);
    digitalWrite(mpuC, HIGH);
      }
  else{
    digitalWrite(mpuA, HIGH);
    digitalWrite(mpuB, HIGH);
    digitalWrite(mpuC, LOW);
  
    }    
  timer = micros();
 
  /* Leitura dos Dados de Aceleração e Gyro do sensor MPU6050 */
  while(i2cRead(0x3B, i2c_data, 14));

  /*Aceleração*/
  accX = (int16_t)((i2c_data[0] << 8) | i2c_data[1]); // ([ MSB ] [ LSB ])
  accY = (int16_t)((i2c_data[2] << 8) | i2c_data[3]); // ([ MSB ] [ LSB ])
  accZ = (int16_t)((i2c_data[4] << 8) | i2c_data[5]); // ([ MSB ] [ LSB ])

 /*Temperatura*/
  tmp = (int16_t)((i2c_data[6] << 8) | i2c_data[7]);
 
  /*Giroscópio*/
  gyroX = (int16_t)((i2c_data[8] << 8) | i2c_data[9]); // ([ MSB ] [ LSB ])
  gyroY = (int16_t)((i2c_data[10] << 8) | i2c_data[11]); // ([ MSB ] [ LSB ])
  gyroZ = (int16_t)((i2c_data[12] << 8) | i2c_data[13]); // ([ MSB ] [ LSB ])

  /* Calculo do Delta Time */
  
  if((micros()-timer) >= 1000){
  /* Transmissão dos dados para porta Serial*/

  Serial.print(n); Serial.print("\t");
  Serial.print(accX); Serial.print("\t");
  Serial.print(accY); Serial.print("\t");
  Serial.print(accZ); Serial.print("\t");
  Serial.print(tmp);Serial.print("\t");
  Serial.print(gyroX); Serial.print("\t");
  Serial.print(gyroY); Serial.print("\t");
  Serial.print(gyroZ); Serial.print("\n");
  timer = micros();
  
    }  
  }
}    
