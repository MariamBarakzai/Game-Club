Game Club 

Arduino Gaming Console mit MAX7219 LED-Matrix und Web-Interface

Eine professionelle Spielekonsole bestehend aus Hardware und Software. Game Club bietet 3 verschiedene Minispiele (Stacker, Catcher, Dodge) auf einer 8×8 LED-Matrix, steuerbar über einen physischen Button ODER über ein Web-Browser-Interface (GameBar).

-Projektbeschreibung

Game Club ist eine interaktive Arduino-Spielekonsole mit erweiterten Features:

- Hardware-Integration: Vollständiger Arduino-Aufbau mit Grove Beginner Kit, MAX7219 LED-Matrix, OLED-Display, Buzzer
- Mehrere Spiele: 3 verschiedene Minispiele mit unterschiedlichen Mechaniken
- Duale Steuerung: Spielbar mit Button ODER über Web-Browser
- Web Serial API: Moderne Webtechnologie zur Kommunikation mit Arduino
- Live-Feedback: OLED-Display zeigt Score, Spielmodus und Nachrichten
- Sound-Effekte: Buzzer für akustisches Feedback

-Die 3 Spiele


1-STACKER (Blöcke Stapeln)

Spielprinzip:
- Blöcke bewegen sich automatisch von links nach rechts
- Der Spieler muss im richtigen Moment auf den Button drücken
- Der Block wird dann übereinander gestapelt
- Mit jedem Level werden die Blöcke schmäler → höhere Schwierigkeit
- Ziel: So viele Blöcke wie möglich stapeln

Steuerung:
- Button drücken = Block platzieren
- Timing ist alles!

Besonderheit:
- Block-Breite nimmt ab (z.B.: Level 1: 3 LED, Level 2: 2 LED, etc.)
- Perfekt für Hand-Augen-Koordination

2-CATCHER (Gegenstände Fangen)

Spielprinzip:
- Ein Gegenstand fällt von oben nach unten
- Der Spieler steuert eine Figur am unteren Rand
- Ziel: Den fallenden Gegenstand aufzufangen
- Wenn der Gegenstand nicht gefangen wird → Game Over
- Mit jedem erfolgreichen Fang wird es schneller

Steuerung:
- Button drücken = Figur bewegt sich
- Rechtzeitig positionieren zum Fangen

Besonderheit:
- Fallgeschwindigkeit erhöht sich mit jedem Fang
- Zufälliges Spawning des Gegenstands
- Perfekt für Reaktionsschnelligkeit

3-DODGE (Hindernisse Ausweichen)

Spielprinzip:
- Wände mit Lücken fallen von oben nach unten
- Der Spieler muss durch die Öffnungen navigieren
- Wenn der Spieler eine Wand berührt → Game Over
- Mit jedem erfolgreichen Dodge wird es schneller

Steuerung:
- Button drücken = Spieler bewegt sich
- Durch die Lücken ausweichen

Besonderheit:
- Schwierigste Spiel zum Programmieren
- Dynamische Lücken-Positionen
- Hohe Reflexanforderung
- Perfekt für Challenge-Spieler

- Hardware


Komponenten:

| Komponente | Beschreibung | Anzahl | Pin |
|-----------|------------|--------|-----|
| **Grove Beginner Kit / Seeeduino Lotus** | Mikrocontroller (Arduino-kompatibel) | 1 | - |
| **MAX7219 LED-Matrix** | 8×8 LED-Display zur Spielanzeige | 1 | DIN: 2, CS: 3, CLK: 4 |
| **Grove OLED Display** | OLED 128×64px für Score/Status | 1 | I2C |
| **Grove Button** | Tastensteuerung für Spiele | 1 | Pin 5 |
| **Grove Buzzer** | Sound-Effekte und Melodien | 1 | Pin 6 |
| **USB-Kabel** | Stromversorgung und Programmierung | 1 | - |
| **Jumper-Kabel** | Zur Verbindung der Komponenten | mehrere | - |

Pin-Konfiguration (Arduino):
```
MAX7219 LED-Matrix:
  VCC   → 5V
  GND   → GND
  DIN   → Pin 2
  CS    → Pin 3
  CLK   → Pin 4

OLED Display (I2C):
  SDA   → A4
  SCL   → A5
  VCC   → 5V
  GND   → GND

Button:
  Signal → Pin 5
  GND    → GND

Buzzer:
  Signal → Pin 6
  GND    → GND
```

---

- Software & Libraries

Erforderliche Libraries:

```cpp
#include "LedControl.h"        // Steuerung MAX7219 LED-Matrix
#include <Wire.h>              // I2C Kommunikation für OLED
#include <U8x8lib.h>           // OLED Display Library
```

Installation in Arduino IDE:
1. Sketch → Include Library → Manage Libraries
2. Installieren:
   - LedControl von Eelkool
   - U8x8 von Oliver Krause
3. Fertig!

Programmiersprache:
- C++ (Arduino IDE)
- Arduino IDE Version 1.8.0 oder höher

Software-Architektur:

```
setup()
  ├─ Hardware initialisieren
  ├─ LED-Matrix vorbereiten
  ├─ OLED konfigurieren
  └─ Buzzer starten

loop()
  ├─ Button überprüfen
  ├─ Aktuelles Spiel updaten (updateStacker/Catcher/Dodge)
  ├─ OLED Display aktualisieren
  └─ Game Over Check

Funktionen:
  ├─ updateStacker() - Stacker Spiel-Logik
  ├─ updateCatcher() - Catcher Spiel-Logik
  ├─ updateDodge() - Dodge Spiel-Logik
  ├─ updateOLED() - Display Update
  ├─ resetVariables() - Spiel zurücksetzen
  ├─ gameOver() - Game Over Handler
  ├─ winGame() - Sieg Handler
  └─ playSound() - Buzzer Sound abspielen
```

---

Web-Interface (GameBar.html)

Was ist GameBar?

Eine Web-basierte Fernbedienung für die Spielekonsole. Der Browser verbindet sich direkt via USB mit dem Arduino!

Funktionen:

Spiel-Auswahl: 3 Karten zum Auswählen (Stacker, Catcher, Dodge)  
Live Score-Monitor: Zeigt Punkte in Echtzeit  
LED-Matrix Preview: Visualisiert das Spiel  
Hardware-Info: Anzeige der verbundenen Komponenten  
Web Serial API: Direkte Kommunikation mit Arduino via USB  

Wie es funktioniert:

```
Browser (GameBar.html)
       ↓ (Web Serial API)
   Arduino/USB
       ↓ (Serial Communication)
   LED-Matrix, OLED, Buzzer
```

Beispiel Kommunikation:
- User klickt auf "Stacker" Button → Browser sendet `'1'`
- Arduino empfängt `'1'` → startet Stacker Spiel
- Spiel läuft auf LED-Matrix
- Arduino sendet Score zurück → Browser zeigt Score

Design:

- Gaming-Stil: Dunkler Hintergrund, Pixel-Art Fonts
- Responsive: Funktioniert auf PC & Tablet
- Live-Daten: Serial Monitor zeigt Kommunikation
- Benutzerfreundlich: Große Buttons, klare Struktur

Browser-Anforderungen:

Wichtig: Web Serial API funktioniert nur im Chrome Browser! (oder Edge/Opera)


Installation & Aufbau

Schritt 1: Hardware zusammenstellen

1. Grove Beginner Kit auspacken
2. MAX7219 LED-Matrix anschließen (Pin 2, 3, 4)
3. OLED Display verbinden (I2C: SDA/SCL)
4. Button anschließen (Pin 5)
5. Buzzer anschließen (Pin 6)
6. Alle Komponenten testen (LEDs blinken, Display zeigt Text)

Schritt 2: Arduino IDE einrichten

1. [Arduino IDE herunterladen](https://www.arduino.cc/en/software)
2. Libraries installieren:
   - LedControl
   - U8x8
3. Board-Typ auswählen: Arduino Uno (oder Seeeduino Lotus)

Schritt 3: Code hochladen

1. Dateien aus diesem Repo herunterladen
2. `GameClub.ino` öffnen
3. Upload drücken (oder Ctrl+U)
4. Warten bis "Done uploading" erscheint

Schritt 4: Web-Interface nutzen (Optional)

1. `GameBar.html` in einem Browser öffnen (Chrome!)
2. "Connect" Button drücken
3. Arduino aus der Liste auswählen
4. Fertig! Jetzt können Sie über den Browser spielen


Wie man spielt

Mit Button (Physical Console):

1. Spiel starten: Nach Upload startet automatisch Stacker
2. Spielen: Button drücken zum Interagieren
3. Score sehen: OLED zeigt Punkte in Echtzeit
4. Sounds: Buzzer gibt Feedback (Klick, Erfolg, Warnung)
5. Game Over: OLED zeigt "GAME OVER", Buzzer warnt
6. Neustart: Button lange drücken zum Neustarten

Mit Web-Interface (GameBar.html):

1. Browser öffnen: GameBar.html in Chrome öffnen
2. Verbinden: "Connect to Arduino" drücken
3. Spiel wählen: Auf Stacker/Catcher/Dodge klicken
4. Spielen: Button drücken (oder physischer Button am Arduino)
5. Score sehen: Oben rechts im Web-Interface
6. Serial Monitor: Unten sehen Sie die Live-Kommunikation

Spiel-Tipps:

Stacker:
- Timing ist alles!
- Je höher, desto schmäler die Blöcke
- Ziel: Perfekt übereinander stapeln

Catcher:
- Schnelle Reaktion erforderlich!
- Früh positionieren
- Ziel: Alle Gegenstände fangen

Dodge:
- Vorbereitung ist wichtig
- Lücken frühzeitig erkennen
- Ziel: So lange wie möglich ausweichen


Projektstruktur

```
Game-Club/
├── GameClub.ino            # Hauptprogramm (Arduino Code)
├── GameBar.html            # Web-Interface
├── README.md               # Diese Datei
├── LICENSE                 # MIT License
└── docs/
    └── Hardware.md         # Detaillierte Hardware-Doku
```

Technische Details

Hauptfunktionen im Code:

```cpp
void setup()              // Initialisierung aller Komponenten
void loop()               // Hauptspiel-Loop

// Spiel-Update Funktionen
void updateStacker()      // Stacker Spiel-Logik
void updateCatcher()      // Catcher Spiel-Logik
void updateDodge()        // Dodge Spiel-Logik

// Display & Sound
void updateOLED()         // OLED Display aktualisieren
void playSound(int freq)  // Buzzer Sound abspielen
void playMelody()         // Party-Melodie bei Sieg

// Spiel-Management
void resetVariables()     // Alles zurücksetzen
void gameOver()           // Game Over Handler
void winGame()            // Win Handler
```

Besondere Features:

Cheat-Modus:
- Button länger als 1,5 Sekunden drücken
- Springt zum nächsten Spiel
- Nützlich zum Testen!

Winner-Animation:
- Herzen auf OLED-Display
- LED blinkt Blitzmuster
- Party-Melodie spielt ab

Sounds:
- Klick-Geräusche:** Beim Block platzieren
- Erfolgsgeräusche:** Bei erfolgreichem Fang
- Warnsignale:** Bei Fehlern
- Party-Melodie:** Beim Sieg


Fehlerbehebung

| Problem | Lösung |
|---------|--------|
| LED-Matrix leuchtet nicht | Pin-Konfiguration überprüfen (2, 3, 4) |
| OLED zeigt nichts | I2C Adresse überprüfen, SDA/SCL Pins korrekt? |
| Button funktioniert nicht | Pin 5 korrekt angeschlossen? |
| Buzzer stumm | Pin 6 überprüfen, Ton-Frequenz zu hoch/niedrig? |
| Web-Interface verbindet nicht | Chrome Browser verwenden! (Web Serial API) |
| USB-Verbindung bricht ab | Kabel prüfen, Arduino neu starten |


Autoren

Maryam Akbari - Hardware & Konzept  
Mariam Barakzai - Software & Web-Interface

HTL Wien West | Informatik Programm | Schuljahr 2026

Lizenz

Dieses Projekt ist unter der MIT License lizenziert.  
Siehe [LICENSE](LICENSE) für Details.
