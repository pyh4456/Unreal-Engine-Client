// Fill out your copyright notice in the Description page of Project Settings.


#include "S1Player.h"
#include "S1MyPlayer.h"

// Sets default values
AS1Player::AS1Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerInfo = new Protocol::PlayerInfo();
	DestInfo = new Protocol::PlayerInfo();
}

AS1Player::~AS1Player()
{
	delete PlayerInfo;
	delete DestInfo;
	PlayerInfo = nullptr;
	DestInfo = nullptr;
}

// Called when the game starts or when spawned
void AS1Player::BeginPlay()
{
	Super::BeginPlay();

	{
		FVector Location = GetActorLocation();
		DestInfo->set_x(Location.X);
		DestInfo->set_y(Location.Y);
		DestInfo->set_z(Location.Z);
		DestInfo->set_yaw(GetControlRotation().Yaw);
	}
}

bool AS1Player::IsMyPlayer()
{
	return Cast<AS1MyPlayer>(this) != nullptr;
}

void AS1Player::SetMoveState(Protocol::MoveState State)
{
	PlayerInfo->set_state(State);
}

void AS1Player::SetPlayerInfo(const Protocol::PlayerInfo& Info)
{
	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	PlayerInfo->CopyFrom(Info);

	FVector Location(Info.x(), Info.y(), Info.z());
	FQuat Rotation(0, 0, 0, 0);
	SetActorLocationAndRotation(Location, Rotation);
}

void AS1Player::SetDestInfo(const Protocol::PlayerInfo& Info)
{
	if (PlayerInfo->object_id() != 0)
	{
		assert(PlayerInfo->object_id() == Info.object_id());
	}

	// Dest에 최종 상태 복사.
	DestInfo->CopyFrom(Info);
}

int AS1Player::GetCharacterType()
{
	switch (PlayerInfo->type())
	{
	case Protocol::PLAYER_TYPE_YOSHIKA:
		return 1;
		break;
	case Protocol::PLAYER_TYPE_LYNETTE:
		return 2;
		break;
	case Protocol::PLAYER_TYPE_SANYA:
		return 3;
		break;
	}

	return 0;
}

// Called every frame
void AS1Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		FVector Location = GetActorLocation();
		PlayerInfo->set_x(Location.X);
		PlayerInfo->set_y(Location.Y);
		PlayerInfo->set_z(Location.Z);
		PlayerInfo->set_yaw(GetControlRotation().Yaw);
	}

	if (IsMyPlayer() == false)
	{
		FVector Location = GetActorLocation();
		FVector DestLocation = FVector(DestInfo->x(), DestInfo->y(), DestInfo->z());

		FVector MoveDir = (DestLocation - Location);

		const float DistToDest = MoveDir.Length();
		MoveDir.Normalize();

		float MoveDist = (MoveDir * 600.f * DeltaTime).Length();
		MoveDist = FMath::Min(MoveDist, DistToDest);
		FVector NextLocation = Location + MoveDir * MoveDist;
		SetActorLocation(NextLocation);
		SetActorRotation(FRotator(0, DestInfo->yaw(), 0));
	}
}

// Called to bind functionality to input
void AS1Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

