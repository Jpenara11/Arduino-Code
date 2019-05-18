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
  float humedad;
  float temperatura;
  char fechaYhora[12];
  char ubicacion[23];
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
char contrasena[TAM_CONTRASENA] = "3515";
char claveUsuario[TAM_CONTRASENA];
int indice = 0;
int alarma = 1;

const int Buzzer = 11;

const int PIR = 13; // Pin de entrada para el sensor PIR (Passive Infrared Sensor)
int estadoPIR = -1; // 4 Estados (-1, 0 ,1 y 2)
int valorPIR = LOW;  // De inicio el PIR está apagado

const int SensorLlama = 10; // Pin de entrada para el sensor de Llama Infrarrojo
int valorSensorLlama = 1; // Estado sensor Llama

const int Led = 12;

int higrometroLCD;
float temperaturaLCD, humedadLCD;
String ubicacionLCD, estadoTalloLCD, fechaLCD;

void setup()
{
  Serial.begin(9600);
  lcd.begin();                      
  lcd.backlight();

  pinMode(Buzzer, OUTPUT);
  pinMode(PIR, INPUT_PULLUP);
  pinMode(SensorLlama, INPUT_PULLUP);
  pinMode(Led, OUTPUT);

  ETout.begin(details(recibido), &Serial);
}

void loop() 
{
  valorPIR = digitalRead(PIR);

  valorSensorLlama = digitalRead(SensorLlama);
  
   if (valorSensorLlama == LOW)
   {
      estadoPIR= 2;
   }
  
  switch(estadoPIR)
  {
    case -1: // Carga condensadores pre-inicio
            delay(2000); // Esperamos 20 segundos a que se carguen los condensadores
            estadoPIR = 0;
            Serial.println("ESTADO CARGA CONDENSADORES");
            break;
            
    case 0: // Alarma apagada, esperando a que el usuario la inicie
            Serial.println("ESTADO ALARMA APAGADA");
            detectarTecla();
            break;
            
     case 1:
            Serial.println("ESTAMOS EN EL ESTADO 1");
            if (valorPIR == HIGH) estadoPIR = 2; 
            else estadoPIR = 1;
            break;
            
     case 2:  
              saltarAlarmaLCD();
              alarma = 1;
              activarAlarma(alarma);
              
              estadoPIR = 0;
              digitalWrite(Led, LOW);
              digitalWrite(Buzzer, LOW);
              delay(5000); //Alarma desactivada
              desactivarAlarmaLCD();
              break; 
  }

 /*if(ETout.receiveData())
 {
  Serial.println(recibido.higrometro);
  Serial.println(recibido.humedad);
  Serial.println(recibido.temperatura);
  Serial.println(recibido.fechaYhora);
  Serial.println(recibido.ubicacion);
  Serial.println(recibido.estadoTallo);

  String fechaYhoraString(recibido.fechaYhora);
  String ubicacionString(recibido.ubicacion);
  String estadoTalloString(recibido.estadoTallo);

  higrometroLCD = recibido.higrometro;
  temperaturaLCD = recibido.temperatura;
  humedadLCD = recibido.humedad;
  fechaLCD = fechaYhoraString;
  ubicacionLCD = ubicacionString;
  estadoTalloLCD = estadoTalloString;

  imprimirLCDTempyHum(temperaturaLCD,humedadLCD);
  delay(5000);
  
  imprimirLocalizacionFecha(ubicacionLCD,fechaLCD);
  delay(5000);

  imprimirHumedadTierraYTallo(higrometroLCD,estadoTalloLCD);
  delay(5000);
 }*/

 if(estadoPIR == 0)
 {
    detectarTecla();
    imprimirLCDTempyHum(26.0,33.0);
    delay(5000);
  
    detectarTecla();
    imprimirLocalizacionFecha("N 44.4 E 55.1","14:56 3/5");
    delay(5000);

    detectarTecla();
    imprimirHumedadTierraYTallo(55,"+Torcido");
    delay(5000);

    detectarTecla();

    if(estadoPIR == 1)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ALARMA CONECTADA");
    }
 }

}

void detectarTecla()
{
  teclaPulsada = tecladoMembrana.getKey();
  
  if (teclaPulsada == '*')
  {
     Serial.println(teclaPulsada);
     estadoPIR = 1;
     avisarAlarmaLCD();     
  }
}

void activarAlarma(int alarma)
{
  digitalWrite(Led, HIGH);
  digitalWrite(Buzzer, HIGH);
  Serial.println("ALARMA!!");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CLAVE: ");
  lcd.setCursor(7,0);
  while(alarma == 1)
  {
    teclaPulsada = tecladoMembrana.getKey();
    if(teclaPulsada)
    {
      claveUsuario[indice] = teclaPulsada;
      indice++;
      lcd.print(teclaPulsada);
    }
  
    if(indice == TAM_CONTRASENA - 1)
    {
      indice = 0;
      if(strcmp(claveUsuario, contrasena) == 0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("CONTRASENA: ");
        lcd.setCursor(0,1);
        lcd.print("CORRECTA");
        analogWrite(Buzzer, LOW);
        alarma = 0;
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("CONTRASENA: ");
        lcd.setCursor(0,1);
        lcd.print("INCORRECTA");
        delay(3000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("CLAVE: ");
        lcd.setCursor(7,0);
      }
    }
  }
}

void avisarAlarmaLCD()
{
  int contador = 10;
  
  lcd.clear();

  while(contador > 0)
  {
    lcd.setCursor(0,0);
    lcd.print("ALARMA INICIADA");
    lcd.setCursor(8,1);
    lcd.print(contador);
    delay(1000);
    contador--;
    lcd.clear();
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ALARMA CONECTADA");
  delay(3000);
}

void saltarAlarmaLCD()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ALARMA ACTIVADA!");
}

void desactivarAlarmaLCD()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ALARMA APAGADA");
  delay(3000);
  lcd.clear();
}

void imprimirLCDTempyHum(float temperatura, float humedad)
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("TEMP: ");
   lcd.print(temperatura);
   lcd.print(" gC");
   lcd.setCursor(0,1);
   lcd.print("HUMD: ");
   lcd.print(humedad);
   lcd.print(" %");
}

void imprimirLocalizacionFecha(String localizacion, String fecha)
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("L: ");
   lcd.print(localizacion);
   lcd.setCursor(0,1);
   lcd.print("HYF: ");
   lcd.print(fecha);
}

void imprimirHumedadTierraYTallo(int higrometro, String estadoTallo)
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("HUMD TIERR: ");
   lcd.print(higrometro);
   lcd.print(" %");
   lcd.setCursor(0,1);
   lcd.print("TALLO: ");
   lcd.print(estadoTallo);
}

void imprimirFecha(DateTime fecha)
{
  Serial.print(fecha.day(), DEC);
  Serial.print('/');
  Serial.print(fecha.month(), DEC);
  Serial.print('/');
  Serial.print(fecha.year(), DEC);
}
