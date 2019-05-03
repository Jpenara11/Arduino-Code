/*
  Programarfacil https://programarfacil.com
  Autor: Luis del Valle @ldelvalleh
  Comunicación I2C entre dos Arduinos, esclavo
*/

#include <Wire.h>

typedef struct structInfo
{
  int higrometro;
  int sensorLluvia;
  int flexometro;
  float humedad;
  float temperatura;
  String fechaYhora;
  String ubicacion;
}Info;

Info informacionRecibida;
void recibirEstructura(byte *punteroAestructura, int longitudEstructura);

void setup() {
  // Pines en modo salida

//recibirEstructura((byte*)&informacionRecibida, sizeof(informacionRecibida));

  // Unimos este dispositivo al bus I2C con dirección 1
  Wire.begin(1);

  // Registramos el evento al recibir datos
  Wire.onReceive(receiveEvent);

  // Iniciamos el monitor serie para monitorear la comunicación
  Serial.begin(9600);

  pinMode(13, OUTPUT);
}

void loop() {
  delay(300);
}

// Función que se ejecuta siempre que se reciben datos del master
// siempre que en el master se ejecute la sentencia endTransmission
// recibirá toda la información que hayamos pasado a través de la sentencia Wire.write
void receiveEvent(int howMany) {


    informacionRecibida = (Info) Wire.read();
    if(informacionRecibida.higrometro == 45)
  {  digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
   }

   
}

void recibirEstructura(byte *punteroAestructura, int longitudEstructura)
{
if(Serial.available() < sizeof(informacionRecibida)) return;
  Serial.readBytes(punteroAestructura, longitudEstructura);

  if(informacionRecibida.higrometro == 45)
  {  digitalWrite (13, HIGH);
   }
  
}
