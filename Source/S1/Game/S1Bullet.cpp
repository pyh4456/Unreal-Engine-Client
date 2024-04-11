// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/S1Bullet.h"

// Sets default values
AS1Bullet::AS1Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AS1Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AS1Bullet::SetBulletInfo(const Protocol::ObjectInfo& Info)
{
	if (ObjectInfo->object_id() != 0)
	{
		assert(ObjectInfo->object_id() == Info.object_id());
	}

	ObjectInfo->CopyFrom(Info);
}

// Called every frame
void AS1Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

