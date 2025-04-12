#pragma once
#include "Status.h"

namespace Validation
{
	extern string ValidateNameBasic(const string& name);
	extern bool TryParse(const wxString& str, int* result);
};

