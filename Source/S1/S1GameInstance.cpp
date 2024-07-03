// Fill out your copyright notice in the Description page of Project Settings.


#include "S1GameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "ClientPacketHandler.h"
#include "S1MyPlayer.h"
#include "S1Enemy.h"
#include "S1MyBullet.h"

bool US1GameInstance::ConnectToGameServer(FString id, FString password)
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting to Server...")));

	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));
	
		// Session
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();

		Protocol::C_LOGIN Pkt;

		Pkt.set_id(std::string(TCHAR_TO_UTF8(*id)));
		Pkt.set_password(std::string(TCHAR_TO_UTF8(*password)));

		SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
		SendPacket(SendBuffer);

		return true;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
		return false;
	}

	return false;
}

void US1GameInstance::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(LeavePkt);

	Players.Empty();
	Enemys.Empty();
	Bullets.Empty();

	//if (Socket)
	//{
	//	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
	//	SocketSubsystem->DestroySocket(Socket);
	//	Socket = nullptr;
	//}
}

void US1GameInstance::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void US1GameInstance::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void US1GameInstance::SelectCharacter(int index)
{
	// 로비에서 캐릭터 선택해서 인덱스 전송.

	Protocol::C_ENTER_GAME EnterGamePkt;

	switch (CharacterType[index])
	{
	case 1:
		EnterGamePkt.mutable_selectedcharacter()->set_player_type(Protocol::PLAYER_TYPE_YOSHIKA);
		break;
	case 2:
		EnterGamePkt.mutable_selectedcharacter()->set_player_type(Protocol::PLAYER_TYPE_LYNETTE);
		break;
	case 3:
		EnterGamePkt.mutable_selectedcharacter()->set_player_type(Protocol::PLAYER_TYPE_SANYA);
		break;
	}
	EnterGamePkt.mutable_selectedcharacter()->mutable_player_info()->set_name(std::string(TCHAR_TO_UTF8(*CharacterName[index])));
	EnterGamePkt.mutable_selectedcharacter()->mutable_player_info()->set_score(CharacterScore[index]);

	SEND_PACKET(EnterGamePkt);
}

void US1GameInstance::SpawnPlayer(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	//중복 체크
	const uint64 ObjectId = ObjectInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	if (IsMine)
	{
		AS1Player* Player = nullptr;

		switch (ObjectInfo.player_type())
		{
		case Protocol::PLAYER_TYPE_YOSHIKA:
			Player = Cast<AS1Player>(World->SpawnActor(MyPlayerYoshika, &SpawnLocation));
			break;
		case Protocol::PLAYER_TYPE_LYNETTE:
			Player = Cast<AS1Player>(World->SpawnActor(MyPlayerLynette, &SpawnLocation));
			break;
		case Protocol::PLAYER_TYPE_SANYA:
			Player = Cast<AS1Player>(World->SpawnActor(MyPlayerSanya, &SpawnLocation));
			break;
		default:
			Player = Cast<AS1Player>(World->SpawnActor(MyPlayerClass, &SpawnLocation));
			break;
		}

		APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
		controller->UnPossess();
		controller->Possess(Player);
		
		if (Player == nullptr)
			return;

		Player->SetPlayerInfo(ObjectInfo);
		Player->SetPosInfo(ObjectInfo.pos_info());
		Player->SetIsMyPlayer(true);
		Player->SetScore(ObjectInfo.player_info().score());

		MyPlayer = Player;
		Players.Add(ObjectInfo.object_id(), Player);
	}
	else
	{
		AS1Player* Player = nullptr;

		switch (ObjectInfo.player_type())
		{
		case Protocol::PLAYER_TYPE_YOSHIKA:
			Player = Cast<AS1Player>(World->SpawnActor(OtherPlayerYoshika, &SpawnLocation));
			break;
		case Protocol::PLAYER_TYPE_LYNETTE:
			Player = Cast<AS1Player>(World->SpawnActor(OtherPlayerLynette, &SpawnLocation));
			break;
		case Protocol::PLAYER_TYPE_SANYA:
			Player = Cast<AS1Player>(World->SpawnActor(OtherPlayerSanya, &SpawnLocation));
			break;
		default:
			Player = Cast<AS1Player>(World->SpawnActor(OtherPlayerClass, &SpawnLocation));
			break;
		}

		if (Player == nullptr)
			return;

		Player->SetPlayerInfo(ObjectInfo);
		Player->SetPosInfo(ObjectInfo.pos_info());
		Player->SetIsMyPlayer(false);
		Player->SetPlayerNameTag();

		Players.Add(ObjectInfo.object_id(), Player);
	}
}

void US1GameInstance::MoveMap(int mapNumber)
{
	TQueue<int64> ObjectIToDespawn;

	for (auto& Elem : Players)
	{
		if (Elem.Value->GetObjectId() == MyPlayer->GetObjectId())
			continue;

		ObjectIToDespawn.Enqueue(Elem.Value->GetObjectId());
	}

	while (!ObjectIToDespawn.IsEmpty())
	{

		DespawnPlayer(*ObjectIToDespawn.Peek());
		ObjectIToDespawn.Pop();
	}

	for (auto& Elem : Enemys)
	{
		ObjectIToDespawn.Enqueue(Elem.Value->GetObjectId());
	}

	while (!ObjectIToDespawn.IsEmpty())
	{
		DespawnEnemy(*ObjectIToDespawn.Peek());
		ObjectIToDespawn.Pop();
	}

	Protocol::C_ENTER_ROOM pkt;

	pkt.set_roomnumber(mapNumber);

	SEND_PACKET(pkt);

	MyPlayer->SetMapNumber(mapNumber);
}

AS1Enemy* US1GameInstance::GetEnemyFromId(int object_id)
{
	return Enemys[object_id];
}

void US1GameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	SpawnPlayer(EnterGamePkt.player(), true);
}

void US1GameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	
	for (auto& Object : SpawnPkt.objects())
	{
		switch (Object.object_type())
		{
		case Protocol::OBJECT_TYPE_PLAYER:
			SpawnPlayer(Object, false);
			break;
		case Protocol::OBJECT_TYPE_ENEMY:
			SpawnEnemy(Object);
			break;
		case Protocol::OBJECT_TYPE_PROJECTILE:
			SpawnBullet(Object);
			break;
		}
	}
}

void US1GameInstance::SpawnEnemy(const Protocol::ObjectInfo& ObjectInfo)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	//중복 체크
	const uint64 ObjectId = ObjectInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	// 적 스폰
	AS1Enemy* Enemy = nullptr;

	Enemy = Cast<AS1Enemy>(World->SpawnActor(EnemyClass, &SpawnLocation));

	if (Enemy == nullptr)
		return;

	Enemy->SetEnemyInfo(ObjectInfo);

	Enemys.Add(ObjectInfo.object_id(), Enemy);
}

void US1GameInstance::SpawnBullet(const Protocol::ObjectInfo& ObjectInfo)
{
	//if (Socket == nullptr || GameServerSession == nullptr)
	//	return;

	//auto* World = GetWorld();
	//if (World == nullptr)
	//	return;

}

void US1GameInstance::HandleDespawn(const Protocol::ObjectInfo& ObjectInfo)
{
	switch (ObjectInfo.object_type())
	{
	case Protocol::OBJECT_TYPE_PLAYER:
		DespawnPlayer(ObjectInfo.object_id());
		break;
	case Protocol::OBJECT_TYPE_ENEMY:
		DespawnEnemy(ObjectInfo.object_id());
		break;
	}
}

void US1GameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& Object : DespawnPkt.objects())
		HandleDespawn(Object);
}

void US1GameInstance::DespawnPlayer(int64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AS1Player** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	Players.FindAndRemoveChecked(ObjectId);
	World->DestroyActor(*FindActor);
}

void US1GameInstance::DespawnEnemy(int64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	AS1Enemy** FindActor = Enemys.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	Enemys.FindAndRemoveChecked(ObjectId);
	World->DestroyActor(*FindActor);
}

void US1GameInstance::HandleEnemyAi(const Protocol::S_AI_TARGET& pkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = pkt.object_id();

	AS1Enemy* enemy = Enemys[ObjectId];

	enemy->SetTargetLocation(pkt.target_location().x(),
		pkt.target_location().y(), pkt.target_location().z());
	enemy->SetMoveState(true);
}

void US1GameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MovePkt.info().object_id();
	AS1Player** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	AS1Player* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return;

	const Protocol::PosInfo& Info = MovePkt.info();
	//Player->SetPlayerInfo(Info); //바로 이동

	Player->SetDestInfo(Info); //이동 목적지를 설정
}

void US1GameInstance::HandleScore(const Protocol::S_SCORE& ScorePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	if (MyPlayer == nullptr)
		return;

	MyPlayer->IncreaseScore(ScorePkt.point());
}

void US1GameInstance::RequestScore(int monsterId)
{
	Protocol::C_SCORE pkt;

	pkt.set_player_id(MyPlayer->GetPlayerInfo()->object_id());
	pkt.set_monster_id(monsterId);

	SEND_PACKET(pkt);
}