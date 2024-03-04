// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketSession.h"

PacketSession::PacketSession(class FSocket* Socket) : Socket(Socket)
{
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{

}

void PacketSession::Recv()
{

}

void PacketSession::Disconnect()
{

}
