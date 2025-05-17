#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846

// Stałe symulacji
#define N 6               // Liczba punktów siatki (N+1 punktów w każdym kierunku)
#define H_DEPTH 1.0       // Głębokość wody [m]
#define H_AMPLITUDE 0.1   // Amplituda fali [m]
#define L (2.0 * M_PI)    // Długość dziedziny [m]
#define G 9.81            // Przyspieszenie grawitacyjne [m/s²]
#define k (2.0 * M_PI / L)  // Liczba falowa [rad/m]
#define omega (sqrt(G * k * tanh(k * H_DEPTH)))  // Częstość kołowa [rad/s]

typedef struct {
    int row;
    int col;
    double val;
} SparseElement;

typedef struct {
    SparseElement* elements;
    int size;
    int n;
} SparseMatrix;

void add_element(SparseMatrix* mat, int row, int col, double val) {
    if (fabs(val) < 1e-10) return;
    mat->elements[mat->size].row = row;
    mat->elements[mat->size].col = col;
    mat->elements[mat->size].val = val;
    mat->size++;
}

double get_element(SparseMatrix* mat, int row, int col) {
    for (int i = 0; i < mat->size; i++) {
        if (mat->elements[i].row == row && mat->elements[i].col == col) {
            return mat->elements[i].val;
        }
    }
    return 0.0;
}

void set_element(SparseMatrix* mat, int row, int col, double val) {
    for (int i = 0; i < mat->size; i++) {
        if (mat->elements[i].row == row && mat->elements[i].col == col) {
            if (fabs(val) < 1e-10) {
                mat->elements[i] = mat->elements[mat->size - 1];
                mat->size--;
            } else {
                mat->elements[i].val = val;
            }
            return;
        }
    }
    add_element(mat, row, col, val);
}

SparseMatrix* build_equation_system(double* b) {
    int n = (N + 1) * (N + 1);
    SparseMatrix* mat = malloc(sizeof(SparseMatrix));
    mat->n = n;
    mat->size = 0;
    mat->elements = malloc(sizeof(SparseElement) * 5 * n);

    double hx = L / N;
    double hz = H_DEPTH / N;

    for (int x_idx = 0; x_idx <= N; x_idx++) {
        for (int z_idx = 0; z_idx <= N; z_idx++) {
            int idx = x_idx * (N + 1) + z_idx;
            double x = x_idx * hx;
            double z = -H_DEPTH + z_idx * hz;

            if (z_idx == 0) { // Warunek brzegowy na dnie
                add_element(mat, idx, idx, 1.0);
                add_element(mat, idx, idx + 1, -1.0);
                b[idx] = 0.0;
            } else if (z_idx == N) { // Warunek na powierzchni
                add_element(mat, idx, idx, 1.0);
                b[idx] = (G * H_AMPLITUDE / (2 * omega)) * sin(k * x);
            } else if (x_idx == 0 || x_idx == N) { // Warunki brzegowe boczne
                add_element(mat, idx, idx, 1.0);
                b[idx] = (G * H_AMPLITUDE / (2 * omega)) * (cosh(k * (z + H_DEPTH)) / cosh(k * H_DEPTH)) * sin(k * x);
            } else { // Równanie Laplace'a we wnętrzu
                add_element(mat, idx, idx - (N + 1), 1.0 / (hx * hx));
                add_element(mat, idx, idx + (N + 1), 1.0 / (hx * hx));
                add_element(mat, idx, idx - 1, 1.0 / (hz * hz));
                add_element(mat, idx, idx + 1, 1.0 / (hz * hz));
                add_element(mat, idx, idx, -2.0 / (hx * hx) - 2.0 / (hz * hz));
                b[idx] = 0.0;
            }
        }
    }
    return mat;
}

void gauss_elimination(SparseMatrix* mat, double* b, double* x) {
    int n = mat->n;

    for (int step = 0; step < n - 1; step++) {
        // Wybór elementu podstawowego
        double max_val = fabs(get_element(mat, step, step));
        int pivot_row = step;
        for (int i = step + 1; i < n; i++) {
            double val = fabs(get_element(mat, i, step));
            if (val > max_val) {
                max_val = val;
                pivot_row = i;
            }
        }

        if (max_val < 1e-10) {
            printf("Macierz osobliwa - brak rozwiązania!\n");
            exit(1);
        }

        if (pivot_row != step) {
            // Zamiana wierszy
            for (int j = 0; j < mat->size; j++) {
                if (mat->elements[j].row == step) mat->elements[j].row = pivot_row;
                else if (mat->elements[j].row == pivot_row) mat->elements[j].row = step;
            }
            double tmp = b[step];
            b[step] = b[pivot_row];
            b[pivot_row] = tmp;
        }

        // Eliminacja Gaussa
        double pivot = get_element(mat, step, step);
        for (int i = step + 1; i < n; i++) {
            double factor = get_element(mat, i, step) / pivot;
            for (int j = step; j < n; j++) {
                double val = get_element(mat, i, j) - factor * get_element(mat, step, j);
                set_element(mat, i, j, val);
            }
            b[i] -= factor * b[step];
        }
    }

    // Rozwiązanie układu równań
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += get_element(mat, i, j) * x[j];
        }
        x[i] = (b[i] - sum) / get_element(mat, i, i);
    }
}

double analytic_solution(double x, double z) {
    return (G * H_AMPLITUDE / (2 * omega)) * (cosh(k * (z + H_DEPTH)) / cosh(k * H_DEPTH)) * sin(k * x);
}

int main() {

    // Inicjalizacja układu równań
    double* b = malloc(sizeof(double) * (N + 1) * (N + 1));
    SparseMatrix* mat = build_equation_system(b);
    
    // Rozwiązanie układu równań
    double* phi_num = malloc(sizeof(double) * (N + 1) * (N + 1));
    gauss_elimination(mat, b, phi_num);

    printf("\nPORÓWNANIE ROZWIĄZAŃ NUMERYCZNYCH I ANALITYCZNYCH:\n");
    printf("(Format: [LOKALIZACJA] φ(x,z) = WART_NUM (analityczna: WART_ANALYTIC, różnica: BŁĄD)\n\n");
    
    for (int x_idx = 0; x_idx <= N; x_idx++) {
        double x = x_idx * (L / N);
        printf("\n--- Wyniki dla x = %.2f m (%.1f%% długości fali) ---\n", 
              x, (x/L)*100);
        
        for (int z_idx = 0; z_idx <= N; z_idx++) {
            int idx = x_idx * (N + 1) + z_idx;
            double z = -H_DEPTH + z_idx * (H_DEPTH / N);
            double phi_analytic = analytic_solution(x, z);
            
            const char* location;
            if (z_idx == 0) location = "DNO";
            else if (z_idx == N) location = "POWIERZCHNIA";
            else if (x_idx == 0 || x_idx == N) location = "BRZEG";
            else location = "WNĘTRZE";
            
            printf("[%-10s] φ(%.2f, %5.2f) = %8.6f (analityczna: %8.6f, błąd: %8.6f)\n",
                  location, x, z, phi_num[idx], phi_analytic, fabs(phi_num[idx] - phi_analytic));
        }
    }

    // Zapis wyników do pliku
    FILE* file = fopen("wyniki_symulacji.csv", "w");
    fprintf(file, "x[m],z[m],potencjal_num,potencjal_analytic\n");
    for (int x_idx = 0; x_idx <= N; x_idx++) {
        for (int z_idx = 0; z_idx <= N; z_idx++) {
            int idx = x_idx * (N + 1) + z_idx;
            double x = x_idx * (L / N);
            double z = -H_DEPTH + z_idx * (H_DEPTH / N);
            double phi_analytic = analytic_solution(x, z);
            fprintf(file, "%.6f,%.6f,%.6f,%.6f\n",
                   x, z, phi_num[idx], phi_analytic);
        }
    }
    fclose(file);
    printf("\nZapisano wyniki do pliku 'wyniki_symulacji.csv'\n");

    // Zwolnienie pamięci
    free(mat->elements);
    free(mat);
    free(b);
    free(phi_num);

    printf("ZAKOŃCZONO POMYŚLNIE\n");
    return 0;
}