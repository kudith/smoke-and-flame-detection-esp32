#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <config.h>

// Deklarasi pin perangkat keras

#define MQ2_PIN 32          
#define FLAME_ANALOG_PIN 33  
#define LED_PIN 2           
#define BUZZER_PIN 25        
#define RELAY_PIN 26         


// Array berisi beberapa chat ID untuk notifikasi
String chatIDs[] = {"1818710520", "5922974167", "1241764345", "5772077183"}; 
int numRecipients = sizeof(chatIDs) / sizeof(chatIDs[0]);   

int smokeThreshold = 1500;   
int flameThreshold = 1000;   

bool smokeAlertSent = false;
bool flameAlertSent = false; 

void setup() {
  Serial.begin(115200);

  // Inisialisasi WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");

  // Inisialisasi pin perangkat keras
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(FLAME_ANALOG_PIN, INPUT);

  // Atur perangkat keras ke keadaan awal
  digitalWrite(LED_PIN, LOW);      
  digitalWrite(BUZZER_PIN, LOW);  
  digitalWrite(RELAY_PIN, HIGH);  
  Serial.println("Smoke and Flame Detection System Initialized");
}

// Fungsi untuk mengirim notifikasi ke semua penerima di Telegram
void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    for (int i = 0; i < numRecipients; i++) {
      HTTPClient http;
      String url = "https://api.telegram.org/bot" + String(telegramToken.c_str()) + "/sendMessage?chat_id=" + chatIDs[i] + "&text=" + message;
      http.begin(url);
      int httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.println("Message sent to chat ID " + chatIDs[i]);
      } else {
        Serial.print("Failed to send message to chat ID " + chatIDs[i] + ": ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int smokeValue = analogRead(MQ2_PIN);       
  int flameValue = analogRead(FLAME_ANALOG_PIN);

  Serial.print("Gas Level: ");
  Serial.println(smokeValue);               
  Serial.print("Flame Level: ");
  Serial.println(flameValue);                

  // Deteksi asap dan kirim notifikasi jika melampaui ambang batas
  if (smokeValue > smokeThreshold) {
    if (!smokeAlertSent) {
      digitalWrite(LED_PIN, HIGH);          
      digitalWrite(BUZZER_PIN, HIGH);        
      delay(5000);                          
      digitalWrite(BUZZER_PIN, LOW);        
      digitalWrite(RELAY_PIN, HIGH);         
      sendTelegramMessage("Asap/Gas Terdeteksi!");
      smokeAlertSent = true;
    }
  } else {
    smokeAlertSent = false;
    if (!flameAlertSent) {
      digitalWrite(LED_PIN, LOW);           
    }
    Serial.println("No smoke detected.");
  }

  // Deteksi api dan kirim notifikasi jika melampaui ambang batas
  if (flameValue < flameThreshold) {
    if (!flameAlertSent) {
      digitalWrite(LED_PIN, HIGH);         
      digitalWrite(BUZZER_PIN, HIGH);      
      digitalWrite(RELAY_PIN, LOW);        
      delay(5000);                         
      digitalWrite(BUZZER_PIN, LOW);        
      sendTelegramMessage("Api Terdeteksi!");
      flameAlertSent = true;
    }
  } else {
    flameAlertSent = false;
    if (!smokeAlertSent) {
      digitalWrite(LED_PIN, LOW);           
      digitalWrite(RELAY_PIN, HIGH);       
    }
    Serial.println("No flame detected within range.");
  }

  delay(100); 
}
