// Bibliotecas ------------------------------------------
#include <DHT.h>
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>

// DHT --------------------------------------------------
#define DHTPIN 13  // Pino de dados do sensor DHT
#define DHTTYPE DHT11  // Tipo do sensor (DHT11)
DHT dht(DHTPIN, DHTTYPE);


// LIQUID CRYSTAL ---------------------------------------
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
int lcdAddress = 0x27;

LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);  

// Firebase -- ------------------------------------------
#define FIREBASE_HOST "https://iot-comparador-de-temperatura-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "BTSW5WezqMv45Aw4hJXUeapcEVVdypMksCPOc37l"

// Wi-Fi ------------------------------------------------
#define ssid     "MatheusWifi"
#define password "1133gabi"



// Setup ------------------------------------------------
void setup() {

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connecting to ");
  lcd.setCursor(0, 1);
  lcd.print("WiFi ");

  // put your setup code here, to run once:
  dht.begin();
  delay(2000);

  Serial.begin(9600);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.println("--------------------------- WIFI CONNECTION ----------------------------");
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(5000);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  // --- LCD CRYSTAL CONNECTION VISUALIZATION
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Online");
  lcd.setCursor(0, 1);
  lcd.println("Updating time...");

  delay(3000);

  Serial.println("-------------------------- FIREBASE CONNECTION -------------------------");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    
  if (Firebase.failed()) {
    Serial.print("Falha na conexão com o Firebase. Código de erro: ");
    Serial.println(Firebase.error());
  } else {
    Serial.println("Conexão com o Firebase bem-sucedida...");
    Serial.println();
  }

  Serial.println("----------------- TEMPERATURE AMD HUMIDITY INFORMATION -----------------");
  Serial.println();

}

int n = 0;

void loop() {

  // Realizar a leitura do sensor
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  if (isnan(temp) || isnan(humid)) {
    Serial.println("Falha na leitura do sensor DHT");
    delay(2000);
    return;
  }
  
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C | ");
  Serial.print("Humidity: ");
  Serial.print(humid);
  Serial.print(" % ");

  Serial.println();

  delay(1000);

  lcd.setCursor(0, 0);           // set cursor to first column, first row
  lcd.print("Temp: ");
  lcd.print(temp);

  lcd.setCursor(0, 1);           // set cursor to first column, first row
  lcd.print("Humidity: ");
  lcd.print(humid);
  lcd.print("%");

  delay(2000);                   // wait for 2 seconds
  //lcd.clear();  

  String data = "{\"temperature\": " + String(temp) + ", \"humidity\": " + 
                                       String(humid) + ", \"timestamp\": \"" "\"}";

  Firebase.pushString("/data", data);

  delay(15000);

}
