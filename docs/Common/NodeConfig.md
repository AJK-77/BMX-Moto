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