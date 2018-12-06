#include <ESP8266WiFi.h>
#include <Wire.h>

const char *ssid = "Teste_de_AP";
const char *password = "mpu6050";

WiFiServer server(80);

String html;//String que armazena o corpo do site.
char dados[100];
uint8_t i2c_data[14];
 
int accX, accY, accZ;
int tmp;
int gyroX, gyroY, gyroZ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 

  Wire.begin();

  //#if ARDUINO >= 157
  //Wire.setClock(400000UL); // Freq = 400kHz.
  //#else
  //TWBR = ((F_CPU/400000UL) - 16) / 2; // Freq = 400kHz 
  //#endif

  i2c_data[0] = 7;      /* 0x19 - Taxa de amostragem  8kHz/(7 + 1) = 1000Hz */
  i2c_data[1] = 0x00;   /* 0x1A - Desabilitar FSYNC, Configurar o Filtro de ACC 260Hz, Configurar Filtro de Gyro 256Hz, Amostragem de 8Khz */
  i2c_data[2] = 0x00;   /* 0x1B - Configurar o fundo de escala do Gyro ±250deg/s - Faixa */
  i2c_data[3] = 0x00;   /* 0x1C - Configurar o fundo de escala do Acelerômetro para ±2g - Faixa */

  /* Configirações do i2c*/
  while(i2cWrite(0x19, i2c_data, 4, false));

  /* PLL tenha como referência o gyro de eixo X, Desabilitando Sleep Mode */
  while(i2cWrite(0x6B, 0x01, true));

  /* */
  while(i2cRead(0x75, i2c_data, 1));

  if(i2c_data[0] != 0x68 && i2c_data[0] != 0x69){
    //Serial.print("Erro. Placa desconhecida\n");
    while(1){
      //Serial.print("Erro. Conecte a MPU6050 no barramento i2c\n");
    }
  }
  IPAddress staticIP(192, 168, 4, 2); // IP set to Static
  IPAddress gateway(192, 168, 4, 1);// gateway set to Static
  IPAddress subnet(255, 255, 255, 0);// subnet set to Static

  WiFi.mode(WIFI_AP);// Working mode only as Acess Point 

  WiFi.softAP(ssid, password, 2, 0);
  WiFi.config(staticIP, gateway, subnet);

  server.begin();

  //Serial.println("Server started"); 
  //Serial.println(WiFi.softAPIP());
}

void loop() {
  
  while(i2cRead(0x3B, i2c_data, 14));

  /*Aceleração*/
  accX = (int)((i2c_data[0] << 8) | i2c_data[1]); // ([ MSB ] [ LSB ])
  accY = (int)((i2c_data[2] << 8) | i2c_data[3]); // ([ MSB ] [ LSB ])
  accZ = (int)((i2c_data[4] << 8) | i2c_data[5]); // ([ MSB ] [ LSB ])

 /*Temperatura*/
  tmp = (int)((i2c_data[6] << 8) | i2c_data[7]);
 
  /*Giroscópio*/
  gyroX = (int)((i2c_data[8] << 8) | i2c_data[9]); // ([ MSB ] [ LSB ])
  gyroY = (int)((i2c_data[10] << 8) | i2c_data[11]); // ([ MSB ] [ LSB ])
  gyroZ = (int)((i2c_data[12] << 8) | i2c_data[13]); // ([ MSB ] [ LSB ])
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  req = req.substring(req.indexOf("/") + 1, req.indexOf("HTTP") - 1);
  //Serial.println(req);
  client.flush();

  // Match the request
  
  client.print("HTTP/1.1 200 OK\n\n");
  if (req.indexOf("D") != -1)
  {
    sprintf(dados, "%d \t %d \t %d \t %d \t %d \t %d \t %d \n",(accX),(accY),(accZ),(tmp),(gyroX),(gyroY),(gyroZ));
    //Serial.println(dados);
    //html = "\nHTTP/1.1 200 OK \n\n Content-Type: text/html\n\n";//Identificaçao do HTML
    //html += "<html><head><title>ESP8266 WEB</title>";//Identificaçao e Titulo.";
    //html += "<meta name='viewport' content='user-scalable=no'>";//Desabilita o Zoom.
    //html += "<style>h1{font-size:2vw;color:black;}</style></head>";//Cria uma nova fonte de tamanho e cor X.
    //html += "<body bgcolor='ffffff'><center><h1>";//Cor do Background
   
 //  client.print(html);
   client.print(dados);
   //client.print("\n");
   // client.print("REcebido seu dado D   ");
    //client.print("</body></html>");
  
    //client.print(html);
  }
  else if (req.indexOf("R") != -1)
  {
    client.print("REcebido seu dado R   ");
  }  
  else {
    client.print("Invalid Request");
    client.flush();
    client.stop();
    return;
  }
  
  //client.print("HTTP/1.1 200 OK\n\n");
  client.flush();

}
