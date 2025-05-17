#!/bin/bash

# Sprawdź, czy plik wykonywalny istnieje
if [[ ! -f ./latex_pdf_generator ]]; then
  echo "Plik wykonywalny nie został znaleziony. Wykonaj make, aby zbudować projekt."
  exit 1
fi

# Sprawdź, czy podano plik wejściowy
if [ $# -lt 1 ]; then
    echo "Użycie: $0 [-C] [-L] [-T] [-S] <plik_wejściowy>"
    exit 1
fi

# Opcje
USE_BLACK=""
TABLE_LAYOUT=""
TEXT_DESC=""
SORT_ORDER=""

# Przetwarzanie opcji
while getopts "CLTS" opt; do
    case $opt in
        C) USE_BLACK="-C" ;;
        L) TABLE_LAYOUT="-L" ;;
        T) TEXT_DESC="-T" ;;
        S) SORT_ORDER="-S" ;;
        *) echo "Nieznana opcja: -$opt" ;;
    esac
done
shift $((OPTIND-1))

# Plik wejściowy
INPUT_FILE="$1"
if [[ -z "$INPUT_FILE" ]]; then
    echo "Podaj plik wejściowy."
    exit 1
fi

# Uruchomienie programu w C z opcjami
./latex_pdf_generator $USE_BLACK $TABLE_LAYOUT $TEXT_DESC $SORT_ORDER "$INPUT_FILE"
if [ $? -ne 0 ]; then
    echo "Błąd walidacji danych wejściowych lub generowania pliku LaTeX"
    exit 1
fi

# Kompilacja PDF
pdflatex output.tex > /dev/null
if [ $? -ne 0 ]; then
    echo "Błąd generowania PDF"
    exit 1
fi

echo "PDF został wygenerowany pomyślnie jako 'output.pdf'."

