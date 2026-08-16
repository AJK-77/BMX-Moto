# BMX-Moto — Events

## Doel

Dit document beschrijft de gebeurtenissen (events) die de race-status kunnen veranderen of de synchronisatie tussen nodes ondersteunen.

Een event beschrijft **wat er is gebeurd**. De actuele officiële toestand wordt vastgelegd in een volledig `StatusReport`.

De HH is normaal de Master (president). De GateNode is de backup-master (vice-president) en kan de Master-rol overnemen wanneer de HH niet meer bereikbaar is.

---

## Event sequence

Elke nieuwe officiële statuswijziging krijgt een oplopende `eventSequence`.

- `0000` is altijd een `SESSION_RESET`.
- Bij een reset wordt alle eerder bekende informatie als ongeldig beschouwd.
- Na `SESSION_RESET` begint iedereen opnieuw te tellen.
- De `eventSequence` staat los van Manche en Moto.
- De teller loopt gedurende de sessie op tot maximaal `9999`.

Voorbeeld:

```text
SESSION_RESET       → 0000
MANUAL_UPDATE       → 0001
GATE_DROP           → 0002
MANUAL_UPDATE       → 0003
GATE_DROP           → 0004
MANUAL_UPDATE       → 0005
```

De Moto is dus **niet** de event counter. Een Moto kan tijdens een wedstrijd worden overgeslagen, teruggezet of opnieuw gereden.

---

## Unieke event-ID en dubbele events

Een event heeft een unieke event-ID binnen de sessie.

De ontvanger verwerkt een event maximaal één keer.

Bijvoorbeeld:

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

De event-ID wordt gebruikt om dubbele verwerking te voorkomen. De technische uitvoering van event-ID, ACK en retry wordt vastgelegd in `RFProtocol.md`.

---

## Event types

### SESSION_RESET

Start een nieuwe sessie/racedag.

Effect:

- alle nodes vergeten de vorige sessiestatus;
- een nieuwe `sessionId` wordt actief;
- `eventSequence` wordt `0000`;
- de race-status wordt teruggezet naar de uitgangssituatie.

`SESSION_RESET` wordt door de HH uitgevoerd.

---

### MANUAL_UPDATE

Een handmatige wijziging van de officiële Manche/Moto-status door de Master.

Er zijn twee vormen.

#### Snelle correctie met +/-

De `+` en `-` bediening is bedoeld voor een snelle correctie van de actuele Moto.

Bijvoorbeeld:

```text
Display: 2055
Werkelijke Moto: 2054

-1
 ↓
Display: 2054
```

Een dergelijke correctie wordt **direct** bevestigd en vormt één `MANUAL_UPDATE` event.

Er is geen `OK` nodig.

#### Handmatige invoer

Bij het invoeren van een nieuwe Moto kan de DNS-manager rustig een waarde invoeren.

Tijdens het invoeren blijft de officiële status ongewijzigd.

Pas bij `OK` wordt de nieuwe waarde bevestigd:

```text
huidige status: 3 / 051

invoer: 3 / 045

OK
 ↓
MANUAL_UPDATE
 ↓
officiële status: 3 / 045
```

De displays mogen dus nooit een gedeeltelijk of nog niet bevestigde invoer tonen.

---

### GATE_DROP

Een fysieke gate drop.

Normaal meldt de GateNode alleen dat er daadwerkelijk een gate drop heeft plaatsgevonden:

```text
GateNode → HH
          GATE_DROP
```

De HH verwerkt een `GATE_DROP` alleen wanneer de HH in **AUTO**-modus staat.

In AUTO:

```text
GATE_DROP
    ↓
eventSequence++
    ↓
moto++
    ↓
nieuwe StatusReport
```

In MANUAL:

```text
GATE_DROP
    ↓
HH ontvangt event
    ↓
geen wijziging van de officiële Moto
```

De GateNode kan een fysieke gate drop blijven melden in MANUAL, maar de HH gebruikt die melding dan niet om de race-status te wijzigen.

#### GateNode als backup-master

Wanneer de HH niet bereikbaar is, kan de GateNode de Master-rol overnemen.

In die situatie verwerkt de GateNode zelf de `GATE_DROP` en verhoogt hij de Moto met `+1`.

De GateNode is daarmee de vice-president/backup-master van het systeem.

---

### MASTER_CHANGED

Geeft aan dat de Master-rol is gewijzigd.

Normale situatie:

```text
HH = Master
GateNode = backup-master
```

Als de HH wegvalt:

```text
HH offline
    ↓
GateNode neemt Master-rol over
```

Een `MASTER_CHANGED` event kan door de GateNode worden gegenereerd wanneer hij vaststelt dat de HH niet meer bereikbaar is.

Wanneer de HH terugkomt, wordt de Master-status opnieuw gesynchroniseerd.

---

### STATUS_REQUEST

Een node vraagt de actuele officiële status op.

Bijvoorbeeld:

```text
Display → HH
STATUS_REQUEST
```

Dit verandert de race-status niet en verhoogt daarom de `eventSequence` niet.

---

### STATUS_RESPONSE

Antwoord op een `STATUS_REQUEST`.

De response bevat de volledige actuele `StatusReport`.

Een `STATUS_RESPONSE` verandert de race-status niet en verhoogt daarom de `eventSequence` niet.

---

### NODE_OFFLINE

Geeft aan dat een node niet meer bereikbaar is.

Dit is een synchronisatie-/status-event en verandert niet automatisch de Manche/Moto.

De exacte detectie en communicatie worden in `RFProtocol.md` uitgewerkt.

---

### NODE_ONLINE

Geeft aan dat een node opnieuw bereikbaar is.

Na het online komen moet de node zijn actuele officiële `StatusReport` kunnen synchroniseren.

Dit event verandert niet automatisch de Manche/Moto.

---

## Master-principe

De HH is normaal de centrale Master van het systeem.

```text
              HH
           MASTER
          /      \
         /        \
   GateNode      Displays
   backup
```

De GateNode heeft echter voldoende informatie en bevoegdheid om bij uitval van de HH de Master-rol tijdelijk over te nemen.

Daarmee ontstaat een eenvoudige failover:

```text
Normaal:

HH → officiële status
HH → displays
HH → GateNode


HH valt weg:

GateNode → Master
GateNode → officiële status
GateNode → displays
```

Bij terugkeer van de HH moet de toestand opnieuw worden gesynchroniseerd. De exacte synchronisatieregels worden in `RFProtocol.md` vastgelegd.

---

## StatusReport

Een event dat de officiële race-status verandert, resulteert in een nieuwe volledige `StatusReport`.

Een StatusReport bevat minimaal:

```text
sessionId
eventSequence
manche
moto
mode
masterId
```

Er worden geen losse status-delta's naar displays gestuurd.

De Display hoeft dus niet te weten **waarom** een waarde veranderd is. Hij hoeft alleen de nieuwste geldige `StatusReport` te verwerken.

---

## Voorbeeld raceverloop

Nieuwe sessie:

```text
SESSION_RESET
eventSequence = 0000
manche = 0
moto = 000
```

Manche 1 wordt gestart:

```text
MANUAL_UPDATE
eventSequence = 0001
manche = 1
moto = 000
```

Gate drop:

```text
GATE_DROP
eventSequence = 0002
manche = 1
moto = 001
```

Snelle correctie naar Moto 003:

```text
MANUAL_UPDATE
eventSequence = 0003
manche = 1
moto = 003
```

Volgende gate drop:

```text
GATE_DROP
eventSequence = 0004
manche = 1
moto = 004
```

Handmatige wijziging terug naar Moto 002:

```text
MANUAL_UPDATE
eventSequence = 0005
manche = 1
moto = 002
```

De event counter blijft dus oplopen, ongeacht welke Moto actief is.

---

## Niet in dit document

De volgende technische details worden in `RFProtocol.md` vastgelegd:

- concrete packet-opbouw;
- bytevolgorde;
- MessageType-codes;
- event-ID-formaat;
- ACK;
- retry;
- deduplicatie op protocolniveau;
- CRC;
- heartbeat en timeout;
- synchronisatie na Master-failover.
