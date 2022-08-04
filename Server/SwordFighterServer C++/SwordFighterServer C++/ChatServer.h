#pragma once
#include <thread>
#include <unordered_map>
#include <WS2tcpip.h>
#include "Client.h"
#include "ChatServerSend.h"
#include "ChatServerHandle.h"

const int MAX_PLAYERS = 4;
const int PORT = 26960;
const int SERVER_MESSAGE = 127;
const int ADMIN_MESSAGE = 126;

typedef void (ChatServerHandle::*MemFuncPtr)(int, Packet); // �Լ� ������
typedef unordered_map<int, MemFuncPtr> fmap;

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

public:
    fmap packetHandlers; // �Լ� �����͸� Ȱ���� packetId �۾�
    ChatServerHandle *chatServerHandle;

    ChatServer() {
        chatServerSend = new ChatServerSend(&clients);
        chatServerHandle = new ChatServerHandle(&clients);
    }

    ~ChatServer() {
        delete chatServerSend;
        delete chatServerHandle;
    }

    int Start();
    void PopMessageQueue();
    void AcceptClient(int index);
    void ReceiveClientsData(int index);
    void InitializeServerData();
    void DisconnectClient(int id);
};