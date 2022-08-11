#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "ChatServerHandle.h"
#include "common.h"

/*
    ��Ŷ ����
*/

//class Client;

struct MessageQueueData { // ä�� �޽��� ����
    int clientIndex; // ���� Ŭ���̾�Ʈ�� index
    int fromId; // ���� Ŭ���̾�Ʈ�� id
    string message; // �޼��� ����

    MessageQueueData(int _clientIndex, int _fromId, string _message) {
        clientIndex = _clientIndex;
        fromId = _fromId;
        message = _message;
    }
};

class ChatServerSend {
private:
    unordered_map<int, Client*> *clients;

    void SendData(int clientId, Packet packet);
    void SendTCPData(int toClient, Packet packet);
    void SendTCPDataToAll(Packet packet, int fromIndex, bool exceptMe);
    mutex mtx; // ������ �浹 ������

public:
    queue<MessageQueueData> messageQueue; // �޽��� ó���� ť

    ChatServerSend(unordered_map<int, Client*> *_clients) {
        clients = _clients;
    }
    void WelcomeMessage(int toClient);
    void SendChatMessageAll(int fromIndex, int fromId, string msg);

    void PushMessageQueueData(int index, int fromId, string message);
    void PopMessageQueueData();
};