// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UINPCTip.generated.h"

enum class ETip : uint8
{
	Question,
	Exclamation,
};

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API UUINPCTip : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Brush, meta=( AllowPrivateAccess="true", DisplayThumbnail="true", DisplayName="Question", AllowedClasses="Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UObject* ResourceObject_Question;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Brush, meta=( AllowPrivateAccess="true", DisplayThumbnail="true", DisplayName="Exclamation", AllowedClasses="Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
	UObject* ResourceObject_Exclamation;

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Tip;

	void SwitchImageTip(ETip Tip);

	void HideImageTip();
};
