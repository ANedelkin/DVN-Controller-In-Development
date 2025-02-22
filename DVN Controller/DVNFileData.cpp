#include "DVNFileData.h"

DVNFileData::DVNFileData(string name)
{
	this->name = name;
	upToDate = true;
}

Status DVNFileData::Rename(string name) {
	const string oldPath = GetPath();
	this->name = name;
	if (exists(oldPath)) {
		const string newPath = GetPath();
		rename(oldPath, newPath);
	}
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

string DVNFileData::GetPath() const
{
	return folder + "\\" + name + extension;
}

void DVNFileData::Save() const
{
	if (!exists(folder)) create_directory(folder);
	ostringstream fileName;
	fileName << GetPath();
	ofstream stream(fileName.str());
	stream << this->SaveString();
	stream.close();
}
