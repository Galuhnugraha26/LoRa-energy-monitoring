# Reconstruction notes — 2023 report to 2026 source tree

## What is verified from the supplied report transcription

- Two program listings: Node End (Transmitter) and Node Gateway (Receiver).
- ESP32; PZEM-004T v3 library; two addresses configured as `0x08` and `0x07` over `Serial2` pins 16/17.
- SX127x-compatible `LoRa.h` interface, pins SCK18, MISO19, MOSI23, CS5, RST14, DIO0=2 and a source setting of 923 MHz.
- 16x2 I2C LCD address `0x27`.
- Nominal 10-second transmitter measurement cycle.
- 11 comma-separated fields per LoRa packet: `sequence, v1, i1, p1, e1, v2, i2, p2, e2, total_p, cost`.
- Receiver used a wired serial integration in the overall Node-RED workflow. The Node-RED flow itself was **not** provided.

## Repairs / additions in the 2026 reconstruction

1. Removed report line-wrap artifacts and invalid standalone prose.
2. Fixed accidentally shadowed LCD/PZEM objects and missing/mis-scoped variables.
3. Made radio initialization retry count deterministic and handle failure.
4. Separated the common CSV wire protocol into a host-testable C++ header.
5. Added field-count/numeric validation at receiver and skips invalid (NaN) sensor readings.
6. Removed automatic `resetEnergy()` on boot to protect cumulative meter values. Resetting a real meter must be deliberate.
7. Removed unused Wi-Fi login from reconstructed receiver. Serial/USB remains the Node-RED bridge.
8. Replaced incomplete `sendtoNodeRed()` with `DATA:`-prefixed JSON lines and an optional demonstrator flow.
9. Corrected the conceptual cost calculation to `sum(energy_kwh) * tariff_idr_per_kwh`. The assumed tariff (`1352 IDR/kWh`) is **illustrative only**, not a validated or current PLN price. The historic formula had inconsistent units.
10. Added hardware checklists, instructions for VS Code/PlatformIO and host unit tests.

## Unverified until physical hardware is available

- Actual ESP32 board and LoRa chip/module, module voltage levels, wiring and RF antenna.
- Whether both PZEM modules really have addresses `0x08` and `0x07` and share the UART reliably.
- Whether the LCD uses the selected I2C library and pins 21/22.
- True tariff policy, time/energy accounting and tariff unit conversions.
- Original Node-RED dashboard structure and behavior; demonstration flow is recreated.
- Firmware compilation using the user's exact board/toolchain, flashing, hardware readings and end-to-end RF delivery.

Never treat these code repairs as evidence of features present in 2023. Both source and tests were reconstructed in 2026.
