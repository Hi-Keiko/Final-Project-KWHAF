#include "data_persistence.h"
#include <fstream>
#include <ctime>
#include <iomanip>    // Untuk std::put_time (digunakan di sini untuk strftime)
#include <algorithm>  // Untuk std::sort
#include <sstream>    // Untuk stringstream, jika diperlukan
#include <iostream> 

void save_to_binary(const std::string& data, const std::string& filename) {
    std::ofstream outfile(filename, std::ios::binary | std::ios::app);
    outfile.write(data.c_str(), data.size());
    outfile << "\n"; // Tambahkan newline agar setiap entri terpisah
    outfile.close();
}

void save_to_json(int hour, const std::string& bpm, const std::string& status) {
    std::ofstream file("data/report.json", std::ios::app);
    
    std::string time_str;
    if (hour >= 0 && hour <= 23) {
        std::tm tm = {}; 
        tm.tm_hour = hour;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        char buffer[9]; 
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm);
        time_str = buffer;
    } else {
        time_str = "Invalid Hour";
    }

    file << "{\n"
         << "  \"timestamp\": \"" << time_str << "\",\n"
         << "  \"bpm\": " << bpm << ",\n"
         << "  \"status\": \"" << status << "\"\n},\n";
    file.close();
}

// Implementasi fungsi baru untuk laporan harian
void generate_daily_report_json(const std::vector<HeartData>& all_data) {
    // Buat salinan data untuk disortir agar tidak mengubah data asli
    std::vector<HeartData> sorted_data = all_data;

    // Sorting data berdasarkan BPM (rendah ke tinggi)
    std::sort(sorted_data.begin(), sorted_data.end(), [](const HeartData& a, const HeartData& b) {
        return a.bpm < b.bpm;
    });

    std::ofstream file("data/daily_report.json"); // File baru untuk laporan harian
    if (!file.is_open()) {
        // Log error jika file tidak bisa dibuka
        // Karena data_persistence tidak punya log_message, bisa pakai std::cerr
        std::cerr << "[ERROR] Could not open data/daily_report.json for writing." << std::endl;
        return;
    }

    // Tulis awal JSON
    file << "{\n";
    file << "  \"report_title\": \"LAPORAN REKAMAN DATA BPM DALAM SEHARI\",\n";
    file << "  \"data\": [\n";

    // Tulis setiap entri data yang sudah disortir
    for (size_t i = 0; i < sorted_data.size(); ++i) {
        const auto& data = sorted_data[i];
        
        // Format jam menjadi HH:MM:SS
        std::string time_str;
        std::tm tm = {}; 
        tm.tm_hour = data.hour;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        char buffer[9]; 
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm);
        time_str = buffer;

        file << "    {\n"
             << "      \"timestamp\": \"" << time_str << "\",\n"
             << "      \"bpm\": " << data.bpm << ",\n"
             << "      \"status\": \"" << data.status << "\"\n    }";
        
        if (i < sorted_data.size() - 1) {
            file << ",\n"; // Tambahkan koma kecuali untuk entri terakhir
        } else {
            file << "\n";
        }
    }

    // Tulis akhir JSON
    file << "  ]\n";
    file << "}\n";
    file.close();
}