// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTutorTest/Player/MyCharacterBase.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class MYTUTORTEST_API AEnemyBase : public AMyCharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyBase();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* HeadTipComp;

protected:
	virtual void BeginPlay() override;

	/** 每当有Actor被摧毁时，就会调用 Destroyed 函数。粒子发射器自身通常不复制，但由于Actor摧毁会复制，
	我们知道若在服务器上摧毁此投射物，则各个连接客户端在摧毁各自的投射物副本时将调用此函数。
	结果，所有玩家都会看到投射物被摧毁时的爆炸效果。 */
	virtual void Destroyed() override;

	/** RepNotify，用于同步对当前生命值所做的更改。*/
    virtual void OnRep_CurrentHealth();

	/** 响应要更新的生命值。修改后，立即在服务器上调用，并在客户端上调用以响应RepNotify*/
	virtual void OnHealthUpdate();

private:
	UPROPERTY()
	class UUIHeadTipBar* HeadUI;
};
