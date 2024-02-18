#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
int solenoidPin = 8;                    //This is the output pin on the Arduino
const int knockSensor = A0;  // the piezo is connected to analog pin 0
const int threshold = 100;   // threshold value to decide when the detected sound is a crash or not

// these variables will change:
int sensorReading = 0;  // variable to store the value read from the sensor pin

void setup() 
{
  pinMode(solenoidPin, OUTPUT);          //Sets that pin as an output
  Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}
bool solenoidState = LOW; // Current state of the solenoid

void loop() 
{

  sensorReading = analogRead(knockSensor);
  // // if the sensor reading is greater than the threshold:
  if (sensorReading >= threshold) {
    Serial.print("Crash");
  }
  delay(100);  // delay to avoid overloading the serial port buffer

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  solenoidState = !solenoidState; // Toggle the state
  digitalWrite(solenoidPin, solenoidState); // Apply the new state to the solenoid
  Serial.println(solenoidState ? "Solenoid ON" : "Solenoid OFF");                //Wait 1 Second
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  delay(1000);
}