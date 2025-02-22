#pragma once

#include "Global.h"

class DVNFileData
{
protected:
	string name;
	string folder;
	string extension;
public:
	bool upToDate;

	DVNFileData(string name);

	vector<DVNFileData*> children;

	Status Rename(string name);
	string GetName();
	static Status ValidateName(string& name);

	string GetPath() const;
	
	virtual string SaveString() const = 0;
	void Save() const;
};

