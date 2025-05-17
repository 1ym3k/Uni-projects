#include "main_functions.h"
#include <getopt.h>


#ifndef UNIT_TEST
int main(int argc, char **argv) {
    int use_black_figures = 0;
    int generate_text_desc = 0;
    int table_layout = 0;
    int sort_configurations = 0;

    int opt;
    while ((opt = getopt(argc, argv, "CLTS")) != -1) {
        switch (opt) {
            case 'C':
                use_black_figures = 1;
                break;
            case 'L':
                table_layout = 1;
                break;
            case 'T':
                generate_text_desc = 1;
                break;
            case 'S':
                sort_configurations = 1;
                break;
            default:
                fprintf(stderr, "Nieznana opcja: -%c\n", optopt);
                fprintf(stderr, "Użycie: %s [-C] [-L] [-T] [-S] <plik_wejściowy>\n", argv[0]);
                return 1;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Brak pliku wejściowego\n");
        fprintf(stderr, "Użycie: %s [-C] [-L] [-T] [-S] <plik_wejściowy>\n", argv[0]);
        return 1;
    }

    FILE *output = fopen("output.tex", "w");
    if (!output) {
        perror("Nie udało się otworzyć pliku wyjściowego");
        return 1;
    }

    if (validate_input(argv[optind], output, use_black_figures, generate_text_desc, table_layout, sort_configurations) != 0) {
        fclose(output);
        return 1;
    }

    fclose(output);
    return 0; // Sukces
}
#endif

