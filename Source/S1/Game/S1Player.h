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

	UFUNCTION(BlueprintCallable)
	void SetMoveState(int State);
	void SetMoveState(Protocol::MoveState State);

	void SetPlayerInfo(const Protocol::ObjectInfo& Info);
	void SetPosInfo(const Protocol::PosInfo& Info);
	void SetDestInfo(const Protocol::PosInfo& Info);
	void SetScore(const int input) { score = input; }
	UFUNCTION(BlueprintCallable)
	void SetMapNumber(int number) { mapNum = number; }

	void IncreaseScore(const int input) { score += input; }

	UFUNCTION(BlueprintCallable)
	int GetMoveState() { return DestInfo->state(); }
	UFUNCTION(BlueprintCallable)
	float GetPitch() { return DestInfo->pitch(); }
	UFUNCTION(BlueprintCallable)
	int GetScore() { return score; }
	UFUNCTION(BlueprintCallable)
	FString GetName() { return UTF8_TO_TCHAR(ObjectInfo->player_info().name().c_str()); }
	UFUNCTION(BlueprintCallable)
	int GetMapNumber() { return mapNum; }


	int GetObjectId() { return ObjectInfo->object_id(); }

	Protocol::PosInfo* GetPlayerInfo() { return PosInfo; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayerNameTag();

protected:
	bool IsMine;
	int score = 0;
	int mapNum = 0;
	class Protocol::PosInfo* PosInfo; // 현재 위치
	class Protocol::PosInfo* DestInfo; // 목적지
	class Protocol::ObjectInfo* ObjectInfo; //플레이어 정보
};
