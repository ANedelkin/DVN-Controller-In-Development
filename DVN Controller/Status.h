#pragma once

#include <wx/msgdlg.h>
#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <string>

using namespace std;

#define DIALOG 0b00000001

namespace Status {
    enum StatusCode {
        Success,
        StartValueOutOfBounds,
        EndValueOutOfBounds,
        StartValueHigherThanEndvalue,
        EndValueLowerThanStartValue,
        BandAtLastPlace,
        InvalidSymbols,
        NameWhitespace,
        JammerNotSelected,
        FileNonexistent,
        FileAlreadyOpen,
        ScenarioAlreadyExists,
        ScenarioAlreadyExistsAs,
        FreqNotPositiveNumber,
        NameTooLong,
        InvalidFileStructure,
        ErrorMessageTooLong,
        ConnectionError,
        InvalidBands
    };
    static map<StatusCode, const char*> errorMessages = {
        {StartValueOutOfBounds , "This frequency can't be below %d!"},
        {EndValueOutOfBounds, "This frequency can't be over %d!"},
        {StartValueHigherThanEndvalue, "Start value higher than end value!"},
        {EndValueLowerThanStartValue, "End value lower than start value!"},
        {BandAtLastPlace, "The range between the start and end values of the last band of this range cannot include the value %d!"},
        {InvalidSymbols, "The name contains invalid symbol/s!"},
        {NameWhitespace, "The name cannot be empty!"},
        {JammerNotSelected, "You haven't selected a jammer!"},
        {FileNonexistent, "The file \"%s\" does not exist!"},
        {FileAlreadyOpen, "The file \"%s\" is open!"},
        {ScenarioAlreadyExists, "A scenario with the name \"%s\" already exists!"},
        {ScenarioAlreadyExistsAs, "Scenario \"%s\"'s old name is \"%s\"! Save it before you can use this name."},
        {FreqNotPositiveNumber, "The frequency has to be a positive whole number!"},
        {NameTooLong, "The name can't be longer than %d symbols!"},
        {InvalidFileStructure, "The structure of the file file \"%s\" is invalid and it can't be opened!"},
        {ErrorMessageTooLong, "The error message the program tried to generate was too long!"},
		{ConnectionError, "A connection error occured!"},
        {InvalidBands, "Selected scenario contains %d invalid band/s. Invalid data is marked in red. Click on invalid data to see the error in the status bar."}
    };

    extern string ToString(StatusCode code, ...);
    extern int ShowError(wxWindow* parent, string error, int style = 0);
}


