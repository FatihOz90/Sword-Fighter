#pragma once
#include "Client.h"
#include "ChatServerHandle.h"

void Client::SetUserData(int _id, string _username) {
    id = _id;
    username = _username;
}

// ��Ŷ RECV
void Client::ReceiveData() {
    char buf[dataBufferSize];

    ZeroMemory(buf, dataBufferSize);

    int bytesReceived = recv(clientSocket, buf, dataBufferSize, 0);

    Packet packet = receivedData;
    packet.Reset(HandleData(buf, bytesReceived));
}

bool Client::HandleData(char* data, int length) {
    int packetLength = 0;
    receivedData.SetBytes(data, length);

    if (receivedData.UnreadLength() >= 4) { // ��Ŷ �� ���� �б�
        packetLength = receivedData.ReadInt();
        if (packetLength <= 0)
        {
            return true;
        }
    }

    while (packetLength > 0 && packetLength <= receivedData.UnreadLength())
    {
        const char* packetBytes = receivedData.ReadBytes(packetLength); // receivedData���� packetLength��ŭ �� ����

        Packet packet(packetBytes, length);

        int packetId = packet.ReadInt();
        try {
            // �Լ� �����͸� ����Ͽ� packetId�� ���� chatServer�� ������ �Լ� ����
            chatServerHandle->HandlePacketId(packetId, index, packet);
            //(chatServerHandle->*(chatServerHandle->packetHandlers[packetId]))(index, packet);
        }
        catch (exception e) {
            cout << "Unknown packet id" << endl;
        }

        packetLength = 0;
        if (receivedData.UnreadLength() >= 4) // byte�� ���Ҵٸ� packetLength�� �а� �ٽ� �б� ����
        {
            packetLength = receivedData.ReadInt();
            if (packetLength <= 0)
            {
                return true;
            }
        }
    }

    if (packetLength <= 1)
    {
        return true;
    }

    return false;
}