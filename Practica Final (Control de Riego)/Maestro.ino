/* CONTROL DE RIEGO: PARTE 2 (ARDUINO MAESTRO)
   AUTORES: De la Peña Ramos, Jaime
            Martin Garcia, Juan Carlos
            Montero Fernandez, Alberto
   FECHA DE LA DEFENSA: 22/05/19

            UNIVERSIDAD DE SALAMANCA 2018-2019
*/

#include <EasyTransfer.h>
#include <Wire.h>
#include <DHT.h>
#include <RTClib.h>
#include <Servo.h>
#include <SoftwareSerial.h>

EasyTransfer ETin; // Objeto para enviar la estructura de datos

const int HigrometroPin = A0; // Declarar pin A0 Higrometro
int valorHigrometro = 0; // Variable donde se almacena el valor leido por el Higrometro
int valorHigrometroMap = 0; // Variable donde almacena el valor mapeado del Higrometro

const int FlexometroPin = A1; // Declarar pin A1 Flexometro
int valorFlexometro = 0; // Variable donde se almacena el valor leido por el Flexometro
int valorFlexometroMap = 0; // Variable donde almacena el valor mapeado del Flexometro
String estadoTallo; // Variable donde se almacena el estado del tallo tiene 3 (Recto, Torcido, +Torcido)
char arrayEstadoTallo[9]; // Estado del tallo

const int SensorTempYHum = 2; // Declarar pin 2 sensor de humedad y temperatura
float valorHumedad = 0.0; // Variable donde se almacena el valor de la humedad leido por el DHT11
float valorTemperatura = 0.0; // Variable donde se almacena el valor de la temperatura leido por el DHT11
DHT dht(SensorTempYHum, DHT11); // Inicializar el sensor DHT11 (Temperatura y Humedad)

const int GpsRx = 10; // Declarar pin 4 GPS pin RX
const int GpsTx = 11; // Declarar pin 3 GPS pin TX
String localizacion; // Declarar variable donde se almacena la localización leída por el GPS
char arrayLocalizacion[30]; // Localización en array
SoftwareSerial gpsSerial(GpsRx, GpsTx); // Crear objeto GPS
const int sentenceSize = 80; // Variable para el GPS
char sentence[sentenceSize]; // Variable para el GPS

const int EntradaServo = 6; // Declarar pin 6 servo
Servo servo; // Declarar objeto servo
int cantidadRiego = 5; // Número de veces que el servo se desplaza para regar


RTC_DS3231 rtc3231; // Inicializar RTC DS3231 (Reloj y Calendario)
String fechaYhoraActual; // Hora y fecha actual convertida
char arrayHoraYFecha[12]; // Array con la hora y fecha convertida

struct SEND_DATA_STRUCTURE // ESTRUCTURA DE DATOS QUE ENVIAREMOS AL ARDUINO ESCLAVO
{
  int higrometro;
  float humedad;
  float temperatura;
  char fechaYhora[12];
  char ubicacion[23];
  char estadoTallo[9];
};

struct SEND_DATA_STRUCTURE informacion; // Variable que contiene la estructura

String horaYFechaActualString (DateTime fechaActual); // Función que calcula le fecha y hora en el formato indicado

void setup()
{
  Serial.begin(9600); // Iniciar el puerto serie
  
  dht.begin(); // Se inicia el sensor de temperatura y humedad 
   
  if (! rtc3231.begin())// En caso de que no se consiga iniciar el reloj
  {
    Serial.println("No se ha podido detectar el RTC_DS3231"); // Se notifica por puerto serie
    while (1); // Bucle infinito para no continuar el programa
  }
  
  rtc3231.adjust(DateTime(F(__DATE__), F(__TIME__))); // Ajustar fecha y hora del reloj

  gpsSerial.begin(9600); // Inicializar GPS

  pinMode(EntradaServo, OUTPUT); // Entrada del servo
  servo.attach(EntradaServo); // Vincular el servo al pin 6
  servo.write(0); // Se situa el ángulo de este en 0 grados

  ETin.begin(details(informacion), &Serial); // Iniciar la transferencia entre arduinos serie
}

void loop() 
{
  valorHigrometro = analogRead(HigrometroPin); // Lectura Higrometro
  valorHigrometroMap = map(valorHigrometro, 0, 1023, 100, 0); // Mapear resultado Higrometro

  valorFlexometro = analogRead(FlexometroPin); // Lectura Flexometro
  valorFlexometroMap = map(valorFlexometro, 32, 85, 1, 10); // Mapear resultado Flexometro

  if(valorFlexometroMap <= 3) estadoTallo = "+Torcido"; // Si el valor del flexometro es bajo es porque el tallo está muy torcido
  else if (valorFlexometroMap >= 4 && valorFlexometroMap <= 7) estadoTallo = "Torcido"; // El tallo está torcido
  else estadoTallo = "Recto"; // El tallo está recto
    
   
  valorHumedad = dht.readHumidity(); // Leer la humedad relativa
  valorTemperatura = dht.readTemperature(); // Leer la temperatura, por defecto en grados centígrados
   
  if (isnan(valorHumedad) || isnan(valorTemperatura)) // Comprobar si ha habido un error en la obtención datos
  {
    Serial.println("ERROR DHT11"); // Avisar del error con el DHT11
    return; // Terminar
  }

  DateTime fechaActual = rtc3231.now(); // Obtener fecha actual
  
  fechaYhoraActual = horaYFechaActualString(fechaActual); // Convertir fecha y hora a String EJ 14:56 3/5

  static int i = 0; 
  if (gpsSerial.available()) // Obtener las coordenadas del GPS
  {
    char ch = gpsSerial.read();
    if (ch != '\n' && i < sentenceSize)
    {
      sentence[i] = ch;
      i++;
    }
    else
    {
     sentence[i] = '\0';
     i = 0;
     displayGPS();
    }
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
  
  fechaYhoraActual.toCharArray(arrayHoraYFecha,12); // Convertir fecha y hora en array de char
  localizacion.toCharArray(arrayLocalizacion, 23); // Convertir localizacion en array de char
  estadoTallo.toCharArray(arrayEstadoTallo, 9); // Convertir estado del tallo en array de char
  
  informacion.higrometro = valorHigrometroMap; // Insertar información en la estructura
  informacion.humedad = valorHumedad; // Insertar información en la estructura
  informacion.temperatura = valorTemperatura; // Insertar información en la estructura
  memcpy(informacion.fechaYhora, arrayHoraYFecha, strlen(arrayHoraYFecha)+1); // Insertar información en la estructura
  memcpy(informacion.ubicacion, arrayLocalizacion, strlen(arrayLocalizacion)+1); // Insertar información en la estructura
  memcpy(informacion.estadoTallo, arrayEstadoTallo, strlen(arrayEstadoTallo)+1); // Insertar información en la estructura

  ETin.sendData(); // Enviar información, contiene la estructura con los datos recogidos
 
 delay(3000); // Espera 3 segundos y vuelve a comenzar el ciclo
  
}

                                    /************************
                                     * FUNCIONES AUXILIARES *
                                     ************************/

void displayGPS()
{
  char field_1[20];
  char field_2[20];
  char letra_1[20];
  char letra_2[20];
  char numero_1[20];
  char numero_2[20];
  char salida[80];
  getField(field_1, 0);
  if (strcmp(field_1, "$GPRMC") == 0)
  {
    //convertimos el primer numero
    getField(field_1, 3);  // number
    number(numero_1,field_1);
    //primera letra
    getField(letra_1, 4); // N/S
   //convertimos el segundo numero
    getField(field_2, 5);  // number
    number(numero_2,field_2);
    //segunda letra
    getField(letra_2, 6);  // E/W

    String letra_1String(letra_1);
    String numero_1String(numero_1);
    String letra_2String(letra_2);
    String numero_2StringString(numero_2);

    localizacion = letra_1String + " " + numero_1String + " " + letra_2String + " " + numero_2StringString;
  }
}

void number(char* numeroViejo,char* numeroNuevo)
{
  char aux;
  int sentencePos = 0;
  //con el while acortamos el numero de decimales ya que solo queremos 2
  while (numeroViejo [sentencePos]!='.')
  {
      numeroNuevo[sentencePos]=numeroViejo[sentencePos];
      sentencePos ++;
  }
  //secuencia para mover las comas
   aux=numeroNuevo[sentencePos-1];
   numeroNuevo[sentencePos-1]=numeroNuevo[sentencePos-2];
   numeroNuevo[sentencePos-2]=',';
   numeroNuevo[sentencePos]=aux;
   
   
   // buffer[sentencePos+1] = '\0';
  
} 

void getField(char* buffer, int index)
{
  int sentencePos = 0;
  int fieldPos = 0;
  int commaCount = 0;
  while (sentencePos < sentenceSize)
  {
    if (sentence[sentencePos] == ',')
    {
      commaCount ++;
      sentencePos ++;
    }
    if (commaCount == index)
    {
      buffer[fieldPos] = sentence[sentencePos];
      fieldPos ++;
    }
    sentencePos ++;
  }
  buffer[fieldPos] = '\0';
} 

String horaYFechaActualString(DateTime fechaActual)
{
  String datos;

  datos = fechaActual.hour();
  datos = datos + ':';
  datos = datos + fechaActual.minute();
  datos = datos + ' ';
  datos = datos + fechaActual.day();
  datos = datos + '/';
  datos = datos + fechaActual.month();

  return datos;
}
