#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

#include <string>
#include <utility> // Untuk std::pair

// Fungsi baru yang mengembalikan seluruh baris (hour,bpm)
std::string read_next_data(const std::string& filename, int& current_line);

#endif