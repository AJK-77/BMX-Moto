# Adressering

## Doel

Dit document beschrijft de adressering van alle componenten binnen het BMX-Moto systeem.

De adressering wordt gebruikt om berichten aan een specifieke node of een groep nodes te versturen.

De fysieke communicatietechniek (ESP-NOW, RS485, enz.) staat hier los van.

---

## Node-ID

Iedere node binnen het systeem beschikt over een unieke Node-ID.

Deze Node-ID verandert tijdens normaal gebruik niet.

---

## Node-types

Binnen het systeem worden de volgende node-types onderscheiden:

- Handheld
- Gate-Node
- Display
- Reserve

---

## Adresbereik

Het beschikbare adresbereik is:

```
0 - 255
```

Niet alle adressen hoeven gebruikt te worden.

---

## Adres

Het systeem gebruikt een 8-bits adres.

Hierdoor zijn maximaal 254 individuele nodes mogelijk.

Dit is ruim voldoende voor de huidige en toekomstige omvang van het BMX-Moto systeem.

---

## Gereserveerde adressen

| Adres | Betekenis |
|-------:|-----------|
| 0 | Broadcast |
| 1-10 | Handheld |
| 11-30 | Gate-Nodes |
| 31-127 | Displays |
| 128-254 | Gereserveerd voor toekomstige uitbreidingen |
| 255 | Ongeldig / Niet toegewezen |

---

## Broadcast

Berichten met adres **0** worden door alle nodes verwerkt.

Broadcast wordt gebruikt voor berichten die voor het volledige systeem bedoeld zijn.

Bijvoorbeeld:

- Heartbeat
- Events
- StatusReport

---

## Unicast

Berichten kunnen ook aan één specifieke node worden gericht.

Bijvoorbeeld:

- Diagnostiek
- Configuratie
- Firmware-update

---

## Toekomstige uitbreidingen

Het adresbereik is bewust ruim gekozen zodat later eenvoudig nieuwe componenten kunnen worden toegevoegd.

Bijvoorbeeld:

- Extra displays
- Extra Gate-Nodes
- Service-tools
- Testapparatuur