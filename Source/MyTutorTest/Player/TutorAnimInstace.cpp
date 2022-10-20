// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorAnimInstace.h"

UTutorAnimInstace::UTutorAnimInstace()
{
	bArrowLoaded = false;
	bFiringArrow = false;
	bBowAiming = false;
}

void UTutorAnimInstace::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	
}

void UTutorAnimInstace::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}
