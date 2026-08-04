# 485-display - Hardware

## Cijferposities

De RS485-displaymodule gebruikt softwarematig een frame met **vier
cijferposities**. De tweede cijferpositie is echter niet fysiek op de PCB
gemonteerd.

| Softwarepositie | Fysieke status |
|---:|---|
| 1 | Aanwezig |
| 2 | Niet gemonteerd op de PCB |
| 3 | Aanwezig |
| 4 | Aanwezig |

De firmware behoudt altijd de vier oorspronkelijke posities in de
displaygegevens. De ontbrekende tweede positie mag de overige posities niet
opschuiven; deze wordt als lege/gereserveerde positie behandeld.

De functionele verdeling van manche- en motowaarden over de zichtbare posities
wordt vastgelegd in de softwaredocumentatie van het 485-display.

## Watchdog-weergave

De RS485-displaymodule verwacht regelmatig een update van de display-ESP32.
Wanneer deze circa 30 seconden geen update ontvangt, toont de module
`_ _ _ _`.

De display-ESP32 stuurt een nieuwe status alleen door wanneer hij een geldige
nieuwe systeemstatus van de actieve master heeft ontvangen. De
watchdog-weergave maakt daardoor een ontbrekende systeemverbinding zichtbaar
in plaats van een verouderde wedstrijdstatus te blijven tonen.
