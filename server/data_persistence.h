#ifndef DATA_PERSISTENCE_H
#define DATA_PERSISTENCE_H

#include <string>
#include <vector>    // Untuk std::vector
#include "../types.h"     // Tambahkan ini untuk HeartData

void save_to_binary(const std::string& data, const std::string& filename);
// Fungsi lama untuk anomali (tetap ada)
void save_to_json(int hour, const std::string& bpm, const std::string& status); 

// Fungsi baru untuk membuat laporan harian yang disortir
void generate_daily_report_json(const std::vector<HeartData>& all_data);

#endif