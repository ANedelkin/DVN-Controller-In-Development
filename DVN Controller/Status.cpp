#include "Status.h"

string Status::ToString(Status::StatusCode code, ...)
{
    if (!code) return "";
    char buffer[256];
    string msg;

    va_list args;
    va_start(args, code);

    int buffSize = sizeof(buffer);
    if (vsnprintf(buffer, buffSize, errorMessages.at(code), args) > buffSize - 1)
        return errorMessages.at(ErrorMessageTooLong);

    return buffer;
}

int Status::ShowError(wxWindow* parent, string msg, int style)
{
    if (style & DIALOG) {
        wxMessageDialog frame(parent, msg, "Error", wxOK | wxCANCEL | wxICON_ERROR);
        frame.SetOKCancelLabels("Enter new value", "Keep old value");
        return frame.ShowModal();
    }

    return wxMessageDialog(parent, msg, "Error", wxOK | wxICON_ERROR).ShowModal();
}
