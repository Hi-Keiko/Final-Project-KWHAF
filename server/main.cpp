#include <iostream>
#include <winsock2.h>
#include "anomaly_detector.h"
#include "data_persistence.h"
#include "server_utils.h"

#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c, recv_size;
    char buffer[2000];

    WSAStartup(MAKEWORD(2,2), &wsa);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    bind(server_socket, (struct sockaddr *)&server, sizeof(server));
    listen(server_socket, 3);

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);

    while ((recv_size = recv(client_socket, buffer, 2000, 0)) > 0) {
        buffer[recv_size] = '\0';
        std::string bpm_str(buffer);
        int bpm = std::stoi(bpm_str);
        std::string status = is_abnormal(bpm) ? (bpm < 60 ? "bradikardia" : "takikardia") : "normal";

        log_message("BPM received: " + bpm_str + " → " + status);

        if (status != "normal") {
            save_to_binary(bpm_str, "data/anomaly_log.bin");
            save_to_json(bpm_str, status);
        }
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

