# Projekt: Generowanie konfiguracji szachownic w LaTeX

## Opis
Projekt polega na generowaniu pliku .pdf zawierającego różne konfiguracje figur na szachownicy. Program odczytuje dane wejściowe (liczba figur, ich pozycje) i generuje dokument w formacie LaTeX, który zawiera:
- Obrazek szachownicy z ustawionymi figurami,
- Tabele z liczbą figur w wierszach i kolumnach,
- Opcjonalny opis w formie tekstowej w liczbach słownych (np. "dziesięć figur").

## Wymagania
- **Komputer z systemem operacyjnym obsługującym C/C++**
- **Kompilator C** (np. GCC)
- **Make** do automatyzacji procesu kompilacji
- **Pakiety LaTeX**: `chessboard`, `array`, `polski`, `inputenc` (zakładając, że system LaTeX jest zainstalowany)

## Instrukcja instalacji

1. **Skopiuj lub pobierz pliki źródłowe**:
    - Skopiuj pliki źródłowe projektu na swój komputer.

2. **Skonfiguruj kompilator C**:
    - Upewnij się, że masz zainstalowany kompilator C (np. GCC).
    - Zainstaluj wymagane pakiety LaTeX (np. `chessboard`, `array`, `polski`, `inputenc`).

3. **Skorzystaj z cmake**:
    - W katalogu projektu uruchom polecenie:
      ```bash
      cmake -S <sciezka do katalogu z plikami zrodlowymi> -B <sciezka do folderu build>
      cd <sciezka do folderu build>
      make
      ```
    - Komenda ta zbuduje projekt i wygeneruje plik wykonywalny `latex_pdf_generator`.

4. **Uruchom program**:
    - Aby uruchomić program, użyj poniższego polecenia:
      ```bash
      ./start.sh [opcje] <ścieżka_do_pliku_wejściowego>
      ```
    - Parametry:
        - `ścieżka_do_pliku_wejściowego` — plik z danymi wejściowymi, który zawiera konfiguracje.
        - Opcje:
            - `-C` — użyj czarnych figur na szachownicy (domyślnie używane są białe).
            - `-T` — dodaj opis tekstowy do konfiguracji.

## Format pliku wejściowego

Plik wejściowy powinien mieć następujący format:
```
n
k1 m1 p1 p2 ... pm1
k2 m2 p1 p2 ... pm2
...
kn mn p1 p2 ... pmn
```

Gdzie:
- `n` — liczba konfiguracji (1 ≤ n ≤ 100),
- `ki` — rozmiar szachownicy (1 ≤ ki ≤ 9),
- `mi` — liczba figur (0 ≤ mi ≤ ki²),
- `pi` — pozycja figury w formacie "Q{kolumna}{wiersz}" (np. "Qa1", "Qb2").

### Przykład pliku wejściowego:
```
2
3 2 Qa1 Qb2
4 3 Qa1 Qc2 Qd3
```


## Format pliku wyjściowego

Po wygenerowaniu pliku wyjściowego, LaTeX zostanie utworzony z następującą strukturą:
- Sekcje, które zawierają:
  - Tytuł sekcji z numerem konfiguracji,
  - Obrazek szachownicy z ustawionymi figurami,
  - Tabele z liczbą figur w wierszach i kolumnach,
  - Opcjonalny opis tekstowy.

## Przykład użycia

1. **Przykład wejściowy**: Plik `input.txt`
    ```
    2
    3 2 Qa1 Qb2
    4 3 Qa1 Qc2 Qd3
    ```

2. **Uruchomienie programu**:
    ```bash
    ./start.sh input.txt
    ```

3. **Rezultat**:
    Program wygeneruje plik LaTeX `output.pdf` zawierający konfiguracje szachownic, tabele i opcjonalne opisy.

## Opcje

- **-C**: Użycie czarnych figur (domyślnie są białe).
    - Przykład:
      ```bash
      ./start.sh -C input.txt
      ```
  
- **-T**: Dodanie opisu tekstowego w liczbach słownych.
    - Przykład:
      ```bash
      ./start.sh -T input.txt
      ```


      ```

