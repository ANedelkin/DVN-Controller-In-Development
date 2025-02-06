#include "DVNFileData.h"

DVNFileData::DVNFileData(string folder, string extension)
{
	this->folder = folder;
	this->extension = extension;
}

Status DVNFileData::Rename(string name) {
	this->name = name;
	return Success;
}

string DVNFileData::GetName() { return name; }

Status DVNFileData::ValidateName(string& name)
{
	string invalidChars = "\\/:*?\"<>|";
	for (char ch : name) {
		if (invalidChars.find(ch) != string::npos) {
			return InvalidSymbols;
		}
	}
	if (name.length() == 0) return NameWhitespace;
	if (all_of(name.begin(), name.end(), [](unsigned char c) { return std::isspace(c); })) return NameWhitespace;
	return Success;
}

void DVNFileData::Save() const
{
	if (!exists(folder)) create_directory(folder);
	ostringstream fileName;
	fileName << folder + name << extension;
	ofstream stream(fileName.str());
	stream << this->SaveString();
	stream.close();
}
