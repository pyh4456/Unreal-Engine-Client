// Fill out your copyright notice in the Description page of Project Settings.


#include "S1Player.h"

// Sets default values
AS1Player::AS1Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AS1Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AS1Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AS1Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

