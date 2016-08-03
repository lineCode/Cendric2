#include "FileIO/ConfigurationIO.h"
#include "ApplicationState.h"

#include <fstream>
#include <sstream>

const std::string ConfigurationIO::CONFIGURATION_PATH = "dialogue_tool.ini";
const std::string ConfigurationIO::SQL_PATH = "path.sql";
const std::string ConfigurationIO::NPC_PATH = "path.npc";
const std::string ConfigurationIO::DIALOGUE_PATH =  "path.dialogue";

bool ConfigurationIO::loadConfiguration() {
	Configuration data = DEFAULT_CONFIGURATION;

	std::string line;
	std::ifstream configuration(CONFIGURATION_PATH);
	bool noError = true;
	if (configuration.is_open()) {
		while (getline(configuration, line)) {
			if (line.empty() || line.compare(0, 1, "#") == 0) {
				continue;
			}
			else if (line.compare(0, SQL_PATH.size(), SQL_PATH) == 0) {
				noError = readPath(data.sqlFolder, line);
			}
			else if (line.compare(0, NPC_PATH.size(), NPC_PATH) == 0) {
				noError = readPath(data.npcFolder, line);
			}
			else if (line.compare(0, DIALOGUE_PATH.size(), DIALOGUE_PATH) == 0) {
				noError = readPath(data.dialogueFolder, line);
			}
			else {
				ERROR("[Configuration]: Unknown tag found in configuration file.");
				noError = false;
			}
			if (!noError) {
				break;
			}
		}
		configuration.close();

		if (!noError) {
			return false;
		}
	}
	else {
		ERROR("[Configuration]: Unable to open configuration file. Default configuration is used.");
		g_state->setConfiguration(DEFAULT_CONFIGURATION);
		return false;
	}

	g_state->setConfiguration(data);
	return true;
}


bool ConfigurationIO::readPath(std::string& path, const std::string& line) {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		ERROR("[Configuration]: No colon found after display mode tag or no value after colon.");
		return false;
	}
	// enforce 100 limit to avoid buffer overflow
	path = line.substr(colon + 1).substr(0, 99);
	return true;
}

bool ConfigurationIO::saveConfiguration() {
	std::ofstream configuration(CONFIGURATION_PATH, std::ios::trunc);
	if (configuration.is_open()) {
		configuration << writeConfigurationHeader();
		configuration << writePath(G_CONF.npcFolder, NPC_PATH);
		configuration << writePath(G_CONF.sqlFolder, SQL_PATH);
		configuration << writePath(G_CONF.dialogueFolder, DIALOGUE_PATH);

		configuration.close();
	}
	else {
		ERROR("[Configuration]: Unable to open file: " + CONFIGURATION_PATH );
		return false;
	}
	return true;
}

std::string ConfigurationIO::writePath(const std::string& path, const std::string& tag) {
	std::string ret = tag + ":" + path + "\n";
	return ret;
}

std::string ConfigurationIO::writeConfigurationHeader() {
	std::stringstream ss;
	ss
		<< "###############################################\n"
		<< "# CENDRIC DIALOGUE TOOL CONFIGURATION         #\n"
		<< "# This file is automatically generated        #\n"
		<< "# If you know what you're doing, you can      #\n"
		<< "# modify the configuration here.              #\n"
		<< "###############################################\n"
		<< "\n";
	return ss.str();
}

