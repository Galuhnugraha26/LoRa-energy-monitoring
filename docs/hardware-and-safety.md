# Hardware assumptions and safety

| Interface | 2023 report reference | Check before flashing |
| --- | --- | --- |
| ESP32 ↔ LoRa | SPI SCK=18, MISO=19, MOSI=23, CS=5, RST=14, DIO0=2 | Verify module pinout/logic level, antenna, and GPIO boot-strapping |
| ESP32 ↔ PZEM-004T v3 | UART2 RX=16, TX=17; slave IDs 0x08 / 0x07 | Confirm two independent addresses and safe, documented multi-drop interface |
| ESP32 ↔ LCD | I2C at 0x27, assumed SDA=21/SCL=22 | Confirm module and LCD library API |
| Receiver LED | GPIO27 | Add appropriate resistor/driver; **not** an AC mains control |
| LoRa RF | 923 MHz in report | Confirm the band and legal transmit-power/duty-cycle limits for your deployment country |

**High-voltage warning:** PZEM-004T systems may involve hazardous AC mains. Never wire or energize the mains side without suitable training, isolation, and an experienced qualified supervisor/electrician. The provided firmware can be reviewed and host-tested without mains voltage. Do not connect ESP32 GPIO directly to dangerous mains potentials. Use an antenna before transmitting with an RF module, and use correct power/logic-level supplies.

**Historical caution:** The report's `pzem.resetEnergy()` in `setup()` would reset the meter on boot. The reconstruction does not do this. Preserve original measurements unless the user explicitly triggers a documented reset.
