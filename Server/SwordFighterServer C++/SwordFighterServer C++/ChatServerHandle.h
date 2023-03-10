#pragma once
#include <unordered_map>
#include "Client.h"

/*
    ���� ��Ŷ ó��
*/

class ChatServerSend; // ���� ����

typedef void (ChatServerHandle::*MemFuncPtr)(int, Packet); // �Լ� ������
typedef unordered_map<int, MemFuncPtr> fmap;

class ChatServerHandle {
private:
    ChatServerSend *chatServerSend;

    void WelcomeMessageReceived(int index, Packet packet);
    void MessageReceived(int index, Packet packet);

public:
    vector<Client*> *clients;
    fmap packetHandlers; // �Լ� �����͸� Ȱ���� packetId �۾�

    ChatServerHandle(vector<Client*> *_clients, ChatServerSend *_chatServerSend) {
        clients = _clients;
        chatServerSend = _chatServerSend;
    }

    void HandlePacketId(int packetId, int index, Packet packet);
    void InitializePacketHandlers();
};