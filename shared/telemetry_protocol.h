#pragma once

// Shared CSV packet format reconstructed from the 2023 report.
// Standard C++ (no Arduino dependencies) so it can be tested on a laptop.

#include <cerrno>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>

namespace energy_monitor {

struct Telemetry {
    uint32_t sequence;
    float voltage1_v;
    float current1_a;
    float power1_w;
    float energy1_kwh;
    float voltage2_v;
    float current2_a;
    float power2_w;
    float energy2_kwh;
    float total_power_w;
    float estimated_cost_idr;
};

inline bool encodeCsv(const Telemetry& f, char* output, size_t size) {
    if (output == nullptr || size == 0) return false;
    const int n = std::snprintf(
        output, size,
        "%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
        static_cast<unsigned long>(f.sequence),
        static_cast<double>(f.voltage1_v), static_cast<double>(f.current1_a),
        static_cast<double>(f.power1_w), static_cast<double>(f.energy1_kwh),
        static_cast<double>(f.voltage2_v), static_cast<double>(f.current2_a),
        static_cast<double>(f.power2_w), static_cast<double>(f.energy2_kwh),
        static_cast<double>(f.total_power_w), static_cast<double>(f.estimated_cost_idr)
    );
    return n > 0 && static_cast<size_t>(n) < size;
}

inline bool decodeCsv(const char* input, Telemetry& decoded) {
    if (input == nullptr || *input == '\0' || *input == '-') return false;
    errno = 0;
    char* end = nullptr;
    const unsigned long seq = std::strtoul(input, &end, 10);
    if (errno == ERANGE || end == input || *end != ',' || seq > UINT32_MAX) return false;

    float* fields[10] = {
        &decoded.voltage1_v, &decoded.current1_a, &decoded.power1_w,
        &decoded.energy1_kwh, &decoded.voltage2_v, &decoded.current2_a,
        &decoded.power2_w, &decoded.energy2_kwh, &decoded.total_power_w,
        &decoded.estimated_cost_idr
    };
    const char* cursor = end + 1;
    for (int i = 0; i < 10; ++i) {
        if (*cursor == '\0') return false;
        errno = 0;
        const float value = std::strtof(cursor, &end);
        if (end == cursor || errno == ERANGE || !std::isfinite(value)) return false;
        const char required = i < 9 ? ',' : '\0';
        if (*end != required) return false;
        *fields[i] = value;
        cursor = end + (i < 9 ? 1 : 0);
    }
    decoded.sequence = static_cast<uint32_t>(seq);
    return true;
}

} // namespace energy_monitor
