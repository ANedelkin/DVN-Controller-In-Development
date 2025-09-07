#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <filesystem>
#include <fstream>

#include "Global.h"
#include "Validation.h"
#include "ModelConstants.h"

using namespace filesystem;
using namespace Status;

class DVNFileData
{
protected:
	string name;
	string extension;
public:
	string folder;
	string oldSaveString;
	bool upToDate;
	bool ok;

	DVNFileData(const string& name);

	static string ValidateName(const string& name);

	string Rename(const string& name);
	string GetName() const;
	string GetNameWithExt() const;

	string GetPath() const;
	string GetExtension() const;

	virtual string SaveString() const;
	void Save();
	
	virtual DVNFileData* Copy() const = 0;
};

