// 2026 reconstruction: LoRa receiver + USB-serial Node-RED bridge.
// The original 2023 Node-RED flow is not available.
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal_I2C.h>

#include "config.h"
#include "../../../shared/telemetry_protocol.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
float setpointWatts = -1.0f; // Disabled until user sends a threshold over serial.

bool startLoRa() {
    SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_CS_PIN);
    LoRa.setPins(LORA_CS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
    for (int attempt = 0; attempt < 10; ++attempt) {
        if (LoRa.begin(LORA_FREQUENCY_HZ)) {
            LoRa.enableCrc();
            return true;
        }
        delay(500);
    }
    return false;
}

void checkSerialSetpoint() {
    // Optional user command, e.g. 'SET:500' followed by newline.
    if (!Serial.available()) return;
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (!line.startsWith("SET:")) return;
    const String number = line.substring(4);
    if (number.length() == 0) return;
    setpointWatts = number.toFloat();
    Serial.printf("Setpoint: %.1f W\n", static_cast<double>(setpointWatts));
}

void emitNodeRedJson(const energy_monitor::Telemetry& f, int rssi) {
    // Only DATA: lines are intended for downstream Node-RED parsing.
    Serial.printf(
        "DATA:{\"sequence\":%lu,\"meter1\":{\"voltage_v\":%.2f,"
        "\"current_a\":%.2f,\"power_w\":%.2f,\"energy_kwh\":%.2f},"
        "\"meter2\":{\"voltage_v\":%.2f,\"current_a\":%.2f,"
        "\"power_w\":%.2f,\"energy_kwh\":%.2f},"
        "\"total_power_w\":%.2f,\"estimated_cost_idr\":%.2f,\"rssi_dbm\":%d}\n",
        static_cast<unsigned long>(f.sequence),
        static_cast<double>(f.voltage1_v), static_cast<double>(f.current1_a),
        static_cast<double>(f.power1_w), static_cast<double>(f.energy1_kwh),
        static_cast<double>(f.voltage2_v), static_cast<double>(f.current2_a),
        static_cast<double>(f.power2_w), static_cast<double>(f.energy2_kwh),
        static_cast<double>(f.total_power_w),
        static_cast<double>(f.estimated_cost_idr), rssi
    );
}

void readLoRa() {
    const int packetSize = LoRa.parsePacket();
    if (packetSize == 0) return;

    char packet[192];
    size_t i = 0;
    bool overflow = false;
    while (LoRa.available()) {
        const int value = LoRa.read();
        if (value < 0) break;
        if (i >= sizeof(packet) - 1) {
            overflow = true;
        } else {
            packet[i++] = static_cast<char>(value);
        }
    }
    packet[i] = '\0';
    if (overflow) {
        Serial.println("Rejected oversized LoRa packet");
        return;
    }

    energy_monitor::Telemetry frame{};
    if (!energy_monitor::decodeCsv(packet, frame)) {
        Serial.printf("Rejected malformed LoRa packet: %s\n", packet);
        return;
    }
    const int rssi = LoRa.packetRssi();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Total: ");
    lcd.print(frame.total_power_w, 1);
    lcd.print(" W");
    lcd.setCursor(0, 1);
    lcd.print("RSSI: ");
    lcd.print(rssi);
    lcd.print(" dBm");

    digitalWrite(LED_OUTPUT_PIN,
                 setpointWatts >= 0.0f && frame.total_power_w > setpointWatts ? HIGH : LOW);
    emitNodeRedJson(frame, rssi);
}

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(50);
    pinMode(LED_OUTPUT_PIN, OUTPUT);
    digitalWrite(LED_OUTPUT_PIN, LOW);
    Wire.begin(21, 22);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("LoRa Receiver");
    if (!startLoRa()) {
        Serial.println("ERROR: LoRa initialization failed; check module/wiring");
    } else {
        Serial.println("LoRa receiver ready");
    }
}

void loop() {
    checkSerialSetpoint();
    readLoRa();
}
