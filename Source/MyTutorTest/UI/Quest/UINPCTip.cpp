// Fill out your copyright notice in the Description page of Project Settings.


#include "UINPCTip.h"
#include "Components/Image.h"

void UUINPCTip::SwitchImageTip(ETip Tip)
{
	if (Image_Tip == nullptr) return;

	UTexture2D* Texture2D = nullptr;
	switch (Tip)
	{
	case ETip::Question:
	{
		if (ResourceObject_Question)
		{
			Texture2D = Cast<UTexture2D>(ResourceObject_Question);
			
		}
	}break;
	case ETip::Exclamation:
	{
		if (ResourceObject_Exclamation)
		{
			Texture2D = Cast<UTexture2D>(ResourceObject_Exclamation);
		}
	}break;
	default:
		break;
	}

	if (Texture2D)
	{
		Image_Tip->SetBrushFromTexture(Texture2D);
	}
}

void UUINPCTip::HideImageTip()
{
	if (Image_Tip)
	{
		Image_Tip->SetVisibility(ESlateVisibility::Hidden);
	}
}
