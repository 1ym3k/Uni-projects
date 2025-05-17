#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char* number_to_words(int num) {
    static char result[100];  // Bufor na wynik, aby uniknąć problemów z pamięcią
    result[0] = '\0';  // Zaczynamy od pustego ciągu

    static const char* ones[] = {
        "zero", "jedna", "dwie", "trzy", "cztery", "pięć", "sześć", "siedem", "osiem", "dziewięć",
        "dziesięć", "jedenaście", "dwanaście", "trzynaście", "czternaście", "piętnaście", "szesnaście", 
        "siedemnaście", "osiemnaście", "dziewiętnaście"
    };
    static const char* tens[] = {
        "", "", "dwadzieścia", "trzydzieści", "czterdzieści", "pięćdziesiąt", "sześćdziesiąt", 
        "siedemdziesiąt", "osiemdziesiąt", "dziewięćdziesiąt"
    };
    static const char* hundreds[] = {
        "", "sto", "dwieście", "trzysta", "czterysta", "pięćset", "sześćset", "siedemset", 
        "osiemset", "dziewięćset"
    };

    // Konwersja liczby na słowa
    if (num < 20) {
        strcpy(result, ones[num]);  // Kopiujemy słowo dla liczb mniejszych niż 20
    } else if (num < 100) {
        strcpy(result, tens[num / 10]);  // Kopiujemy słowo dla dziesiątek
        if (num % 10 != 0) {
            strcat(result, " ");  // Dodajemy spację, jeśli są jednostki
            strcat(result, ones[num % 10]);  // Dodajemy słowo dla jednostek
        }
    } else {
        strcpy(result, hundreds[num / 100]);  // Kopiujemy słowo dla setek
        if (num % 100 != 0) {
            strcat(result, " ");  // Dodajemy spację, jeśli są dziesiątki lub jednostki
            if (num % 100 < 20) {
                strcat(result, ones[num % 100]);  // Dodajemy słowo dla liczb mniejszych niż 20
            } else {
                strcat(result, tens[(num % 100) / 10]);  // Dodajemy słowo dla dziesiątek
                if (num % 10 != 0) {
                    strcat(result, " ");  // Dodajemy spację, jeśli są jednostki
                    strcat(result, ones[num % 10]);  // Dodajemy słowo dla jednostek
                }
            }
        }
    }

    // Określenie odpowiedniej formy słowa "figura"
    if (num == 1) {
        strcat(result, " figura");
    } else if (num == 2 || num == 3 || num == 4) {
        strcat(result, " figury");
    } else {
        strcat(result, " figur");
    }

    return result;
}

int validate_input(const char *file_path, FILE *output, int use_black_figures, int generate_text_desc, int table_layout, int sort_configurations) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Nie udało się otworzyć pliku");
        return 1; // Kod błędu
    }

    int n;
    if (fscanf(file, "%d", &n) != 1 || n < 1 || n > 100) {
        fprintf(stderr, "Niepoprawna liczba konfiguracji (n musi być w zakresie 1-100).\n");
        fclose(file);
        return 1;
    }

    fprintf(output, "\\documentclass{article}\n");
    fprintf(output, "\\usepackage{chessboard}\n\\usepackage{array}\n\\usepackage{polski}\n\\usepackage[utf8]{inputenc}\n");
    
    // Strona tytulowa
    fprintf(output, "\\title{Generowanie konfiguracji szachownic}\n");
    fprintf(output, "\\author{Tymoteusz Herkowiak}\n");
    fprintf(output, "\\date{Data: %s}\n", __DATE__);  // Automatycznie dodaje datę kompilacji
    fprintf(output, "\\begin{document}\n");
    fprintf(output, "\\maketitle\n");  // Tworzy stronę tytulową
    fprintf(output, "\\clearpage\n"); 
    (void)generate_text_desc;
    (void)table_layout;
    (void)sort_configurations;

    for (int i = 0; i < n; i++) {
        int k, m;
        if (fscanf(file, "%d %d", &k, &m) != 2 || k < 1 || k > 9 || m < 0 || m > k * k) {
            fprintf(stderr, "Niepoprawne dane dla konfiguracji %d (k: 1-9, m: 0-k*k).\n", i + 1);
            fclose(file);
            return 1;
        }

        char maxfield[4];
        snprintf(maxfield, sizeof(maxfield), "%c%d", 'a' + k - 1, k);

        int board[9][9] = {0};
        int row_counts[9] = {0};
        int col_counts[9] = {0};

        fprintf(output, "\\clearpage\n");
        fprintf(output, "\\section{Konfiguracja %d}\n", i + 1);

        

        // Generowanie obrazka szachownicy dopiero po tytule sekcji
        fprintf(output, "\\begin{figure}[h!]\n");
        fprintf(output, "\\begin{center}\n");
        fprintf(output, "\\chessboard[showmover=false, maxfield=%s, %s={", maxfield, use_black_figures ? "setblack" : "setwhite");

        char position[4];
        for (int j = 0; j < m; j++) {
            if (fscanf(file, "%3s", position) != 1) {
                fprintf(stderr, "Brak wystarczających danych dla konfiguracji %d.\n", i + 1);
                fclose(file);
                return 1;
            }

            if (position[0] != 'Q' ||
                !isalpha(position[1]) || !isdigit(position[2]) ||
                position[1] < 'a' || position[1] >= 'a' + k || 
                position[2] < '1' || position[2] > '0' + k) {
                fprintf(stderr, "Niepoprawna pozycja figury w konfiguracji %d: %s.\n", i + 1, position);
                fclose(file);
                return 1;
            }

            int col = position[1] - 'a';
            int row = position[2] - '1';
            if (board[row][col]) {
                fprintf(stderr, "Duplikat pozycji figury w konfiguracji %d: %s.\n", i + 1, position);
                fclose(file);
                return 1;
            }
            board[row][col] = 1;
            row_counts[row]++;
            col_counts[col]++;

            if (j > 0) fprintf(output, ",");
            fprintf(output, "%s", position);
        }

        fprintf(output, "}]\n\\end{center}\n");
        fprintf(output, "\\caption{Konfiguracja numer: %d}\n", i + 1);
        fprintf(output, "\\end{figure}\n");

        // Tabela z liczbą figur w wierszach
        fprintf(output, "\\begin{tabular}{|c|c|}\n");
        fprintf(output, "\\hline\n");
        fprintf(output, "Numer wiersza & Liczba figur w wierszu \\\\\n");
        fprintf(output, "\\hline\n");
        for (int r = 0; r < k; r++) {
            fprintf(output, "%d & %d\\\\\n", r + 1, row_counts[r]); // Numery wierszy zaczynają się od 1
            fprintf(output, "\\hline\n");
        }
        fprintf(output, "\\end{tabular}\n");

        // Tabela z liczbą figur w kolumnach
        fprintf(output, "\\begin{tabular}{|c|c|}\n");
        fprintf(output, "\\hline\n");
        fprintf(output, "Litera kolumny & Liczba figur w kolumnie \\\\\n");
        fprintf(output, "\\hline\n");
        for (int c = 0; c < k; c++) {
            fprintf(output, "%c & %d\\\\\n", 'a' + c, col_counts[c]); // Kolumny literowe zaczynają się od 'a'
            fprintf(output, "\\hline\n");
        }
        fprintf(output, "\\end{tabular}\n");

        // Generowanie opisu w formie tekstu tylko, jeśli funkcja -T jest aktywna
        if (generate_text_desc) {
            fprintf(output, "\\paragraph{Opis:} Na szachownicy %d x %d jest %s.\n", k, k, number_to_words(m));
        }
    }

    fprintf(output, "\\end{document}\n");
    fclose(file);
    return 0;
}

