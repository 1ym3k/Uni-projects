import matplotlib.pyplot as plt
import numpy as np

# Funkcja do wczytywania danych z pliku
def load_data(filename):
    data = np.loadtxt(filename)
    n = data[:, 0]  # Pierwsza kolumna: n
    sum_x = data[:, 1]  # Druga kolumna: suma_x
    sum_y = data[:, 2]  # Trzecia kolumna: suma_y
    return n, sum_x, sum_y

# Wczytanie danych z plików
n_h2, sum_x_h2, sum_y_h2 = load_data("h2.txt")
n_h3, sum_x_h3, sum_y_h3 = load_data("h3.txt")

# Obliczenie błędów H2 i H3
H2 = np.sqrt(sum_x_h2**2 + sum_y_h2**2)  # Błąd H2
H3 = np.sqrt(sum_x_h3**2 + sum_y_h3**2)  # Błąd H3

# Stosunek błędów
ratio_H3_H2 = H3 / H2  # Stosunek H3/H2

# Różnice błędów
diff_H2_H3 = H2 - H3  # Różnica H2 - H3

# Tworzenie wykresów
plt.figure(figsize=(14, 10))

# Wykres 1: Błąd sumowania H2
plt.subplot(2, 2, 1)
plt.plot(n_h2, H2, label="H2", marker="o", markersize=4, linewidth=1, color="blue")
plt.xlabel("n", fontsize=12)
plt.ylabel("Błąd sumowania", fontsize=12)
plt.title("Błąd sumowania H2", fontsize=14)
plt.legend(fontsize=12)
plt.grid()
plt.ticklabel_format(axis='both', style='sci', scilimits=(0,0), useMathText=True)  # Format osi w notacji naukowej
plt.tick_params(axis='both', which='major', labelsize=10)  # Zwiększenie czytelności ticków

# Wykres 2: Błąd sumowania H3
plt.subplot(2, 2, 2)
plt.plot(n_h3, H3, label="H3", marker="o", markersize=4, linewidth=1, color="orange")
plt.xlabel("n", fontsize=12)
plt.ylabel("Błąd sumowania", fontsize=12)
plt.title("Błąd sumowania H3", fontsize=14)
plt.legend(fontsize=12)
plt.grid()
plt.ticklabel_format(axis='both', style='sci', scilimits=(0,0), useMathText=True)  # Format osi w notacji naukowej
plt.tick_params(axis='both', which='major', labelsize=10)  # Zwiększenie czytelności ticków

# Wykres 3: Stosunek błędów (H3/H2)
plt.subplot(2, 2, 3)
plt.plot(n_h2, ratio_H3_H2, label="H3/H2", marker="o", color="red", markersize=4, linewidth=1)
plt.xlabel("n", fontsize=12)
plt.ylabel("Stosunek błędów", fontsize=12)
plt.title("Stosunek błędów (H3/H2)", fontsize=14)
plt.legend(fontsize=12)
plt.grid()
plt.ticklabel_format(axis='y', style='sci', scilimits=(0,0), useMathText=True)  # Format osi w notacji naukowej
plt.tick_params(axis='both', which='major', labelsize=10)  # Zwiększenie czytelności ticków

# Wykres 4: Różnice błędów (H2 - H3)
plt.subplot(2, 2, 4)
plt.plot(n_h2, diff_H2_H3, label="H2 - H3", marker="o", color="purple", markersize=4, linewidth=1)
plt.xlabel("n", fontsize=12)
plt.ylabel("Różnice błędów", fontsize=12)
plt.title("Różnice błędów: (H2 - H3)", fontsize=14)
plt.legend(fontsize=12)
plt.grid()
plt.ticklabel_format(axis='y', style='sci', scilimits=(0,0), useMathText=True)  # Format osi w notacji naukowej
plt.tick_params(axis='both', which='major', labelsize=10)  # Zwiększenie czytelności ticków

# Wyświetlenie wykresów
plt.tight_layout(pad=3.0)  # Dodanie większych odstępów między wykresami
plt.show()