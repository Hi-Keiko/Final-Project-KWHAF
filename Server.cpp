// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 65432
#define MAX_CLIENTS 5
#define LOW_THRESHOLD 50.0
#define HIGH_THRESHOLD 120.0

FILE *binFile;
FILE *jsonFile;
pthread_mutex_t lock;

void write_to_json(float bpm) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", t);

    pthread_mutex_lock(&lock);
    fprintf(jsonFile, "{\"timestamp\":\"%s\",\"bpm\":%.2f},\n", timestamp, bpm);
    fflush(jsonFile);
    pthread_mutex_unlock(&lock);
}

void* handle_client(void *arg) {
    int client_sock = *(int *)arg;
    float bpm;

    while (read(client_sock, &bpm, sizeof(float)) > 0) {
        printf("Received BPM: %.2f\n", bpm);

        if (bpm < LOW_THRESHOLD || bpm > HIGH_THRESHOLD) {
            pthread_mutex_lock(&lock);
            fwrite(&bpm, sizeof(float), 1, binFile);
            fflush(binFile);
            pthread_mutex_unlock(&lock);

            write_to_json(bpm);
        }
    }

    close(client_sock);
    free(arg);
    return NULL;
}

int main() {
    int server_fd, client_sock, *new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    binFile = fopen("anomaly_data.bin", "ab");
    jsonFile = fopen("anomaly_report.json", "a");

    fprintf(jsonFile, "[\n");

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, MAX_CLIENTS);
    printf("[Server] Listening on port %d...\n", PORT);

    while (1) {
        client_sock = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
        printf("[+] Client connected\n");
        new_sock = malloc(sizeof(int));
        *new_sock = client_sock;
        pthread_t t;
        pthread_create(&t, NULL, handle_client, new_sock);
        pthread_detach(t);
    }

    fclose(binFile);
    fprintf(jsonFile, "{}]\n");  // penutup JSON array
    fclose(jsonFile);
    pthread_mutex_destroy(&lock);

    return 0;
}
