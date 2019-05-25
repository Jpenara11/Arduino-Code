/* CONTROL DE RIEGO: PARTE 2 (ARDUINO ESCLAVO)
   AUTORES: De la Peña Ramos, Jaime
            Martin Garcia, Juan Carlos
            Montero Fernandez, Alberto
   FECHA DE LA DEFENSA: 22/05/19

            UNIVERSIDAD DE SALAMANCA 2018-2019
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

struct RECEIVE_DATA_STRUCTURE recibido; // Estructura con la información recibida

LiquidCrystal_I2C lcd(POS_LCD, COL, FIL);  // Inicia el LCD con los datos introducidos

char teclas[FIL_TECLADO][COL_TECLADO] // Matriz que contiene las teclas del teclado de membrana
{
  { '1','2','3', 'A' },
  { '4','5','6', 'B' },
  { '7','8','9', 'C' },
  { '*','0','#', 'D' }
};

const byte filasPins[FIL_TECLADO] = { 9, 8, 7, 6 }; // Array con los pines de las filas del teclado
const byte columnasPins[COL_TECLADO] = { 5, 4, 3, 2 }; // Array con los pines de las columnas del teclado
Keypad tecladoMembrana = Keypad(makeKeymap(teclas), filasPins, columnasPins, FIL_TECLADO, COL_TECLADO); // Declarar el objeto teclado de membrana
char teclaPulsada; // Tecla pulsada
char contrasena[TAM_CONTRASENA] = "3515"; // Contrasena para desactivar la alarma
char claveUsuario[TAM_CONTRASENA]; // Clave que introduce el usario cuando se activa la alarma
int indice = 0; // Variable contador indice
int alarma = 1; // Flag que indica si la alarma ha sido activada o no

const int Buzzer = 11; // Pin de entrada del Buzzer

const int PIR = 13; // Pin de entrada para el sensor PIR (Passive Infrared Sensor)
int estadoPIR = -1; // 4 Estados (-1, 0 ,1 y 2)
int valorPIR = LOW;  // De inicio el PIR está apagado

const int SensorLlama = 10; // Pin de entrada para el sensor de Llama Infrarrojo
int valorSensorLlama = 1; // Estado sensor Llama

const int Led = 12; // Pin de entrada del LED

int higrometroLCD; // Variable donde se almacena el valor a mostrar por la pantalla LCD
float temperaturaLCD, humedadLCD; // Variable donde se almacena el valor a mostrar por la pantalla LCD
String ubicacionLCD, estadoTalloLCD, fechaLCD; // Variable donde se almacena el valor a mostrar por la pantalla LCD

void setup()
{
  Serial.begin(9600); // Iniciar el puerto serie
  lcd.begin(); // Iniciar la pantalla LCD                       
  lcd.backlight(); // Encender la retroiluminación de la pantalla LCD

  pinMode(Buzzer, OUTPUT); // Declarar el Buzzer como salida
  pinMode(PIR, INPUT_PULLUP); // Declarar el PIR como entrada de tipo PULLUP
  pinMode(SensorLlama, INPUT_PULLUP); // Declarar el SENSOR DE FUEGO como entrada de tipo PULLUP
  pinMode(Led, OUTPUT); // Declarar el LED como salida

  ETout.begin(details(recibido), &Serial); // Iniciar la transferencia de información
}

void loop() 
{
  valorPIR = digitalRead(PIR); // Leer el valor del PIR

  valorSensorLlama = digitalRead(SensorLlama); // Leer el valor del sensor de llama
  
   if (valorSensorLlama == LOW) // Si se activa el sensor de llama iniciamos la alarma de igual forma que el PIR
   {
      estadoPIR= 2;
   }
  
  switch(estadoPIR) // En función del estado del PIR se realiza una u otra acción
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

 if(ETout.receiveData()) // Recibimos la información del Arduino maestro
 {
  Serial.println(recibido.higrometro);
  Serial.println(recibido.humedad);
  Serial.println(recibido.temperatura);
  Serial.println(recibido.fechaYhora);
  Serial.println(recibido.ubicacion);
  Serial.println(recibido.estadoTallo);

  String fechaYhoraString(recibido.fechaYhora); // Convertimos la variable recibida en String
  String ubicacionString(recibido.ubicacion); // Convertimos la variable recibida en String
  String estadoTalloString(recibido.estadoTallo); // Convertimos la variable recibida en String

  higrometroLCD = recibido.higrometro; // Pasamos la información recibida a las variables del Arduino esclavo
  temperaturaLCD = recibido.temperatura; // Pasamos la información recibida a las variables del Arduino esclavo
  humedadLCD = recibido.humedad; // Pasamos la información recibida a las variables del Arduino esclavo
  fechaLCD = fechaYhoraString; // Pasamos la información recibida a las variables del Arduino esclavo
  ubicacionLCD = ubicacionString; // Pasamos la información recibida a las variables del Arduino esclavo
  estadoTalloLCD = estadoTalloString; // Pasamos la información recibida a las variables del Arduino esclavo
 }

 if(estadoPIR == 0) // Sacar la información por pantalla mientras no se inicie la alarma
 {
    detectarTecla(); // Comprobar si el usuario quiere iniciar la alarma
    imprimirLCDTempyHum(temperaturaLCD,humedadLCD); // Sacar información por pantalla
    delay(5000); // Esperar 5 segundos para cambiar la información
  
    detectarTecla(); // Comprobar si el usuario quiere iniciar la alarma
    imprimirLocalizacionFecha(ubicacionLCD,fechaLCD); // Sacar información por pantalla
    delay(5000); // Esperar 5 segundos para cambiar la información

    detectarTecla(); // Comprobar si el usuario quiere iniciar la alarma
    imprimirHumedadTierraYTallo(higrometroLCD,estadoTalloLCD); // Sacar información por pantalla
    delay(5000); // Esperar 5 segundos para cambiar la información

    detectarTecla(); // Comprobar si el usuario quiere iniciar la alarma

    if(estadoPIR == 1) // Si se conecta la alarma lo mostramos por pantalla
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ALARMA CONECTADA");
    }
 }

}


                                    /************************
                                     * FUNCIONES AUXILIARES *
                                     ************************/


void detectarTecla() // DETECTAR POR EL TECLADO QUÉ TECLA SE HA PULSADO
{
  teclaPulsada = tecladoMembrana.getKey();
  
  if (teclaPulsada == '*')
  {
     Serial.println(teclaPulsada);
     estadoPIR = 1;
     avisarAlarmaLCD();     
  }
}

void activarAlarma(int alarma) // SE ACTIVA LA ALARMA, ES PORQUE EL PIR HA DETECTADO LA PRESENCIA DE ALGO
{
  digitalWrite(Led, HIGH);
  digitalWrite(Buzzer, HIGH);
  Serial.println("ALARMA!!");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CLAVE: ");
  lcd.setCursor(7,0);
  while(alarma == 1) // La alarma se activa y tiene que introducir correctamente la contraseña, sino no se desconecta
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

void avisarAlarmaLCD() // AVISAR CON MARGEN DE 10 SEGUNDOS PARA QUE EL USUARIO SE ALEJE ANTES DE INICIAR LA ALARMA
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

void saltarAlarmaLCD() // AVISO DE QUE LA ALARMA HA SALTADO
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ALARMA ACTIVADA!");
}

void desactivarAlarmaLCD() // AVISO DE QUE LA ALARMA SE HA APAGADO
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ALARMA APAGADA");
  delay(3000);
  lcd.clear();
}

void imprimirLCDTempyHum(float temperatura, float humedad) // IMPRIMIR INFORMACIÓN RECIBIDA POR EL ARDUINO MAESTRO POR PANTALLA LCD
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

void imprimirLocalizacionFecha(String localizacion, String fecha) // IMPRIMIR INFORMACIÓN RECIBIDA POR EL ARDUINO MAESTRO POR PANTALLA LCD
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("L: ");
   lcd.print(localizacion);
   lcd.setCursor(0,1);
   lcd.print("HYF: ");
   lcd.print(fecha);
}

void imprimirHumedadTierraYTallo(int higrometro, String estadoTallo) // IMPRIMIR INFORMACIÓN RECIBIDA POR EL ARDUINO MAESTRO POR PANTALLA LCD
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
