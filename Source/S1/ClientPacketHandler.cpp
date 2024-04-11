#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "S1.h"
#include "S1GameInstance.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return true;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance());
	//srand((unsigned int)time(NULL));
	int index = 0;

 	for (auto& Player : pkt.players())
	{
		GameInstance->SetName(index, Player.player_info().name().c_str());
		GameInstance->SetScore(index, Player.player_info().score());
		GameInstance->SetCharacterType(index, Player.player_type());
		index++;
	}
	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		// TODO : 게임 종료 or 로비로
	}

	return true;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDespawn(pkt);
	}

	return true;
}

bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleMove(pkt);
	}

	return true;
}

bool Handle_S_SHOOT(PacketSessionRef& session, Protocol::S_SHOOT& pkt)
{
	return false;
}

bool Handle_S_REMOVE_BULLET(PacketSessionRef& session, Protocol::S_REMOVE_BULLET& pkt)
{
	return false;
}

bool Handle_S_SCORE(PacketSessionRef& session, Protocol::S_SCORE& pkt)
{
	if (auto* GameInstance = Cast<US1GameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleScore(pkt);
	}

	return true;
}

bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	auto Msg = pkt.msg();

	return true;
}
