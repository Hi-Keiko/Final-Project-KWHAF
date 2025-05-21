#ifndef DATA_PERSISTENCE_H
#define DATA_PERSISTENCE_H

#include <string>

void save_to_binary(const std::string& data, const std::string& filename);
void save_to_json(const std::string& bpm, const std::string& status);

#endif
