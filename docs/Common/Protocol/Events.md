# Events

**Status:** Concept v1.0

## Doel

Dit document definieert de betekenis van gebeurtenissen binnen het BMX
Moto-systeem. Een event beschrijft **wat er is gebeurd**. De concrete
codering, verzendwijze, bevestiging en eventuele herhaling worden later
vastgelegd in `RFProtocol.md`.

Een event is niet hetzelfde als een heartbeat. De heartbeat bevestigt elke
5 seconden de aanwezigheid en actuele status. Een event wordt direct verstuurd
wanneer er iets relevants verandert.

Er zijn twee soorten events:

- **Wedstrijd-events:** veranderen de wedstrijdstatus, zoals een gate-drop of
  een handmatige wijziging.
- **Systeem-events:** beschrijven de beschikbaarheid of rol van een node,
  bijvoorbeeld een uitval, terugkeer of masterwissel.

## Algemene regels

- Alleen de actieve master mag de officiële wedstrijdstatus wijzigen.
- Een wijziging van de wedstrijdstatus verhoogt `eventSequence` en wordt direct
  als actuele status naar de andere nodes verstuurd.
- Events en statusberichten behoren bij een `sessionId`.
- Berichten uit een oude sessie worden genegeerd.
- De exacte datavelden en identifiers volgen in het RF-protocol.

## Events

### `GATE_DROP`

De Gate-Node heeft een geldig groen startlicht gedetecteerd.

- Alleen de Gate-Node mag dit event maken.
- Per groen startlicht mag maximaal één `GATE_DROP` ontstaan.
- In AUTO-modus verwerkt de actieve master dit event door `moto` met één te
  verhogen en vervolgens de gewijzigde wedstrijdstatus te versturen.
- In MANUAL-modus verandert dit event de wedstrijdstatus niet.
- De technische detectie van het startlicht staat in de Gate-Node-documentatie.

### `MANUAL_UPDATE`

De gebruiker heeft op de Handheld een wedstrijdwaarde gewijzigd en bevestigd.

Dit event ontstaat bij:

- bevestigen van numerieke invoer met **OK**;
- een druk op **+**;
- een druk op **-**.

De Handheld verwerkt de wijziging als actieve master, verhoogt
`eventSequence` en verstuurt de actuele status direct naar de Gate-Node en
alle displays. De Gate-Node slaat deze status op als failoverbasis.

De `manche` is uitsluitend handmatig aanpasbaar. Een handmatige wijziging van
`moto` is ook toegestaan wanneer de gebruiker in MANUAL-modus werkt.

### `SESSION_RESET`

De Handheld start bij aanvang van een wedstrijddag een nieuwe sessie.

- `sessionId` krijgt een nieuwe waarde.
- `eventSequence` wordt teruggezet naar `0`.
- De actuele wedstrijdstatus wordt direct naar alle nodes verstuurd.
- De Gate-Node slaat de nieuwe sessie op als failoverbasis.

### `MASTER_CHANGED`

De actieve masterrol is gewijzigd.

Dit event wordt direct met de actuele status verstuurd in de volgende
situaties:

- de Gate-Node neemt de masterrol over nadat de Handheld niet beschikbaar is;
- de Handheld komt terug, leest de actuele status bij de Gate-Node uit en
  neemt de masterrol weer over.

Bij overname door de Gate-Node wordt tijdelijk `AUTO` gebruikt, ook wanneer
de Handheld voor de uitval in MANUAL-modus stond.

### `NODE_OFFLINE`

Een node heeft twee verwachte heartbeats gemist en wordt lokaal als niet
beschikbaar gemarkeerd.

- Iedere ontvangende node maakt dit systeem-event voor zichzelf.
- Dit event wordt afgeleid uit de heartbeat en hoeft niet als apart bericht
  door de uitgevallen node te worden verzonden.
- De lokale foutindicatie volgt de documentatie van de betreffende node.
- De gevolgen hangen af van de rol van de uitgevallen node: een display mist
  alleen zijn weergave, terwijl de Gate-Node bij verlies van de Handheld de
  masterrol overneemt.

### `NODE_ONLINE`

Een eerder niet-beschikbare node heeft weer een geldige heartbeat of
statusupdate verzonden.

- Iedere ontvangende node maakt dit systeem-event voor zichzelf.
- De herstelde node vraagt de actuele status op bij de actieve master.
- De actieve master antwoordt met de volledige status, zodat de herstelde
  node zonder handmatige ingreep verder kan werken.
- Een teruggekeerde Gate-Node slaat deze status weer op als failoverbasis.
- Een teruggekeerd display hervat uitsluitend de weergave.

### `STATUS_REQUEST`

Een node vraagt de actuele wedstrijdstatus op bij de actieve master.

Dit event wordt gebruikt wanneer een node opnieuw online komt of na een reboot
geen actuele lokale status heeft. De Handheld gebruikt het bovendien na
terugkeer om de status bij de Gate-Node op te vragen voordat hij de masterrol
terugneemt. Dit event wijzigt zelf geen wedstrijdstatus.

### `STATUS_RESPONSE`

Een node antwoordt op een `STATUS_REQUEST` met de volledige, actuele
wedstrijdstatus. Minimaal `sessionId`, `eventSequence`, `manche`, `moto`,
modus en actieve masterrol worden geleverd.

## Periodieke synchronisatie

Een gewone periodieke synchronisatie is geen afzonderlijk event. Deze verloopt
via de heartbeat.

## Samenhang met andere protocollen

- `Heartbeat.md` definieert aanwezigheid, foutdetectie en de periodieke
  synchronisatie.
- `StatusReport.md` definieert de volledige gedeelde wedstrijdstatus.
- `RFProtocol.md` definieert hoe events en statusberichten over ESP-NOW worden
  verzonden.
