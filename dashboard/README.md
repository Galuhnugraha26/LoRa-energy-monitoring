# Node-RED dashboard

## Original interface evidence (July 2023)

Four original screenshots from the 2023 monitoring setup are stored in [`screenshots/`](screenshots/). They document the UI with real-time total power, dual PZEM-004T readings, historical graphs, LoRa packet metadata (reading ID and RSSI), estimated energy cost, and power setpoint controls. These screenshots are **evidence of the original UI**, not proof that the 2026 reconstruction has been tested on hardware.

## Reconstructed demo flow (2026)

The original 2023 Node-RED flow was not available. The provided flow is an **illustrative 2026 reconstruction** that parses the receiver's `DATA:` JSON lines. It does not claim to be the original dashboard or to have been tested against a physical gateway.

## Import example

1. Open Node-RED → menu → Import → select `demo_flow.json`.
2. Deploy. Trigger **Synthetic sample DATA line** and inspect the debug output.
3. For hardware, install the appropriate `node-red-node-serialport` package if needed, add a **serial in** node, set the receiver COM port and 115200 baud with newline split, then connect it to **Parse DATA lines**.
4. To build a real dashboard, add dashboard nodes for instantaneous power, energy, RSSI and estimated cost. All cost values are only estimates based on your configured tariff.

Do not connect the same serial port to Node-RED and VS Code serial monitor simultaneously.
