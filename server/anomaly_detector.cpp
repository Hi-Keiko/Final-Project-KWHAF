#include "anomaly_detector.h"

bool is_abnormal(int bpm) {
    return bpm < 60 || bpm > 100;
}
