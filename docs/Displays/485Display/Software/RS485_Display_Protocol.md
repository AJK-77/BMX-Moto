# RS485 Display Protocol

## 4-Digit RS485 Display

> **Status:** Reverse engineering / working protocol notes  
> **Project:** BMX Manche / Moto Counter  
> **Display:** 4-digit RS485 display  


---

## 1. Hardware

The display LED PCB uses **MAX7219** driver ICs.

The physical display has **4 digit positions**, although one of the positions is not populated with LEDs. Earlier work incorrectly treated this display as a 3-digit display; the later captures confirm the 4-position protocol/layout.

The original controller also has a **light sensor**. The original system automatically controls display brightness.

---

## 2. RS485

Confirmed:

- RS485 communication
- **9600 baud**
- A/B polarity must be correct
- A Nano + MAX485 was used as a passive sniffer
- With the sniffer connected correctly, the original controller traffic could be captured reliably

The sniffer was connected with the MAX485 receiver output (`RO`) to Nano RX.

---

## 3. Normal display-data frame

The confirmed frame used for the actual displayed value is:

```text
04 04 81 02 3D [MANCH] 20 [TENS] [ONES] 03 [CHECKSUM] 06 06
```

### Fields

| Field | Meaning |
|---|---|
| `04 04 81 02 3D` | Fixed header/function bytes |
| `[MANCH]` | First displayed value |
| `20` | Blank/separator position |
| `[TENS]` | Tens digit of second value, ASCII |
| `[ONES]` | Units digit of second value, ASCII |
| `03 06 06` | Frame termination |
| `[CHECKSUM]` | Check byte |

---

## 4. First value / manche

The first value is encoded as:

```text
0 → B0
1 → B1
2 → B2
3 → B3
4 → B4
5 → B5
6 → B6
7 → B7
8 → B8
9 → B9
```

In other words:

```cpp
mancheByte = 0xB0 + manche;
```

This was confirmed with captures including:

```text
100 → B1
200 → B2
```

---

## 5. Second value / moto

The two right-hand digits are transmitted as normal ASCII characters.

Examples:

```text
000 → B0 20 30 30
001 → B0 20 30 31
002 → B0 20 30 32
003 → B0 20 30 33
...
009 → B0 20 30 39

010 → B0 20 31 30
020 → B0 20 32 30
030 → B0 20 33 30
...
090 → B0 20 39 30

100 → B1 20 30 30
200 → B2 20 30 30
```

Therefore:

```cpp
tens = '0' + (moto / 10);
ones = '0' + (moto % 10);
```

For the tested normal numeric range this mapping is confirmed.

---

## 6. Checksum

For normal numeric values the checksum is:

```cpp
checksum = ones ^ 0x1D;
```

Examples:

```text
00:
0x30 ^ 0x1D = 0x2D

01:
0x31 ^ 0x1D = 0x2C

02:
0x32 ^ 0x1D = 0x2F

03:
0x33 ^ 0x1D = 0x2E

09:
0x39 ^ 0x1D = 0x24
```

Important:

**This checksum rule is confirmed for normal numeric digits.**

The blank case has a different observed checksum and should be handled separately rather than assuming the same formula.

---

## 7. Blank digit

A blank digit is:

```text
0x20
```

Blank means the digit is **completely OFF**.

It is not:

- `_`
- `-`

Example notation used during testing:

```text
0B0
```

means:

```text
0 / blank / 0
```

Captured frame:

```text
04 04 81 02 3D B0 20 20 30 03 3D 06 06
```

The `0x20` byte therefore represents a completely blank digit position.

---

## 8. Known working frames

These frames were captured from the original controller and/or reproduced successfully on the test display.

### 0 00

```text
04 04 81 02 3D B0 20 30 30 03 2D 06 06
```

### 0 01

```text
04 04 81 02 3D B0 20 30 31 03 2C 06 06
```

### 3 12

```text
04 04 81 02 3D B3 20 31 32 03 2D 06 06
```

### 4 12

```text
04 04 81 02 3D B4 20 31 32 03 2A 06 06
```

### 1 00

```text
04 04 81 02 3D B1 20 30 30 03 2C 06 06
```

### 2 00

```text
04 04 81 02 3D B2 20 30 30 03 2F 06 06
```

---

## 9. 30 02 frame / brightness

The original controller also sends a separate frame:

```text
04 04 30 02 4C XX 94 XX 94 20 03 5D 06 06
```

This frame was observed repeatedly while the normal display-value frame remained unchanged.

A controlled test was performed with the display showing `000` and the light sensor covered.

During the test the following pattern was observed:

```text
4C 94 94 94 94
4C 95 94 95 94
4C 96 94 96 94
4C 97 94 97 94
...
4C A0 94 A0 94
...
4C B0 94 B0 94
...
4C BC 94 BC 94
```

At the same time the physical display brightness changed step by step.

The normal display frame stayed constant:

```text
04 04 81 02 3D B0 20 30 30 03 2D 06 06
```

### Current conclusion

The `30 02` frame is very likely related to:

- brightness
- display configuration
- or periodic display housekeeping

The exact meaning of the bytes has **not** been fully decoded.

The original controller has the light sensor and supports up to eight displays, so the current working hypothesis is that the central controller determines/distributes brightness information.

This is a **working hypothesis**, not a fully decoded part of the protocol.

For the first BMX implementation, brightness can be treated separately from the confirmed display-value protocol.

---

## 10. Other observed frames

The original controller also sends additional frames, for example:

```text
04 04 81 02 3F 41 FF 98 20 03 B9
```

and:

```text
04 04 81 02 44 3F 83 ... 03 00
```

These frames were repeatedly observed but their exact function has not yet been decoded.

They should **not** be confused with the confirmed display-value frame:

```text
04 04 81 02 3D ...
```

---

## 11. Display update / keepalive

When the ESP32 directly drove the display, it was observed that after approximately **45 seconds** without the expected regular communication, the display changed to four blank positions:

```text
_ _ _ _
```

The exact visual representation is not a protocol value; the important observation is that the display stops showing the normal data.

For the BMX implementation a practical rule is therefore:

> Send a regular display update at least every 30 seconds.

The proposed implementation is to send the display update on **every second heartbeat**.

---

## 12. Implementation basis

The confirmed normal display frame can be generated dynamically.

Conceptually:

```cpp
uint8_t mancheByte = 0xB0 + manche;

uint8_t tens = '0' + (moto / 10);
uint8_t ones = '0' + (moto % 10);

uint8_t checksum = ones ^ 0x1D;
```

Then:

```cpp
uint8_t frame[] =
{
    0x04,
    0x04,
    0x81,
    0x02,
    0x3D,
    mancheByte,
    0x20,
    tens,
    ones,
    0x03,
    checksum,
    0x06,
    0x06
};
```

This applies to the confirmed normal numeric range.

Blank values need separate handling because the checksum observed for the blank case does not follow the normal numeric rule.

---

## 13. Reverse-engineering status

### Confirmed

- RS485 communication
- 9600 baud
- 4 physical digit positions
- One physical digit position is unpopulated
- Normal display-value frame
- `B0..B9` first-value mapping
- Two right-hand values are ASCII digits
- `0x20` means a completely blank digit
- Numeric checksum `ONES ^ 0x1D`
- Original controller supports up to eight displays
- Original controller has a light sensor
- A separate `30 02` frame exists
- `30 02` traffic changes during the observed brightness ramp
- Display requires periodic communication

### Strong working hypothesis

- `30 02` is related to brightness/configuration/housekeeping
- The central controller is involved in brightness control for the displays

### Not yet decoded

- Exact meaning of the `30 02` fields
- Exact meaning of the other `81 02` frames
- Exact checksum rule for blank values
- Exact MAX7219 digit-register mapping of the physically unpopulated digit

---

## 14. Important distinction

This document describes the **4-digit RS485 display currently being integrated into the BMX Manche / Moto Counter**.

It is **not the GanXin display protocol**.

The GanXin display is a separate hardware/protocol investigation and will be documented separately.
