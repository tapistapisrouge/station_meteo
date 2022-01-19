#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

// Initalisation de la librairie
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// dht22 (température humidité)
#define brocheDHT 2
#define brocheDHT_bis 6
#define typeDHT 22

// led
#define LED_Green 3
#define LED_Yellow 4
#define LED_Red 5

// capteur humidité sol (résistivité électrique)
int pinSoilHumidity = A0;
int hsol; //mesure analogique du sol

// light sensor
const int lightPin = A1;

char annonce[]="Programme de test du DHT";
  
// instanciation de la library DHT
DHT dht(brocheDHT, typeDHT);
DHT dht_bis(brocheDHT_bis, typeDHT);


// creation variable millis
unsigned long currentTime=0;
unsigned long previousTime=0;
int interval=5000;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setCursor(1, 0);
  lcd.print("Initialisation");
  lcd.setCursor(2, 1);
  lcd.print("en cours ...");
  
  // initialisation liaison série
  Serial.begin(9600);
  strcat(annonce,typeDHT);
  Serial.println(annonce);
  Serial.println("==========================");
  Serial.println();

  // Initialisation du DHT22;
  dht.begin();
  dht_bis.begin();

  // vérification fonctionnement leds
  pinMode(brocheDHT,INPUT);
  pinMode(LED_Green,OUTPUT);
  pinMode(LED_Yellow,OUTPUT);
  pinMode(LED_Red,OUTPUT);

  digitalWrite(LED_Green,HIGH);
  digitalWrite(LED_Yellow,HIGH);
  digitalWrite(LED_Red,HIGH);
  delay(2000);
  digitalWrite(LED_Green,LOW);
  digitalWrite(LED_Yellow,LOW);
  digitalWrite(LED_Red,LOW);
  delay(2000);

  for (int i=3; i <= 5; i++){
    digitalWrite(i,HIGH);
    delay(2000);
    digitalWrite(i,LOW);
  }

  pinMode(pinSoilHumidity,INPUT);
  pinMode(lightPin,INPUT);
  
}

// =================
// Boucle principale
// =================
void loop() {

  currentTime = millis();

  if ((currentTime-previousTime)>interval) {

    // lecture des données
    float tauxHumidite = dht.readHumidity();  // (taux en %)
    float temperatureEnCelsius = dht.readTemperature(); // Lecture temp en degrés celsius

    // 2ème dht
    float tauxHumidite_bis = dht_bis.readHumidity();  // (taux en %)
    float temperatureEnCelsius_bis = dht_bis.readTemperature(); // Lecture temp en degrés celsius    
  
    // vérif de la réception des données
    if (isnan(tauxHumidite) || isnan(temperatureEnCelsius)) {
      Serial.println("une ou plusieurs valeurs manquent : Est_il bien branché ?");
      delay(2000);
      return;
    }
  
    // calcul temp ressentie
    float temperatureRessentieCelsius = dht.computeHeatIndex(temperatureEnCelsius, tauxHumidite, false); // false = °C, true = °F
    // calcul temp ressentie bis
    float temperatureRessentieCelsius_bis = dht_bis.computeHeatIndex(temperatureEnCelsius_bis, tauxHumidite_bis, false); // false = °C, true = °F
      
    // affichage valeurs sur moniteur série
    Serial.print("Humidité = "); Serial.print(tauxHumidite); Serial.println(" %");
    Serial.print("Température = "); Serial.print(temperatureEnCelsius); Serial.println(" °C");
    Serial.print("Température ressentie = "); Serial.print(temperatureRessentieCelsius); Serial.println(" °C");
    Serial.println();
    // affichage des valeurs du 2ème dht22 en série
    Serial.print("Humidité bis = "); Serial.print(tauxHumidite_bis); Serial.println(" %");
    Serial.print("Température = "); Serial.print(temperatureEnCelsius_bis); Serial.println(" °C");
    Serial.print("Température ressentie = "); Serial.print(temperatureRessentieCelsius_bis); Serial.println(" °C");
    Serial.println();   

    // affichage valeurs humidité du sol
    hsol = analogRead(pinSoilHumidity); // lecture tension analogique
    Serial.print("Humidité du sol = "); Serial.print(hsol); Serial.println(" ");
    Serial.println();

    // affichage valeurs light
    int lightValue = analogRead(lightPin); // lecture tension analogique
    lightValue = map(lightValue, 0, 800, 0, 10);
    Serial.print("Luminosité = "); Serial.print(lightValue); Serial.println(" ");
    Serial.println();         

    // affichage valeurs sur écran lcd
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Hum : ");
    lcd.setCursor(6,0);
    lcd.print(tauxHumidite);
    lcd.setCursor(12,0);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Temp : ");
    lcd.setCursor(7,1);
    lcd.print(temperatureRessentieCelsius);
    lcd.setCursor(12,1);
    lcd.print(" C");              
    

    if (temperatureRessentieCelsius < 18) {
      digitalWrite(LED_Red,HIGH);
      digitalWrite(LED_Green,LOW);
      digitalWrite(LED_Yellow,LOW);
    }
    else if ((temperatureRessentieCelsius >= 18)&&(temperatureRessentieCelsius <= 20)) {
      digitalWrite(LED_Red,LOW);
      digitalWrite(LED_Green,HIGH);
      digitalWrite(LED_Yellow,LOW);
    }
    else if (temperatureRessentieCelsius > 20) {
      digitalWrite(LED_Red,LOW);
      digitalWrite(LED_Green,LOW);
      digitalWrite(LED_Yellow,HIGH);
    }
    
    // Temporisation de 10 secondes (pour rappel : il ne faut pas essayer de faire plus d'1 lecture toutes les 2 secondes, avec le DHT22, selon le fabricant)
    // delay(10000);

    previousTime = currentTime;
  }

}

// concat avec string
// String str = "message test.";
// concatenation = str + " concatenation faite.";
// remplacement = str.replace("test, "reussi");
// comparaison ==> if(str==str1)
// 
