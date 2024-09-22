#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Paramètres WiFi
#define WLAN_SSID       "SSID"       // Nom du réseau WiFi (SSID)
#define WLAN_PASS       "PASSWORD" // Mot de passe du réseau WiFi

// Paramètres Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "kogab76422"
#define AIO_KEY         "aio_TvwZ45eLhdBxYRBvzvOEt5Ai6Qyy"

// Création d'un client WiFi pour se connecter au serveur MQTT
WiFiClient client;

// Création d'un client MQTT en passant les informations de connexion
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Création d'une feed MQTT pour publier les résultats dans "RFID_Attendance"
Adafruit_MQTT_Publish rfidAttendance = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/RFID_Attendance");

void setup() {
  Serial.begin(9600);  // Port série pour recevoir de l'Arduino Uno
  
  // Connexion au réseau WiFi
  Serial.println();
  Serial.print("Connexion à ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wi-Fi Connecté!");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());
  
  // Connexion à Adafruit IO
  connectToAdafruit();
}
void loop() {
  // Si des données sont disponibles sur le port série, les lire
  if (Serial.available()) 
  {
    String message = Serial.readStringUntil('\n');  // Lire le message envoyé par l'Arduino jusqu'au caractère '\n'
    message.trim(); // Supprime les espaces et caractères vides
   
    publishOnAdafruit(message);
  }
  
  // Garder la connexion avec Adafruit IO
  if (!mqtt.ping(3))
  {
    if (!mqtt.connected())
    {
      connectToAdafruit();  // Reconnexion si déconnecté
    }
  }
}
