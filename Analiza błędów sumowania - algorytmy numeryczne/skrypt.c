#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846
#define TOLERANCE 1e-10

typedef struct {
    double x;
    double y;
} point;

int compareAsc(const void *a, const void *b) {
    double diff = (*(double*)a) - (*(double*)b);
    return (diff > 0) - (diff < 0);
}

int compareDesc(const void *a, const void *b) {
    double diff = (*(double*)b) - (*(double*)a);
    return (diff > 0) - (diff < 0);
}

point obracaniemtrx(double theta, point a) {
    return (point){
        a.x * cos(theta) - a.y * sin(theta),
        a.x * sin(theta) + a.y * cos(theta)
    };
}

int main() {
    int granica = 100000; // Zadana granica dla n
    FILE *file_h2 = fopen("h2.txt", "w");
    FILE *file_h3 = fopen("h3.txt", "w");

    if (file_h2 == NULL || file_h3 == NULL) {
        printf("Nie mozna otworzyc plikow do zapisu.\n");
        return 1;
    }

    for (int n = 4; n <= granica; n+=1500) {
        double theta = (2 * M_PI) / n;

        // Alokacja dynamiczna zamiast statycznej
        point *v = (point *)malloc((n + 1) * sizeof(point));
        point *w = (point *)malloc((n + 1) * sizeof(point));

        if (v == NULL || w == NULL) {
            printf("Błąd alokacji pamięci dla n = %d\n", n);
            fclose(file_h2);
            fclose(file_h3);
            return 1;
        }

        v[0] = (point){1, 0};
        w[0] = (point){cos(theta) - 1, sin(theta)};

        for (int i = 1; i < (n + 1); i++) {
            v[i].x = v[i - 1].x + w[i - 1].x;
            v[i].y = v[i - 1].y + w[i - 1].y;

            w[i] = obracaniemtrx(theta, w[i - 1]);
        }

        // Badanie teorii H2
        point sumaw = {0, 0};
        for (int i = 0; i < n; i++) {
            sumaw.x += w[i].x;
            sumaw.y += w[i].y;
        }
        fprintf(file_h2, "%d %.15f %.15f\n", n, sumaw.x, sumaw.y);

        // Badanie teorii H3
        double *Xplus = (double *)malloc(n * sizeof(double));
        double *Xminus = (double *)malloc(n * sizeof(double));
        double *Yplus = (double *)malloc(n * sizeof(double));
        double *Yminus = (double *)malloc(n * sizeof(double));

        if (Xplus == NULL || Xminus == NULL || Yplus == NULL || Yminus == NULL) {
            printf("Błąd alokacji pamięci dla n = %d\n", n);
            free(v);
            free(w);
            fclose(file_h2);
            fclose(file_h3);
            return 1;
        }

        int xp = 0, xm = 0, yp = 0, ym = 0;

        for (int i = 0; i < n; i++) {
            if (w[i].x >= 0) Xplus[xp++] = w[i].x;
            else Xminus[xm++] = w[i].x;
            if (w[i].y >= 0) Yplus[yp++] = w[i].y;
            else Yminus[ym++] = w[i].y;
        }

        qsort(Xplus, xp, sizeof(double), compareAsc);
        qsort(Xminus, xm, sizeof(double), compareDesc);
        qsort(Yplus, yp, sizeof(double), compareAsc);
        qsort(Yminus, ym, sizeof(double), compareDesc);

        double Sx_plus = 0, Sx_minus = 0, Sy_plus = 0, Sy_minus = 0;
        for (int i = 0; i < xp; i++) Sx_plus += Xplus[i];
        for (int i = 0; i < xm; i++) Sx_minus += Xminus[i];
        for (int i = 0; i < yp; i++) Sy_plus += Yplus[i];
        for (int i = 0; i < ym; i++) Sy_minus += Yminus[i];

        double final_sx = Sx_plus + Sx_minus;
        double final_sy = Sy_plus + Sy_minus;
        fprintf(file_h3, "%d %.15f %.15f\n", n, final_sx, final_sy);

        // Zwolnienie pamięci
        free(v);
        free(w);
        free(Xplus);
        free(Xminus);
        free(Yplus);
        free(Yminus);
    }

    fclose(file_h2);
    fclose(file_h3);

    return 0;
}