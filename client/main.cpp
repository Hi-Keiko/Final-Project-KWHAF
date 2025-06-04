#include <iostream>
#include <winsock2.h>
#include "client_utils.h"
#include <string>
#include <thread>   // Untuk std::this_thread::sleep_for
#include <chrono>   // Untuk std::chrono::seconds, milliseconds

#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char server_ip[] = "127.0.0.1";
    int port = 8888;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "[CLIENT] Failed to initialize Winsock. Error Code : " << WSAGetLastError() << std::endl;
        return 1;
    }
    std::cout << "[CLIENT] Winsock Initialized." << std::endl;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "[CLIENT] Could not create socket : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "[CLIENT] Socket created." << std::endl;

    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "[CLIENT] Connection error : " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }
    std::cout << "[CLIENT] Connected to server." << std::endl;

    int current_line = 0;
    std::string data_line;
    while (current_line < 24 && !(data_line = read_next_data("client/heart_data.txt", current_line)).empty()) {
        std::cout << "[CLIENT] Attempting to send data: " << data_line << std::endl; // Log untuk debugging
        int bytes_sent = send(s, data_line.c_str(), data_line.size(), 0);
        if (bytes_sent == SOCKET_ERROR) {
            std::cerr << "[CLIENT] Send failed for '" << data_line << "' with error: " << WSAGetLastError() << std::endl;
            break; // Keluar dari loop jika send gagal
        } else {
            std::cout << "[CLIENT] Successfully sent " << bytes_sent << " bytes for data: " << data_line << std::endl; // Log sukses
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Penundaan lebih singkat antar kirim, atau biarkan 1000ms
    }
    std::cout << "[CLIENT] Finished sending all data." << std::endl;

    // Beri server waktu untuk menerima dan memproses semua data terakhir
    std::cout << "[CLIENT] Waiting for server to process data before disconnecting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Tunggu 2 detik

    closesocket(s);
    WSACleanup();
    std::cout << "[CLIENT] Disconnected from server." << std::endl; // Log disconnect
    return 0;
}