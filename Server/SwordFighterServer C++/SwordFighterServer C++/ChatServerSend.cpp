#pragma once
#include "ChatServerSend.h"

// index : ä�� ���� client�� index
// fromId : ä�� ���� ���� id

// ������ ���� ����
void ChatServerSend::SendData(int toIndex, Packet packet) {
    const char* send_buffer = packet.ToArray();
    send((*clients)[toIndex]->clientSocket, send_buffer, packet.Length(), 0);
}

// �� �տ� ��Ŷ �� ���� ���̱�
void ChatServerSend::SendTCPData(int toIndex, Packet packet) {
    packet.WriteLength();
    SendData(toIndex, packet);
}

// �ٸ� ���� ��ü���� ��Ŷ ����
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

// ���� ���� �� �޼��� ���� + Ŭ���̾�Ʈ id ��û
void ChatServerSend::WelcomeMessage(int toIndex) {
    Packet packet((int)ChatServerPackets::welcomeMessage); // packet id

    string msg = string(u8"ä�� ������ �����ϼ̽��ϴ�.");

    packet.Write(MessageType::SERVER_MESSAGE);
    packet.Write(msg);

    SendTCPData(toIndex, packet);
}

// �÷��̾� ���� �˸� �޼���
void ChatServerSend::SendClientStateNotice(int fromIndex, int fromId, int state) {
    Packet packet((int)ChatServerPackets::clientStateNotice); // packet id
    string username = (*clients)[fromIndex]->username;

    packet.Write(fromId); // ��� Ŭ���̾�Ʈ id
    packet.Write(username);
    packet.Write(state); // ��� Ŭ���̾�Ʈ id�� ����

    SendTCPDataToAll(packet, fromIndex, true);
}

// ä�� ����
void ChatServerSend::SendChatMessageAll(int fromIndex, int fromId, string msg) {
    Packet packet((int)ChatServerPackets::chatServerMessage); // packet id

    packet.Write(fromId); // ä�� ����
    packet.Write(msg); // ä�� ����

    SendTCPDataToAll(packet, fromIndex, true);
}



void ChatServerSend::PushMessageQueueData(int index, int fromId, string message) { // Producer
    MessageQueueData messageQueueData = MessageQueueData(index, fromId, message);

    // Critical Section
    mtx_messageQueue.lock();
    messageQueue.push(messageQueueData);
    mtx_messageQueue.unlock();

    ctrl_var.notify_one(); // Consumer���� �˸�
}

void ChatServerSend::PopMessageQueueData() { // Consumer
    while (true) {
        unique_lock<mutex> lock(mtx_messageQueue);

        ctrl_var.wait(lock, [&]() { return !messageQueue.empty(); }); // Block ���·� ����. Block ���� �� mutex lock �� ����
        // queue�� �޼����� �����ϸ� wait ���� �ʰ� mutex lock �� ����

        while (!messageQueue.empty()) { // ť�� ���� ��� �޼��� ó��
            MessageQueueData messageQueueData = messageQueue.front();
            SendChatMessageAll(messageQueueData.clientIndex, messageQueueData.fromId, messageQueueData.message);
            messageQueue.pop();
        }
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(MS_BROADCASTING)); // �ּ� ��� �ð�
    }
}