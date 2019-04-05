/* CONTROL DE RIEGO: PARTE 1 (SENSORES)
   AUTORES: De la Peña Ramos, Jaime
            */
#include <Wire.h>
#include "RTClib.h"
#include <DHT.h>

const int HigrometroPin = A0; // Declarar pin A0 Higrometro
int valorHigrometro = 0; // Variable donde se almacena el valor leido por el Higrometro
int valorHigrometroMap = 0; // Variable donde almacena el valor mapeado del Higrometro

const int FlexometroPin = A1; // Declarar pin A1 Flexometro
int valorFlexometro = 0; // Variable donde se almacena el valor leido por el Flexometro
int valorFlexometroMap = 0; // Variable donde almacena el valor mapeado del Flexometro

const int SensorTempYHum = 2; // Declarar pin 2 sensor de humedad y temperatura
float valorHumedad = 0.0; // Variable donde se almacena el valor de la humedad leido por el DHT11
float valorTemperatura = 0.0; // Variable donde se almacena el valor de la temperatura leido por el DHT11
DHT dht(SensorTempYHum, DHT11); // Inicializar el sensor DHT11 (Temperatura y Humedad)


RTC_DS3231 rtc3231; // Inicializar RTC DS3231 (Reloj y Calendario)

void setup()
{
  Serial.begin(9600);
  
  dht.begin(); // Se inicia el sensor de temperatura y humedad 
   
  if (! rtc3231.begin())// En caso de que no se consiga iniciar el reloj
  {
    Serial.println("No se ha podido detectar el RTC_DS3231"); // Se notifica por puerto serie
    while (1); // Bucle infinito para no continuar el programa
  }
  
  rtc3231.adjust(DateTime(F(__DATE__), F(__TIME__))); // Ajustar fecha y hora del reloj

}

void loop() 
{
  valorHigrometro = analogRead(HigrometroPin); // Lectura Higrometro
  valorHigrometroMap = map(valorHigrometro, 0, 1023, 0, 10); // Mapear resultado Higrometro

  valorFlexometro = analogRead(FlexometroPin); // Lectura Flexometro
  valorFlexometroMap = map(valorFlexometro, 32, 85, 0, 90); // Mapear resultado Flexometro
   
  valorHumedad = dht.readHumidity(); // Leer la humedad relativa
  valorTemperatura = dht.readTemperature(); // Leer la temperatura, por defecto en grados centígrados
   
  if (isnan(valorHumedad) || isnan(valorTemperatura)) // Comprobar si ha habido un error en la obtención datos
  {
    Serial.println("ERROR DHT11");
    return;
  }

  DateTime fechaActual = rtc3231.now(); // Obtener fecha actual
  
}
