# CRC

## Doel

De CRC (Cyclic Redundancy Check) wordt gebruikt om transmissiefouten in RF-berichten te detecteren.

Iedere node controleert de CRC voordat een ontvangen bericht wordt verwerkt.

Berichten met een ongeldige CRC worden direct verworpen.

---

## Toepassing

Ieder RF-bericht bevat één CRC.

De CRC wordt berekend over het volledige bericht, met uitzondering van de CRC zelf.

De CRC bevindt zich aan het einde van ieder RF-bericht.

---

## Algoritme

Het BMX-Moto systeem gebruikt één vast CRC-algoritme voor alle nodes.

Hierdoor wordt de compatibiliteit tussen alle componenten gewaarborgd.

De exacte implementatie wordt tijdens de softwareontwikkeling vastgelegd.

---

## Foutafhandeling

Wanneer een CRC-controle mislukt:

- wordt het bericht genegeerd;
- wordt de inhoud niet verwerkt;
- wordt geen foutcorrectie uitgevoerd.

Een eventueel nieuw bericht wordt afgewacht.

---

## Ontwerpfilosofie

De CRC is uitsluitend bedoeld voor foutdetectie.

Het herstellen of opnieuw verzenden van berichten valt buiten de verantwoordelijkheid van de CRC.