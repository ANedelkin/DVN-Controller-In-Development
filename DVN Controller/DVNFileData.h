#pragma once

#include "Global.h"
#include "Validation.h"

using namespace Status;

class DVNFileData
{
protected:
	string name;
	string extension;


public:
	bool ok = true;
	string folder;
	string oldSaveString;
	bool upToDate;

	DVNFileData(const string& name);

	static string ValidateName(const string& name);

	vector<DVNFileData*> children;

	string Rename(const string& name);
	string GetName() const;
	string GetNameWithExt() const;

	string GetPath() const;
	string GetExtension() const;

	virtual string SaveString() const;
	void Save();
};

