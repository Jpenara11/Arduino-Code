/* CONTROL DE RIEGO: PARTE 2 (OTROS DISPOSITIVOS)
   AUTORES: De la Peña Ramos, Jaime
            */

#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"

void imprimirFecha(DateTime fecha);

void setup()
{
  
}

void loop() 
{

}

void imprimirFecha(DateTime fecha)
{
  Serial.print(fecha.day(), DEC);
  Serial.print('/');
  Serial.print(fecha.month(), DEC);
  Serial.print('/');
  Serial.print(fecha.year(), DEC);
}
