/*Objectif : établir une connexion a Adafruit via le protocole MQTT et gérer les afficher les messages d'erreurs possibles */
void connectToAdafruit() {
  Serial.println("Connexion à Adafruit IO... ");
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println("Protocole incorrect"); break;
      case 2: Serial.println("ID rejeté"); break;
      case 3: Serial.println("Serveur indisponible"); break;
      case 4: Serial.println("Mauvais utilisateur/mot de passe"); break;
      case 5: Serial.println("Non autorisé"); break;
      case 6: Serial.println("Échec de l'abonnement"); break;
      default: Serial.println("Connexion échouée"); break;
    }
    mqtt.disconnect();
    Serial.println("Nouvelle tentative de connexion...");
    delay(5000);  // Attendre avant de réessayer
  }
  Serial.println("Connexion à Adafruit IO réussie !");
}

/* Input : Un`String` provenant de l'Arduino, à publier sur le feed Adafruit IO
   Output : Affichage dans le moniteur série des informations sur la réussite ou l'échec de la publication du message
   Objectif : Vérifie que la chaine n'ai pas vie et publie un message sur le feed Adafruit IO en gérant les erreurs de publication */
void publishOnAdafruit(String message){
     if (message.length() > 0)
    {
      Serial.print("Message reçu de l'Arduino : ");
      Serial.println(message);
      // Publier le message sur le feed Adafruit IO "RFID_Attendance"
      if (!rfidAttendance.publish(message.c_str()))
      {
        Serial.println("Publication échouée!");
      }
    else
      {
        Serial.println("Message publié avec succès sur Adafruit IO.");
      }
    } 
    else
    {
      Serial.println("Message vide ignoré");
    }
}
