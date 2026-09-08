#include <Arduino.h>

#include <math.h>

#include "AdafruitIO_WiFi.h"
#include "secrets.h"

AdafruitIO_WiFi io(
    IO_USERNAME,
    IO_KEY,
    WIFI_SSID,
    WIFI_PASS);

// referencia ao feed temperatura
AdafruitIO_Feed *feedTemperatura = io.feed("temperatura");

const int LED_PIN = 14;
const float TEMPERATURA_LIMITE = 20.0;

void handleTemperatura(AdafruitIO_Data *data);

void piscaLed(int pino, int qtdePiscas)
{
  for (byte i = 0; i < qtdePiscas; i++)
  {
    digitalWrite(pino, HIGH);
    delay(500);
    digitalWrite(pino, LOW);
    delay(500);
  }
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);

  // Define o ADC (Analog-to-Digital Converter) da ESP32 com resolução de 12 bits (0-4095)
  analogReadResolution(12);

  piscaLed(LED_PIN, 10);

  Serial.println();

  Serial.println("Iniciando a ESP...");

  Serial.print("Conectando ao Adafruit IO");

  // Iniciar a conexão Wi-fi e com a Adafruit IO
  io.connect();

  // Registra a função que será acionada quando chegar um novo valor no feed
  feedTemperatura->onMessage(handleTemperatura);

  // Aguardar até que a conexão seja estabelecida
  while (io.status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  // Exibe o estado atual da conexão

  Serial.println(io.statusText());
  Serial.println("Adafruit IO conectado");
}

void handleTemperatura(AdafruitIO_Data *data){
  float temperatura = data->toFloat();

  Serial.print("Temperatura recebida: ");
  Serial.print(temperatura, 2);
  Serial.println(" °C");

  if(temperatura > TEMPERATURA_LIMITE){
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Alerta ligado!!!");
  }
  else{
    digitalWrite(LED_PIN, LOW);
  Serial.println("Temperatura normal: LED DESLIGADO!!!");
  }
}

void loop()
{
  io.run();
}
