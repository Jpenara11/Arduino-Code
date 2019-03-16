/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 2. Detector de luz / oscuridad
  Asignatura: Periféricos
      USAL 2018 - 2019
*/


int salidaLED = 0; // LED APAGADO = 0, LED ENCENDIDO = 1
const int sensorLDR = A0; // Seleccionar la entrada analógica de la LDR
int valorLDR; // variable que almacena el valor de la LDR
int valorEscalado; // Variable escalada
void setup() 
{
  pinMode(9, OUTPUT); // Declarar el LED como salida en el pin 4
  pinMode(13, OUTPUT); // Declarar el LED integrado en Arduino pin 13
  Serial.begin(9600); // Abre el puerto serie y fija la velocidad de 9600 baudios en este
}

void loop() 
{
  valorLDR = analogRead(sensorLDR); // Leer el valor LDR ¿Cantidad luz?
  valorEscalado = map(valorLDR, 840, 1000, 0, 10); // Escalado de los valores máximos y mínimos de la LDR
  Serial.println(valorLDR); // Sacar por pantalla el valor de la LDR
  
  if (valorEscalado <= 5) // Comprobar cantidad luz LDR, menor a un umbral considerado
  {
    digitalWrite(9, HIGH); // Encender LED 9
    digitalWrite(13, LOW); // Apagar LED 13
    delay(100); // Esperar 0.1 segundos
  }

  if(valorEscalado > 5) // Comprobar cantidad luz LDR, mayor a un umbral considerado
  {
    digitalWrite(9, LOW); // Apagar LED 9
    digitalWrite(13, HIGH); // Encender LED 13
    delay(100); // Esperar 0.1 segundo
  }
}
