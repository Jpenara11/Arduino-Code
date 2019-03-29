/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 4. Distancia mediante ultrasonidos HC-SR04
  Asignatura: Periféricos
      USAL 2018 - 2019
  Demostracion Teorica obtenida de: https://www.youtube.com/watch?v=ZejQOX69K5M
*/

/* DEMOSTRACIÓN TEÓRICA

  Vel Sonido1 = 343 m/s
  Vel Sonido2 (cm/microSeg) = 343 m/s * 100 cm/cm * 1/1000000 s/microSeg = 1/29.2 cm/microSeg

  Distancia = Tiempo(microSeg) / 29.2 * 2 (Pulso de ida y vuelta, por eso dividido por 2)
*/

const int LED = 9; // Declaramos pin del LED
const int EchoPin = 5; // Declaramos pin del Echo del HC-SR04
const int TriggerPin = 6; // Declaramos pin del Trigger del HC-SR04

void setup() 
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT); // Declaramos LED como salida
  pinMode(EchoPin, INPUT); // Declaramos el Echo como entrada
  pinMode(TriggerPin, OUTPUT); // Declaramos el Trigger como salida

}

void loop() 
{
  int cm = ping(TriggerPin, EchoPin); // Obtenemos la distancia en centímetros

  Serial.println(cm); // Sacamos por pantalla el valor de la distancia
  analogWrite(LED, cm); // En función de la distancia, cambia la intensidad del LED

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
