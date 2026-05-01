# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

RC car GPS speedometer firmware for AI-Thinker A7/A9G (GPS+GPRS, RDA8955 MIPS SoC). No ESP or other MCU — A7/A9G is the only processor.

**This repo contains only:** firmware source (`src/`), build script, CLAUDE.md, README.md, and build outputs (`builds/`, `releases/`).

**SDK location (external, not in repo):** `~/workspace/AI-Thinker/GPRS_C_SDK/`
**Toolchain:** Docker `neucrack/gprs_build` (macOS — native toolchain in `~/workspace/AI-Thinker/a9g-sdk-ception/` is Windows-only `.exe` files)

---

## Roadmap

| Stage | Scope | Status |
|-------|-------|--------|
| POC | GPS open → parse speed → Trace to serial | in progress |
| MVP | Peak/avg stats + GPRS HTTP uplink | next |

---

## Build

```bash
# one-time
docker pull neucrack/gprs_build

# build (run from repo root)
make build        # copies src/ into SDK app/, runs Docker build, copies .lod to builds/

# flash
make flash        # opens Coolwatcher instructions
```

**Makefile** syncs `src/` → `SDK/app/src/`, builds inside Docker, copies output `.lod` files to `builds/`.
First flash to device: `builds/app_B*.lod`. Incremental: `builds/app_flash*.lod`.

Flash tool: Coolwatcher in `~/workspace/AI-Thinker/a9g-sdk-ception/cooltools/`
Flash programmer: `~/workspace/a9g/host_8955_flsh_spi32m_ramrun.lod`

---

## Architecture

Entry point: `app_Main()` in `src/speedometer.c`.

All code runs in RTOS tasks via `OS_CreateTask`. Event loop in main task dispatches `API_EVENT_ID_GPS_UART_RECEIVED` → `GPS_Update()`. GPS data read from `Gps_GetInfo()->rmc`.

Key SDK calls:
```c
GPS_Init(); GPS_Open(NULL);           // start GPS engine
GPS_Update(data, len);                // feed NMEA bytes (from event)
GPS_Info_t *i = Gps_GetInfo();        // parsed struct
i->rmc.speed.value / i->rmc.speed.scale  // knots (minmea_float)
i->rmc.valid                          // fix status
Trace(1, "fmt %d", int_val);          // no %f — use integers only
```

MVP adds files one at a time: `stats.c`, `uplink.c`. `speedometer.c` stays as wiring only.

---

## Dev Workflow

- Trunk-based: `main` always builds. Short-lived feature branches, same-day merge.
- **Before every commit:** `make build` must pass (0 errors).
- **Before every push:** flash to hardware, confirm speed in Coolwatcher tracer.
- Commit format: `type(scope): message` — `feat(gps): add peak speed`
- Small commits — one working increment per commit.

---

## Token / Debug Rules

- Read only files needed for the current change.
- No preamble, no post-summary. Lead with the change.
- **Two-attempt limit:** attempt 1 → most likely fix. Attempt 2 → one alternative. If both fail: write diagnosis, stop, wait for user.
- One search round — if first grep is enough, stop.
- Ask user before any destructive action (delete branch, force push, rm files).
