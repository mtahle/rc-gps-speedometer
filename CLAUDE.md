# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

**RC GPS Speedometer** — C firmware for the AI-Thinker A9G (GPS+GPRS, RDA8955 MIPS SoC) targeting RC car speed analysis. All application code lives in `app/src/`. The SDK + RTOS + libs are pre-built; only `app/` is ours.

**Working directory:** `~/workspace/AI-Thinker/GPRS_C_SDK/`
**Chip:** RDA8955 (MIPS, not ARM)
**Toolchain:** CSDTK — Windows-only `.exe` files; macOS builds require Docker

---

## Build

```bash
# One-time: pull the Linux MIPS cross-compiler image
docker pull neucrack/gprs_build

# Build app/ → build/hex/app_B*.lod  +  build/hex/app_flash*.lod
docker run --rm -v $(pwd):/sdk neucrack/gprs_build /bin/bash -c "cd /sdk && ./build.sh app"

# Shortcut (add to shell alias or Makefile)
alias sdk-build='docker run --rm -v $(pwd):/sdk neucrack/gprs_build /bin/bash -c "cd /sdk && ./build.sh app"'
```

**Why Docker:** The `~/workspace/AI-Thinker/a9g-sdk-ception/` toolchain contains only Windows `.exe` files. The Docker image `neucrack/gprs_build` is the macOS/Linux-compatible equivalent — same MIPS cross-compiler, Linux binary.

**First flash:** use `app_B*.lod`. Subsequent flashes: `app_flash*.lod`. After any SDK update, always use `app_B*.lod`.

---

## Flash

**Tool:** Coolwatcher (GUI) in `~/workspace/AI-Thinker/a9g-sdk-ception/cooltools/`
**Flash programmer:** `~/workspace/a9g/host_8955_flsh_spi32m_ramrun.lod`
**Steps:**
1. Connect USB-serial adapter to A9G UART0 (download port)
2. Open Coolwatcher → select profile `8955`
3. Set flash programmer to `host_8955_flsh_spi32m_ramrun.lod`
4. Set firmware to `build/hex/app_B*.lod`
5. Click download

Pre-built demo firmwares for hardware verification: `~/workspace/a9g/` (gpio, gps_tracker, uart).

---

## App Architecture

### POC (current)
`app/src/speedometer.c` — single file:
- `GPS_Open(NULL)` + event loop dispatches `API_EVENT_ID_GPS_UART_RECEIVED` → `GPS_Update()`
- `Gps_GetInfo()->rmc.speed` (minmea_float) → integer km/h via `value / scale * 1852 / 1000`
- `Trace(1, ...)` outputs to UART0 (visible in Coolwatcher tracer)
- Entry point: `app_Main()`

### MVP (add one file at a time)
| File | Purpose |
|------|---------|
| `app/src/stats.c/h` | Peak / avg speed, rolling window |
| `app/src/display.c/h` | UART-driven serial display or I2C OLED |
| `app/src/uplink.c/h` | GPRS HTTP POST of speed JSON |

`speedometer.c` stays as wiring only when MVP modules are added.

### Key CSDK APIs
```c
GPS_Init();                          // must call before GPS_Open
GPS_Open(NULL);                      // NULL = event-based NMEA delivery
GPS_Update(data, len);               // feed NMEA bytes to SDK parser
GPS_Info_t *info = Gps_GetInfo();    // access parsed struct
info->rmc.speed.value / info->rmc.speed.scale  // knots as float
info->rmc.valid                      // true = fix active
Trace(1, "fmt", ...);                // no %f support — use integers
OS_CreateTask(fn, NULL, NULL, stack, prio, 0, 0, "name");
OS_Sleep(ms);
OS_WaitEvent(handle, &event, OS_TIME_OUT_WAIT_FOREVER);
```

---

## Development Workflow (Trunk-Based)

- `master` = always buildable and flashable.
- Short-lived branches (`feat/stats`), same-day merge.
- **Before every commit:** Docker build must succeed (0 errors).
- **Before every push:** flash to hardware, confirm speed traces correctly.
- **Commit format:** `type(scope): message` — e.g. `feat(gps): add peak speed tracking`

---

## Key Constraints

- `Trace()` has no `%f` — use integer arithmetic or `snprintf` to a char buffer.
- GPS cold fix: 30–90 s. Code must handle `info->rmc.valid == false` gracefully.
- All code runs in RTOS tasks via `OS_CreateTask`. No bare `while(1)` in `app_Main`.
- A9G needs peak ~2 A during GPRS TX — use a LiPo or beefy LDO, not USB power alone.
- `.clangd` in repo root adds SDK include paths for IDE (clang LSP errors are IDE-only; Docker build is authoritative).
