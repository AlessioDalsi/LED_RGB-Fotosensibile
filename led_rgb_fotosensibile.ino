// costanti per individuare i LED R,G e B rispettivamente ai pin 3,5 e 6
const int LED_B_Pin = 5;
const int LED_G_Pin = 6;
const int LED_R_Pin = 10;

const int res = A0;

int value = 0;

int value_R = 0;


void setup() {

	//connessione seriale
	Serial.begin(9600);

	//impostiamo i pin RGB come uscita
	pinMode(LED_G_Pin, OUTPUT);
	pinMode(LED_R_Pin, OUTPUT);
	pinMode(LED_B_Pin, OUTPUT);
}

void loop() {

	//lettura sensore A0 ovvero la tensione della fotoresistenza
	value = analogRead(res);
	value_R = value / 4;

	Serial.println(value_R);
	//il 255 serve a invertire il funzionamento più c'è luce meno si accende il led
	//Il led rosso serve a rappresentare i colori caldi mentre il blu i freddi
	analogWrite(LED_R_Pin, 255 - value_R);
	analogWrite(LED_B_Pin, value_R);
	delay(200);
}

