SDK_DIR   := $(HOME)/workspace/AI-Thinker/GPRS_C_SDK
APP_SRC   := $(SDK_DIR)/app/src
BUILD_OUT := $(SDK_DIR)/build/hex

.PHONY: help build flash clean

help:
	@echo "make build   — compile firmware (requires Docker)"
	@echo "make flash   — print flash instructions"
	@echo "make clean   — remove local builds/"

build:
	@echo "→ Syncing src/ to SDK app..."
	@rm -f $(APP_SRC)/*.c $(APP_SRC)/*.h
	@cp src/*.c src/*.h $(APP_SRC)/ 2>/dev/null || cp src/*.c $(APP_SRC)/
	@echo "→ Building in Docker..."
	docker run --rm -v $(SDK_DIR):/sdk neucrack/gprs_build /bin/bash -c "cd /sdk && ./build.sh app"
	@echo "→ Copying outputs to builds/..."
	@mkdir -p builds
	@cp $(BUILD_OUT)/app_*.lod builds/ 2>/dev/null && echo "✓ builds/ updated" || echo "✗ No .lod files found — check Docker build output"

flash:
	@echo ""
	@echo "Flash steps:"
	@echo "  1. Connect USB-serial adapter to A9G UART0"
	@echo "  2. Open: ~/workspace/AI-Thinker/a9g-sdk-ception/cooltools/coolwatcher"
	@echo "  3. Profile: 8955"
	@echo "  4. Flash programmer: ~/workspace/a9g/host_8955_flsh_spi32m_ramrun.lod"
	@echo "  5. Firmware (first flash): builds/app_B*.lod"
	@echo "  6. Firmware (incremental): builds/app_flash*.lod"
	@echo ""

clean:
	@rm -rf builds/
