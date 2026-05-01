# RC GPS Speedometer

GPS speedometer firmware for the AI-Thinker A7/A9G (GPS+GPRS module, RDA8955 MIPS SoC). Designed for RC car speed analysis.

**Current stage:** POC — reads GPS, outputs speed in km/h over serial.

---

## Getting Started

### Prerequisites

- AI-Thinker A7 or A9G module (GPS+GPRS, two antennas)
- USB-serial adapter (3.3V) connected to A7/A9G UART0
- Docker Desktop (for building on macOS/Linux)
- [Coolwatcher](https://github.com/Ai-Thinker-Open/GPRS_C_SDK) (for flashing — Windows GUI or run via Wine)
- `gh` CLI (for GitHub operations)

### 1. Clone this repo

```bash
git clone https://github.com/mtahle/rc-gps-speedometer.git
cd rc-gps-speedometer
```

### 2. Set up the SDK (external dependency)

```bash
cd ~/workspace   # or wherever you keep it
git clone https://github.com/Ai-Thinker-Open/GPRS_C_SDK.git AI-Thinker/GPRS_C_SDK
```

Update the `SDK_DIR` path in `Makefile` if your SDK lives elsewhere.

### 3. Pull the build toolchain

```bash
docker pull neucrack/gprs_build
```

> **Why Docker?** The native CSDTK toolchain (in `a9g-sdk-ception/`) contains Windows-only `.exe` files. Docker provides the same MIPS cross-compiler as a Linux binary.

### 4. Build

```bash
make build
```

Output `.lod` firmware files land in `builds/`.

### 5. Flash to device

```bash
make flash   # prints step-by-step instructions
```

First flash: use `builds/app_B*.lod` (full image).  
Subsequent flashes: `builds/app_flash*.lod` (incremental, faster).

### 6. View output

Open Coolwatcher tracer (UART0, 115200 baud). You should see:

```
[spd] RC GPS Speedometer POC starting
[spd] GPS open, waiting for fix...
[spd] GPS fixed
[spd] 42.30 km/h
```

GPS cold fix takes 30–90 s — go outside or near a window.

---

## Development Environment

| Tool | Purpose | Install |
|------|---------|---------|
| Docker Desktop | MIPS cross-compiler | [docker.com](https://www.docker.com/products/docker-desktop) |
| `gh` CLI | GitHub operations | `brew install gh` |
| Coolwatcher | Flash + serial trace | In `a9g-sdk-ception/cooltools/` |
| VS Code + clangd | IDE with SDK headers | `.clangd` in repo root handles includes |

### Recommended VS Code extensions

- `clangd` — C/C++ language server (uses `.clangd` for SDK paths)
- `EditorConfig` — consistent formatting

### SDK include paths (for IDE)

The `.clangd` file in repo root points clangd at the SDK headers. If you moved the SDK, update the paths in `.clangd`.

---

## Project Structure

```
rc-gps-speedometer/
├── src/               ← firmware source (only our code)
│   └── speedometer.c
├── builds/            ← compiled .lod files (gitignored)
├── releases/          ← tagged release .lod files
├── Makefile           ← build, flash, clean
├── CLAUDE.md          ← AI assistant guidance
└── README.md
```

SDK lives externally at `~/workspace/AI-Thinker/GPRS_C_SDK/` — not in this repo.

---

## Roadmap

- [x] POC: GPS open → parse speed → serial output
- [ ] MVP: Peak/avg speed tracking
- [ ] MVP: GPRS HTTP uplink (POST speed JSON)
- [ ] Display: I2C OLED speed readout
