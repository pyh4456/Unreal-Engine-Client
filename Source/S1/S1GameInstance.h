// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "S1.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "S1GameInstance.generated.h"

class AS1Player;
class AS1Enemy;
class AS1Bullet;
/**
 * 
 */
UCLASS()
class S1_API US1GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	bool ConnectToGameServer(FString id, FString password);

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

	UFUNCTION(BlueprintCallable)
	void SelectCharacter(int character);

	UFUNCTION(BlueprintCallable)
	FString GetName(int index) { return CharacterName[index]; }
	UFUNCTION(BlueprintCallable)
	int GetScore(int index) { return CharacterScore[index]; };
	UFUNCTION(BlueprintCallable)
	int GetCharacterType(int index) { return CharacterType[index]; };
	UFUNCTION(BlueprintCallable)
	int GetSelectedCharacter() { return SelectedCharacter; }
	UFUNCTION(BlueprintCallable)
	void SetSelectedCharacter(int index) { SelectedCharacter = index; }
	UFUNCTION(BlueprintCallable)
	void MoveMap(int mapNumber);

	void SetName(int index, FString name) { CharacterName[index] = name; }
	void SetScore(int index, int score) { CharacterScore[index] = score; }
	void SetCharacterType(int index, int type) { CharacterType[index] = type; }

public:
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);
	void SpawnPlayer(const Protocol::ObjectInfo& ObjectInfo, bool IsMine);
	void SpawnEnemy(const Protocol::ObjectInfo& ObjectInfo);
	void SpawnBullet(const Protocol::ObjectInfo& ObjectInfo);

	void HandleDespawn(const Protocol::ObjectInfo& ObjectInfo);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void DespawnPlayer(int64 ObjectId);
	void DespawnEnemy(int64 ObjectId);


	void HandleMove(const Protocol::S_MOVE& MovePkt);

	void HandleScore(const Protocol::S_SCORE& ScorePkt);
	
	//UPROPERTY(BlueprintCallable)
	//int64 RequestFire(FTransform transform);

	UFUNCTION(BlueprintCallable)
	void RequestScore(int monsterId);

public:
	// GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("192.168.35.92");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Player>  OtherPlayerClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Player>  OtherPlayerYoshika;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Player>  OtherPlayerLynette;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Player>  OtherPlayerSanya;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Player>  MyPlayerClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Player>  MyPlayerYoshika;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Player>  MyPlayerLynette;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Player>  MyPlayerSanya;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Enemy> EnemyClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Bullet> OtherBulletYoshika;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Bullet> OtherBulletLynette;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Bullet> OtherBulletSanya;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Bullet> MyBulletYoshika;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Bullet> MyBulletLynette;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AS1Bullet> MyBulletSanya;

	AS1Player* MyPlayer;
	TMap<uint64, AS1Player*> Players;
	TMap<uint64, AS1Enemy*> Enemys;
	TMap<uint64, AS1Bullet*> Bullets;

	FString CharacterName[5];
	uint64 CharacterScore[5];
	uint64 CharacterType[5];
	uint64 SelectedCharacter;
};
