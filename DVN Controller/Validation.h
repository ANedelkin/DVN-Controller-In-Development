#pragma once
#include "Status.h"

#define NAME_MAX_LENGTH	27

namespace Validation
{
	extern string ValidateNameBasic(const string& name);
	extern bool TryParse(const wxString& str, int* result);
	extern bool HasNonStdChars(const string& str);
};

