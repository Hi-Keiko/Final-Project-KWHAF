#include <iostream>
#include <winsock2.h>
#include <windows.h> // Untuk SetConsoleOutputCP
#include <string>    // Untuk std::string
#include <sstream>   // Untuk std::istringstream
#include <vector>    // Untuk std::vector
#include "anomaly_detector.h"
#include "data_persistence.h"
#include "server_utils.h"
#include "../types.h"   // Tambahkan ini untuk HeartData

#pragma comment(lib,"ws2_32.lib")

int main() {
    SetConsoleOutputCP(CP_UTF8);

    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c, recv_size;
    char buffer[2000];

    // Vektor untuk menyimpan semua data BPM yang diterima
    std::vector<HeartData> all_heart_data;
    int received_data_count = 0; // Menghitung jumlah data yang diterima

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "[SERVER] Failed to initialize Winsock. Error Code : " << WSAGetLastError() << std::endl;
        return 1;
    }
    log_message("Winsock Initialized.");

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "[SERVER] Could not create server socket : " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    log_message("Server socket created.");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "[SERVER] Bind failed with error code : " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    log_message("Server bind done.");

    // Listen to incoming connections
    listen(server_socket, 3);
    log_message("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);

    // Accept an incoming connection
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "[SERVER] Accept failed with error code : " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    log_message("Connection accepted from client.");

    // Receive data from client
    // Batasi loop penerimaan hingga 24 data (atau sampai klien memutuskan koneksi)
    while (received_data_count < 24 && (recv_size = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[recv_size] = '\0';
        std::string received_data_str(buffer); // Akan berisi "hour,bpm"
        
        std::istringstream iss(received_data_str);
        std::string hour_str;
        std::string bpm_str;
        int hour = -1;
        int bpm = 0;

        if (std::getline(iss, hour_str, ',') && std::getline(iss, bpm_str)) {
            try {
                hour = std::stoi(hour_str);
                bpm = std::stoi(bpm_str);
            } catch (const std::invalid_argument& ia) {
                log_message("Invalid data format received: " + received_data_str);
                continue;
            } catch (const std::out_of_range& oor) {
                log_message("Data out of range: " + received_data_str);
                continue;
            }
        } else {
            log_message("Incomplete data received: " + received_data_str);
            continue;
        }

        std::string status = is_abnormal(bpm) ? (bpm < 60 ? "bradikardia" : "takikardia") : "normal";

        log_message("Data received [Hour: " + hour_str + ", BPM: " + bpm_str + "] -> Status: " + status);

        // Simpan data ke dalam vektor
        all_heart_data.push_back({hour, bpm, status});
        received_data_count++;

        // Jika data abnormal, simpan juga ke file anomali
        if (status != "normal") {
            save_to_binary(received_data_str, "data/anomaly_log.bin");
            save_to_json(hour, bpm_str, status); 
        }
    }

    // Setelah semua data diterima atau klien terputus
    if (received_data_count > 0) {
        log_message("All data received (" + std::to_string(received_data_count) + " entries). Generating daily report...");
        generate_daily_report_json(all_heart_data);
        log_message("Daily report generated in data/daily_report.json.");
    } else if (recv_size == 0) {
        log_message("Client disconnected.");
    } else if (recv_size == SOCKET_ERROR) {
        std::cerr << "[SERVER] Receive failed with error code : " << WSAGetLastError() << std::endl;
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}