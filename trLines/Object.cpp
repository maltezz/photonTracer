#include "stdafx.h"
#include "Object.h"

void app::Object::SetColor( app::Color3 color )
{
	objectColor = color;
}

app::Color3 app::Object::GetColor() const
{
	return objectColor;
}

app::ColorFloat app::Object::GetFloatColor() const
{
	return{ ( real )objectColor.b / 255.0f, ( real )objectColor.g / 255.0f, ( real )objectColor.r / 255.0f };
}
