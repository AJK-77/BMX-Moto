# BMX Handheld - Pinout

**Project:** BMX Handheld  
**Hardware:** CYD ESP32-2432S028  
**Revision:** V1.0  
**Status:** Frozen

---

# Hardware overzicht

| Component | Status |
|-----------|--------|
| ESP32-2432S028 | Definitief |
| TCA8418 Front Panel Controller | Definitief |
| IP5306 Power Manager | Definitief |
| Toetsenmatrix | Definitief |
| Batterij LED's | Definitief |
| Touchscreen | Hardware aanwezig, niet gebruikt |
| MicroSD | Hardware aanwezig, gereserveerd |

---

# ESP32 GPIO

| GPIO   | Originele CYD functie  | BMX Functie      | Status       | Opmerking                    |
|-------:|------------------------|------------------|--------------|------------------------------|
| GPIO0  | BOOT knop              | BOOT             | Vast         | Bootknop + Auto Program      |
| GPIO1  | UART TX                | Debug            | Vast         | USB (CH340)                  |
| GPIO2  | TFT RS/DC              | Display          | Vast         |                              |
| GPIO3  | UART RX                | Debug            | Vast         | USB (CH340)                  |
| GPIO4  | RGB LED                | Niet gebruikt    | Vast         | Originele CYD RGB LED        |
| GPIO5  | MicroSD CS             | MicroSD          | Gereserveerd |                              |
| GPIO12 | TFT SDO (MISO)         | Display          | Vast         |                              |
| GPIO13 | TFT SDI (MOSI)         | Display          | Vast         |                              |
| GPIO14 | TFT SCK                | Display          | Vast         |                              |
| GPIO15 | TFT CS                 | Display          | Vast         |                              |
| GPIO16 | RGB LED                | Niet gebruikt    | Vast         | Originele CYD RGB LED        |
| GPIO17 | RGB LED                | Niet gebruikt    | Vast         | Originele CYD RGB LED        |
| GPIO18 | MicroSD CLK            | MicroSD          | Gereserveerd |                              |
| GPIO19 | MicroSD MISO           | MicroSD          | Gereserveerd |                              |
| GPIO21 | TFT Backlight          | TFT Backlight    | Vast         | PWM via MOSFET Q2            |
| GPIO22 | CN1                    | I²C SCL          | Definitief   | TCA8418                      |
| GPIO23 | MicroSD MOSI           | MicroSD          | Gereserveerd |                              |
| GPIO25 | Touch CLK              | Niet gebruikt    | Vast         | Touch aanwezig               |
| GPIO26 | Audio                  | Speaker          | Definitief   | Via versterker SC8002B (P4)  |
| GPIO27 | CN1                    | I²C SDA          | Definitief   | TCA8418                      |
| GPIO32 | Touch DIN              | Niet gebruikt    | Vast         | Touch aanwezig               |
| GPIO33 | Touch CS               | Niet gebruikt    | Vast         | Touch aanwezig               |
| GPIO34 | LightSensor            | AmbientLightSens | Vast         |Automatische helderheidsmeting|
| GPIO35 | P3                     | TCA8418 INT      | Definitief   | Active LOW                   |
| GPIO36 | Touch IRQ              | Niet gebruikt    | Vast         | Touch aanwezig               |
| GPIO39 | Touch DOUT             | Niet gebruikt    | Vast         | Touch aanwezig               |



---

# I²C

| Signaal | GPIO   |
|---------|--------|
| SDA     | GPIO27 |
| SCL     | GPIO22 |

---

# TCA8418 Front Panel Controller

## Algemeen

| Eigenschap     | Waarde     |
|----------------|------------|
| I²C adres      | 0x34       |
| Interrupt      | GPIO35     |
| Interrupt type | Active LOW |
| Pull-up        | Ja         |

---

## Keyboard Matrix

Rows    : R0 - R6
Columns : C0 - C3

Effectieve matrix : 7 × 4 (28 toetsen)


| Toets | Rij | Kolom | Status |
|:------|:---:|:-----:|--------|
| 0     | 3   | 1     | Definitief   |
| 1     | 2   | 2     | Definitief   |
| 2     | 2   | 1     | Definitief   |
| 3     | 2   | 0     | Definitief   |
| 4     | 0   | 2     | Definitief   |
| 5     | 0   | 1     | Definitief   |
| 6     | 0   | 0     | Definitief   |
| 7     | 1   | 2     | Definitief   |
| 8     | 1   | 1     | Definitief   |
| 9     | 1   | 0     | Definitief   |
| +     | 3   | 2     | Definitief   |
| -     | 3   | 0     | Definitief   |
| ESC   | 4   | 2     | Definitief   |
| AUX   | 4   | 0     | Definitief   |
| Down  | 4   | 1     | Definitief   |
| Right | 5   | 0     | Definitief   |
| OK    | 5   | 1     | Definitief   |
| Left  | 5   | 2     | Definitief   |
| SKR   | 6   | 0     | Definitief   |
| Up    | 6   | 1     | Definitief   |
| SKL   | 6   | 2     | Definitief   |
| 2ndF  | 7   | 3     | Definitief   |

| Connector Pin | Functie       |
| :-----------: | ------------- |
|       1       | R0            |
|       2       | R1            |
|       3       | R2            |
|       4       | R3            |
|       5       | R4            |
|       6       | ON/OFF GND    |
|       7       | R5            |
|       8       | R6            |
|       9       | C0            |
|       10      | C1            |
|       11      | ON/OFF C5/Key |
|       12      | C2            |
|       13      | 2ndF          |
|       14      | 2ndF          |

>Opmerking: De 2ndF-toets is hardwarematig geen onderdeel van de toetsenmatrix.
>De toets is als afzonderlijke schakelaar uitgevoerd zodat deze gelijktijdig met een matrixtoets gebruikt kan worden zonder de matrixscan te beïnvloeden.
>Binnen de firmware wordt 2ndF behandeld als modifier-toets
> R7 en C4-C9 zijn beschikbaar als General Purpose I/O.
> De ON/OFF knop is aangesloten tussen C5 (KEY) en GND.
> En zit op pin 6 en 11 van de keyboad connector.

---

## Front Panel I/O

| TCA Pin | Functie          | Status |
|---------|------------------|--------|
| C4      | Reserve          | Vrij |
| C5      | IP5306 KEY       | Definitief |
| C6      | Battery LED 25%  | Definitief |
| C7      | Battery LED 50%  | Definitief |
| C8      | Battery LED 75%  | Definitief |
| C9      | Battery LED 100% | Definitief |
| R7      | Reserve          | Vrij       |

---

# Power

## IP5306

| Signaal | Verbonden met |
|---------|---------------|
| KEY     | TCA8418 C5    |
| OUT5V   | CYD 5V        |
| BAT     | BAT. +        |
| VIN     | CSB VCC       |
| GND     | 3x GND        |
---

# Touchscreen

Hardware aanwezig op de CYD.

Niet gebruikt binnen firmware V1.x.

---

# MicroSD

Hardware aanwezig.

Gereserveerd voor toekomstige uitbreidingen.

---

# Ontwerpregels

- Alle ESP32 GPIO's zijn toegewezen.
- Nieuwe uitbreidingen verlopen via de TCA8418 of een extra I²C-device.
- De TCA8418 fungeert als Front Panel Controller.
- GPIO21 blijft de originele CYD backlight-aansturing.
- Firmware gebruikt uitsluitend logische functies; TCA-pinnummers worden afgeschermd binnen de FrontPanel-driver.

---

# Audio

| Signaal | GPIO   | Connector |   Status   |
|---------|--------|-----------|------------|
| Speaker | GPIO26 |    P4     | Definitief |

---

# Wijzigingshistorie

| Versie  | Omschrijving              |
|---------|---------------------------|
| 1.0     | Eerste definitieve pinout |