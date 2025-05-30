#include "client_utils.h"
#include <fstream>
#include <vector>

// Fungsi baru yang membaca seluruh baris dari file
std::string read_next_data(const std::string& filename, int& current_line) {
    std::ifstream file(filename);
    std::string line;
    int line_num = 0;
    while (std::getline(file, line)) {
        if (line_num == current_line) {
            current_line++;
            return line; // Mengembalikan seluruh baris, misal "0,70"
        }
        ++line_num;
    }
    return ""; // Kembali string kosong jika EOF
}