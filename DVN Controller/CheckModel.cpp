#include "CheckModel.h"

bool CheckModel(std::stringstream& data) {
	std::string jammerName;
	std::getline(data, jammerName);
	return jammerName == JAMMER_NAME;
}