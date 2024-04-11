// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"
#include "Game/S1Bullet.h"
#include "S1MyBullet.generated.h"

/**
 * 
 */
UCLASS()
class S1_API AS1MyBullet : public AS1Bullet
{
	GENERATED_BODY()
	
public:
	class Protocol::ObjectInfo* ObjectInfo; //내 투사체 정보
};
