
/* Título : LED REGULADO MEDIANTE PWM (PULSE WIDTH MODULATION) 
 * Autor  : Jaime de la Peña Ramos
 * Año  : 2019
*/

const int ledPWM = 3;
const int inputPWM = A5; // Entrada analógica conectada a A5
int salidaLED = 0;
int valorPotenciometro = 0;

void setup() {
  pinMode(ledPWM, OUTPUT);
  
  // Los pines de entrada analógicos se declaran automáticamente
}

void loop() {
  valorPotenciometro = analogRead(inputPWM);

  salidaLED = 0.25 * valorPotenciometro;
  
  analogWrite(ledPWM, salidaLED); 
}
