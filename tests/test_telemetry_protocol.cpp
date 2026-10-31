#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include "../shared/telemetry_protocol.h"

using namespace energy_monitor;

int main() {
    const Telemetry expected = {7, 220.4f, 1.25f, 275.5f, 1.5f,
                                219.8f, 0.75f, 165.0f, 1.0f, 440.5f, 3380.0f};
    char buffer[192];
    assert(encodeCsv(expected, buffer, sizeof buffer));
    Telemetry parsed{};
    assert(decodeCsv(buffer, parsed));
    assert(parsed.sequence == 7);
    assert(std::fabs(parsed.total_power_w - 440.5f) < 0.01f);
    assert(std::fabs(parsed.estimated_cost_idr - 3380.0f) < 0.01f);

    // Reject truncated, oversized, non-finite and non-CSV payloads.
    assert(!decodeCsv("7,220.4,1.25", parsed));
    assert(!decodeCsv("7,220.4,1.25,275.5,1.5,219.8,0.75,165,1,440.5,nan", parsed));
    assert(!decodeCsv("7,220.4,1.25,275.5,1.5,219.8,0.75,165,1,440.5,3380,EXTRA", parsed));
    assert(!decodeCsv("invalid", parsed));
    char tiny[5];
    assert(!encodeCsv(expected, tiny, sizeof tiny));

    std::cout << "PASS: CSV round-trip and malformed-packet tests\n";
    std::cout << "Example CSV: " << buffer << "\n";
}
