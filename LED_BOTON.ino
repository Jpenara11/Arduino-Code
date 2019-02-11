
/* Título : LED CON BOTÓN 
 * Autor  : Jaime de la Peña Ramos
 * Año  : 2019
*/

int estadoBoton = 0; // Estado actual del botón
int salidaLED = 0; // 0 -> LED APAGADO, 1 -> LED ENCENDIDO
int estadoAnteriorBoton = 0; // Estado anterior del botón

void setup() {
  pinMode(8, INPUT); // Declarar botón como entrada
  pinMode(4, OUTPUT); // Declarar LED como salida

}

void loop() {
  estadoBoton = digitalRead(8); // Leer el estado del botón
  
  if((estadoBoton == HIGH)&&(estadoAnteriorBoton == LOW))
  {
    salidaLED = 1 - salidaLED;
    delay(10); // Impedir efecto rebote botón
    
  }

  estadoAnteriorBoton = estadoBoton; // Guardar el estado actual

  if(salidaLED == 1) // Si hay que encender el LED
  {
    digitalWrite(4, HIGH); // Se enciende el LED
  }
  else
  {
    digitalWrite(4, LOW); // Se apaga el LED
  }

}
