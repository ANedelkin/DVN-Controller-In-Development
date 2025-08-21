#pragma once
#include "Validation.h"

using namespace Status;

struct BandInfo {
	string name;

	int rangeIndex;

	int startValue;
	int endValue;

	bool enabled = true;
	bool working;

	enum BandProperty {
		Name,
		Start,
		End,
	};

	string errors[3];

	BandInfo() : BandInfo(0, 0, 0) {}
	BandInfo(int rangeIndex, int startValue, int endvalue) : BandInfo("Unnamed band", rangeIndex, startValue, endvalue, false) {}
	BandInfo(const string& name, int rangeIndex, int startValue, int endvalue, bool working) {
		this->name = name;
		this->rangeIndex = rangeIndex;
		this->startValue = startValue;
		this->endValue = endvalue;
		this->working = working;
	}

	static string ValidateName(const string& name) {
		return Validation::ValidateNameBasic(name);
	}
};