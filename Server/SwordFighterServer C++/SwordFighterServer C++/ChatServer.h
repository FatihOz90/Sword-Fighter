#pragma once
#include <thread>
#include <unordered_map>
#include <WS2tcpip.h>
#include "Client.h"
#include "ChatServerSend.h"
#include "ChatServerHandle.h"
#include "common.h"

class ChatServer
{
private:
    SOCKET listenSocket;
    unordered_map<int, Client*> clients; // ������ Ŭ���̾�Ʈ ��� (0 = ������ ����)

    int total_socket_count = 0; // Ȱ��ȭ�� ���� ��
    WSAEVENT handle_array[MAX_PLAYERS + 1];
    DWORD wsaIndex;
    WSANETWORKEVENTS wsaNetEvents;
    ChatServerSend *chatServerSend;
    //MessageQueueManager *messageQueueManager;

public:
    ChatServerHandle *chatServerHandle;

    ChatServer() {
        chatServerSend = new ChatServerSend(&clients);
        chatServerHandle = new ChatServerHandle(&clients, chatServerSend);
        //messageQueueManager = new MessageQueueManager(chatServerSend);
    }

    ~ChatServer() {
        delete chatServerSend;
        delete chatServerHandle;
        //delete messageQueueManager;
    }

    int Start();
    void PopMessageQueue();
    void AcceptClient(int index);
    void ReceiveClientsData(int index);
    void InitializeServerData();
    void DisconnectClient(int id);
};