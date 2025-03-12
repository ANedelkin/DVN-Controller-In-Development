#include "DVNFileData.h"

DVNFileData::DVNFileData(const string& name) : name(name), upToDate(true), oldName(name)
{
}

Status DVNFileData::Rename(const string& name) {
	this->name = name;
	return Success;
}

string DVNFileData::GetName() const { return name; }

string DVNFileData::GetNameWithExt() const { return name + extension; }

Status DVNFileData::ValidateName(const string& name)
{
	string invalidChars = "\\/:*?\"<>|";
	for (char ch : name) {
		if (invalidChars.find(ch) != string::npos) {
			return InvalidSymbols;
		}
	}
	if (name.length() == 0) return NameWhitespace;
	if (all_of(name.begin(), name.end(), [](unsigned char c) { return isspace(c); })) return NameWhitespace;
	return Success;
}

string DVNFileData::GetOldPath() const
{
	return folder + "\\" + oldName + extension;
}

string DVNFileData::GetNewPath() const {
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
	const string oldPath = GetOldPath();
	oldName = name;
	if (exists(oldPath)) {
		const string newPath = GetOldPath();
		rename(oldPath, newPath);
	}
	ostringstream fileName;
	fileName << GetOldPath();
	ofstream stream(fileName.str());
	stream << SaveString();
	oldSaveString = SaveString();
	stream.close();
}