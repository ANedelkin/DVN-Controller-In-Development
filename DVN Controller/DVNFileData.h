#pragma once

#include "Global.h"

class DVNFileData
{
protected:
	string name;
	string extension;
public:
	string oldName;
	string folder;
	string oldSaveString;
	bool upToDate;

	DVNFileData(const string& name);

	vector<DVNFileData*> children;

	Status Rename(const string& name);
	string GetName() const;
	string GetNameWithExt() const;
	static Status ValidateName(const string& name);

	string GetOldPath() const;
	string GetNewPath() const;
	string GetExtension() const;

	virtual string SaveString() const;
	void Save();
};

