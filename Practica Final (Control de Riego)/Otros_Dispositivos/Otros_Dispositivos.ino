/* CONTROL DE RIEGO: PARTE 2 (OTROS DISPOSITIVOS)
   AUTORES: De la Peña Ramos, Jaime
            */
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RTClib.h>
#include <Key.h>
#include <Keypad.h>

#define FIL 2
#define COL 16
#define POS_LCD 0x3F

#define FIL_TECLADO 4
#define COL_TECLADO 4

#define TAM_CONTRASENA 5

LiquidCrystal_I2C lcd(POS_LCD, COL, FIL);  // Inicia el LCD con los datos introducidos

char teclas[FIL_TECLADO][COL_TECLADO]
{
  { '1','2','3', 'A' },
  { '4','5','6', 'B' },
  { '7','8','9', 'C' },
  { '*','0','#', 'D' }
};

const byte filasPins[FIL_TECLADO] = { 12, 10, 9, 8 };
const byte columnasPins[COL_TECLADO] = { 7, 6, 5, 4 };
Keypad tecladoMembrana = Keypad(makeKeymap(teclas), filasPins, columnasPins, FIL_TECLADO, COL_TECLADO);
char teclaPulsada;
char contrasena[TAM_CONTRASENA] = "1998";
char claveUsuario[TAM_CONTRASENA];
int indice = 0;
int alarma = 1;

const int pinBuzzer = 11;

void imprimirFecha(DateTime fecha);

void setup()
{
  Serial.begin(9600);
  lcd.begin();                      
  lcd.backlight();

  pinMode(pinBuzzer, OUTPUT);
}

void loop() 
{/*
  imprimirLCDTempyHum(26,55.5);
  delay(10000);
  imprimirLocalizacionFecha("Salamanca","22-Abril");
  delay(10000);
  */
  delay(5000);
  alarma = 1;
  activarAlarma(alarma);
}

void activarAlarma(int alarma)
{
  analogWrite(pinBuzzer, HIGH);
  Serial.println("ALARMA!!");
  while(alarma == 1)
  {
    teclaPulsada = tecladoMembrana.getKey();
  
    if(teclaPulsada)
    {
      claveUsuario[indice] = teclaPulsada;
      indice++;
      Serial.print(teclaPulsada);
    }
  
    if(indice == TAM_CONTRASENA - 1)
    {
      indice = 0;
      if(strcmp(claveUsuario, contrasena) == 0)
      {
        Serial.println("Contraseña: Correcta");
        analogWrite(pinBuzzer, LOW);
        alarma = 0;
      }
      else
      {
        Serial.println("Contraseña: Incorrecta");
      }
    }
  }
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
