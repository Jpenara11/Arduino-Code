/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 3. Secuencia del Coche Fantastico
  Asignatura: Periféricos
      USAL 2018 - 2019
*/

int salidaLED = 0; // LED APAGADO = 0, LED ENCENDIDO = 1
int pinLED = 9; // Pin LED inicial de la secuencia
int pinLEDAux = 2; // Pin LED final de la secuencia
void setup() 
{
  pinMode(9, OUTPUT); // Declarar el LED como salida en el pin 9
  pinMode(8, OUTPUT); // Declarar el LED como salida en el pin 8
  pinMode(7, OUTPUT); // Declarar el LED como salida en el pin 7
  pinMode(6, OUTPUT); // Declarar el LED como salida en el pin 6
  pinMode(5, OUTPUT); // Declarar el LED como salida en el pin 5
  pinMode(4, OUTPUT); // Declarar el LED como salida en el pin 4
  pinMode(3, OUTPUT); // Declarar el LED como salida en el pin 3
  pinMode(2, OUTPUT); // Declarar el LED como salida en el pin 2

}

void loop() 
{
  if(pinLED < 2) // Termina la secuencia de 9-2 y comienza la secuencia 2-9
  {
    if (pinLEDAux > 9) pinLEDAux = 2; // Cuando termine la primera vuelta atrás, se reinicia el valor
    
    analogWrite(pinLEDAux,255); // Iluminación "máxima", considerada, del LED
    delay(100);
    analogWrite(pinLEDAux,175); // Iluminación "casi máxima", considerada, del LED
    delay(100);
    
    if(pinLEDAux!=2)  analogWrite(pinLEDAux-1,0); // Apagar el LED anterior
    
    analogWrite(pinLEDAux,100); // Iluminación "media", considerada, del LED
    delay(100);
    analogWrite(pinLEDAux,20); // Iluminación "mínima", considerada, del LED
    delay(100);
  
    pinLEDAux++; // Pasamos al LED siguiente, uno más

    if(pinLEDAux > 9) pinLED = 9; // Si termina la secuencia 9-2, vuelve a empezar desde el principio
  }
  
  analogWrite(pinLED,255); // Iluminación "máxima", considerada, del LED
  delay(100);
  analogWrite(pinLED,185); // Iluminación "casi máxima", considerada, del LED
  delay(100);
  
  if(pinLED!=9) analogWrite(pinLED+1,0); // Apagar el LED siguiente
  
  analogWrite(pinLED,100); // Iluminación "media", considerada, del LED
  delay(100);
  analogWrite(pinLED,20); // Iluminación "mínima", considerada, del LED
  delay(100);

  pinLED--; // Pasamos al LED siguiente, uno menos
}
