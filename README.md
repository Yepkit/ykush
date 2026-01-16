# ykushcmd - YKUSH Command Application

`ykushcmd` is a command-line utility for controlling **Yepkit YKUSH USB hub devices**, allowing you to programmatically power USB ports on and off.

It is commonly used for:
- Automated USB device testing
- Power-cycling USB peripherals
- CI / lab automation
- Embedded and hardware development workflows

---

## Features

- Control individual YKUSH USB hub ports
- Cross-platform support:
  - Linux
  - macOS
  - Windows
- Uses **HIDAPI** (no direct libusb dependency)
- Modern **CMake** build system
- Supports:
  - `make install` / `cmake --install`
  - Linux `.deb` packages
  - Windows ZIP and NSIS installer
- Works with shared HIDAPI (recommended)

---

## Supported Devices
- [YKUSH](https://www.yepkit.com/products/ykush)
- [YKUSHXS](https://www.yepkit.com/product/300115/YKUSHXS)
- [YKUSH3](https://www.yepkit.com/product/300110/YKUSH3)


---

## Dependencies

### Build-time

- CMake ≥ 3.15
- C++ compiler with C++17 support
- HIDAPI (vendored)

### Runtime

- **Linux**: `libhidapi-hidraw`
- **macOS**: none (uses native IOKit)
- **Windows**: `hidapi.dll` + MinGW runtime DLLs

---

## 🚀 Quick Start (End Users)

### 1. Plug in your YKUSH device

Connect your YKUSH hub to your computer via USB.

---

### 2. Verify the device is detected

```bash
# For YKUSH
ykushcmd ykush -l

# For YKUSH3
ykushcmd ykush3 -l

# For YKUSHXS
ykushcm ykushxs -l
```
---

### 3. Control USB ports

Turn port 1 ON:
```bash
ykushcmd ykush3 -u 1
```

Turn port 1 OFF:
```bash
ykushcmd ykush3 -d 1
```
---

### 4. Select a specific device (multiple YKUSH hubs)
 ```bash
ykushcmd -s YK12345 -u 1
 ```
---

### 5. Get help
```bash
ykushcmd -h
```
---

## 🔐 Linux Permissions (udev rules)

On Linux, access to HID devices is restricted by default.

### Install udev rules (recommended)

Create a new rules file:
```bash
sudo nano /etc/udev/rules.d/99-ykush.rules
```

Add:
```bash
# Yepkit YKUSH USB hub
SUBSYSTEM=="usb", ATTR{idVendor}=="04d8", MODE="0666"
```
Reload rules and replug device:
```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```
Verify:
```bash
ykushcmd ykush3 -l
```

---

## 🪟 Windows Notes
- No special permissions required
- Uses native Windows HID
- Ensure `hidapi.dll` is present next to `ykushcmd.exe` or in `PATH`

---

## 🍎 macOS Notes
- No additional permissions required
- Uses native IOKit HID support
- Works out of the box

---

## Building

### Linux / macOS
```bash
cmake -S . -B build
cmake --build build
```

---

### Windows (MinGW cross-build)
```bash
./scripts/install-mingw64.sh
```
Runtime DLLs:
```bash
./scripts/windows-copy-dlls.sh dist/windows/bin
```

---

## Installing

## Linux
```bash
./scripts/install.sh
```

## macOS
```bash
./scripts/install-macos.sh
```

Custom prefix:
```bash
PREFIX=/opt/ykush ./scripts/install.sh
```

---

## Uninstalling
```bash
cmake --build build --target uninstall
```
---

## Packaging
### Linux (.deb)
```bash
cmake -S . -B build
cmake --build build
cpack --config build/CPackConfig.cmake
```
---

### Windows (ZIP / NSIS installer)
```bash
cpack --config build-mingw64/CPackConfig.cmake
```
---

## Notes on HIDAPI
- Uses shared HIDAPI by default
- Static HIDAPI builds are intentionally avoided
- Includes a MinGW-specific include-path workaround

---

## Contributing
Contributions are welcome. Please ensure:
- Linux and MinGW builds succeed
- CMake remains cross-platform
- No static HIDAPI assumptions are introduced

---

## Licensing
The source code is licensed Apache License, Version 2.0. 
Refer to [LICENSE](LICENSE.md) file.

---

## Acknowledgements
- HIDAPI maintainers





