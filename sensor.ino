#include <MQTT.h>
#include <ESP8266WiFi.h>

//WiFi
const char *ssid = "Projeto";
const char *password = "123456789";
WiFiClient wificlient;

//MQTT
const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;


#define mqtt_topic "FBV/ficha"
#define mqtt_id "teste-fbv"
MQTTClient MQTT;

void conectaWiFi()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return;
  }

  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Conectado");
  Serial.print("IP na rede: ");
  Serial.print(WiFi.localIP());
}

void conectaMQTT()
{
  while (!MQTT.connected())
  {
    Serial.print("Conectando-se ao broken ");
    Serial.println(mqtt_server);

    if (MQTT.connect(mqtt_id))
    {
      Serial.println("Conectado ao broken");
    }
    else
    {
      Serial.println("Tentando novamente em 5s");
      delay(5000);
    }
  }

  MQTT.subscribe(mqtt_topic);
  MQTT.onMessage(mqtt_callback);
}

int volts;
void mqtt_callback(String &topic, String &payload)
{


  //toma ação dependendo da string recebida:
  //verifica se deve colocar nivel alto de tensão na saída D0:
  //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
  //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)

  Serial.println(payload);
  volts = payload.toInt();


}
void mantemConexao()
{

  conectaWiFi();

  if (!MQTT.connected())
  {
    conectaMQTT();
  }
}


const int sensor = A0;
void setup() {
  // put your setup code here, to run once:
  pinMode(sensor, INPUT);
  Serial.begin(9600);
  conectaWiFi();
  MQTT.begin(mqtt_server, mqtt_port, wificlient);

}
int valor;
void capturaTensao() {
  valor = analogRead(sensor);
  /* if (valor > 0) {
     Serial.println(valor);
    }*/
}
void loop() {
  mantemConexao();
  MQTT.loop();
 delay(3000);
    Serial.println(valor);
    Serial.println(volts);


}
