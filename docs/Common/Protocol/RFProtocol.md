# RF Protocol

## Doel

Dit document beschrijft de algemene opbouw van het RF-protocol dat wordt gebruikt voor de communicatie tussen alle nodes binnen het BMX-Moto systeem.

De gebruikte radiotechniek (ESP-NOW) staat los van dit protocol.

Alle node-types gebruiken dezelfde protocoltaal. De concrete functionaliteit van events en status is beschreven in `Events.md` en `StatusReport.md`.

---

## Protocolopbouw

Alle berichten binnen het systeem gebruiken dezelfde pakketstructuur.

Hierdoor kunnen alle node-types dezelfde protocolparser gebruiken.

Een RF-bericht bestaat uit:

```text
┌──────────────┬──────────────┬──────────┐
│    HEADER    │   PAYLOAD    │   CRC    │
└──────────────┴──────────────┴──────────┘
```

---

## Header

De header bevat informatie die nodig is voor de afhandeling van het bericht.

De header bevat minimaal:

- Protocolversie
- Bronadres
- Doeladres
- Berichttype
- Berichtlengte

De exacte byte-indeling en veldgroottes worden bij de technische implementatie vastgesteld.

---

## Payload

De payload bevat de daadwerkelijke gegevens van het bericht.

De inhoud is afhankelijk van het berichttype.

Voorbeelden:

- Heartbeat
- Event
- StatusReport
- StatusRequest
- StatusResponse

De payload bevat alleen gegevens die bij het betreffende berichttype horen.

---

## Berichttypes

Het protocol kent minimaal de volgende berichttypes:

### HEARTBEAT

Wordt gebruikt om de aanwezigheid en bereikbaarheid van een node kenbaar te maken.

Heartbeat verandert de race-status niet en verhoogt daarom de `eventSequence` niet.

De heartbeat is onderdeel van de node-bewaking en failover-logica.

---

### EVENT

Wordt gebruikt om een gebeurtenis door te geven.

Voorbeelden:

- `SESSION_RESET`
- `MANUAL_UPDATE`
- `GATE_DROP`
- `MASTER_CHANGED`
- `NODE_OFFLINE`
- `NODE_ONLINE`

Een event dat de officiële race-status verandert, leidt bij verwerking tot een nieuwe `eventSequence` en vervolgens tot een volledige `StatusReport`.

Een event heeft een unieke event-ID binnen de sessie.

Een ontvanger verwerkt hetzelfde event maximaal één keer.

Voorbeeld:

```text
GATE_DROP #0042
     ↓
HH verwerkt
     ↓
moto 045 → 046
```

Als hetzelfde event door een retry opnieuw wordt ontvangen:

```text
GATE_DROP #0042 opnieuw
     ↓
HH: al verwerkt
     ↓
geen tweede moto++
```

De technische vorm van de event-ID wordt later definitief vastgesteld.

---

### STATUS_REPORT

Bevat de volledige actuele officiële race-status.

Een `StatusReport` bevat minimaal:

```text
sessionId
eventSequence
manche
moto
mode
masterId
```

Er worden geen losse status-delta's naar displays gestuurd.

Een node die een `StatusReport` ontvangt, gebruikt deze om zijn lokale toestand met de officiële Master-status te synchroniseren.

---

### STATUS_REQUEST

Wordt gebruikt wanneer een node de actuele officiële status wil opvragen.

Een `STATUS_REQUEST` verandert de race-status niet en verhoogt daarom de `eventSequence` niet.

---

### STATUS_RESPONSE

Antwoord op een `STATUS_REQUEST`.

Bevat een volledige `StatusReport`.

Een `STATUS_RESPONSE` verandert de race-status niet en verhoogt daarom de `eventSequence` niet.

---

## Master en backup-master

De HH is normaal de Master van het systeem.

De GateNode is de backup-master.

```text
Normaal:

HH = Master
GateNode = backup-master
Displays = volgers
```

De HH is verantwoordelijk voor de officiële race-status en verwerkt normaal de `GATE_DROP`-events.

Wanneer de HH niet meer bereikbaar is, kan de GateNode de Master-rol overnemen.

```text
HH offline
    ↓
GateNode → Master
```

In deze situatie mag de GateNode zelf een `GATE_DROP` verwerken en de Moto met `+1` verhogen.

De precieze failover- en synchronisatieregels worden verder uitgewerkt in het protocol.

---

## GATE_DROP en MANUAL mode

De GateNode meldt normaal uitsluitend dat een fysieke gate drop heeft plaatsgevonden.

```text
GateNode → HH
          GATE_DROP
```

De HH verwerkt een `GATE_DROP` voor de race-status alleen wanneer de HH in `AUTO`-modus staat.

In `MANUAL`-modus mag de GateNode de gate drop nog steeds melden, maar de HH gebruikt deze melding niet om de officiële Moto automatisch te verhogen.

Wanneer de HH offline is en de GateNode Master is geworden, verwerkt de GateNode de gate drop wel zelf.

---

## ACK en retry

Betrouwbare aflevering van belangrijke berichten moet mogelijk zijn zonder dat hetzelfde event meerdere keren wordt uitgevoerd.

Daarom kunnen berichten waarvoor bevestiging nodig is opnieuw worden verzonden.

Een retry mag nooit leiden tot dubbele verwerking van een event.

De ontvanger gebruikt hiervoor de unieke event-ID.

De exacte ACK-indeling, retry-timing en het aantal retries worden nog vastgesteld.

---

## Event sequence

De `eventSequence` is de teller van officiële statuswijzigingen binnen een sessie.

Belangrijk:

- `eventSequence` is niet hetzelfde als de Moto.
- `eventSequence = 0000` is een `SESSION_RESET`.
- Na een reset begint iedereen opnieuw.
- De teller loopt maximaal tot `9999`.

Een voorbeeld:

```text
SESSION_RESET  → eventSequence 0000
MANUAL_UPDATE  → eventSequence 0001
GATE_DROP      → eventSequence 0002
MANUAL_UPDATE  → eventSequence 0003
GATE_DROP      → eventSequence 0004
```

De Moto kan ondertussen vrij worden gewijzigd.

Bijvoorbeeld:

```text
moto 045
   ↓ MANUAL_UPDATE
moto 047
   ↓ GATE_DROP
moto 048
   ↓ MANUAL_UPDATE
moto 045
```

De `eventSequence` blijft gewoon oplopen.

---

## Sessies

Een `sessionId` maakt onderscheid tussen verschillende sessies/racedagen.

Na `SESSION_RESET` wordt de oude sessiestatus ongeldig.

Een bericht uit een vorige sessie mag daarom niet als een nieuw geldig event voor de huidige sessie worden verwerkt.

De exacte grootte en codering van `sessionId` worden later vastgesteld.

---

## CRC

Ieder bericht wordt afgesloten met een CRC.

De CRC wordt gebruikt om transmissiefouten te detecteren.

Een bericht met een ongeldige CRC wordt niet verwerkt.

De exacte implementatie wordt beschreven in **CRC.md**.

---

## Protocolversie

Iedere header bevat een protocolversie.

Hierdoor kunnen toekomstige protocolwijzigingen worden ondersteund zonder bestaande software direct onbruikbaar te maken.

Een node die een niet-ondersteunde protocolversie ontvangt, mag het bericht niet verwerken.

---

## Adressering

Iedere node heeft een eigen node-adres.

De header bevat:

```text
SourceAddress
DestinationAddress
```

Hiermee kan een bericht gericht aan één specifieke node worden verzonden.

Broadcast-communicatie kan later worden toegevoegd wanneer dit nodig blijkt.

---

## Ontwerpfilosofie

Het protocol is ontworpen voor:

- eenvoudige implementatie
- hoge betrouwbaarheid
- lage overhead
- eenvoudige uitbreidbaarheid
- dezelfde protocoltaal voor alle node-types
- veilige herhaling van berichten
- geen dubbele verwerking van events

De protocollaag blijft onafhankelijk van de gebruikte radiotechniek.

---

## Nog technisch vast te leggen

De volgende punten zijn functioneel bepaald, maar moeten nog naar concrete bytes/velden worden vertaald:

- exacte byte-indeling van de header;
- veldgroottes;
- MessageType-codes;
- event-ID-formaat;
- `sessionId`-formaat;
- `eventSequence`-formaat;
- ACK-formaat;
- retry-timing;
- aantal retries;
- CRC-type en positie;
- heartbeat-interval en timeout;
- exacte failover-detectie;
- synchronisatie wanneer de HH terugkomt als Master.

Deze details worden vastgelegd voordat de definitieve RF-protocolcode wordt geïmplementeerd.
