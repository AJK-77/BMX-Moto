# Node Configuration

## Doel

Alle BMX-Moto nodes gebruiken dezelfde firmware/codebase.

De node-specifieke configuratie bepaalt welke rol de betreffende ESP32 heeft en welk node-adres deze gebruikt.

## Opslag

De nodeconfiguratie wordt opgeslagen in de **ESP32 NVS (Non-Volatile Storage)**.

De configuratie blijft daardoor behouden wanneer de firmware via OTA wordt bijgewerkt.

## Configuratie via USB

Een node kan via USB worden geconfigureerd.

Via de USB-configuratie kunnen onder andere worden ingesteld:

- Node type
- Node address

De configuratie wordt vervolgens opgeslagen in NVS.

De nodes worden normaal niet via USB gevoed. USB wordt daarom uitsluitend gebruikt als configuratie-/serviceverbinding.

## Service interface

Via USB zijn de volgende functies beschikbaar:

- Debug en logging
- Firmware update
- NodeConfig uitlezen
- NodeConfig wijzigen

De NodeConfig kan alleen via USB worden gewijzigd.

### OTA

OTA is bedoeld voor firmware-updates.

Een OTA-update mag de bestaande NodeConfig in NVS niet wijzigen.

Debug en logging kunnen technisch ook via de netwerkverbinding beschikbaar worden gemaakt, maar dit is geen vereiste voor de OTA-functionaliteit.

In de normale praktijk kan de Handheld via USB met een laptop worden verbonden. Hierdoor zijn debug/logging en configuratie beschikbaar en kan vanuit de Handheld een firmware-update naar de overige nodes worden uitgevoerd.

## Normale werking

Bij het opstarten:

1. De firmware leest de configuratie uit NVS.
2. De node bepaalt hiermee zijn type en adres.
3. De applicatie initialiseert de voor deze node relevante onderdelen.

Tijdens normale werking wordt de nodeconfiguratie niet gewijzigd.

## OTA

Een OTA-update vervangt de firmware.

De bestaande NVS-configuratie blijft behouden.

Daarmee blijft bijvoorbeeld het ingestelde node-adres behouden na een firmware-update.

## Architectuur

Node configuration is onderdeel van `Common`.

De overige software hoeft niet te weten hoe of waar de configuratie is opgeslagen.

Conceptueel:

```text
             NodeConfig
                 │
                 ▼
                NVS
                 │
        ┌────────┼────────┐
        ▼        ▼        ▼
    Handheld   GateNode  Display