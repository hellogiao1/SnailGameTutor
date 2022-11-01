// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayStatic.h"
#include "GameFramework/Actor.h"

bool UMyGameplayStatic::IsTeam(AActor* Src, AActor* Dest)
{
	if (Src && Dest)
	{
		for (const auto& Name : Src->Tags)
		{
			if (Dest->Tags.Contains(Name))
			{
				return true;
			}
		}
	}
	return false;
}
