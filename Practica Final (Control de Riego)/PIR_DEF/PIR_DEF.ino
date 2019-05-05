const int buzzer = 8;
const int sensorPIR = 2;
const int led = 13;
const int ledWiFi = 12;
const int boton = 3;

int estadoPIR = -1; //
int valorPIR = 0;
int valorBoton = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(sensorPIR, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(ledWiFi, OUTPUT);
  pinMode(boton, INPUT);
}

void loop() 
{
  valorPIR = digitalRead(sensorPIR);
  valorBoton = digitalRead(boton);
  Serial.print("INICIO: ");
  Serial.println(valorBoton);

  switch(estadoPIR)
  {
    case -1: // Carga condensadores pre-inicio
            delay(2000); // Esperamos 20 segundos a que se carguen los condensadores
            Serial.println("FIN CARGA");
            estadoPIR = 0;
            break;
    case 0: // Alarma apagada, esperando a que se el usuario la inicie
            if (valorBoton == 0)
            {
              estadoPIR = 1;
              digitalWrite(led, HIGH);
              delay(5000); // Esperamos 10 segundos a que el usuario se aleje
              digitalWrite(led, LOW);
            }
            break;
    case 1:
            Serial.println("ESTAMOS EN EL ESTADO 1");
            if (valorPIR == HIGH) estadoPIR = 2; 
            else estadoPIR = 1;
            break;
    case 2:  
            while(valorBoton != 0)
            {
              Serial.println("ALARMA ACTIVADA");
              digitalWrite(ledWiFi, HIGH);
              digitalWrite(led, HIGH);
              digitalWrite(buzzer, HIGH);
              delay(500);
              digitalWrite(led, LOW);
              digitalWrite(buzzer, LOW);
              delay(500);
              delay(1000);
              valorBoton = digitalRead(boton);
            }
            digitalWrite(ledWiFi, LOW);
            estadoPIR = -1;
            digitalWrite(led, HIGH);
            delay(5000); //Alarma desactivada
            Serial.println("ALARMA DESACTIVADA");
            digitalWrite(led, LOW);
            break;     
  }

  delay(2000);
}
