// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "MyBTService_ReceiveTarget.generated.h"

/**
 * 
 */
UCLASS()
class S1_API UMyBTService_ReceiveTarget : public UBTService_BlueprintBase
{
public:
	GENERATED_BODY()

protected:
	FVector TargetLocation;
	bool MoveState;
};
