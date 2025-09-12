#include "TrimToWidth.h"

const wxString TrimToWidth(wxString text, const int maxWidth, wxFont font)
{
	wxScreenDC dc;
	dc.SetFont(font);

	int textWidth, textHeight;
	dc.GetTextExtent(text, &textWidth, &textHeight);

	if (textWidth > maxWidth) {
		do {
			text = text.Left(text.Length() - 3);
			dc.GetTextExtent(text, &textWidth, &textHeight);
		} while (textWidth > maxWidth);
		text += "...";
	}
	return text;
}
