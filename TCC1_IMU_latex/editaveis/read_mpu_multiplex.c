#include <Wire.h>

uint8_t i2c_data[14];

double accX, accY, accZ;
double tmp;
double gyroX, gyroY, gyroZ;

uint32_t timer;

void setup() {

  Serial.begin(115200);
  Wire.begin();

#if ARDUINO >= 157
  Wire.setClock(400000UL); // Freq = 400kHz.
#else
  TWBR = ((F_CPU/400000UL) - 16) / 2; // Freq = 400kHz
#endif

  i2c_data[0] = 7;      /* 0x19 - Taxa de amostragem  8kHz/(7 + 1) = 1000Hz */
  i2c_data[1] = 0x00;   /* 0x1A - Desabilitar FSYNC, Configurar o Filtro de ACC 260Hz, Configurar Filtro de Gyro 256Hz, Amostragem de 8Khz */
  i2c_data[2] = 0x00;   /* 0x1B - Configurar o fundo de escala do Gyro 250deg/s - Faixa */
  i2c_data[3] = 0x00;   /* 0x1C - Configurar o fundo de escala do Acelerometro para 2g - Faixa */


  while(i2cWrite(0x19, i2c_data, 4, false));

  /* PLL tenha como referencia o gyro de eixo X, Desabilitando Sleep Mode */
  while(i2cWrite(0x6B, 0x01, true));

  /* */
  while(i2cRead(0x75, i2c_data, 1));

  if(i2c_data[0] != 0x68 && i2c_data[0] != 0x69){
    Serial.print("Erro. Placa desconhecida\n");
    while(1){
      Serial.print("Erro. Conecte a MPU6050 no barramento i2c\n");
    }
  }

  /* Tempo de estabilizacao do Sensor MPU6050 */
  delay(100);

  /* 1 - Leitura dos dados de Acc XYZ */
  while(i2cRead(0x3B, i2c_data, 14));

  /* 2 - Organizar os dados de Acc XYZ */
  accX = (int16_t)((i2c_data[0] << 8) | i2c_data[1]); // ([ MSB ] [ LSB ])
  accY = (int16_t)((i2c_data[2] << 8) | i2c_data[3]); // ([ MSB ] [ LSB ])
  accZ = (int16_t)((i2c_data[4] << 8) | i2c_data[5]); // ([ MSB ] [ LSB ])

  timer = micros();


}

void loop() {

  /* Leitura dos Dados de Aceleracao e Gyro do sensor MPU6050 */
  while(i2cRead(0x3B, i2c_data, 14));

  /*Aceleracao*/
  accX = (int16_t)((i2c_data[0] << 8) | i2c_data[1]); // ([ MSB ] [ LSB ])
  accY = (int16_t)((i2c_data[2] << 8) | i2c_data[3]); // ([ MSB ] [ LSB ])
  accZ = (int16_t)((i2c_data[4] << 8) | i2c_data[5]); // ([ MSB ] [ LSB ])

 /*Temperatura*/
  tmp = (int16_t)((i2c_data[6] << 8) | i2c_data[7]);
 
  /*Giroscopio*/
  gyroX = (int16_t)((i2c_data[8] << 8) | i2c_data[9]); // ([ MSB ] [ LSB ])
  gyroY = (int16_t)((i2c_data[10] << 8) | i2c_data[11]); // ([ MSB ] [ LSB ])
  gyroZ = (int16_t)((i2c_data[12] << 8) | i2c_data[13]); // ([ MSB ] [ LSB ])

  /* Calculo do Delta Time */
  double dt = (double)(micros() - timer)/1000000;
  timer = micros();

  /* Transmissao dos dados para porta Serial*/
  
  Serial.print(dt);Serial.print("\t");
  Serial.print(accX); Serial.print("\t");
  Serial.print(accY); Serial.print("\t");
  Serial.print(accZ); Serial.print("\t");
  Serial.print(tmp);Serial.print("\t");
  Serial.print(gyroX); Serial.print("\t");
  Serial.print(gyroY); Serial.print("\t");
  Serial.print(gyroZ); Serial.print("\n");
}    
