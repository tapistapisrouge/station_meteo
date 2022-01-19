#include <DHT.h>
#include <DHT_U.h>

#define brocheDHT 2
#define typeDHT 22

char annonce[]="Programme de test du DHT";
  
// instanciation de la library DHT
DHT dht(brocheDHT, typeDHT);

void setup() {
  // initialisation liaison série
  Serial.begin(9600);
  strcat(annonce,typeDHT);
  Serial.println(annonce);
  Serial.println("==========================");
  Serial.println();

  // Initialisation du DHT22;
  dht.begin();
  
}

// =================
// Boucle principale
// =================
void loop() {

  // lecture des données
  float tauxHumidite = dht.readHumidity();  // (taux en %)
  float temperatureEnCelsius = dht.readTemperature(); // Lecture temp en degrés celsius
    
  // calcul temp ressentie
  float temperatureRessentieCelsius = dht.computeHeatIndex(temperatureEnCelsius, tauxHumidite, false); // false = °C, true = °F
    
  // affichage valeurs
  Serial.print("Humidité = "); Serial.print(tauxHumidite); Serial.println(" %");
  Serial.print("Température = "); Serial.print(temperatureEnCelsius); Serial.println(" °C");
  Serial.print("Température ressentie = "); Serial.print(temperatureRessentieCelsius); Serial.println(" °C");
  Serial.println();
      
  // Temporisation de 10 secondes (pour rappel : il ne faut pas essayer de faire plus d'1 lecture toutes les 2 secondes, avec le DHT22, selon le fabricant)
  delay(10000);
}
