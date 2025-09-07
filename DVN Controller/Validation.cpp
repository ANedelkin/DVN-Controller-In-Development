#include "Validation.h"

using namespace Status;

string Validation::ValidateNameBasic(const string& name)
{
    if (HasNonStdChars(name)) return ToString(InvalidSymbols);
    if (name.length() == 0) return ToString(NameWhitespace);
    if (all_of(name.begin(), name.end(), [](unsigned char c) { return isspace(c); })) return ToString(NameWhitespace);
    return ToString(Success);
}

bool Validation::TryParse(const char* str, int* result)
{
    char* endptr = nullptr;
    *result = strtol(str, &endptr, 10);
    if (*result > numeric_limits<int>::max() || *result < 0 || *endptr != '\0') return false;
    return true;
}

bool Validation::TryParse(const wxString& str, int* result)
{
    return Validation::TryParse((const char*)str.mb_str(), result);
}

bool Validation::HasNonStdChars(const string& str)
{
    for (char ch : str) {
        if (ch < 32 || ch > 126) return true;
    }
    return false;
}
