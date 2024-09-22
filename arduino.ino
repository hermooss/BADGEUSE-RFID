#include <SPI.h>
#include <RFID.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>  

#define PIN_ROUGE 7 
#define PIN_VERT 6
#define PIN_BLEU 5

String knownUIDs[] = {"2C28DC22",""};  // Liste des UID connues
unsigned char str[MAX_LEN];  // Buffer pour stocker l'UID


// Initialisation de l'écran LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Initialisation du lecteur rfid  pin 10 : lecteur SDA  pin 9 : lecteur RST 
RFID rfid(10, 9);

// Création d'un port série software pour communiquer avec l'ESP8266
SoftwareSerial espSerial(2, 3); // RX (D2) et TX (D3)

void setup() {
  
  if (!i2CAddrTest(0x27))
  {
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }
  
  // Initialisation du LCD
  lcd.init(); //Initialisation du module lcd 
  lcd.backlight();  // Allumer le rétroéclairage du LCD
  
  // Initialisation des LED
  pinMode(PIN_ROUGE, OUTPUT);
  pinMode(PIN_VERT, OUTPUT);
  pinMode(PIN_BLEU, OUTPUT);
  
  Serial.begin(9600);   // Port série pour communiquer avec le PC
  espSerial.begin(9600); // Port série pour communiquer avec l'ESP8266
  SPI.begin(); //Configurer le bus SPI sur l'Arduino pour permettre la communication avec le module RFID 
  rfid.init();  // Initialisation du module RFID
  Serial.println("Veuillez présenter la carte...");
  noblink(); //eteint la led rgb pour eviter les couleurs du programme précédent 
}

void loop() {

   // Vérifie s'il y a une carte RFID à proximité
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK) 
  {
    Serial.println("Carte détectée!");

    // Vérifie si la communication avec la carte est exempte de collisions (anticollision)
    if (rfid.anticoll(str) == MI_OK) 
    {
        String cardUid = recuperationUid(str) ; // Récupère l'UID de la carte 
        Serial.println("UID de la carte : " + cardUid);  // Vérifie si l'UID de la carte est autorisée et envoie l'outcome a l'ESP8266 
        isAuthorized(cardUid);
    }
    
    // Arrête la communication avec la carte en envoyant une commande HALT
    rfid.halt();
  }
}
