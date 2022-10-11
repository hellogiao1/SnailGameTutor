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

	/** ÿ����Actor���ݻ�ʱ���ͻ���� Destroyed ���������ӷ���������ͨ�������ƣ�������Actor�ݻٻḴ�ƣ�
	����֪�����ڷ������ϴݻٴ�Ͷ�����������ӿͻ����ڴݻٸ��Ե�Ͷ���︱��ʱ�����ô˺�����
	�����������Ҷ��ῴ��Ͷ���ﱻ�ݻ�ʱ�ı�ըЧ���� */
	virtual void Destroyed() override;

	/** RepNotify������ͬ���Ե�ǰ����ֵ�����ĸ��ġ�*/
    virtual void OnRep_CurrentHealth();

	/** ��ӦҪ���µ�����ֵ���޸ĺ������ڷ������ϵ��ã����ڿͻ����ϵ�������ӦRepNotify*/
	virtual void OnHealthUpdate();

private:
	UPROPERTY()
	class UUIHeadTipBar* HeadUI;
};
