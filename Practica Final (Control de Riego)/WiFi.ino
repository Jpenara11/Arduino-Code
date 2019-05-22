/* CONTROL DE RIEGO: PARTE 3 (WIFI NODEMCU)
   AUTORES: De la Peña Ramos, Jaime
            Martin Garcia, Juan Carlos
            Montero Fernandez, Alberto
   FECHA DE LA DEFENSA: 22/05/19

            UNIVERSIDAD DE SALAMANCA 2018-2019
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Inicializamos la conexion WIFI con el Router
char ssid[] = "Jpenara11";     // el nombre de tu Red
char password[] = "3141592jpenara"; // la clave de tu Red

// Initialize Telegram BOT
#define BOTtoken "825106201:AAHgcq5pGZeD1A_3si00lXXjty3DRHQ76Iw"  // el token de tu BOT, lo obtenemos de BotFather

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 50; //Tiempo medio entre el escane0 de omensajes
long Bot_lasttime;   //exploracion de el ultimo mensaje
bool Start = false;
String chat_id2;
bool flag = false;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    chat_id2 = chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/start") 
    {
      String welcome = "Hola!, Bienvenido, " + from_name + ".\n";
      welcome += "Escribe /alarma para conocer el estado de tu alarma!\n\n";

      bot.sendMessage(chat_id, welcome);
    }

    if (text == "/alarma") {
      if (digitalRead(4) == LOW) {
        bot.sendMessage(chat_id, "ALARMA EN REPOSO...");
      } else {
        bot.sendMessage(chat_id, "ALARMA CONECTADA!");
      }
    }
  }
}


void setup() {
  Serial.begin(9600);

  // Establecer el modo WiFi y desconectarse de un AP si fue Anteriormente conectada
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Intentar conectarse a la red
  Serial.print("Conectando al Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi conectada");
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    if (digitalRead(4) == HIGH) {
      if (flag == false) {
        bot.sendMessage(chat_id2, "¡¡¡ALARMA ACTIVADA!!!");
        flag = true;
      }
    } else {
      flag = false;
    }

    Bot_lasttime = millis();
  }
}
