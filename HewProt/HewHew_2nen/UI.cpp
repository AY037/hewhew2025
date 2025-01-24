#include "UI.h"
void UI::Init()
{
	Initialize(GetObjectTexName());   //”wŒi‚ğ‰Šú‰»
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void UI::Update(void)
{

}

void UI::Draw(DirectX::XMMATRIX& _pm)
{
	if(activeDraw)
	{
		DrawUiObject(_pm);
	}
}


void UI::SetActiveDraw(bool active)
{
	activeDraw = active;
}

bool UI::GetActiveDraw()
{
	return activeDraw;
}

