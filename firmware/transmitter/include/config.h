#pragma once
#include <stdint.h>

// Illustrative ESP32 DevKit wiring from the 2023 report; verify against your board.
constexpr int LORA_SCK_PIN = 18;
constexpr int LORA_MISO_PIN = 19;
constexpr int LORA_MOSI_PIN = 23;
constexpr int LORA_CS_PIN = 5;
constexpr int LORA_RST_PIN = 14;
constexpr int LORA_DIO0_PIN = 2;
constexpr long LORA_FREQUENCY_HZ = 923000000L; // Verify local radio regulations.

constexpr int PZEM_RX_PIN = 16;
constexpr int PZEM_TX_PIN = 17;
constexpr uint8_t PZEM_METER_1_ADDRESS = 0x08;
constexpr uint8_t PZEM_METER_2_ADDRESS = 0x07;
constexpr uint32_t SAMPLE_INTERVAL_MS = 10000;

// Example cost assumption only. This is NOT a verified current PLN tariff.
// Set this according to the relevant tariff if cost estimates are needed.
constexpr float TARIFF_IDR_PER_KWH = 1352.0f;
