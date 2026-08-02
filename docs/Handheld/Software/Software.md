# BMX Handheld - Software

**Project:** BMX Handheld  
**Hardware:** CYD ESP32-2432S028  
**Revision:** V1.0  
**Status:** Definitief

---

# Doel

Dit document beschrijft de softwarearchitectuur van de BMX Handheld.

Het document definieert de structuur van de firmware, de verantwoordelijkheden van iedere softwaremodule en de onderlinge communicatie.

Elektrische aansluitingen zijn vastgelegd in **Pinout.md**.

Hardware-opbouw is beschreven in **Hardware.md**.

---

# Ontwerpdoelen

De firmware is ontworpen volgens de volgende uitgangspunten:

- Modulair opgebouwd
- Onderhoudbaar
- Uitbreidbaar
- Event-driven
- Niet-blokkerend
- Hardware-onafhankelijk waar mogelijk

---

# Software Architectuur

```
                 App
                  │
 ┌────────────────┼─────────────────┐
 │                │                 │
 │                │                 │
Display      FrontPanel          Radio
 │                │                 │
 │                │                 │
GUI          TCA8418 Driver     ESP-NOW
 │
 │
Settings
 │
PowerManager
 │
Storage
```

Iedere module heeft één duidelijke taak.

Modules communiceren uitsluitend via de App en het Event-systeem.

---

# Hoofdmodules

## App

De App vormt de centrale besturing van de firmware.

Verantwoordelijkheden:

- Initialisatie
- Hoofdloop
- Eventverwerking
- Modulestart
- Statusbewaking

De App bevat geen hardware-afhankelijke code.

---

## Display

Verzorgt alle communicatie met het TFT-display.

Taken:

- Initialiseren display
- Pagina's tekenen
- Statusbalk
- Popups
- Refresh management

Display bevat geen logica.

---

## GUI

De GUI verzorgt de gebruikersinterface.

Taken:

- Schermnavigatie
- Menustructuur
- Dialogen
- Meldingen
- Gebruikersinvoer verwerken

---

## FrontPanel

Interface tussen firmware en bedieningspaneel.

Verantwoordelijkheden:

- Toetsenmatrix uitlezen
- 2ndF verwerken
- ON/OFF knop
- Batterijstatus
- TCA8418 communicatie

Alle TCA8418-afhandeling bevindt zich uitsluitend binnen deze module.

---

## Radio

Verzorgt de draadloze communicatie.

Taken:

- ESP-NOW
- Pairing
- Heartbeat
- Status
- Ontvangen berichten
- Verzenden berichten

De GUI communiceert nooit rechtstreeks met ESP-NOW.

---

## Audio

Verantwoordelijkheden:

- Afspelen van systeemgeluiden
- Toetsfeedback
- Waarschuwingen
- Bevestigingstonen
- Opstartgeluid (optioneel)

De Audio-module biedt een eenvoudige interface voor het afspelen van vooraf gedefinieerde geluiden. Andere modules genereren geen audio rechtstreeks.

---

## Settings

Beheer van alle instellingen.

Voorbeelden:

- Display helderheid
- RF instellingen
- Systeemconfiguratie
- Gebruikersinstellingen

---

## PowerManager

Beheer van energieverbruik.

Taken:

- Batterijstatus
- Sleep
- Wake-up
- Automatisch uitschakelen
- Accubewaking

---

## Storage

Opslag van permanente gegevens.

Voorbeelden:

- Instellingen
- Pairinggegevens
- Kalibratie
- Versienummer

Opslag vindt plaats in de interne ESP32 NVS.

MicroSD wordt binnen Firmware V1.x niet gebruikt.

---

# Drivers

Drivers vormen de hardwarelaag.

```
Application

↓

Module

↓

Driver

↓

Hardware
```

Drivers bevatten uitsluitend hardwarecode.

---

## Display Driver

Communicatie met TFT.

---

## TCA8418 Driver

Communicatie via I²C.

Taken:

- Registers lezen
- Registers schrijven
- Interrupt afhandeling

Geen applicatielogica.

---

## ESP-NOW Driver

Verzorgt de lage communicatie.

Geen GUI-functionaliteit.

---

# Event Systeem

De firmware is event-driven.

Voorbeelden:

```
KEY_PRESSED

KEY_RELEASED

RADIO_RX

RADIO_TX

BATTERY_CHANGED

SCREEN_CHANGED

TIMEOUT

START_REQUEST

STOP_REQUEST
```

Modules communiceren uitsluitend via events.

---

# Initialisatie

Opstartvolgorde:

```
main()

↓

App.begin()

↓

Display.begin()

↓

FrontPanel.begin()

↓

Radio.begin()

↓

Settings.load()

↓

PowerManager.begin()

↓

GUI.begin()

↓

System Ready
```

---

# Hoofdloop

```
loop()

↓

App.run()

↓

FrontPanel.update()

↓

Radio.update()

↓

GUI.update()

↓

Display.update()

↓

PowerManager.update()
```

Alle modules draaien niet-blokkerend.

---

# Software Ontwerpregels

## Geen delay()

De firmware gebruikt geen blokkerende delays.

Timing gebeurt met millis().

---

## Eén verantwoordelijkheid

Iedere module heeft één taak.

---

## Hardware-afhankelijkheid

Hardware wordt uitsluitend benaderd via drivers.

---

## Geen globale variabelen

Globale variabelen worden zoveel mogelijk vermeden.

---

## Constantes

Hardwaregegevens worden opgeslagen in:

```
Pins.h
Config.h
Version.h
```

---

# Bestandsstructuur

```
src/
    main.cpp
    App.cpp
    Display.cpp
    GUI.cpp
    FrontPanel.cpp
    Radio.cpp
    PowerManager.cpp
    Settings.cpp
    Storage.cpp

include/
    App.h
    Display.h
    GUI.h
    FrontPanel.h
    Radio.h
    PowerManager.h
    Settings.h
    Storage.h
    Pins.h
    Config.h
    Version.h

lib/
    Drivers/
        TCA8418
        ESPNow
        Display
```

---

# Afhankelijkheden

```
App
├── Display
├── GUI
├── FrontPanel
├── Radio
├── Settings
├── PowerManager
└── Storage
```

Modules communiceren niet rechtstreeks met elkaar.

Alle communicatie verloopt via de App.

---

# Foutafhandeling

Fouten mogen de firmware niet blokkeren.

Mogelijke foutmeldingen:

- Radio offline
- TCA8418 niet gevonden
- Display fout
- Lage batterij
- Configuratiefout

Alle fouten worden via de GUI aan de gebruiker gemeld.

---

# Debug

Debug-uitvoer verloopt via USB Serial.

Debugcode wordt compile-time in- of uitgeschakeld.

---

# Firmware Update

Firmware wordt geladen via USB.

OTA is gereserveerd voor toekomstige uitbreidingen.

---

# Toekomstige uitbreidingen

Ondersteuning is voorzien voor:

- OTA updates
- MicroSD
- Logging
- Extra ESP-NOW nodes
- Uitgebreide diagnostiek

---

# Gerelateerde documentatie

- Hardware.md
- Pinout.md
- GUI.md
- Protocol.md

---

# Wijzigingshistorie

| Versie | Omschrijving |
|---------|--------------|
| 1.0 | Eerste definitieve softwarearchitectuur |