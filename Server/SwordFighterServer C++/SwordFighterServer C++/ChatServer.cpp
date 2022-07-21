#include "ChatServer.h"

using namespace std;

/*
    Server
*/

void ChatServer::Broadcast() {
    while (true) {
        // TODO - Mutext ���?

        //mtx.lock();
        while (!messageQueue.empty()) {
            pair<int, string> msg = messageQueue.front();
            cout << msg.first << ": " << msg.second << endl;
            messageQueue.pop();
        }
        //mtx.unlock();
    }
}

void ChatServer::InitializeServerData()
{
    for (int i = 0; i < MAX_PLAYERS; i++) // �ִ� �÷��̾� �� ��ŭ �̸� clients ����
    {
        clients[i] = new Client(i);
    }
}

void ChatServer::AcceptClient() {
    while (true) {
        int key;
        SOCKADDR_IN acceptedClientSockaddr;
        int clientSize = sizeof(SOCKADDR_IN);
        // connection queue�� ���� �տ� �ִ� Ŭ���̾�Ʈ ��û�� accept�ϰ�, client ������ ��ȯ�մϴ�.
        SOCKET acceptedClientSocket = accept(listenSocket, (SOCKADDR*)& acceptedClientSockaddr, &clientSize); // accept�� Ŭ���̾�Ʈ�� clientSocket���� ���

        if (acceptedClientSocket == INVALID_SOCKET)
        {
            cerr << "Can't accept a socket! Quitting" << endl;
            closesocket(listenSocket);
            WSACleanup();
            return;
        }

        for (int i = 0; i < MAX_PLAYERS; i++) // ����ִ� ���� ù clients Dictionary�� ����
        {
            if (clients[i]->clientSocket == INVALID_SOCKET)
            {
                clients[i]->clientSocket = acceptedClientSocket;
                key = i;
                break;
            }
        }

        // close listening socket
        //int closeResult = closesocket(listenSocket);

        char host[NI_MAXHOST];             // Ŭ���̾�Ʈ�� host �̸�
        char service[NI_MAXHOST];        // Ŭ���̾�Ʈ�� PORT ��ȣ
        ZeroMemory(host, NI_MAXHOST);    // memset(host, 0, NI_MAXHOST)�� ����
        ZeroMemory(service, NI_MAXHOST);

        // clientAddr�� ����� IP �ּҸ� ���� ������ ������ ����ϴ�. host �̸��� host��, ��Ʈ ��ȣ�� service�� ����˴ϴ�.
        // getnameinfo()�� ���� �� 0�� ��ȯ�մϴ�. ���� �� 0�� �ƴ� ���� ��ȯ�մϴ�.
        if (getnameinfo((const SOCKADDR*)&acceptedClientSockaddr, sizeof(SOCKADDR_IN), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        {
            cout << host << " connected on port " << service << endl;
        }
        else
        {
            inet_ntop(AF_INET, &acceptedClientSockaddr.sin_addr, host, NI_MAXHOST);
            cout << host << " connected on port " << ntohs(acceptedClientSockaddr.sin_port) << endl;
        }

        Packet packet;
        string str = string(u8"ȯ���մϴ�.");

        packet.Write(SERVER_MESSAGE);
        packet.Write(str);
        packet.WriteLength();

        const char* send_buffer = packet.ToArray();

        send(clients[key]->clientSocket, send_buffer, packet.Length(), 0);

        clients[key]->Connect();
    }
}

int ChatServer::Start() {
    WSADATA wsaData;
    int iniResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iniResult != 0)
    {
        cerr << "Can't Initialize winsock! Quitiing" << endl;
        return -1;
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // ������ ��� ����
    if (listenSocket == INVALID_SOCKET)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in servAddr{}; // �⺻ �ʱ�ȭ ����
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT); // short �ڷ����� ��Ʈ��ũ byte order�� ���� (Big Endian)

    int bindResult = ::bind(listenSocket, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr));
    if (bindResult == SOCKET_ERROR)
    {
        cerr << "Can't bind a socket! Quitting" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return -1;
    }

    cout << "Starting Server..." << endl;
    int listenResult = listen(listenSocket, SOMAXCONN);
    if (listenResult == SOCKET_ERROR)
    {
        cerr << "Can't listen a socket! Quitting" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return -1;
    }
    InitializeServerData();

    thread(Broadcast).detach();

    AcceptClient();


    while (true) {
        // ���� ����
    }


    // Close the client socket
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        if (clients[i]->clientSocket != INVALID_SOCKET) {
            closesocket(clients[i]->clientSocket);
        }
    }

    // Cleanup winsock <-> WSAStartup
    WSACleanup();

    delete *clients;
}