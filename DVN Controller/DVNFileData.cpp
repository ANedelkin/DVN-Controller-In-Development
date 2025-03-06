#include "DVNFileData.h"

DVNFileData::DVNFileData(const string& name) : name(name), upToDate(true), oldName(name)
{
}

Status DVNFileData::Rename(const string& name) {
	this->name = name;
	return Success;
}

string DVNFileData::GetName() const { return name; }

Status DVNFileData::ValidateName(const string& name)
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
	return folder + "\\" + oldName + extension;
}

string DVNFileData::SaveString() const
{
	return string();
}

void DVNFileData::Save() {
	if (!exists(folder)) create_directory(folder);
	const string oldPath = GetPath();
	oldName = name;
	if (exists(oldPath)) {
		const string newPath = GetPath();
		rename(oldPath, newPath);
	}
	ostringstream fileName;
	fileName << GetPath();
	ofstream stream(fileName.str());
	stream << SaveString();
	oldSaveString = SaveString();
	stream.close();
}