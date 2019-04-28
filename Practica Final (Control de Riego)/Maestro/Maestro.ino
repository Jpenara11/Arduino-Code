#include <Wire.h>

void setup() {
  // Unimos este dispositivo al bus I2C
  Wire.begin();
}
 
byte estado = 0;
 
void loop() {

    // Comenzamos la transmisión al dispositivo 1
    Wire.beginTransmission(1);
 
    // Enviamos un byte, será el pin a encender
    Wire.write(13);
 
    // Enviamos un byte, L pondrá en estado bajo y H en estado alto
    Wire.write(estado);
 
    // Paramos la transmisión
    Wire.endTransmission();
 
    // Esperamos 1 segundo
    delay(1000);

 
  // Cambiamos el estado
  if (estado == 0)
  {
    estado = 1;
  }
  else
  {
    estado = 0;
  }
}
