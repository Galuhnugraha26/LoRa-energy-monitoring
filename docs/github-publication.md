# Publishing the reconstructed project to GitHub

This project was originally developed in **2023**. The files in this repository were recovered, reorganized and documented in **2026** from listings in a written report. Do not backdate new commits to simulate GitHub contributions in 2023.

## Before publishing

- Read `README.md`, `docs/reconstruction-notes.md` and `docs/hardware-and-safety.md`.
- Confirm that only synthetic demo data is included and that no personal data, Wi-Fi credentials, API keys or third-party material requiring permission are present.
- Confirm that you have the necessary rights to publish the source listings and project materials.
- Understand the current validation status: the host-side CSV protocol test passed, but the firmware has **not** been fully built for the target ESP32 or physically tested.
- Decide whether to provide an open-source license separately; there is intentionally no `LICENSE` file yet.

## Create an empty repository

On GitHub, use **New repository**:

- **Owner:** your GitHub account
- **Repository name:** `lora-energy-monitoring`
- **Description:** `ESP32-based energy monitoring using PZEM-004T sensors, LoRa communication, and Node-RED. Originally developed in 2023; reconstructed in 2026.`
- **Visibility:** Public only after your publication/permissions review; otherwise start Private.
- **Initialize:** do **not** add a README, .gitignore or license using the GitHub form, as these are handled locally.

## Publish from PowerShell in the repository root

```powershell
git init -b main
git add .
git status --short
git commit -m "docs: publish reconstruction of 2023 LoRa energy monitoring project"
git remote add origin https://github.com/YOUR-USERNAME/lora-energy-monitoring.git
git push -u origin main
```

Replace `YOUR-USERNAME` with your GitHub username. If Git prompts for authentication, follow the GitHub browser/device sign-in flow. Don't include personal access tokens in code or committed files.

## Suggested GitHub topics

`esp32`, `pzem004t`, `lora`, `energy-monitoring`, `embedded-cpp`, `nodered`, `wireless-telemetry`

## After publication

Verify the repository renders the README, test and source folders, and no secrets. In GitHub's repository settings, you can add the topics. Consider pinning it on your profile.
