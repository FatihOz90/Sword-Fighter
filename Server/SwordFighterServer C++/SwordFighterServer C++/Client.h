#pragma once
#include "common.h"
#include <winsock2.h>

const int dataBufferSize = 4096;

static void MessageReceived(int fromClient, Packet packet) {
    string str = packet.ReadString();
    //mtx.lock();
    messageQueue.push(make_pair(fromClient, str));
    //mtx.unlock();
}

class Client
{
private:
    const int id;
    Packet receivedData;
    //void(Client::*fp[1]) (int, Packet) = { &MessageReceived };
    //void(Client::*fp) (int, Packet) = &MessageReceived;
    void(*fp[1]) (int, Packet) = { MessageReceived }; // void ��ȯ��, int, Packet �Ű������� �Լ� ������ ����

public:
    SOCKET clientSocket = INVALID_SOCKET;

    Client() : id(0) {
    }

    Client(int _id) : id(_id) {
    }

    //void MessageReceived(int fromClient, Packet packet);
    void Connect();
    bool HandleData(char* data, int length);
};