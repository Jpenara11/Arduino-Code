/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 5. Sensor de flexion
  Asignatura: Periféricos
      USAL 2018 - 2019
*/

/*
  DIVISOR DE TENSION

  Vout = (R2 / (R1 + R2)) * Vin 
*/

const int InputDivTension = A0; // Entrada analogica del divisor de tension
const int Vin = 5; // Voltaje de entrada 5 Voltios
const int R2 = 2200; // R2 tiene valor de 2200 Ohmios -> 2.2K Ohomios
const int LEDVerde = 7; // Pin conectado LED verde
const int LEDRojo = 8; // Pin conectado LED rojo
int R1 = 0; // R1 tendra un valor dependiendo de la flexion del flexiometro
int calculoDivTension = 0; // Calculo matematico del Vout del divisor de tension
int Vout = 0; // Variable donde se almacena el valor obtenido por arduino del Vout
int VoutMap = 0; // Variable donde se almacena el valor mapeado de Vout
int calculoDivTensionMap = 0; // Variable donde se almacena el valor mapeado de calculoDivTension

void setup() 
{
  Serial.begin(9600); // Establecemos la velocidad del puerto serie
  pinMode(LEDRojo,OUTPUT); // Pin LED Rojo
  pinMode(LEDVerde,OUTPUT); // Pin LED Verde
}

void loop() 
{
  Vout = analogRead(InputDivTension); // Obtenemos el Vout directamente de la entrada A0
  R1 = ((R2 * Vin)/Vout) - R2; // Calculamos el valor de la R1 (Resistencia ejercida por el flexiometro)
  Serial.print("Vout Divisor Tension: "); // Sacamos por pantalla
  Serial.println(Vout); // Sacamos por pantalla
  
  calculoDivTension = (R2 / (R1 + R2)) * Vin; // Calculamos teóricamente el valor de Vout
  Serial.print("Vout Calculo Divisor Tension: "); // Sacamos por pantalla
  Serial.println(calculoDivTension); // Sacamos por pantalla

  VoutMap = map(Vout,45, 80, 0, 5); // Calculamos el mapeado para el voltaje de salida
  calculoDivTensionMap = map(calculoDivTension, 45, 80, 0, 5); // Calculamos el mapeado para el calculo del divisor de tensión

  Serial.print("Vout Divisor de Tension Mapeado: "); // Sacamos por pantalla
  Serial.println(VoutMap); // Sacamos por pantalla
  Serial.print("Vout Calculo Divisor Tension Mapeado: "); // Sacamos por pantalla
  Serial.println(calculoDivTensionMap); // Sacamos por pantalla
  
  if (VoutMap == calculoDivTensionMap) // Comparamos el valor obtenido teóricamente con el valor práctico
  {
    digitalWrite(LEDVerde, HIGH); // Encendemos LED verde
    digitalWrite(LEDRojo, LOW); // Apagamos LED rojo
  }

  else // No son iguales los valores teórico y práctico
  {
    digitalWrite(LEDRojo, HIGH); // Encendemos LED rojo
    digitalWrite(LEDVerde, LOW); // Apagamos LED verde
  }
  delay(5000); // Esperamos 5 segundos
}
