/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 4. Distancia mediante ultrasonidos HC-SR04 con pantalla LCD y módulo I2C
  Asignatura: Periféricos
      USAL 2018 - 2019
  Demostracion Teorica obtenida de: https://www.youtube.com/watch?v=ZejQOX69K5M
*/

/* DEMOSTRACIÓN TEÓRICA

  Vel Sonido1 = 343 m/s
  Vel Sonido2 (cm/microSeg) = 343 m/s * 100 cm/cm * 1/1000000 s/microSeg = 1/29.2 cm/microSeg

  Distancia = Tiempo(microSeg) / 29.2 * 2 (Pulso de ida y vuelta, por eso dividido por 2)
*/

/*
 Board I2C / TWI pins
Uno       =   A4 (SDA), A5 (SCL)
Mega2560  =   20 (SDA), 21 (SCL)
Leonardo  =   2 (SDA), 3 (SCL)
Due       =   20 (SDA), 21 (SCL), SDA1, SCL1

esta configuracion de estos pines se encuentran dentro de la librería "wire" mas info: https://www.arduino.cc/en/Reference/Wire
 */

#include <LiquidCrystal_I2C.h> // Libreria LCD_I2C

const int LED = 9; // Declaramos pin del LED
const int EchoPin = 5; // Declaramos pin del Echo del HC-SR04
const int TriggerPin = 6; // Declaramos pin del Trigger del HC-SR04
const int COLS = 16; //Definimos el número de columnas del LCD 16
const int ROWS = 2; //Definimos el número de filas del LCD 2

int var = 1;
LiquidCrystal_I2C lcd(0x3f,COLS,ROWS); // si no te sale con esta direccion  puedes usar (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2) 

void setup() 
{
  Serial.begin(9600);
  
  lcd.init(); // Inicializamos el LCD
  lcd.backlight(); // Inicializamos la retroiluminación de la pantalla
  lcd.clear(); // Limpiamos la pantalla en caso de que pueda haber algo
  lcd.setCursor(0,0); // Situamos el cursor en la primera fila y primera columna
  
  pinMode(LED, OUTPUT); // Declaramos LED como salida
  pinMode(EchoPin, INPUT); // Declaramos el Echo como entrada
  pinMode(TriggerPin, OUTPUT); // Declaramos el Trigger como salida

}

void loop() 
{
  int cm = ping(TriggerPin, EchoPin); // Obtenemos la distancia en centímetros
  lcd.print("Distancia: ");
  lcd.print(cm);
  lcd.print(" cm");
  lcd.display(); // Mostramos
  delay(500);
  
  Serial.println(cm); // Sacamos por pantalla el valor de la distancia
  analogWrite(LED, cm); // En función de la distancia, cambia la intensidad del LED
  
  lcd.noDisplay(); // Desaparece
  delay(500);
  lcd.clear();
}

int ping(int Trigger, int Echo)
{
  long duracion, distancia;

  digitalWrite(Trigger, LOW); // Generar un pulso limpio
  delayMicroseconds(4); // Se pone a LOW 4 microSeg para conseguir el pulso limpio
  digitalWrite(Trigger, HIGH); // Generar disparo de 10 microSeg
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  duracion = pulseIn(Echo, HIGH); // Se mide el tiempo entre pulsos

  distancia = duracion * 10 / 292 / 2; // Convertir la distancia a cm

  return distancia; // Devolvemos valor de la distancia calculado
}
