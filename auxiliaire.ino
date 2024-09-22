
/*Objectif : cette fonction a pour but de faire clignoter la led rgb en vert pendant 2 secondes */
void blinkVert(){
  unsigned long startTime = millis();
  while (millis() - startTime <= 2000){
    digitalWrite(PIN_VERT, LOW);
    digitalWrite(PIN_ROUGE, HIGH);
    digitalWrite(PIN_BLEU, HIGH);
    delay(100);
    digitalWrite(PIN_VERT, HIGH);
    delay(100);
  }
  lcd.clear();
}

/*Objectif : cette fonction a pour but de faire clignoter la led rgb en rouge pendant 2 secondes */
void blinkRouge(){
  unsigned long startTime = millis();
  while (millis() - startTime <= 2000){
    digitalWrite(PIN_ROUGE, LOW);
    digitalWrite(PIN_VERT, HIGH);
    digitalWrite(PIN_BLEU, HIGH);
    delay(100);
    digitalWrite(PIN_ROUGE, HIGH);
    delay(100);
  }
  lcd.clear();
}

/*Objectif : cette fonction permet d'eteindre la led  */
void noblink(){
  digitalWrite(PIN_ROUGE, HIGH);
  digitalWrite(PIN_VERT, HIGH);
  digitalWrite(PIN_BLEU, HIGH);
}


/* Input : String qui va etre vérifier
   Output : boolean qui indique si l'uid est connue ou pas 
   Objectif : vérifier si un certain uid appartient a la liste des uid connue */
bool isKnownUID(String uid) {
  int taille = sizeof(knownUIDs) / sizeof(knownUIDs[0]);
  //parcourir la liste des knownUIDs et retourner vrai si il ya correspendace 
  for (int i = 0; i < taille ; i++) {
    if (knownUIDs[i] == uid) {
      return true;
    }
  }
  return false;
}

/* Input : une adresse I2C (uint8_t)
   Output : un booléen qui indique si l'appareil à l'adresse donnée répond
   Objectif : tester si un appareil à une adresse I2C donnée est présent sur le bus I2C */
bool i2CAddrTest(uint8_t addr) {
  Wire.begin();
  Wire.beginTransmission(addr);
  
  if (Wire.endTransmission() == 0)   return true;

  return false;
}

/* Input : une chaîne de caractères (const char*) représentant l'UID lue par le lecteur RFID
   Output : une chaîne de caractères (String) contenant l'UID convertie en format hexadécimal
   Objectif : récupérer l'UID lue d'une carte RFID, la formater en une chaîne hexadécimale lisible et la retourner */
String recuperationUid( const uint8_t* str) {
      String cardUID = "";
        for (int i = 0; i < 4; i++) {  //UID de 4 octets
          if (str[i] < 0x10) {
            cardUID += "0";  // Ajoute un zéro pour les valeurs inférieures à 0x10
          }
          cardUID += String(str[i], HEX);  // Convertit en hexadécimal
        }
        cardUID.toUpperCase();  // Pour avoir le même format que dans `knownUIDs`
        return cardUID ; 
}

/* Input : Une chaîne de caractères `cardUid` représentant l'UID de la carte RFID lue
   Output : Un affichage sur l'écran LCD, une LED allumée (verte ou rouge), et un message envoyé à l'ESP8266
   Objectif : Vérifier si une carte RFID est autorisée et effectuer des actions en fonction du statut de la carte (connue ou inconnue) et envoie l'output a l'ESP8266 */
void isAuthorized ( String cardUid ) {
        if (isKnownUID(cardUid)) {
          lcd.print("Carte valide!");
          blinkVert();  // Si la carte est valide, LED verte
          espSerial.println("La carte " + cardUid + " est valide!");  // Envoi du message à l'ESP8266
        } else {
          lcd.print("Carte inconnue!");
          blinkRouge();  // Si la carte est inconnue, LED rouge
          espSerial.println("La carte " + cardUid + " est inconnue!");  
      }
}
