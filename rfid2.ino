
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RST_PIN 9          // Configurable, see typical pin layout above
#define SS_PIN 10         // Configurable, see typical pin layout above
#define BUZZER 2

#define LED_R 4
#define LED_G 7

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  Serial.println(F("--------------WELCOME TO MCA LAB---------------------"));
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("MCA LAB");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Start scanning");
  delay(3000);
  lcd.clear();
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	//mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  //Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "03 F7 B8 02") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Access Granted");
    Serial.println("Student Name: Krishna Prasad");

    lcd.setCursor(0,0);
    lcd.print("ACCESS GRANTED..");
    lcd.setCursor(0,1);
    lcd.print("KRISHNA PRASAD");
    delay(500);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 600);
    delay(100);
    noTone(BUZZER);
    Serial.println();
    delay(500);
    digitalWrite(LED_G, LOW);
    delay(2000);
    lcd.clear();
  }
 
 else   {
    Serial.println(" Access denied");
    lcd.setCursor(0,0);
    lcd.print("ACCESS DENIED.");
    lcd.setCursor(5,1);
    lcd.print(" :( ");
    delay(500);
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 200);
    delay(800);
    noTone(BUZZER);
    Serial.println();
    delay(500);
    digitalWrite(LED_R, LOW);
    delay(2000);
    lcd.clear();
  }
}
