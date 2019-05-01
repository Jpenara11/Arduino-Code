/* CONTROL DE RIEGO: PARTE 1 (SENSORES) - ARDUINO MAESTRO
   AUTORES: De la Peña Ramos, Jaime
*/
#include <Wire.h>
#include <DHT.h>
#include <RTClib.h>
#include <Servo.h>
#include <SoftwareSerial.h>

const int HigrometroPin = A0; // Declarar pin A0 Higrometro
int valorHigrometro = 0; // Variable donde se almacena el valor leido por el Higrometro
int valorHigrometroMap = 0; // Variable donde almacena el valor mapeado del Higrometro

const int FlexometroPin = A1; // Declarar pin A1 Flexometro
int valorFlexometro = 0; // Variable donde se almacena el valor leido por el Flexometro
int valorFlexometroMap = 0; // Variable donde almacena el valor mapeado del Flexometro
String estadoTallo; // Variable donde se almacena el estado del tallo tiene 3 (Recto, Torcido, +Torcido)

const int SensorTempYHum = 2; // Declarar pin 2 sensor de humedad y temperatura
float valorHumedad = 0.0; // Variable donde se almacena el valor de la humedad leido por el DHT11
float valorTemperatura = 0.0; // Variable donde se almacena el valor de la temperatura leido por el DHT11
DHT dht(SensorTempYHum, DHT11); // Inicializar el sensor DHT11 (Temperatura y Humedad)

const int SensorLluvia = A2; // Declarar pin A2 Sensor de Lluvia
int valorSensorLluvia = 0; // Variable donde se almacena el valor leido por el SensorLluvia
int valorSensorLluviaMap = 0; // Variable donde almacena el valor mapeado del SensorLluvia

const int GpsRx = 4; // Declarar pin 4 GPS pin RX
const int GpsTx = 3; // Declarar pin 3 GPS pin TX
char localizacion = ' '; // Declarar variable donde se almacena la localización leída por el GPS
SoftwareSerial gps(4,3); // Declarar objeto GPS

const int EntradaServo = 6; // Declarar pin 6 servo
Servo servo; // Declarar objeto servo
int cantidadRiego = 5; // Número de veces que el servo se desplaza para regar


RTC_DS3231 rtc3231; // Inicializar RTC DS3231 (Reloj y Calendario)

void setup()
{
  Serial.begin(115200);
  
  dht.begin(); // Se inicia el sensor de temperatura y humedad 
   
  if (! rtc3231.begin())// En caso de que no se consiga iniciar el reloj
  {
    Serial.println("No se ha podido detectar el RTC_DS3231"); // Se notifica por puerto serie
    while (1); // Bucle infinito para no continuar el programa
  }
  
  rtc3231.adjust(DateTime(F(__DATE__), F(__TIME__))); // Ajustar fecha y hora del reloj

  gps.begin(9600); // Inicializar GPS

  pinMode(EntradaServo, OUTPUT);
  servo.attach(EntradaServo); // Vincular el servo al pin 6
  servo.write(0); // Se situa el ángulo de este en 0 grados
}

void loop() 
{
  valorHigrometro = analogRead(HigrometroPin); // Lectura Higrometro
  valorHigrometroMap = map(valorHigrometro, 0, 1023, 100, 0); // Mapear resultado Higrometro

  valorSensorLluvia = analogRead(SensorLluvia); // Lectura SensorLluvia
  valorSensorLluviaMap = map(valorSensorLluvia, 0, 1023, 100, 0); // Mapear resultado SensorLluvia

  valorFlexometro = analogRead(FlexometroPin); // Lectura Flexometro
  valorFlexometroMap = map(valorFlexometro, 32, 85, 1, 10); // Mapear resultado Flexometro

  if(valorFlexometroMap <= 3) estadoTallo = "+Torcido";
  else if (valorFlexometroMap >= 4 && valorFlexometroMap <= 7) estadoTallo = "Torcido";
  else estadoTallo = "Recto";
    
   
  valorHumedad = dht.readHumidity(); // Leer la humedad relativa
  valorTemperatura = dht.readTemperature(); // Leer la temperatura, por defecto en grados centígrados
   
  if (isnan(valorHumedad) || isnan(valorTemperatura)) // Comprobar si ha habido un error en la obtención datos
  {
    Serial.println("ERROR DHT11");
    return;
  }

  DateTime fechaActual = rtc3231.now(); // Obtener fecha actual


  if (gps.available()) // Si el GPS está disponible
  {
    localizacion = gps.read(); // Leemos el dato de la localización
    Serial.println(localizacion);
  }

  if(valorHigrometroMap < 50 || valorFlexometroMap <= 5) // La planta necesita agua, se riega con el motor
  {
    for(int i = 1; i <= cantidadRiego; i++) // Bucle para ejecutar el movimiento del servo 5 veces
    {
      servo.write(30);
      delay(500);// Incrementar en 30 grados el eje del servo
      servo.write(60);
      delay(500);// Incrementar en 60 grados el eje del servo
      servo.write(90);
      delay(500);// Incrementar en 90 grados el eje del servo
      servo.write(120);
      delay(500);// Incrementar en 120 grados el eje del servo
      servo.write(150);
      delay(500);// Incrementar en 150 grados el eje del servo
      servo.write(180);
      delay(1000);// Incrementar en 180 grados el eje del servo
      servo.write(0); // El servo vuelve a su estado normal
      delay(500);
    }
  }
  
 delay(3000);
  
}
