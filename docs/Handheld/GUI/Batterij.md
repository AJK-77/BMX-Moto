Status:
**Definitief (v1.2)**

# Batterij

## Doel

De batterij-indicatie geeft dezelfde informatie weer als de vier LED's van de IP5306.
Er worden geen percentages berekend of weergegeven. De GUI is een digitale weergave van de hardwarestatus.

---

# Batterij-icoon

- Liggend batterij-icoon.
- Witte rand.
- Vier segmenten.
- Pluspool rechts.
- Lege segmenten donkergrijs.

## Normale weergave

| IP5306 LED's              | Batterij |
|---------------------------|----------|
| LED1                      | 🟩⬜⬜⬜ |
| LED1 + LED2               | 🟩🟩⬜⬜ |
| LED1 + LED2 + LED3        | 🟩🟩🟩⬜ |
| LED1 + LED2 + LED3 + LED4 | 🟩🟩🟩🟩 |

Segmenten:
- Groen = actief
- Donkergrijs = niet actief

---

# Laden

Tijdens het laden:

- Voor de batterij wordt een bliksem-icoon weergegeven.
- Alleen het actieve laadsegment knippert groen (0,5 Hz).
- Reeds bereikte segmenten blijven continu groen.

Voorbeeld:

75% laden

⚡ 🟩🟩🟩⬜

(Het vierde segment knippert.)

---

### Lage batterij

Trigger:
- De 25%-LED van de IP5306 knippert met 1,5 Hz.

Gedrag:
- Het volledige batterij-icoon knippert rood.
- Er verschijnt een waarschuwing-banner.
- De banner blijft zichtbaar totdat de gebruiker op **OK** drukt.
- Zolang de lage batterijstatus actief blijft, verschijnt de banner iedere **3 minuten** opnieuw.
- Het batterij-icoon blijft rood knipperen totdat de accu wordt opgeladen.
- Zodra de accu wordt opgeladen, wordt de waarschuwing en de timer automatisch gereset.

Banner:

```
┌────────────────────────────┐
│                            │
│    ⚠ BATTERIJ BIJNA LEEG   │
│                            │
│   Sluit een lader of       │
│     powerbank aan.         │
│                            │
│           [ OK ]           │
│                            │
└────────────────────────────┘
```

Eigenschappen:

- Verschijnt éénmaal zodra de lage batterij wordt gedetecteerd.
- Blijft zichtbaar totdat de gebruiker op **OK** drukt maar komt terug na 3 minuten.
- Na bevestiging verdwijnt de banner.
- Het rode knipperende batterij-icoon blijft zichtbaar zolang de IP5306 de lage batterijstatus aangeeft.
- Bij opladen wordt de melding automatisch gereset.

---

# Opmerkingen

- De GUI toont uitsluitend de status van de IP5306.
- Er worden geen batterijpercentages berekend.
- De GUI blijft daardoor volledig synchroon met de hardware-indicatie.

---