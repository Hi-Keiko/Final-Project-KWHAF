#ifndef TYPES_H
#define TYPES_H

#include <string>

// Struktur untuk menyimpan setiap data poin BPM
struct HeartData {
    int hour;
    int bpm;
    std::string status; // Status (normal, bradikardia, takikardia)
};

#endif