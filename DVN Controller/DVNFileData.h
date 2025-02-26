#pragma once

#include "Global.h"

class DVNFileData
{
protected:
	string name;
	string extension;
public:
	string folder;
	bool upToDate;

	DVNFileData(string name);

	vector<DVNFileData*> children;

	Status Rename(string name);
	string GetName();
	static Status ValidateName(string& name);

	string GetPath() const;
	
	virtual string SaveString() const = 0;
	virtual void Save() const = 0;
};

