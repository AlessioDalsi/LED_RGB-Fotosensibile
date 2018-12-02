// costanti per individuare i LED R, G e B rispettivamente ai pin 5, 6, 10 (a quanto pare il pin 7 del mio arduino non funziona)

#include <ESP8266WiFi.h>

const int LED_B_Pin = 5;
const int LED_G_Pin = 6;
const int LED_R_Pin = 10;
const int HALL_Pin = 11;
const int res = A0;

int value = 0;
int value_R = 0;

WiFiServer server(80);

const char* ssid = "TestAP";
const char* password = "TestAP";
const char* value = "";

void setup() {

	//connessione seriale
	Serial.begin(9600);

	WiFi.mode(WIFI_AP);
	WiFi.softAP(ssid, password, 1, 1);

	//impostiamo i pin RGB come uscita
	pinMode(LED_G_Pin, OUTPUT);
	pinMode(LED_R_Pin, OUTPUT);
	pinMode(LED_B_Pin, OUTPUT);
	pinMode(HALL_Pin, INPUT);

	server.begin();
}

void loop() {

	int hall_state = digitalRead(HALL_Pin);

	WiFiClient client = server.available();
	if (!client) {
		return;
	}

	String request = client.readStringUntil('\r');
	Serial.println(request);
	client.flush();

	String s = "HTTP/1.1 200 OK\r\n";
	s += "Content-Type: application/json\r\n\r\n";
	s += "{\"data\":{\"message\":\"success\",\"value\":\"";
	s += value;
	s += "\"}}\r\n";
	s += "\n";

	client.print(s);
	delay(1);
	Serial.println("Client disconnected");

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

