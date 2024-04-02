// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Protocol.pb.h"
#include "S1Player.generated.h"


UCLASS()
class S1_API AS1Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AS1Player();
	virtual ~AS1Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	int GetCharacterType();

public:
	bool IsMyPlayer();
	void SetPlayerInfo(const Protocol::PlayerInfo& Info);
	Protocol::PlayerInfo* GetPlayerInfo() { return PlayerInfo; }
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	class Protocol::PlayerInfo* PlayerInfo; //현재 위치
};
