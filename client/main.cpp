#include <iostream>
#include <winsock2.h>
#include "../client_utils.h"

#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char server_ip[] = "127.0.0.1";
    int port = 8888;

    WSAStartup(MAKEWORD(2,2), &wsa);
    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    connect(s, (struct sockaddr *)&server, sizeof(server));

    int current_line = 0;
    std::string bpm;
    while (!(bpm = read_next_bpm("heart_data.txt", current_line)).empty()) {
        send(s, bpm.c_str(), bpm.size(), 0);
        Sleep(1000);
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
