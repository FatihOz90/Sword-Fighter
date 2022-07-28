#pragma once
#include "common.h"
#include "ChatServerHandle.h"

const int dataBufferSize = 4096;

struct MessageQueueData;

class ChatServer;

class Client
{
private:
    const int index;
    Packet receivedData;
    ChatServer *chatServer;
    //void(*fp[2]) (int, Packet) = { GetUserId, MessageReceived }; // void ��ȯ��, int, Packet �Ű������� �Լ� ������ ����

public:
    SOCKET clientSocket = INVALID_SOCKET;
    HANDLE evnt;
    ChatServerHandle chatServerHandle;
    char ip_address[INET_ADDRSTRLEN];
    int port;
    int id;

    Client() : index(0) {
    }

    Client(int _index, ChatServer *_chatServer) : index(_index) {
        chatServer = _chatServer;
    }

    Client(SOCKET _clientSocket, HANDLE _evnt, ChatServer *_chatServer) : index(0) {
        clientSocket = _clientSocket;
        evnt = _evnt;
        chatServer = _chatServer;
    }

    //void MessageReceived(int fromClient, Packet packet);
    void ReceiveData();
    bool HandleData(char* data, int length);
};