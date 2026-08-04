# StatusReport

**Status:** Concept v1.0

## Doel

Een StatusReport is een volledige momentopname van de officiële
wedstrijdstatus. Het bericht wordt gebruikt voor directe statusupdates,
heartbeats, failover en herstel na een reboot of tijdelijke verbindinguitval.

StatusReports zijn altijd volledig; er worden geen losse deltas naar displays
of andere nodes gestuurd. Daardoor kan een node met één geldig bericht weer
synchroon komen.

## Canonieke wedstrijdstatus

| Veld | Betekenis |
|---|---|
| `sessionId` | Identiteit van de actieve wedstrijddag/sessie. |
| `eventSequence` | Monotoon oplopend nummer van de wedstrijdstatus binnen de sessie. |
| `manche` | Actueel manchenummer. Alleen handmatig wijzigbaar. |
| `moto` | Actueel motonummer. In AUTO verhoogd na een geldige gate-drop; in MANUAL handmatig wijzigbaar. |
| `mode` | Actuele wedstrijdmodus: `AUTO` of `MANUAL`. |
| `masterId` | Identiteit van de node die de officiële wedstrijdstatus beheert. |

De exacte veldgroottes, codering en bytevolgorde worden later vastgelegd in
`RFProtocol.md`.

## Geldigheid en volgorde

- Een StatusReport is alleen geldig binnen zijn `sessionId`.
- Binnen dezelfde sessie is een hogere `eventSequence` nieuwer.
- Een node negeert een oudere sequence binnen de huidige sessie.
- Een nieuw `sessionId` wordt alleen geaccepteerd als geldige sessie-reset van
  de Handheld.
- Een StatusReport met dezelfde sequence mag worden gebruikt om een herstelde
  node opnieuw te synchroniseren, maar wijzigt de lokale status niet.

## Wie mag de status wijzigen

Alleen de actieve master maakt een nieuwe officiële wedstrijdstatus.

- Normaal is de Handheld de master.
- De Gate-Node wordt tijdelijk master wanneer de Handheld niet beschikbaar is.
- De Gate-Node werkt tijdens die overname in `AUTO`.
- Wanneer de Handheld terugkeert, vraagt hij eerst de actuele status bij de
  Gate-Node op, neemt deze over en wordt daarna weer master.
- Displays tonen de status alleen en mogen deze nooit wijzigen.

Iedere statuswijziging verhoogt `eventSequence` precies één keer en wordt
direct als volledig StatusReport verstuurd.

## Wanneer wordt een StatusReport verstuurd

Een volledig StatusReport wordt direct verstuurd bij:

- een verwerking van een geldige `GATE_DROP` in AUTO-modus;
- een bevestigde handmatige wijziging op de Handheld;
- een sessie-reset;
- een wissel van masterrol;
- een antwoord op `STATUS_REQUEST`;
- het herstellen van een node die opnieuw moet synchroniseren.

Daarnaast bevat iedere heartbeat de actuele kernstatus volgens `Heartbeat.md`.

## Permanente opslag en herstel

De Handheld en Gate-Node slaan de laatst geldige volledige wedstrijdstatus op
in de interne niet-vluchtige opslag (NVS).

- Opslag vindt plaats na iedere geaccepteerde statuswijziging.
- De actieve master slaat de nieuwe status op voordat deze als definitieve
  status wordt uitgezonden.
- Een niet-actieve Gate-Node slaat een nieuw, geldig StatusReport van de
  Handheld op als failoverbasis.
- Een display hoeft de wedstrijdstatus niet permanent op te slaan; het haalt
  na herstel een actuele StatusReport op bij de master.

Na een reboot herstelt een Handheld of Gate-Node eerst zijn lokaal opgeslagen
status en vraagt vervolgens de actuele status bij andere beschikbare nodes op.
Een nieuwere geldige status wordt overgenomen.

### Meerdere nodes opnieuw gestart

Wanneer Handheld en Gate-Node tegelijk opnieuw starten, vergelijken zij hun
opgeslagen StatusReport zodra de verbinding beschikbaar is.

- Binnen dezelfde sessie wint de hoogste `eventSequence`.
- Bij gelijke `eventSequence` moeten alle velden identiek zijn.
- Verschillen de velden toch bij een gelijke sequence, dan is de status
  ongeldig/conflicterend. De nodes tonen een fout en wachten op bevestiging of
  een expliciete handmatige correctie via de Handheld.

Hiermee kan de wedstrijd ook na tijdelijke spanningsuitval worden hervat,
zonder dat een oudere status stilzwijgend de nieuwere overschrijft.

## Geen onderdeel van de wedstrijdstatus

De volgende gegevens horen niet in de canonieke wedstrijdstatus:

- individuele beschikbaarheid van nodes;
- lokale foutcodes en ledstatussen;
- batterijstatus;
- RSSI, MAC-adressen en andere radio-informatie;
- display-specifieke instellingen.

Deze gegevens zijn lokaal of worden apart via heartbeat, diagnostiek of de
node-eigen documentatie afgehandeld.

## Samenhang met andere protocollen

- `Events.md` beschrijft welke gebeurtenissen een StatusReport veroorzaken.
- `Heartbeat.md` definieert het interval voor periodieke synchronisatie en
  foutdetectie.
- `RFProtocol.md` definieert de concrete overdracht via ESP-NOW.
