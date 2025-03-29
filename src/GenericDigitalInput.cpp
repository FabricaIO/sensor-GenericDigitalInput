#include"GenericDigitalInput.h"

/// @brief Creates a generic analog input
/// @param Name The device name
/// @param Pin Pin to use
/// @param configFile Name of the config file to use
GenericDigitalInput::GenericDigitalInput(String Name, int Pin, String configFile) : Sensor(Name) {
	config_path = "/settings/sen/" + configFile;
	digital_config.Pin = Pin;
}

/// @brief Starts an analog input 
/// @return True on success
bool GenericDigitalInput::begin() {
	// Set description
	Description.type = "digital input";
	Description.parameterQuantity = 1;
	Description.parameters = {"Digital Signal"};
	Description.units = {"binary"};
	values.resize(Description.parameterQuantity);
	// Check if config exists
	if (!checkConfig(config_path)) {
		// Set default settings
		digital_config.mode = "Input";
		return saveConfig(config_path, getConfig());
	} else {
		// Load settings
		return setConfig(Storage::readFile(config_path), false);
	}
}

/// @brief Gets the current config
/// @return A JSON string of the config
String GenericDigitalInput::getConfig() {
	// Allocate the JSON document
	JsonDocument doc;
	// Assign current values
	doc["Name"] = Description.name;
	doc["Pin"] = digital_config.Pin;
	doc["Mode"]["current"] = digital_config.mode;
	doc["Mode"]["options"][0] = "Input";
	doc["Mode"]["options"][1] = "Pullup";
	doc["Mode"]["options"][2] = "Pulldown";
	doc["Mode"]["options"][3] = "Open Drain";

	// Create string to hold output
	String output;
	// Serialize to string
	serializeJson(doc, output);
	return output;
}

/// @brief Sets the configuration for this device
/// @param config A JSON string of the configuration settings
/// @param save If the configuration should be saved to a file
/// @return True on success
bool GenericDigitalInput::setConfig(String config, bool save) {
	// Allocate the JSON document
  	JsonDocument doc;
	// Deserialize file contents
	DeserializationError error = deserializeJson(doc, config);
	// Test if parsing succeeds.
	if (error) {
		Logger.print(F("Deserialization failed: "));
		Logger.println(error.f_str());
		return false;
	}
	// Assign loaded values
	Description.name = doc["Name"].as<String>();
	digital_config.Pin = doc["Pin"].as<int>();
	digital_config.mode = doc["Mode"]["current"].as<String>();

	if (save) {
		if (!saveConfig(config_path, getConfig())) {
			return false;
		}
	}
	return configureInput();
}

/// @brief Takes a measurement
/// @return True on success
bool GenericDigitalInput::takeMeasurement() {
	values[0] = digitalRead(digital_config.Pin);
	return true;
}

/// @brief Configures the pin for use
/// @return True on success
bool GenericDigitalInput::configureInput() {
	pinMode(digital_config.Pin, modes[digital_config.mode]);
	return true;
}