/* CONTROL DE RIEGO: PARTE 2 (OTROS DISPOSITIVOS)
   AUTORES: De la Peña Ramos, Jaime
            */
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RTClib.h>

#define FIL 2
#define COL 16
#define POS_LCD 0x3F

LiquidCrystal_I2C lcd(POS_LCD, COL, FIL);  // Inicia el LCD con los datos introducidos
void imprimirFecha(DateTime fecha);

void setup()
{
   lcd.begin();                      
   lcd.backlight();
   
}

void loop() 
{
  imprimirLCDTempyHum(26,55.5);
  delay(10000);
  imprimirLocalizacionFecha("Salamanca","22-Abril");
  delay(10000);
}

void imprimirLCDTempyHum(float temperatura, float humedad)
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("TEMP: ");
   lcd.print(temperatura);
   lcd.print(" %");
   lcd.setCursor(0,1);
   lcd.print("HUMD: ");
   lcd.print(humedad);
   lcd.print(" %");
}

void imprimirLocalizacionFecha(String localizacion, String fecha)
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("LOC: ");
   lcd.print(localizacion);
   lcd.setCursor(0,1);
   lcd.print("FECHA: ");
   lcd.print(fecha);
}

void imprimirFecha(DateTime fecha)
{
  Serial.print(fecha.day(), DEC);
  Serial.print('/');
  Serial.print(fecha.month(), DEC);
  Serial.print('/');
  Serial.print(fecha.year(), DEC);
}
