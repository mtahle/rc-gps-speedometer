# RC GPS Speedometer

GPS speedometer firmware for the **AI-Thinker A9G** (GPS+GPRS, RDA8955 MIPS SoC). Built for RC car speed analysis. The A9G is the sole processor — no host MCU.

**Current stage:** POC — reads GPS, outputs speed in km/h over serial.

---

## Hardware

- **Module:** AI-Thinker A9G (two antennas: GSM + GPS)
- **Chip:** RDA8955 (MIPS, 312 MHz, 32MB Flash, 32MB RAM)
- **Connection:** USB-to-serial adapter (3.3V logic) → A9G `HST_TX` / `HST_RX` pins (UART0)
- **Power:** 3.7–4.2V LiPo or regulated supply capable of 2A peak. Do not power from USB-serial adapter alone.

```
USB-Serial Adapter      A9G
──────────────────      ───
TX               ──────> HST_RX
RX               <────── HST_TX
GND              ──────  GND
                         VCC ← separate LiPo / PSU
```

---

## Getting Started

### Prerequisites

| Requirement | Notes |
|-------------|-------|
| AI-Thinker A9G module | GPS+GPRS, two antennas |
| USB-to-serial adapter | CP2102 or CH340, 3.3V logic |
| LiPo battery or bench PSU | 3.7–4.2V, 2A capable |
| Docker Desktop | macOS/Linux build — [docker.com](https://www.docker.com/products/docker-desktop) |
| Coolwatcher | Flash tool — Windows native or Wine on macOS |
| GPRS_C_SDK | External SDK dependency |
| `git`, `make` | Standard tools |

### 1. Clone this repo

```bash
git clone https://github.com/mtahle/rc-gps-speedometer.git
cd rc-gps-speedometer
```

### 2. Set up the SDK

The SDK is an external dependency — not bundled in this repo.

```bash
git clone https://github.com/Ai-Thinker-Open/GPRS_C_SDK.git ~/workspace/AI-Thinker/GPRS_C_SDK
```

If you clone it elsewhere, update `SDK_DIR` at the top of the `Makefile`.

### 3. Set up the build toolchain

**macOS / Linux — Docker:**
```bash
docker pull neucrack/gprs_build
```

> The native CSDTK (`a9g-sdk-ception/`) contains Windows-only `.exe` files. Docker provides the equivalent MIPS cross-compiler for macOS/Linux.

**Windows — native CSDTK:**
```bat
cd a9g-sdk-ception
config_env_admin.bat
```
Then use `build.bat app` inside the SDK instead of `make build`.

### 4. Build

```bash
make build
```

Copies `src/` into the SDK's `app/src/`, runs the Docker build, and drops the output `.lod` firmware files into `builds/`.

### 5. Flash to A9G

```bash
make flash   # prints step-by-step instructions
```

**Tool:** Coolwatcher (in `a9g-sdk-ception/cooltools/coolwatcher.exe`)  
**Flash programmer:** `~/workspace/a9g/host_8955_flsh_spi32m_ramrun.lod`

| Flash file | When to use |
|------------|-------------|
| `builds/app_B*.lod` | First flash, or after SDK update |
| `builds/app_flash*.lod` | Incremental update (faster) |

### 6. Read serial output

Open Coolwatcher tracer on the same COM port (115200 baud):

```
[spd] RC GPS Speedometer POC starting
[spd] GPS open, waiting for fix...
[spd] GPS fixed
[spd] 42.30 km/h
```

> GPS cold fix takes 30–90 s. Go outdoors or near a window.

---

## Development Environment

| Tool | Purpose | Install |
|------|---------|---------|
| Docker Desktop | MIPS cross-compiler (macOS/Linux) | [docker.com](https://www.docker.com/products/docker-desktop) |
| Coolwatcher | Flash firmware + serial trace | `a9g-sdk-ception/cooltools/` |
| VS Code + clangd extension | IDE with full SDK header resolution | `.clangd` in repo root |
| `gh` CLI | GitHub operations | `brew install gh` |

### IDE setup (VS Code)

1. Install the [clangd extension](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)
2. Open the `rc-gps-speedometer/` folder
3. clangd reads `.clangd` automatically — SDK headers resolve, LSP errors clear

If you moved the SDK, update the include paths in `.clangd`.

---

## Project Structure

```
rc-gps-speedometer/
├── src/
│   └── speedometer.c      ← all firmware (POC: single file)
├── builds/                ← compiled .lod files (gitignored)
├── releases/              ← tagged release firmware
├── Makefile               ← build / flash / clean
├── .clangd                ← IDE SDK include paths
├── CLAUDE.md              ← AI assistant guidance
└── README.md
```

SDK lives at `~/workspace/AI-Thinker/GPRS_C_SDK/` — external, not in this repo.

---

## Roadmap

- [x] POC: GPS fix → speed in km/h → serial output
- [ ] MVP: Peak / avg speed tracking
- [ ] MVP: GPRS HTTP POST speed data
- [ ] Display: I2C OLED readout
