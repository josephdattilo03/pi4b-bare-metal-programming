# Independent Study Journal
## Programming the Raspberry Pi 4 Model B
### Cortex-A72 Architecture & BCM2711 SoC

---

**Student Name:** Joseph Dattilo
**Course / Department:** CIS 5990
**Faculty Supervisor:** Joel Ramirez
**Term / Semester:** Summer 2026
**Start Date:** May 26 **End Date:** ___________________________

---

## Table of Contents

1. [Study Overview & Objectives](#1-study-overview--objectives)
2. [Hardware Reference](#2-hardware-reference)
3. [Development Environment](#3-development-environment)
4. [Weekly Journal Entries](#4-weekly-journal-entries)
5. [Project Log](#5-project-log)
6. [Code Experiments & Findings](#6-code-experiments--findings)
7. [Challenges & Troubleshooting](#7-challenges--troubleshooting)
8. [Resources & References](#8-resources--references)
9. [Final Reflection & Outcomes](#9-final-reflection--outcomes)
10. [Supervisor Sign-Off](#10-supervisor-sign-off)

---

## 1. Study Overview & Objectives

### Abstract
> *(Write a 2–4 sentence summary of the independent study focus, methods, and expected outcomes.)*

### Goals & Learning Objectives

| # | Objective | Target Date | Status |
|---|-----------|-------------|--------|
| 1 |           |             | Not Started |
| 2 |           |             | Not Started |
| 3 |           |             | Not Started |
| 4 |           |             | Not Started |
| 5 |           |             | Not Started |

**Status options:** `Not Started` · `In Progress` · `Complete` · `Deferred`

### Scope & Deliverables
- **Primary deliverable:** Create a basic operating system on bare metal using the rasp pi4

---

## 2. Hardware Reference

### Raspberry Pi 4 Model B — Specification Sheet

| Component | Specification |
|-----------|--------------|
| **SoC** | Broadcom BCM2711 |
| **CPU** | Quad-core ARM Cortex-A72 @ 1.5 GHz (1.8 GHz with Pi 4B Rev 1.5+) |
| **CPU Architecture** | ARMv8-A (64-bit), out-of-order, 3-way superscalar |
| **L1 Cache** | 32 KB instruction + 32 KB data per core |
| **L2 Cache** | 1 MB shared (unified) |
| **GPU** | VideoCore VI @ 500 MHz |
| **RAM Options** | 1 GB / 2 GB / 4 GB / 8 GB LPDDR4-3200 SDRAM |
| **RAM (this unit)** | *(fill in — e.g., 4 GB)* |
| **Storage** | MicroSD (primary boot) + optional USB 3.0 boot |
| **USB** | 2× USB 3.0, 2× USB 2.0 |
| **Networking** | Gigabit Ethernet; 802.11ac Wi-Fi; Bluetooth 5.0 |
| **GPIO** | 40-pin header (UART, SPI, I²C, PWM, 3.3 V & 5 V) |
| **Video Out** | 2× micro-HDMI (up to 4Kp60) |
| **Power** | 5 V / 3 A via USB-C |
| **OS (this unit)** | *(e.g., Raspberry Pi OS 64-bit Bookworm)* |

### Cortex-A72 Architecture Notes - TODO from this boilerplate
- **Pipeline:** 15-stage, out-of-order execution, branch prediction
- **SIMD / FP:** ARMv8 NEON / Advanced SIMD, VFPv4 floating-point
- **Security:** ARMv8 TrustZone
- **Endianness:** Little-endian
- **Instruction sets:** AArch64 (primary), AArch32 (backward-compatible)
- **Relevant for programming:** NEON intrinsics usable for SIMD optimization; `aarch64` target in compiler flags

### BCM2711 SoC Notes - TODO from this boilerplate
- **Memory controller:** Dual-channel LPDDR4; significant bandwidth improvement over BCM2837
- **PCIe:** 1× PCIe Gen 2 lane exposed via USB 3.0 controller (xHCI via VL805)
- **Display pipeline:** HEVC decode up to 4Kp60 (hardware); H.264 up to 1080p60
- **Thermal:** Thermal throttling begins at 80 °C; throttled clock at 600 MHz; heatsink recommended for sustained workloads
- **DMA:** 7-channel DMA engine available via `/dev/mem` or kernel driver

### My Hardware Setup
TODO if necessary


---

## 3. Development Environment

### OS & System Configuration

(paste output here)

### Software & Tools Installed - TODO change this as needed

| Tool / Library | Version | Purpose |
|----------------|---------|---------|
| OS             |         |         |
| GCC / Clang    |         | Compilation |
| Python         |         |         |
| cmake / make   |         | Build system |
| git            |         | Version control |
|                |         |         |
|                |         |         |

### Compiler Flags Used - TODO fill this in as needed

---

## 4. Weekly Journal Entries

> Copy this template block for each week. Aim for an entry at every session.

---

### Week 1

**Dates:** ________________
**Hours Logged:** ________________

#### What I Did
*(Describe activities, experiments, or code written this week.)*

#### What I Learned
*(Key takeaways — concepts, architecture details, best practices.)*

#### BCM2711 / Cortex-A72 Observations
*(Note any hardware-specific behavior — clock speeds, thermal behavior, SIMD performance, memory bandwidth, etc.)*

#### Next Steps
- [ ] ________________
- [ ] ________________

---

### Week 2

**Dates:** ________________
**Hours Logged:** ________________

#### What I Did


#### What I Learned


#### BCM2711 / Cortex-A72 Observations


#### Next Steps
- [ ] ________________
- [ ] ________________

---

### Week 3

**Dates:** ________________
**Hours Logged:** ________________

#### What I Did


#### What I Learned


#### BCM2711 / Cortex-A72 Observations


#### Next Steps
- [ ] ________________
- [ ] ________________

---

*(Add additional Week N sections as needed)*

---

## 5. Project Log

### Milestone Tracker

| Milestone | Description | Due Date | Completed | Notes |
|-----------|-------------|----------|-----------|-------|
| M1 |  |  |  |  |
| M2 |  |  |  |  |
| M3 |  |  |  |  |
| M4 |  |  |  |  |
| Final |  |  |  |  |

### Time Log

| Date | Duration | Activity Summary |
|------|----------|-----------------|
|      |          |                 |
|      |          |                 |
|      |          |                 |
|      |          |                 |
| **Total** | **0 hrs** | |

---

## 6. Code Experiments & Findings

> Document significant code snippets, benchmarks, and experimental results here.

---

### Experiment 1 — *(Title)*

**Date:** ________________
**Goal:** ________________

**Code:**
```c
// Paste relevant code here

```

**Results / Output:**
```
(paste output or benchmark results)
```

**Analysis:**
*(What does this tell you about the BCM2711 / Cortex-A72? Memory behavior? Pipeline effects? NEON speedup?)*

---

### Experiment 2 — *(Title)*

**Date:** ________________
**Goal:** ________________

**Code:**
```python
# Paste relevant code here

```

**Results / Output:**
```

```

**Analysis:**


---

*(Add Experiment N sections as needed)*

---

## 7. Challenges & Troubleshooting

| # | Challenge / Error | Root Cause | Solution / Workaround | Status |
|---|-------------------|------------|-----------------------|--------|
| 1 |  |  |  |  |
| 2 |  |  |  |  |
| 3 |  |  |  |  |
| 4 |  |  |  |  |

---

## 8. Resources & References

> Use a consistent citation style (APA, IEEE, etc.) as required by your institution.

### Official Documentation
- Raspberry Pi Foundation. (2024). *Raspberry Pi 4 Model B product brief*. https://www.raspberrypi.com/products/raspberry-pi-4-model-b/
- Broadcom. (2021). *BCM2711 ARM Peripherals datasheet*. https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf
- Arm Limited. (2015). *Cortex-A72 Technical Reference Manual*. https://developer.arm.com/documentation/100095/latest/

### Books & Articles
-
-
-

### Online Resources
-
-
-

### Code / Libraries
-
-

---

## 9. Final Reflection & Outcomes

### Summary of Work Completed
*(Write 2–4 paragraphs summarizing the full arc of the study — what you set out to do, what you built or investigated, and what you discovered.)*


### Key Takeaways
1. *(Most important thing learned)*
2.
3.

### Future Work
*(What would you explore next, given more time?)*
