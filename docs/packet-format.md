# Legacy LoRa protocol (11-field CSV)

The transmitter and receiver communicate using the 11-field comma-separated packet preserved from the 2023 report:

| Position | Name | Units |
| --- | --- | --- |
| 1 | sequence | integer |
| 2 | voltage1 | V |
| 3 | current1 | A |
| 4 | power1 | W |
| 5 | energy1 | kWh |
| 6 | voltage2 | V |
| 7 | current2 | A |
| 8 | power2 | W |
| 9 | energy2 | kWh |
| 10 | total_power | W |
| 11 | estimated_cost | IDR (illustrative only) |

Example (synthetic, **not** recovered experimental data):

```text
7,220.40,1.25,275.50,1.50,219.80,0.75,165.00,1.00,440.50,3380.00
```

The 2026 reconstruction sends a `DATA:{...}` JSON line over USB serial on the **receiver**, so Node-RED can parse it more reliably. That serial format is new, not recovered from the 2023 report.
