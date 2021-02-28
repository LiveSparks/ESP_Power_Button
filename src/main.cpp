/**
 * ESP Power Button Sketch by @livesparks
 * For Instructions Visit : <GitHub Page Link Here>
 * Watch the Video : <Youtube Video Link Here>
 * 
 * >>>>>>>>>>>>>>>> FEATURES <<<<<<<<<<<<<<<<
 * ----------------WEB UI------------------
 * The device hosts a web server with a button that can be 
 * used to manipulate the power button of a PC. Also provides 
 * a slider that contols the PWM output can be used to adjust 
 * the brightness of a PC Case LED Strip.
 * 
 * ------------Stat Monitoring--------------
 * Requires you to run the following program on your PC : <Link to PC Software>
 * Allows you to monitor the following Statistics of your PC in the Web UI :
 * 	-> CPU Usage
 * 	-> Ram Usage
 * 	-> Hard Disk Usage
 * 
 * --------------Macro Board----------------
 * Requires you to run the following program on your PC : <Link to PC Software>
 * Requires a button matrix : <Link to schematics>
 * By Connecting a matrix keypad you can send keyboard hotkeys to your PC.
 * 
 * 
 * >>>>>>>>>>>>>>>>>>>> REQUIRNMENTS <<<<<<<<<<<<<<<<<<<<
 * This Code is written for the ESP8266 and requires the following libraries:
 * 		-> ESP8266WiFi
 * 		-> ESPAsyncTCP
 * 		-> ESPAsyncWebServer
 * 		-> ESPUI
 * 		-> ESPAsyncWiFiManager
 * 		-> DNSServer
 * 
 * >>>>>>>>>>>>>>>>>>> USAGE <<<<<<<<<<<<<<<<<<<<<
 * POWER BUTTON		: GPIO 5
 * LED STRIP		: GPIO 4
 * MATRIX ROW 1		: GPIO 15
 * MATRIX ROW 2		: GPIO 16
 * MATRIX COL 1		: GPIO 13
 * MATRIX COL 2		: GPIO 12
 * MATRIX COL 3		: GPIO 14
 * 
 * On first boot up, the device will host a WiFi Access Point by the name of
 * "ESP Power Btn". Connect to if and it should prompt you to sign-in. If not
 * the go to the IP Address 192.168.4.1 and enter your WiFi details. The
 * device should reboot and print the IP Address on serial.
 * It is recommended that you assign the board a Static IP from your router.
 * When you go to the new IP address, you should see the Web UI.
 * 
 * To use with Adruino IDE, rename this file to main.ino and comment out the
 * first include statement : #include <Arduino.h> -> //#include <Arduino.h>
 **/

#include <Arduino.h>
#include <DNSServer.h>
#include <ESPUI.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWiFiManager.h>

#define LED_PIN	4
#define PWR_PIN	5

#define LONG_PRESS_DELAY	3	//n*200 milliseconds

uint8_t btn_state = 0xFF;

//to use a bigger keypad, modify the following variables
uint8_t btn_row[] = {15, 16};
uint8_t btn_col[] = {13, 12, 14};
#define TOTAL_ROW 2
#define TOTAL_COL 3

int cpuLabelID;
int memLabelID;
int dskLabelID;

DNSServer dnsServer;
const char *hostname = "ESPPowerBtn";

void buttonCallback(Control *sender, int type) {
  switch (type) {
  case B_DOWN:
    Serial.println("Button DOWN");
	digitalWrite(PWR_PIN, LOW);
    break;

  case B_UP:
    Serial.println("Button UP");
	digitalWrite(PWR_PIN, HIGH);
    break;
  }
}

void sliderCallback(Control *sender, int type) {
	int val = sender->value.toInt();
	val = map(val,0,100,0,1023);
	analogWrite(LED_PIN, val);
}

AsyncWebServer server(80);

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);

	for (uint8_t i = 0; i < TOTAL_ROW; i++)
	{
		pinMode(btn_row[i], OUTPUT);
	}

	for (uint8_t i = 0; i < TOTAL_COL; i++)
	{
		pinMode(btn_col[i], INPUT_PULLUP);
	}
	
	pinMode(LED_PIN, OUTPUT);
	pinMode(PWR_PIN, OUTPUT);
	digitalWrite(PWR_PIN, HIGH);

	// connect to WiFi
	WiFi.mode(WIFI_STA);
	AsyncWiFiManager wifiManager(&server,&dnsServer);
	bool res = wifiManager.autoConnect("ESP_PowerBtn");
	if(!res){
		Serial.println("Failed to Connect");
	}
	else{
		Serial.println("Connected to WiFi");
	}

	// Print IP
	Serial.print("IP Address: ");
	Serial.println(WiFi.localIP());

	ESPUI.button("Power Button", &buttonCallback, ControlColor::Peterriver, "Press");
	ESPUI.slider("LED Brightness", &sliderCallback, ControlColor::Emerald, 50, 0, 100);
	cpuLabelID = ESPUI.label("CPU Usage",ControlColor::Peterriver,"0%");
	memLabelID = ESPUI.label("Memory Usage",ControlColor::Peterriver,"0%");
	dskLabelID = ESPUI.label("Disk Usage",ControlColor::Peterriver,"0%");

	ESPUI.sliderContinuous = true;
	ESPUI.begin("ESP Power Btn 🔘");
}

void loop() {
	// put your main code here, to run repeatedly:
	static bool debounceDelayActive = false;
	static long debounceMillis = 0;
	static uint8_t pressTime[6] = {0,0,0,0,0,0};
	if(!debounceDelayActive){
		for (uint8_t row = 0; row < TOTAL_ROW; row++)
		{
			digitalWrite(btn_row[row],LOW);
			for (uint8_t col = 0; col < TOTAL_COL; col++)
			{
				bool state = digitalRead(btn_col[col]);
				bool last_state = btn_state & (1 << (col + row*TOTAL_COL));
				if (state != last_state)
				{
					Serial.print("F");
					Serial.print(13+col+row*TOTAL_COL);
					if(state){
						Serial.print("UP");
						btn_state |= 1 << (col+row*TOTAL_COL);
						pressTime[col+row*TOTAL_COL] = 0;
					}
					else{
						Serial.print("DW");
						btn_state &= ~(1 << (col+row*TOTAL_COL));
					}
					debounceDelayActive = true;
					debounceMillis = millis();
				}
				else if(!state){
					pressTime[col+row*TOTAL_COL]++;
					if(pressTime[col+row*TOTAL_COL] >= LONG_PRESS_DELAY){
						Serial.print("F");
						Serial.print(13+col+row*TOTAL_COL);
						Serial.print("DW");
						pressTime[col+row*TOTAL_COL]--;
					}
					debounceDelayActive = true;
					debounceMillis = millis();
				}
			}
			digitalWrite(btn_row[row],HIGH);
		}
	}
	if(millis() - debounceMillis > 200){
		debounceDelayActive = false;
	}

	// static long oldMillis = 0;
	// if(millis() - oldMillis > 500){
	// 	String str = String((int)random(0, 100));
	// 	str += " %";
	// 	ESPUI.print(cpuLabelID,str);
	// 	oldMillis = millis();
	// }

	if(Serial.available()){
		String text = Serial.readStringUntil('\n');
		int index = text.indexOf(':');
		if(index != -1){
			String cpuPer = text.substring(index+1,text.indexOf('\t'));
			text = text.substring(text.indexOf('\t')+1);
			String memPer = text.substring(0,text.indexOf('\t'));
			String dskPer = text.substring(text.indexOf('\t')+1);

			cpuPer += " %";
			memPer += " %";
			dskPer += " %";

			ESPUI.print(cpuLabelID,cpuPer);
			ESPUI.print(memLabelID,memPer);
			ESPUI.print(dskLabelID,dskPer);
		}
	}
}

//TO-Do: Replace delay function.