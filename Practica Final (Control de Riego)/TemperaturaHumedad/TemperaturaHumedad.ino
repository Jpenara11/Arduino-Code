/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 7. Estudio Funcionamiento Microservos
  Asignatura: Periféricos
      USAL 2018 - 2019
*/

#include <Wire.h>
#include "RTClib.h"
#include <DHT.h>
#include <SoftwareSerial.h>

const int SensorTempYHum = 2; // Declarar pin 2 sensor de humedad y temperatura
float valorHumedad = 0.0; // Variable donde se almacena el valor de la humedad leido por el DHT11
float valorTemperatura = 0.0; // Variable donde se almacena el valor de la temperatura leido por el DHT11
DHT dht(SensorTempYHum, DHT11); // Inicializar el sensor DHT11 (Temperatura y Humedad)


void setup() 
{
  Serial.begin(9600);
  dht.begin(); // Se inicia el sensor de temperatura y humedad 
}

void loop() 
{

 
  valorHumedad = dht.readHumidity(); // Leer la humedad relativa
  valorTemperatura = dht.readTemperature(); // Leer la temperatura, por defecto en grados centígrados
  if (isnan(valorHumedad) || isnan(valorTemperatura)) // Comprobar si ha habido un error en la obtención datos
  {
    Serial.println("ERROR DHT11");
    return;
  }else{
      Serial.println(valorHumedad);
      Serial.println(valorTemperatura);
    }
    delay(3000);
}
