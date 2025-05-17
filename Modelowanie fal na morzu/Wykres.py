import matplotlib.pyplot as plt
import numpy as np

def load_data(filename):
    """Wczytuje dane z pliku CSV"""
    data = np.genfromtxt(filename, delimiter=',', skip_header=1)
    x = data[:, 0]
    z = data[:, 1]
    phi = data[:, 2]
    phi_analytic = data[:, 3]
    return x, z, phi, phi_analytic

def create_line_plot(x, z, phi, phi_analytic):
    """Tworzy wykres liniowy 2D porównujący rozwiązania"""
    # Wybieramy jedną wartość z (np. średnią)
    unique_z = np.unique(z)

    #selected_z = unique_z[len(unique_z)//2]  # wybieramy środkową wartość z
    selected_z = 0
    
    # Filtrujemy dane dla wybranej wartości z
    mask = z == selected_z
    x_filtered = x[mask]
    phi_filtered = phi[mask]
    phi_analytic_filtered = phi_analytic[mask]
    
    # Sortujemy dane względem x
    sort_idx = np.argsort(x_filtered)
    x_sorted = x_filtered[sort_idx]
    phi_sorted = phi_filtered[sort_idx]
    phi_analytic_sorted = phi_analytic_filtered[sort_idx]
    
    # Tworzymy wykres
    plt.figure(figsize=(12, 6))
    
    # Rozwiązanie numeryczne
    plt.plot(x_sorted, phi_sorted, 'b-', linewidth=2, label='Rozwiązanie numeryczne')
    
    # Rozwiązanie analityczne
    plt.plot(x_sorted, phi_analytic_sorted, 'r--', linewidth=2, label='Rozwiązanie analityczne')
    
    # Różnica między rozwiązaniami
    plt.plot(x_sorted, phi_sorted - phi_analytic_sorted, 'g:', linewidth=1.5, label='Różnica')
    
    # Konfiguracja wykresu
    plt.xlabel('Współrzędna x [m]', fontsize=12)
    plt.ylabel('Wartość potencjału φ', fontsize=12)
    plt.title(f'Porównanie rozwiązań dla z = {selected_z:.2f} m', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, linestyle='--', alpha=0.7)
    
    # Dopasowanie zakresu osi Y
    y_min = min(np.min(phi_sorted), np.min(phi_analytic_sorted))
    y_max = max(np.max(phi_sorted), np.max(phi_analytic_sorted))
    plt.ylim(1.1*y_min, 1.1*y_max)
    
    plt.tight_layout()
    plt.show()

# Wczytanie danych i generowanie wykresu
x, z, phi, phi_analytic = load_data('wyniki_symulacji.csv')
create_line_plot(x, z, phi, phi_analytic)