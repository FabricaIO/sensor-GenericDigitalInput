/*
* This file and associated .cpp file are licensed under the GPLv3 License Copyright (c) 2025 Sam Groveman
* 
* External libraries needed:
* ArduinoJSON: https://arduinojson.org/
*
* Contributors: Sam Groveman
*/
#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Sensor.h>
#include <map>

/// @brief Class describing a generic output on a GPIO pin
class GenericDigitalInput : public Sensor {
	public:
		GenericDigitalInput(int Pin, String configFile = "GenericDigitalInput.json");
		bool begin();
		String getConfig();
		bool setConfig(String config, bool save);
		bool takeMeasurement();

	protected:
		/// @brief Map for input modes
		std::map<String, int> modes = {{"Input", INPUT}, {"Pullup", INPUT_PULLUP}, {"Pulldown", INPUT_PULLDOWN}, {"Open Drain", OPEN_DRAIN}};

		/// @brief Output configuration
		struct {
			/// @brief The pin number attached to the output
			int Pin;
			
			/// @brief The mode of the pin
			String mode;
		} digital_config;

		/// @brief Path to configuration file
		String config_path;

		bool configureInput();
};