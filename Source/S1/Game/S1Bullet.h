// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "GameFramework/Actor.h"
#include "S1Bullet.generated.h"

UCLASS()
class S1_API AS1Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS1Bullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetBulletInfo(const Protocol::ObjectInfo& Info);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	class Protocol::ObjectInfo* ObjectInfo; //다른사람 투사체 정보
};
