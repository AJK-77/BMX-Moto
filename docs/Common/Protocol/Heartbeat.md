# Heartbeat

## Doel

De Heartbeat wordt gebruikt om periodiek de basisstatus van een node te delen.

Hierdoor kunnen alle nodes vaststellen:

- dat een node actief is;
- wat de actuele basisstatus is;
- wanneer een node weer online komt.

---

## Heartbeat-interval

Iedere node verzendt eenmaal per seconde een Heartbeat.

Een interval van één seconde biedt een goede balans tussen:

- snelle detectie van uitval;
- snelle detectie van herstel;
- lage netwerkbelasting.

---

## Inhoud

Een Heartbeat bevat uitsluitend de actuele basisstatus van de verzendende node.

Hierin worden geen wedstrijdgegevens opgenomen.

---

## Wedstrijdstatus

De actuele wedstrijdstatus wordt niet via de Heartbeat gesynchroniseerd.

Hiervoor wordt StatusReport gebruikt.

---

## RF-protocol

De Heartbeat is een protocolblok binnen het RF-protocol.

Een Heartbeat kan zelfstandig worden verzonden, maar kan ook gecombineerd worden met andere protocolblokken.

Bijvoorbeeld:

- Event
- StatusReport

Hierdoor blijft het aantal RF-pakketten zo laag mogelijk.

---

## Ontvangst

Na ontvangst van een Heartbeat werkt de ontvangende node uitsluitend de basisstatus van de verzendende node bij.

Wijzigingen aan de wedstrijdstatus worden uitsluitend verwerkt via StatusReport.