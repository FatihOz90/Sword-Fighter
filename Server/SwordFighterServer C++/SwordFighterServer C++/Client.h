#pragma once
#include <WS2tcpip.h>
#include "Packet.h"

class ChatServerHandle; // ���� ����

const int dataBufferSize = 4096;

class Client
{
private:
    const int index; // clients unordered_map������ index (key)
    Packet receivedData;
    ChatServerHandle *chatServerHandle;

public:
    SOCKET clientSocket = INVALID_SOCKET;
    char ip_address[INET_ADDRSTRLEN];
    int port;
    int id; // Ŭ���̾�Ʈ�� id

    Client(int _index, ChatServerHandle *_chatServerHandle) : index(_index) {
        chatServerHandle = _chatServerHandle;
    }

    Client(SOCKET _clientSocket, ChatServerHandle *_chatServerHandle) : index(0) {
        clientSocket = _clientSocket;
        chatServerHandle = _chatServerHandle;
    }

    void ReceiveData();
    bool HandleData(char* data, int length);
};