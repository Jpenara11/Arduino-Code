/* CONTROL DE RIEGO: PARTE 2 (OTROS DISPOSITIVOS)
   AUTORES: De la Peña Ramos, Jaime
            */
#include <EasyTransfer.h>
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

EasyTransfer ETout;

struct RECEIVE_DATA_STRUCTURE
{
  int higrometro;
  int sensorLluvia;
  int flexometro;
  float humedad;
  float temperatura;
  char fechaYhora[12];
  char ubicacion[30];
  char estadoTallo[9];
};

struct RECEIVE_DATA_STRUCTURE recibido;

LiquidCrystal_I2C lcd(POS_LCD, COL, FIL);  // Inicia el LCD con los datos introducidos

char teclas[FIL_TECLADO][COL_TECLADO]
{
  { '1','2','3', 'A' },
  { '4','5','6', 'B' },
  { '7','8','9', 'C' },
  { '*','0','#', 'D' }
};

const byte filasPins[FIL_TECLADO] = { 9, 8, 7, 6 };
const byte columnasPins[COL_TECLADO] = { 5, 4, 3, 2 };
Keypad tecladoMembrana = Keypad(makeKeymap(teclas), filasPins, columnasPins, FIL_TECLADO, COL_TECLADO);
char teclaPulsada;
char contrasena[TAM_CONTRASENA] = "1998";
char claveUsuario[TAM_CONTRASENA];
int indice = 0;
int alarma = 1;

const int Buzzer = 11;

const int PIR = 12; // Pin de entrada para el sensor PIR (Passive Infrared Sensor)
int estadoPIR = LOW; // De inicio el PIR no detecta movimiento
int valorPIR = 0;  // Estado del PIR

void imprimirFecha(DateTime fecha);

void setup()
{
  Serial.begin(9600);
  lcd.begin();                      
  lcd.backlight();

  pinMode(Buzzer, OUTPUT);
  pinMode(PIR, INPUT);

  ETout.begin(details(recibido), &Serial);
}

void loop() 
{
  /*imprimirLCDTempyHum(26,55.5);
  delay(10000);
  imprimirLocalizacionFecha("Salamanca","22-Abril");
  delay(10000);*/
  /*
  delay(5000);

  valorPIR = digitalRead(PIR);
  
  if (valorPIR == HIGH)   // Se activa el sensor
  { 
    if (estadoPIR == LOW)  // Si anteriormente estaba apagado
    {
      Serial.println("Sensor activado");
      estadoPIR = HIGH; // Este se enciende
      alarma = 1;
      activarAlarma(alarma);
    }
  } 
   else   // Si no esta activado
   {
      if (estadoPIR == HIGH)  // Y antes estaba encendido
      {
        Serial.println("Sensor parado");
        estadoPIR = LOW; // Este se para
        alarma = 0;
      }
   }
 */

 if(ETout.receiveData())
 {
  Serial.println(recibido.higrometro);
  Serial.println(recibido.sensorLluvia);
  Serial.println(recibido.flexometro);
  Serial.println(recibido.humedad);
  Serial.println(recibido.temperatura);
  Serial.println(recibido.fechaYhora);
  Serial.println(recibido.ubicacion);
  Serial.println(recibido.estadoTallo);
  
 }

 delay(1000);
}

void activarAlarma(int alarma)
{
  analogWrite(Buzzer, HIGH);
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
        analogWrite(Buzzer, LOW);
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
