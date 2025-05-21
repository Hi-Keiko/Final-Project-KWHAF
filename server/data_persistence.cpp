#include "data_persistence.h"
#include <fstream>
#include <ctime>

void save_to_binary(const std::string& data, const std::string& filename) {
    std::ofstream outfile(filename, std::ios::binary | std::ios::app);
    outfile.write(data.c_str(), data.size());
    outfile.close();
}

void save_to_json(const std::string& bpm, const std::string& status) {
    std::ofstream file("data/report.json", std::ios::app);
    std::time_t now = std::time(nullptr);
    file << "{\n  \"timestamp\": " << now << ",\n"
         << "  \"bpm\": " << bpm << ",\n"
         << "  \"status\": \"" << status << "\"\n},\n";
    file.close();
}
