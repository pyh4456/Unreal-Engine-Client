// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct PacketHeader
{
	uint16 size;
	uint16 id; // 프로토콜 ID
};

class SendBuffer : public TSharedFromThis<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.GetData(); }
	int32 WriteSize() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.Num()); }

	void CopyData(void* data, int32 len);
	void Close(uint32 wirteSize);

private:
	TArray<BYTE>	_buffer;
	int32			_writeSize = 0;
};


#define USING_SHARED_PTR(name)	using name##Ref = TSharedPtr<class name>;

USING_SHARED_PTR(Session);
USING_SHARED_PTR(PacketSession);
USING_SHARED_PTR(SendBuffer);


#define SEND_PACKET(pkt)														\
	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);		\
	Cast<US1GameInstance>(GWorld->GetGameInstance())->SendPacket(sendBuffer);	\

#include "ClientPacketHandler.h"
#include "S1GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#define BP_Character Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/501/BP_character.BP_character")));
#define BP_Sanya Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/501/Sanya_V_Litvyak/BP_sanya.BP_sanya")));
#define BP_Yoshika Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/501/Miyafuji_Yoshika/BP_yoshika.BP_yoshika")));
#define BP_Lynette Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Characters/501/Lynette_Bishop/BP_lynette.BP_lynette")));
