#include "DVNFileData.h"

DVNFileData::DVNFileData(const string& name) : name(name), upToDate(true)
{
}

Status DVNFileData::ValidateName(const string& name)
{
	const char* charsToSeek = "\\/:*?\"<>|";
	const char len = strlen(charsToSeek);

	for (char i = 0; i < len; i++)
	{
		if (name.find(charsToSeek[i]) != string::npos) {
			return InvalidSymbols;
		}
	}
	return ValidateNameBasic(name);
}

Status DVNFileData::Rename(const string& name) {
	this->name = name;
	return Success;
}

string DVNFileData::GetName() const { return name; }

string DVNFileData::GetNameWithExt() const { return name + extension; }

string DVNFileData::GetPath() const {
	return folder + "\\" + name + extension;
}

string DVNFileData::GetExtension() const
{
	return extension;
}

string DVNFileData::SaveString() const
{
	return string();
}

void DVNFileData::Save() {
	if (!exists(folder)) create_directory(folder);
	ofstream stream(GetPath());
	stream << SaveString();
	oldSaveString = SaveString();
	stream.close();
}