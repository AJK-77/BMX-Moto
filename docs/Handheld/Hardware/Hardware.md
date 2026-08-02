# BMX Handheld - Hardware

**Project:** BMX Handheld  
**Hardware:** CYD ESP32-2432S028  
**Revision:** V1.0  
**Status:** Definitief

---

# Doel

De BMX Handheld is de centrale bedieningsunit van het BMX Moto-systeem.

De handheld wordt gebruikt voor:

- Weergeven van manche- en moto informatie
- Bedienen van displays
- Configureren van het systeem
- Communicatie met de Gate Node
- Communicatie met LED-displays
- Diagnose en onderhoud

Het ontwerp is modulair opgebouwd zodat toekomstige uitbreidingen mogelijk blijven zonder wijzigingen aan de basisarchitectuur.

---

# Ontwerpfilosofie

Bij het ontwerp van de BMX Handheld zijn de volgende uitgangspunten gehanteerd:

- De originele CYD hardware blijft zoveel mogelijk ongewijzigd.
- Alle uitbreidingen worden gerealiseerd op een eigen Front Panel PCB.
- De TCA8418 fungeert als Front Panel Controller.
- Nieuwe hardware-uitbreidingen verlopen bij voorkeur via I²C.
- De firmware wordt modulair opgebouwd.
- Hardware-afhankelijke functies worden afgeschermd door drivers.
- Het ontwerp blijft onderhoudsvriendelijk en uitbreidbaar.

---

# Hardware Configuratie

| Onderdeel              | Type                        |
|------------------------|-----------------------------|
| MCU                    | ESP32-WROOM-32              |
| Display                | 2.8" TFT (240×320)          |
| Touch                  | XPT2046 (niet gebruikt)     |
| Front Panel Controller | TCA8418                     |
| Power Management       | IP5306                      |
| Audio                  | SC8002B                     |
| USB-UART               | CH340C                      |
| Opslag                 | MicroSD (gereserveerd)      |
| Draadloos              | Wi-Fi / ESP-NOW / Bluetooth |
| Voeding                | LiPo + IP5306               |

---

# Hardware Architectuur

De handheld bestaat uit twee hoofdmodules:

1. CYD ESP32-module
2. Front Panel PCB

```
                 BMX Handheld

        +-------------------------+
        |        CYD Module        |
        |-------------------------|
        | ESP32                   |
        | TFT Display             |
        | USB-C                   |
        | RGB LED                 |
        | Touch Controller        |
        | MicroSD                 |
        +-----------+-------------+
                    |
                  I²C
                    |
        +-----------+-------------+
        |     Front Panel PCB     |
        |-------------------------|
        | TCA8418                 |
        | Keyboard Matrix         |
        | IP5306                 |
        | Battery LEDs            |
        | ON/OFF Key             |
        | LiPo Battery           |
        +-------------------------+
```

---

# Hoofdcomponenten

## ESP32-2432S028 (CYD)

De CYD vormt de hoofdprocessor van het systeem.

Verantwoordelijkheden:

- Firmware
- Display
- ESP-NOW communicatie
- GUI
- Eventverwerking
- Power Management
- Configuratie

De originele CYD hardware blijft zoveel mogelijk ongewijzigd.

---

## TFT Display

2.8" TFT kleurenbeeldscherm.

Functies:

- Hoofdscherm
- Menustructuur
- Diagnostiek
- Instellingen
- Batterijstatus
- Verbindingsstatus

De display-backlight wordt aangestuurd via een MOSFET op GPIO21.

Fysieke resolutie:
320 × 240 pixels

Door de gekozen behuizing zijn de bovenste en onderste
20 pixels niet zichtbaar.

Hierdoor blijft een bruikbaar werkgebied over van:

320 × 200 pixels.

## Opmerking

Alle GUI-ontwerpen zijn gebaseerd op een bruikbaar werkgebied van
320 × 200 pixels.

---

## Touchscreen

De CYD bevat een resistief touchscreen.

Binnen Hardware Revision V1.x wordt het touchscreen niet gebruikt.

De hardware blijft volledig aanwezig voor eventuele toekomstige toepassingen.

---

## MicroSD

De CYD beschikt over een MicroSD-slot.

Binnen Hardware Revision V1.x wordt deze niet gebruikt.

De interface blijft beschikbaar voor toekomstige uitbreidingen zoals:

- Logging
- Firmwarebestanden
- Configuratiebestanden

---

## TCA8418 Front Panel Controller

De TCA8418 vormt het complete frontpanel-interface.

Verantwoordelijkheden:

- Keyboard Matrix
- Modifier-toets
- Batterij LED's
- Power Button
- Toekomstige uitbreidingen

Alle frontpanel-I/O loopt via de TCA8418.

Hierdoor blijven ESP32 GPIO's volledig vrij voor systeemfuncties.

---

## Keyboard

Het toetsenbord bestaat uit:

- Numeriek gedeelte
- Navigatietoetsen
- Functietoetsen
- Modifier-toets (2ndF)

De toetsenmatrix gebruikt:

- 7 Rows
- 4 Columns

De 2ndF-toets is geen onderdeel van de matrix.

Hierdoor kunnen toetscombinaties worden gebruikt zonder de matrixscan te beïnvloeden.

---

## Power Management

De handheld wordt gevoed door een interne LiPo-accu.

Power Management bestaat uit:

- LiPo accu
- IP5306 laadcontroller
- USB-C laadfunctie
- Batterijstatus

De IP5306 verzorgt:

- Laden
- 5V boost
- Power-key functie

---

## Batterijstatus

De PI5306 laadcontroller beschikt over vier status-LED's die de accustatus weergeven.

De statussignalen worden door de Front Panel Controller ingelezen, zodat de firmware de actuele accustatus kan bepalen.

De uiteindelijke implementatie van de batterijstatus is afhankelijk van de mogelijkheden van de IP5306 en kan in toekomstige hardware-revisies worden aangepast.

---

## RGB Status LED

De originele RGB LED op de CYD blijft behouden.

Deze wordt niet gebruikt.


---

# Connectoren

De handheld bevat de volgende interne connectoren.

## Keyboard

14-polige connector.

Verbindt:

- Toetsenmatrix
- ON/OFF knop
- 2ndF toets

Pinout:
Zie Pinout.md

---

## Front Panel

Interne verbinding tussen:

- CYD
- Front Panel PCB
- Keypad

Verzorgt:

- I²C
- Voeding
- Interrupt

---

## USB-C

Wordt gebruikt voor:

- Programmeren
- Debuggen
- Accu laden

---

# Communicatie

## ESP-NOW

Primaire communicatie tussen:

- Handheld
- Gate Node
- Displays

---

## USB Serial

Wordt gebruikt voor:

- Firmware upload
- Debug logging

---

## I²C

Interne communicatie tussen:

- ESP32
- TCA8418

---

# Voeding

```
USB-C
   │
   ▼
IP5306
   │
LiPo
   │
5V
   │
CYD
   │
3.3V
   │
ESP32 + TCA8418
```

---

# Ontwerpkeuzes

## CYD

Gekozen vanwege:

- Compact ontwerp
- Lage kosten
- Complete ESP32-oplossing
- Ingebouwd display
- USB-C
- Goede beschikbaarheid

---

## TCA8418

De TCA8418 wordt gebruikt als volledige Front Panel Controller.

Hierdoor:

- Minder ESP32 GPIO's nodig
- Compact ontwerp
- Uitbreidbaar
- Eenvoudige softwarearchitectuur

---

## Touchscreen

Niet gebruikt.

Fysieke toetsen bieden een betrouwbaardere bediening tijdens BMX-wedstrijden.

---

## Modifier-toets

De 2ndF-toets bevindt zich buiten de toetsenmatrix.

Hierdoor blijven toetscombinaties betrouwbaar detecteerbaar.

---

## Uitbreidbaarheid

Nieuwe hardware-uitbreidingen verlopen uitsluitend via:

- Vrije TCA8418 I/O
- Extra I²C-devices

Nieuwe directe ESP32 GPIO-aansluitingen worden vermeden.

---

# Gerelateerde documentatie

- Pinout.md
- GUI.md
- Software.md
- Protocol.md

---

## Audio

De handheld beschikt over een interne luidspreker voor akoestische signalering.

Audio wordt gebruikt voor:

- Toetsfeedback
- Waarschuwingen
- Systeemmeldingen
- Bevestigingssignalen

De audiofunctionaliteit is bedoeld als aanvulling op de visuele gebruikersinterface en maakt bediening mogelijk zonder voortdurend naar het display te kijken.

---

## Ambient Light Sensor

De CYD beschikt over een geïntegreerde omgevingslichtsensor die is aangesloten op GPIO34.

Binnen Hardware Revision V1.x wordt deze sensor niet gebruikt.

De sensor kan in toekomstige firmwareversies worden gebruikt voor automatische aanpassing van de displayhelderheid.

---

## USB

De USB-C aansluiting wordt gebruikt voor:

- Voeding
- Firmware upload
- Debug logging

De USB-interface wordt verzorgd door een geïntegreerde CH340C USB-UART converter.

---

# Wijzigingshistorie

| Versie | Omschrijving |
|---------|--------------|
| 1.0 | Eerste definitieve hardwarebeschrijving |