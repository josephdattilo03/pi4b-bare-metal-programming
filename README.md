# Programming the Raspberry Pi 4 Model B (Bare Metal)

Independent Study Journal — Cortex-A72 / BCM2711

**Student:** Joseph Dattilo · **Course:** CIS 5990 · **Supervisor:** Joel Ramirez · **Term:** Summer 2026

**Goal:** Build a basic operating system on bare metal for the Raspberry Pi 4B.

---

## Setup

Requires the `aarch64-none-elf` GCC toolchain (installed via Homebrew at `/opt/homebrew/bin`).

```sh
make          # builds kernel8.img
make clean    # removes build artifacts
```

Use the official Raspberry Pi Imager to install the Pi4 Raspberry Pi OS (64-bit)
Delete the existing images in the Pi4 boot partition
Copy `kernel8.img` to the boot partition of the Pi's SD card and power on.

---

## Progress

### Week 1
Ordered all the hardware: the Pi 4B, an LED, a TX/RX (USB-to-serial) cable, and Dupont jumper cables to set up and experiment with the board. Worked with my advisor to choose which Pi to target and settled on the Pi 4B — its Cortex-A72 CPU, BCM2711 SoC, and peripherals are all well documented, which makes bare-metal work practical.

### Week 2
Studied the relationship between the two reference manuals (the Cortex-A72 TRM and the BCM2711 peripherals datasheet) and how they fit together. Settled on the bare-metal toolchain: the `aarch64-none-elf` GCC cross-compiler with freestanding flags (`-ffreestanding -nostdinc -nostdlib -nostartfiles`) since there's no OS or standard library. Wrote `linker.ld` to lay the image out in memory — load address `0x80000`, keep the boot code first in `.text`, and reserve a zeroed `.bss`. A small `boot.S` provides the entry point: it parks the three secondary cores, sets the stack, clears `.bss`, then `bl main` to hand off to C. Used that understanding to lay out a plan for the first concrete goal: turning on an LED.

### Week 3
Researched how to drive GPIO from bare metal. The BCM2711 exposes peripherals to the ARM at base `0xFE000000` (Low Peripheral mode), so GPIO sits at `0xFE200000`. Mapped out the registers needed: `GPFSELn` (function select) to make a pin an output, `GPSETn` (`+0x1C`) to drive it high, and `GPCLRn` (`+0x28`) to drive it low. Each pin's function takes 3 bits, so the pin number picks the register (`pin / 10`) and the bit offset within it (`(pin % 10) * 3`); set/clear use one bit per pin (`pin % 32`). All accesses go through `volatile` pointers so the compiler doesn't optimize the hardware writes away.

### Week 4
Wrote the C to do it: configure pin 18 as an output via `gpio_set_function`, then loop writing `GPSET`/`GPCLR` with a busy-wait `delay` in between to blink the LED. Built `kernel8.img`, flashed it to the SD card, and confirmed the LED turns on — proving the full toolchain → linker → boot → C → GPIO path works end to end.

### Week 5
Learned about interrupts and implemented handlers for each one, triggering the LED in reaction to each interrupt to prove the interrupt path is operational.

---

## Resources

- [Raspberry Pi 4 Model B product brief](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/)
- [BCM2711 ARM Peripherals datasheet](https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf)
- [Cortex-A72 Technical Reference Manual](https://developer.arm.com/documentation/100095/latest/)
