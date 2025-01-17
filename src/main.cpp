//      2     1      //  (15, 14, 13)  (0, 1, 2)  //
//       \___/       //                           //
//       /   \       //                           //
//   3--|     |--0   //  (12, 11, 10)  (3, 4, 5)  //
//       \___/       //                           //
//       /   \       //                           //
//      4     5      //  (9, 8, 7)     (6, 7, 8)  //

#include <Arduino.h>
#include <ArduinoJSON.h>

#include "NimBLE.h"
#include "Hexa.h"

BLE ble;
Hexa hexa;

void RTBLEReceive(void *params)
{
	while(true)
	{
		JsonDocument data = ble.receive();

		// hexa.targetJoyX = data["joyX"].as<float>();
		// hexa.targetJoyY = data["joyY"].as<float>();
		// hexa.targetHeight = data["height"].is<int>() ? data["height"].as<int>() : 0;
		// hexa.gait = data["gait"].is<String>() ? data["gait"].as<String>() : "Stance";
		// hexa.mode = data["mode"].is<String>() ? data["mode"].as<String>() : "Car";

		// float speed = data["speed"].is<float>() ? data["speed"].as<float>() : 0;
		// hexa.targetDuration = (speed == 0) ? 0 : 1000.0 / speed;

		hexa.joyX = data["joyX"].as<float>();
		hexa.joyY = data["joyY"].as<float>();
		hexa.height = data["height"].is<int>() ? data["height"].as<int>() : 0;
		hexa.gait = data["gait"].is<String>() ? data["gait"].as<String>() : "Stance";
		hexa.mode = data["mode"].is<String>() ? data["mode"].as<String>() : "Car";

		float speed = data["speed"].is<float>() ? data["speed"].as<float>() : 0;
		hexa.duration = (speed == 0) ? 0 : 500.0 / speed;

		vTaskDelay(1);
	}
}

void RTgaitTimer(void *params)
{
	while(true)
	{
		static int startTick = xTaskGetTickCount();

		hexa.timer = (xTaskGetTickCount() - startTick);
		if(hexa.timer >= hexa.duration) hexa.timer = 0, startTick = xTaskGetTickCount();

		hexa.gaitGenerate();
		
		vTaskDelay(10);
	}
}

void setup()
{
	Serial.begin(115200);

	initLUT(), ble.begin(), hexa.begin();

	xTaskCreate
	(
		RTBLEReceive,
		"RTBLEReceive",
		2048,
		NULL,
		1,
		NULL
	);

	xTaskCreate
	(
		RTgaitTimer,
		"RTgaitTimer",
		32768,
		NULL,
		1,
		NULL
	);
}

void loop() {}