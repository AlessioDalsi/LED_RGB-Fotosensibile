// costanti per individuare i LED R, G e B rispettivamente ai pin 5, 6, 10 (a quanto pare il pin 7 del mio arduino non funziona)


#include <LiquidCrystal_I2C.h>
#include <DHT_U.h>
#include <DHT.h>
#include <Wire/Wire.h>
#include <SoftwareSerial.h>

const int LED_B_Pin = 5;
const int LED_G_Pin = 6;
const int LED_R_Pin = 10;
const int HALL_Pin = 11;
const int res = A0;

LiquidCrystal_I2C lcd(0x27, 20, 4);

int value = 0;
int value_R = 0;

#define DHTPIN 4
#define DHTTYPE DHT11

float temperatura;
float umidita;

DHT dht(DHTPIN, DHTTYPE);


//Bluetooth

int rxPin = 9;
int txPin = 8;
SoftwareSerial bluetooth(rxPin, txPin);

String message; //string that stores the incoming message
const int BT_Led = 13;



void setup() {

	//connessione seriale
	Serial.begin(9600);
	bluetooth.begin(9600);

	//impostiamo i pin RGB come uscita
	pinMode(LED_G_Pin, OUTPUT);
	pinMode(LED_R_Pin, OUTPUT);
	pinMode(LED_B_Pin, OUTPUT);
	pinMode(HALL_Pin, INPUT);

	pinMode(BT_Led, OUTPUT);

	lcd.begin();
	lcd.backlight();

	lcd.println("Intializing...");

	dht.begin();
}

void loop() {


	while (bluetooth.available())
	{
		message = char(bluetooth.read());
	}

	delay(2000);
	lcd.clear();
	umidita = dht.readHumidity();
	temperatura = dht.readTemperature();

	if (isnan(umidita) || isnan(temperatura)) {
		Serial.println("Failed to read from DHT sensor!");
		lcd.clear();
		lcd.println("Failed to read from DHT sensor!");
		return;
	}

	lcd.println("Umidita: ");
	lcd.print(umidita);
	lcd.print(" % ");
	lcd.println("Temperatura: ");
	lcd.print(temperatura);
	lcd.print(" C");
	String text = "";

	int hall_state = digitalRead(HALL_Pin);


	if (!bluetooth.available()) {

		if (message != "") {

			if (message == "1") {

				digitalWrite(BT_Led, HIGH);

				bluetooth.println("Sistema acceso");

				text = "Temperatura :" + String(temperatura) + " Umidita: " + String(umidita) + ";";
				bluetooth.print(text);


				if (hall_state == HIGH) {
					//lettura sensore A0 ovvero la tensione della fotoresistenza
					value = analogRead(res);
					value_R = value / 4;

					//Serial.println(value_R);

					//il 255 serve a invertire il funzionamento più c'è luce meno si accende il led
					//Il led rosso serve a rappresentare i colori caldi mentre il blu i freddi
					analogWrite(LED_R_Pin, 255 - value_R);
					analogWrite(LED_B_Pin, value_R);
					analogWrite(LED_G_Pin, 0);
					delay(500);
				}
				else {
					analogWrite(LED_R_Pin, 0);
					analogWrite(LED_B_Pin, 0);
					analogWrite(LED_G_Pin, 255);
				}
			}
			else if (message == "0") {

				digitalWrite(BT_Led, LOW);

				bluetooth.println("Sistema spento");
				bluetooth.print(";");

				text = "Temperatura :" + String(temperatura) + " Umidita: " + String(umidita) + ";";
				bluetooth.print(text);


				analogWrite(LED_R_Pin, 0);
				analogWrite(LED_B_Pin, 0);
				analogWrite(LED_G_Pin, 0);
			}

		}
	}

}

