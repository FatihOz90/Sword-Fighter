#pragma once
#include "ChatServerHandle.h"
#include "ChatServerSend.h"

// Packet id = 1 : Ŭ���̾�Ʈ id ȹ��
void ChatServerHandle::WelcomeMessageReceived(int index, Packet packet) {
    int id = packet.ReadInt();
    (*clients)[index]->id = id;

    printf("%s:%d connected successfully and is now player %d.\n", (*clients)[index]->ip_address, (*clients)[index]->port, (*clients)[index]->id);
}

// Packet id = 2 : Ŭ���̾�Ʈ ä�� �޼���
void ChatServerHandle::MessageReceived(int index, Packet packet) {
    int fromId = packet.ReadInt();
    string message = packet.ReadString();

    chatServerSend->PushMessageQueueData(index, fromId, message);
}

void ChatServerHandle::HandlePacketId(int packetId, int index, Packet packet) {
    (this->*(this->packetHandlers[packetId]))(index, packet);
}

void ChatServerHandle::InitializePacketHandlers() {
    packetHandlers.emplace(ChatClientPackets::welcomeMessageReceived, &ChatServerHandle::WelcomeMessageReceived);
    packetHandlers.emplace(ChatClientPackets::chatClientMessage, &ChatServerHandle::MessageReceived);
}


