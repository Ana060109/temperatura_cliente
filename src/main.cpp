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

const int LED_VERDE_PIN = 14;
const int LED_VERMELHO_PIN = 18;
const float TEMPERATURA_LIMITE = 22.0;

void handleTemperatura(AdafruitIO_Data *data);
void atualizarLeds(float temperatura);

void piscaLed(int pino, int qtdePiscas)
{
  for (byte i = 0; i < qtdePiscas; i++)
  {
    digitalWrite(pino, HIGH);
    delay(300);
    digitalWrite(pino, LOW);
    delay(200);
  }
}

void atualizarLeds(float temperatura)
{
  if (temperatura > TEMPERATURA_LIMITE)
  {
    digitalWrite(LED_VERDE_PIN, HIGH);
    digitalWrite(LED_VERMELHO_PIN, LOW);
    Serial.println("Temperatura acima de 22°C: LED VERDE LIGADO.");
  }
  else
  {
    digitalWrite(LED_VERDE_PIN, LOW);
    digitalWrite(LED_VERMELHO_PIN, HIGH);
    Serial.println("Temperatura menor ou igual a 22°C: LED VERMELHO LIGADO.");
  }
}

void setup()
{
  pinMode(LED_VERDE_PIN, OUTPUT);
  pinMode(LED_VERMELHO_PIN, OUTPUT);
  Serial.begin(115200);

  // Define o ADC (Analog-to-Digital Converter) da ESP32 com resolução de 12 bits (0-4095)
  analogReadResolution(12);

  digitalWrite(LED_VERDE_PIN, LOW);
  digitalWrite(LED_VERMELHO_PIN, LOW);
  piscaLed(LED_VERDE_PIN, 3);
  piscaLed(LED_VERMELHO_PIN, 3);

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

void handleTemperatura(AdafruitIO_Data *data)
{
  float temperatura = data->toFloat();

  Serial.print("Temperatura recebida: ");
  Serial.print(temperatura, 2);
  Serial.println(" °C");

  atualizarLeds(temperatura);
}

void loop()
{
  io.run();
}
