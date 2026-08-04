# RF Protocol

## Doel

Dit document beschrijft de algemene opbouw van het RF-protocol dat wordt gebruikt voor de communicatie tussen alle nodes binnen het BMX-Moto systeem.

De gebruikte radiotechniek (ESP-NOW) staat los van dit protocol.

---

## Protocolopbouw

Alle berichten binnen het systeem gebruiken dezelfde pakketstructuur.

Hierdoor kunnen alle node-types dezelfde protocolparser gebruiken.

---

## Berichtopbouw

Een RF-bericht bestaat uit:

- Header
- Payload
- CRC

---

## Header

De header bevat informatie die nodig is voor de afhandeling van het bericht.

De header bevat onder andere:

- Protocolversie
- Bronadres
- Doeladres
- Berichttype
- Berichtlengte

---

## Payload

De payload bevat de daadwerkelijke gegevens van het bericht.

De inhoud is afhankelijk van het berichttype.

Voorbeelden:

- Heartbeat
- Event
- StatusReport

---

## CRC

Ieder bericht wordt afgesloten met een CRC.

De CRC wordt gebruikt om transmissiefouten te detecteren.

De exacte implementatie wordt beschreven in **CRC.md**.

---

## Berichttypes

Het systeem gebruikt verschillende berichttypes.

De exacte inhoud van ieder berichttype wordt beschreven in de afzonderlijke protocoldocumenten.

Voorbeelden:

- Heartbeat
- Event
- StatusReport

---

## Protocolversie

Iedere header bevat een protocolversie.

Hierdoor kunnen toekomstige protocolwijzigingen worden ondersteund zonder bestaande software direct onbruikbaar te maken.

---

## Ontwerpfilosofie

Het protocol is ontworpen voor:

- eenvoudige implementatie
- hoge betrouwbaarheid
- lage overhead
- eenvoudige uitbreidbaarheid