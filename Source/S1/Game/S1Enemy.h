// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Protocol.pb.h"
#include "S1Enemy.generated.h"

UENUM(BlueprintType)
enum class AiState : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	IDLE UMETA(DisplayName = "IDLE"),
	MOVE UMETA(DisplayName = "MOVE"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	RUNAWAY UMETA(DisplayName = "RUNAWAY")
};

UCLASS()
class S1_API AS1Enemy : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS1Enemy();
	~AS1Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetEnemyInfo(const Protocol::ObjectInfo& Info);
	void SetTargetLocation(float x, float y, float z);
	UFUNCTION(BlueprintCallable)
	void SetMoveState(AiState state);

	UFUNCTION(BlueprintCallable)
	FVector GetTargetLocation() { return TargetLocation; }
	UFUNCTION(BlueprintCallable) 
	AiState GetState() { return MoveState; }
	UFUNCTION(BlueprintCallable)
	int GetObjectId() { return ObjectInfo->object_id(); }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	class Protocol::ObjectInfo* ObjectInfo; //Àû Á¤º¸
	FVector TargetLocation;
	AiState MoveState;
};
