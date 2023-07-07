# Arduino alarm system

Prosty system alarmowy, stworzony na potrzeby projektu indywidualnego z zajęć z Systemów Wbudowanych.
Przy tworzeniu użyto symulatora Tinkercad.

## Widok układu

![Circuit](https://github.com/Mr-Victor16/arduino-alarm-system/blob/main/circuit.png?raw=true)

Zrzut ekranu układu stworzonego w symulatorze Tinkercad.

## Użyte komponenty
- Arduino Uno R3
- Klawiatura 4x4
- Wyświelacz LCD
- Potencjometr
- Rezystor
- Pasek LED
- Piezo
- Czujnik PIR
- Przełącznik suwakowy
- Czujnik przechyłu

## Możliwości systemu

- Uzbrajanie / rozbrajanie alarmu za pomocą kodu PIN (2 rodzaje: dla użytkownika oraz administratora) wpisywanego na klawiaturze matrycowej
- Możliwość podłączenia ponad 100 podsystemów oraz wielu czujników dla jednego podsystemu
- Możliwość zmiany kodu dostępu administratora oraz użytkownika
- Limit błędnych wprowadzeń kodu dostępu
- Dane zapisywane są w pamięci EEPROM, zatem w przypadku zaniku zasilania status systemu nie jest tracony
- Wykrywanie utraty komunikacji z podsystemem, możliwość dodawania podsystemów w trakcie działania
- Wybór działania za pomocą liter A-D na klawiaturze matrycowej:
    - A - Uzbrajanie systemu
    - B - zmiana kodu użytkownika,
    - C - zmiana kodu administratora
    - D - przeprowadzenie ponownej konfiguracji
- Sygnalizowanie statusu alarmu za pomocą wyświetlacza LCD, paska LED oraz piezo (dźwięk)

## FAQ

#### Jaki jest domyślny PIN użytkownika oraz administratora?

Dla użytkownika są to cyfry - 4321, a dla administratora 1234.

#### Co oznaczają kolory na pasku LED?

- Kolor niebieski - Alarm uzbrojony
- Kolor zielony na całym pasku - Alarm rozbrojony
- Kolor żółty na pojedyńczej diodzie - Brak połączenia z danym podsystemem.
- Kolor czerwony na pojedyńczej diodzie - Oznacza wykrycie ruchu na danym podsystemie. 
- Kolor czerwony na całym pasku LED - Uruchomienie alarmu po wykryciu ruchu.
