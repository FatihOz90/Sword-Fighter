//common.h
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Packet.h"
#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996)

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

static queue<MessageQueueData> messageQueue; // �޽��� ó���� ť
static mutex mtx; // ������ �浹 ������