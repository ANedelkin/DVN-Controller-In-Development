#include "Log.h"

void Log(const string& message){
	ofstream stream("log.txt", std::ios::app);
	stream << message << endl;
	stream.close();
};