// 2026 reconstruction from the author's 2023 project report.
// Hardware: ESP32 + 2x PZEM-004T v3 + SX127x LoRa + 16x2 I2C LCD.
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <PZEM004Tv30.h>
#include <LiquidCrystal_I2C.h>
#include <cmath>

#include "config.h"
#include "../../../shared/telemetry_protocol.h"

using energy_monitor::Telemetry;

LiquidCrystal_I2C lcd(0x27, 16, 2);
PZEM004Tv30 meter1(Serial2, PZEM_RX_PIN, PZEM_TX_PIN, PZEM_METER_1_ADDRESS);
PZEM004Tv30 meter2(Serial2, PZEM_RX_PIN, PZEM_TX_PIN, PZEM_METER_2_ADDRESS);

uint32_t sequenceNumber = 0;
bool loraReady = false;

bool readMeters(Telemetry& frame) {
    frame.sequence = sequenceNumber;
    frame.voltage1_v = meter1.voltage();
    frame.current1_a = meter1.current();
    frame.power1_w = meter1.power();
    frame.energy1_kwh = meter1.energy();
    frame.voltage2_v = meter2.voltage();
    frame.current2_a = meter2.current();
    frame.power2_w = meter2.power();
    frame.energy2_kwh = meter2.energy();

    // PZEM returns NaN on missing/invalid communication. Do not transmit junk.
    const float sensorValues[] = {
        frame.voltage1_v, frame.current1_a, frame.power1_w, frame.energy1_kwh,
        frame.voltage2_v, frame.current2_a, frame.power2_w, frame.energy2_kwh
    };
    for (float value : sensorValues) {
        if (!std::isfinite(value)) return false;
    }

    frame.total_power_w = frame.power1_w + frame.power2_w;
    frame.estimated_cost_idr = (frame.energy1_kwh + frame.energy2_kwh) * TARIFF_IDR_PER_KWH;
    return true;
}

void updateLcd(const Telemetry& frame) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("M1: ");
    lcd.print(frame.power1_w, 1);
    lcd.print(" W");
    lcd.setCursor(0, 1);
    lcd.print("M2: ");
    lcd.print(frame.power2_w, 1);
    lcd.print(" W");
}

bool startLoRa() {
    SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_CS_PIN);
    LoRa.setPins(LORA_CS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
    for (int attempt = 0; attempt < 10; ++attempt) {
        if (LoRa.begin(LORA_FREQUENCY_HZ)) {
            LoRa.enableCrc();
            return true;
        }
        Serial.println("LoRa initialization retry...");
        delay(500);
    }
    return false;
}

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22); // Generic ESP32 default I2C pins; verify wiring.
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Energy Monitor");

    // Important: do NOT call resetEnergy() at boot; that destroys cumulative readings.
    loraReady = startLoRa();
    Serial.printf("PZEM addresses configured: 0x%02X, 0x%02X\n",
                  meter1.getAddress(), meter2.getAddress());
    Serial.println(loraReady ? "LoRa ready" : "LoRa NOT ready");
}

void loop() {
    Telemetry frame{};
    if (!readMeters(frame)) {
        Serial.println("PZEM read failed (NaN). Check wiring, power, and addresses.");
        delay(SAMPLE_INTERVAL_MS);
        return;
    }
    updateLcd(frame);

    char csvPacket[192];
    if (!energy_monitor::encodeCsv(frame, csvPacket, sizeof(csvPacket))) {
        Serial.println("Could not encode LoRa packet");
    } else if (loraReady) {
        LoRa.beginPacket();
        LoRa.print(csvPacket);
        const int ok = LoRa.endPacket();
        if (ok) {
            Serial.printf("TX #%lu: %s\n",
                          static_cast<unsigned long>(sequenceNumber), csvPacket);
            ++sequenceNumber;
        } else {
            Serial.println("LoRa transmission failed");
        }
    } else {
        Serial.println("Measurement OK, but LoRa is unavailable");
    }
    delay(SAMPLE_INTERVAL_MS);
}
