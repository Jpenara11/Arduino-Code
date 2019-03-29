/*
  Código Realizado por: Grupo de Laboratorio 1
  Título:8 Estudio Funcionamiento Teclado Membrana
  Asignatura: Periféricos
      USAL 2018 - 2019
*/

#include "Keypad.h" // Librería externa para manejar el teclado
#include <LiquidCrystal.h> // Libería para manjear el LCD

const int RS = 7; // Pin LCD encargado de seleccionar entre comandos y datos
const int E = 8; // Pin LCD encargado de sincronizar la lectura de datos
const int D4 = 9; // Pin LCD de datos
const int D5 = 10; // Pin LCD de datos
const int D6 = 11; // Pin LCD de datos
const int D7 = 12; // Pin LCD de datos

LiquidCrystal lcd(RS, E, D4, D5, D6, D7); // Inicializar indicando los pines utilizados

const int LCD_FILAS = 2; // Declaración cte que representa las filas de la LCD
const int LCD_COLUMNAS = 16; // Declaración cte que representa las columnas de la LCD

const int FILAS = 4; // Declaración de la cte que representa las FILAS del teclado
const int COLUMNAS = 4; // Declaración de la cte que representa las columnas del teclado

char teclas[FILAS][COLUMNAS] // Declaración de la matriz 4x4 con todos los elementos del teclado
{
  { '1','2','3', 'A' },
  { '4','5','6', 'B' },
  { '7','8','9', 'C' },
  { '#','0','*', 'D' }
};

const byte filasPins[FILAS] = { 13, 6, 5, 4 }; // Declaración del vector de las filas del teclado (con sus pines)
const byte columnasPins[COLUMNAS] = { 3, 2, 1, 0 }; // Declaración del vector de las columnas del teclado (con sus pines)

Keypad tecladoMembrana = Keypad(makeKeymap(teclas), filasPins, columnasPins, FILAS, COLUMNAS); // Declaración de un objeto
// del tipo Keypad el cual efecturará las operaciones necesarias para conocer las teclas pulsadas

char teclaPulsada; // Variable donde se almacena la tecla pulsada

void setup() 
{
  Serial.begin(9600);
  
  lcd.begin(LCD_COLUMNAS, LCD_FILAS); // Inicializamos la matriz del LCD
  lcd.setCursor(0,0); // Situamos el cursor al comienzo de la matriz
}

void loop() 
{
  teclaPulsada = tecladoMembrana.getKey(); // Obtenemos la tecla pulsada
  
  if(teclaPulsada) // Si se ha pulsado una tecla, la imprimimos por los dos medios disponibles
  {
    Serial.println(teclaPulsada); // Si se pulsa una tecla se saca por el canal de puerto serie
    lcd.clear(); // Limpiamos la pantalla LCD
    lcd.print(teclaPulsada); // Sacar tecla pulsada por LCD
  }
}
