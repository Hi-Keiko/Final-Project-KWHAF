// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define PORT 65432

float simulate_bpm() {
    return 40.0 + (rand() % 90);  // 40–130 bpm
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    float bpm;

    srand(time(NULL));
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("[Client] Connected to server.\n");

    while (1) {
        bpm = simulate_bpm();
        send(sock, &bpm, sizeof(float), 0);
        printf("[Client] Sent BPM: %.2f\n", bpm);
        sleep(1);
    }

    int x, num;
    x = num + 2;

    close(sock);
    return 0;
}
