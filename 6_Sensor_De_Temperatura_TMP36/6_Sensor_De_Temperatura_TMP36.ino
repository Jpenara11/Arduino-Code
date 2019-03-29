/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 6. Sensor de temperatura TMP36
  Asignatura: Periféricos
      USAL 2018 - 2019
*/
/*
  CONVERSIÓN GRADOS KELVIN -> GRADOS CENTÍGRADOS

  ºK = ºC + 273.15
*/

 int InputTMP36 = A0; // Entrada analogica del sensor de temperatura
float gradosKelvin = 0; // Grados Kelvin obtenidos por el sensor de temperatura
float gradosCentigrados = 0; // Grados Centigrados, se pide mostrar por pantalla
float tempActual = 0; // Variable donde almacenamos la temperatura en grados centigrados

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  gradosCentigrados = analogRead(InputTMP36); // Obtenemos la temperatura captada por el sensor desde su entrada A0

  tempActual = (500.0 * gradosCentigrados/1024.0) - 50; // Ajustamos la temperatura al sensor
  
  gradosKelvin = tempActual + 273.15; // Conversion a grados Kelvin

  Serial.print("La temperatura en ºC es : "); // Sacamos por pantalla
  Serial.print(tempActual); // Sacamos por pantalla
  Serial.print(" La temperatura en ºK es : "); // Sacamos por pantalla
  Serial.println(gradosKelvin); // Sacamos por pantalla

  delay(5000); // Esperamos 5 segundos
}
