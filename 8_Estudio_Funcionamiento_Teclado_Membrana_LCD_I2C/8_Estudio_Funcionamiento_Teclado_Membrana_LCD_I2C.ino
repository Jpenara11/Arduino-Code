/*
  Código Realizado por: Grupo de Laboratorio 1
  Título:8 Estudio Funcionamiento Teclado Membrana con pantalla LCD y módulo I2C
  Asignatura: Periféricos
      USAL 2018 - 2019
*/

/*
 Board I2C / TWI pins
Uno       =   A4 (SDA), A5 (SCL)
Mega2560  =   20 (SDA), 21 (SCL)
Leonardo  =   2 (SDA), 3 (SCL)
Due       =   20 (SDA), 21 (SCL), SDA1, SCL1

esta configuracion de estos pines se encuentran dentro de la librería "wire" mas info: https://www.arduino.cc/en/Reference/Wire
 */

#include "Keypad.h" // Librería externa para manejar el teclado
#include <LiquidCrystal_I2C.h> // Libreria LCD_I2C

const int LCD_FILAS = 2; // Declaración cte que representa las filas de la LCD
const int LCD_COLUMNAS = 16; // Declaración cte que representa las columnas de la LCD

LiquidCrystal_I2C lcd(0x3f,LCD_COLUMNAS,LCD_FILAS); // si no te sale con esta direccion  puedes usar (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2) 

const int FILAS = 4; // Declaración de la cte que representa las FILAS del teclado
const int COLUMNAS = 4; // Declaración de la cte que representa las columnas del teclado

char teclas[FILAS][COLUMNAS] // Declaración de la matriz 4x4 con todos los elementos del teclado
{
  { '1','2','3', 'A' },
  { '4','5','6', 'B' },
  { '7','8','9', 'C' },
  { '#','0','*', 'D' }
};

const byte filasPins[FILAS] = { 11, 10, 9, 8 }; // Declaración del vector de las filas del teclado (con sus pines)
const byte columnasPins[COLUMNAS] = { 7, 6, 5, 4 }; // Declaración del vector de las columnas del teclado (con sus pines)

Keypad tecladoMembrana = Keypad(makeKeymap(teclas), filasPins, columnasPins, FILAS, COLUMNAS); // Declaración de un objeto
// del tipo Keypad el cual efecturará las operaciones necesarias para conocer las teclas pulsadas

char teclaPulsada; // Variable donde se almacena la tecla pulsada

void setup() 
{
  Serial.begin(9600);
  
  lcd.init(); // Inicializamos el LCD
  lcd.backlight(); // Inicializamos la retroiluminación de la pantalla
  lcd.clear(); // Limpiamos la pantalla en caso de que pueda haber algo
  lcd.setCursor(0,0); // Situamos el cursor en la primera fila y primera columna
}

void loop() 
{
  teclaPulsada = tecladoMembrana.getKey(); // Obtenemos la tecla pulsada

  if(teclaPulsada) // Si se ha pulsado una tecla, la imprimimos por los dos medios disponibles
  {
    Serial.println(teclaPulsada); // Si se pulsa una tecla se saca por el canal de puerto serie
    lcd.display(); // Mostramos
    delay(500);
    lcd.print(teclaPulsada); // Sacar tecla pulsada por LCD
    lcd.noDisplay(); // Desaparece
    delay(500);
    lcd.clear(); // Limpiar pantalla LCD
  }
}
