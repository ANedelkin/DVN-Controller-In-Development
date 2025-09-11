#include "DVNFileData.h"

DVNFileData::DVNFileData(const string& name, const string& extension) : extension(extension), upToDate(true), ok(true)
{
	if (name == GetExtension())
		Rename("");
	else
		Rename(name);
}

string DVNFileData::ValidateName(const string& name)
{
	const char* charsToSeek = "\\/:*?\"<>|&";
	const char len = strlen(charsToSeek);

	for (char i = 0; i < len; i++)
	{
		if (name.find(charsToSeek[i]) != string::npos) {
			return ToString(InvalidSymbols);
		}
	}
	return Validation::ValidateNameBasic(name);
}

string DVNFileData::Rename(const string& name) {
	this->name = name;
	return ToString(Success);
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
	stream << JAMMER_NAME << endl;
	stream << SaveString();
	oldSaveString = SaveString();
	stream.close();
}