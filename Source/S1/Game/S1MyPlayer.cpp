// Fill out your copyright notice in the Description page of Project Settings.


#include "S1MyPlayer.h"
#include "S1.h"

// Sets default values
AS1MyPlayer::AS1MyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AS1MyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AS1MyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePacketSendTimer -= DeltaTime;

	if (MovePacketSendTimer <= 0)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

		Protocol::C_MOVE MovePkt;

		//현재 위치 정보
		{
			Protocol::PlayerInfo* Info = MovePkt.mutable_info();
			Info->CopyFrom(*PlayerInfo);
		}

		SEND_PACKET(MovePkt);
	}
}

// Called to bind functionality to input
void AS1MyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

