// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1Enemy.h"

// Sets default values
AS1Enemy::AS1Enemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectInfo = new Protocol::ObjectInfo();
	MoveState = AiState::NONE;
}

AS1Enemy::~AS1Enemy()
{
	delete ObjectInfo;
	ObjectInfo = nullptr;
}

// Called when the game starts or when spawned
void AS1Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AS1Enemy::SetEnemyInfo(const Protocol::ObjectInfo& Info)
{
	if (ObjectInfo->object_id() != 0)
	{
		assert(ObjectInfo->object_id() == Info.object_id());
	}

	ObjectInfo->CopyFrom(Info);
}

void AS1Enemy::SetTargetLocation(float x, float y, float z)
{
	TargetLocation.X = x;
	TargetLocation.Y = y;
	TargetLocation.Z = z;
}

void AS1Enemy::SetMoveState(AiState state)
{
	MoveState = state;
}

// Called every frame
void AS1Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

