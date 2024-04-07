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
	bool IsMyPlayer() { return IsMine; }
	void SetIsMyPlayer(bool isMyPlayer) { IsMine = isMyPlayer; }
	Protocol::MoveState GetMoveState() { return PosInfo->state(); }
	void SetMoveState(Protocol::MoveState State);

	void SetPlayerInfo(const Protocol::ObjectInfo& Info);
	void SetPosInfo(const Protocol::PosInfo& Info);
	void SetDestInfo(const Protocol::PosInfo& Info);
	Protocol::PosInfo* GetPlayerInfo() { return PosInfo; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	bool IsMine;
	class Protocol::PosInfo* PosInfo; // 현재 위치
	class Protocol::PosInfo* DestInfo; // 목적지
	class Protocol::ObjectInfo* ObjectInfo; //플레이어 정보
};
