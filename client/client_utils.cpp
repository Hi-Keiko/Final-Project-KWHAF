#include "client_utils.h"
#include <fstream>
#include <vector>

std::string read_next_bpm(const std::string& filename, int& current_line) {
    std::ifstream file(filename);
    std::string line;
    int line_num = 0;
    while (std::getline(file, line)) {
        if (line_num == current_line) {
            current_line++;
            return line;
        }
        ++line_num;
    }
    return "";
}
