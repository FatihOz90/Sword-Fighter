#pragma once
#include "ChatServer.h"

// index : ä�� ���� client�� index
// fromId : ä�� ���� ���� id

void ChatServerSend::SendData(int toIndex, Packet packet) {
    const char* send_buffer = packet.ToArray();
    send((*clients)[toIndex]->clientSocket, send_buffer, packet.Length(), 0);
}


void ChatServerSend::SendTCPData(int toIndex, Packet packet) {
    packet.WriteLength(); // �� �տ� ��Ŷ �� ���� ���̱�
    SendData(toIndex, packet);
}

void ChatServerSend::SendTCPDataToAll(Packet packet, int fromIndex, bool exceptMe = false) {
    packet.WriteLength();

    for (int i = 1; i <= MAX_PLAYERS; ++i) {
        if ((*clients)[i]->clientSocket == INVALID_SOCKET) {
            continue;
        }
        if (fromIndex == i && exceptMe) { // exceptMe�� true�� �ڱ� �ڽ��� ����
            continue;
        }
        //cout << i << ", " << fromId << ", " << exceptMe << endl;
        SendData(i, packet);
    }
}


// Packets
void ChatServerSend::WelcomeMessage(int toIndex) { // ���� ���� �� �޽��� ���� + Ŭ���̾�Ʈ id ��û
    Packet packet = Packet(ChatServerPackets::welcomeMessage);

    string msg = string(u8"ä�� ������ �����ϼ̽��ϴ�.");

    packet.Write(SERVER_MESSAGE);
    packet.Write(msg);

    SendTCPData(toIndex, packet);
}

void ChatServerSend::SendChatMessageAll(int fromIndex, int fromId, string msg) { // ä�� ����
    Packet packet((int)ChatServerPackets::chatServerMessage); // packet id

    packet.Write(fromId); // ä�� ����
    packet.Write(msg); // ä�� ����

    SendTCPDataToAll(packet, fromIndex, true);
}