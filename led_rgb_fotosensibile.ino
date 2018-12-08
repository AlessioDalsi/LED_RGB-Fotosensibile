// costanti per individuare i LED R, G e B rispettivamente ai pin 5, 6, 10 (a quanto pare il pin 7 del mio arduino non funziona)

#include <DHT_U.h>
#include <DHT.h>

const int LED_B_Pin = 5;
const int LED_G_Pin = 6;
const int LED_R_Pin = 10;
const int HALL_Pin = 11;
const int res = A0;

int value = 0;
int value_R = 0;

#define DHT_PIN 2
#define DHT_TYPE DHT22

float temperatura;
float umidita;

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {

	//connessione seriale
	Serial.begin(9600);

	//impostiamo i pin RGB come uscita
	pinMode(LED_G_Pin, OUTPUT);
	pinMode(LED_R_Pin, OUTPUT);
	pinMode(LED_B_Pin, OUTPUT);
	pinMode(HALL_Pin, INPUT);

	dht.begin();
}

void loop() {

	delay(2000);

	umidita = dht.readHumidity();
	temperatura = dht.readTemperature();

	if (isnan(umidita) || isnan(temperatura)) {
		Serial.println("Failed to read from DHT sensor!");
		return;
	}

	Serial.println("Umidità: ");
	Serial.print(umidita);
	Serial.print(" %\t");
	Serial.println("Temperatura: ");
	Serial.print(temperatura);
	Serial.print(" °C ");

	int hall_state = digitalRead(HALL_Pin);

	//if (hall_state == HIGH) {
	//	//lettura sensore A0 ovvero la tensione della fotoresistenza
	//	value = analogRead(res);
	//	value_R = value / 4;
	//		

	//	Serial.println(value_R);
	//	//il 255 serve a invertire il funzionamento più c'è luce meno si accende il led
	//	//Il led rosso serve a rappresentare i colori caldi mentre il blu i freddi
	//	analogWrite(LED_G_Pin, 255 - value_R);
	//	analogWrite(LED_B_Pin, value_R);
	//	delay(200);
	//}else{
	//	analogWrite(LED_R_Pin, 0);
	//	analogWrite(LED_B_Pin, 0);
	//}
	if (hall_state == HIGH) {

		value = analogRead(res);
		value_R = value / 4;

		analogWrite(LED_R_Pin, 255 - value_R);
		analogWrite(LED_B_Pin, value_R);
		analogWrite(LED_G_Pin, 0);
		delay(200);
	}
	else {
		analogWrite(LED_R_Pin, 0);
		analogWrite(LED_B_Pin, 0);
		analogWrite(LED_G_Pin, 0);
	}
}

