#include "Client.h"

using namespace std;

void Client::Connect() {
    // While loop: Ŭ���̾�Ʈ�� �޼����� �޾Ƽ� ��� �� Ŭ���̾�Ʈ�� �ٽ� �����ϴ�.
    char buf[dataBufferSize];

    while (true)
    {
        ZeroMemory(buf, dataBufferSize);

        // Wait for client to send data
        // �޼����� ���������� ������ recv �Լ��� �޼����� ũ�⸦ ��ȯ�Ѵ�.
        //cout << "Receive Ready" << endl;
        int bytesReceived = recv(clientSocket, buf, dataBufferSize, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            cerr << "Error in recv(). Quitting" << endl;
            closesocket(clientSocket);
            clientSocket = NULL;
            continue;
        }

        if (bytesReceived == 0)
        {
            cout << "Client disconnected " << endl;
            closesocket(clientSocket);
            clientSocket = NULL;
            continue;
        }

        //Packet packet(buf);
        //packetQueue.push(packet);
        //clients[i].Reset(HandleData(buf));
        //clients[i].SetBytes(buf);
        Packet packet = receivedData;
        packet.Reset(HandleData(buf, bytesReceived));

        // Echo message back to client
        /*
        for (int i = 0; i < 24; ++i) {
            cout << (int) buf[i] << " ";
        }*/
        //cout << "received: " << buf << endl;
        //send(clientSocket, buf, bytesReceived + 1, 0);
    }
}

bool Client::HandleData(char* data, int length) {
    int packetLength = 0;
    receivedData.SetBytes(data, length);

    if (receivedData.UnreadLength() >= 4) {
        packetLength = receivedData.ReadInt();
        if (packetLength <= 0)
        {
            return true;
        }
    }

    while (packetLength > 0 && packetLength <= receivedData.UnreadLength())
    {
        const char* packetBytes = receivedData.ReadBytes(packetLength); // receivedData���� packetLength��ŭ �� ����
        //ThreadManager.ExecuteOnMainThread(() = >
        Packet packet(packetBytes, length);

        int packetId = packet.ReadInt() - 1;
        try {
            fp[packetId](id, packet);
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