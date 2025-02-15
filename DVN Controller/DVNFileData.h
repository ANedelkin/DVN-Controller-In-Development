#pragma once

#include "Global.h"

class DVNFileData
{
protected:
	string name;
	string folder;
	string extension;
public:
	DVNFileData(string folder, string extension);

	vector<DVNFileData*> children;

	Status Rename(string name);
	string GetName();
	static Status ValidateName(string& name);
	
	virtual string SaveString() const;
	void Save() const;
};

