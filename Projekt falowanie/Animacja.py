import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter  # Dodano PillowWriter
import pandas as pd
from matplotlib import cm

# Wczytanie danych z pliku wygenerowanego przez program w C
data = pd.read_csv('wyniki_symulacji.csv')

# Zmiana nazw kolumn dla wygody
data.columns = ['x', 'z', 'phi_num', 'phi_analytic']

# Parametry symulacji
g = 9.81  # przyspieszenie ziemskie [m/s^2]
h = 1.0   # głębokość wody [m]
H = 0.1   # wysokość fali [m]
L = 2*np.pi  # długość fali [m]
k = 2*np.pi/L  # liczba falowa
omega = np.sqrt(g*k*np.tanh(k*h))  # częstotliwość kołowa
T = 2*np.pi/omega  # okres fali

# Przygotowanie siatki przestrzennej
x_unique = np.sort(data['x'].unique())
z_unique = np.sort(data['z'].unique())
X, Z = np.meshgrid(x_unique, z_unique)

# Interpolacja danych numerycznych do regularnej siatki
from scipy.interpolate import griddata
points = data[['x', 'z']].values
phi_num = data['phi_num'].values

# Funkcje pomocnicze
def get_phi_num(x, z):
    return griddata(points, phi_num, (x, z), method='cubic')

def phi_analytic(x, z, t):
    return (g*H/(2*omega)) * (np.cosh(k*(z + h))/np.cosh(k*h)) * np.sin(k*x - omega*t)

def eta(x, t):
    return H/2 * np.cos(k*x - omega*t)

# Inicjalizacja animacji
fig, ax2 = plt.subplots(figsize=(12, 6))

# Wykres 2: Powierzchnia swobodna
line, = ax2.plot(x_unique, [0]*len(x_unique), 'b-', linewidth=2)
ax2.set_xlim(0, L)
ax2.set_ylim(-H, H)
ax2.set_xlabel('x [m]')
ax2.set_ylabel('η [m]')
ax2.set_title('Powierzchnia swobodna wody')
ax2.grid(True)

# Cząstki wody do wizualizacji ruchu
particles_x = np.linspace(0, L, 15)
particles_z = np.linspace(-h*0.8, -h*0.1, 3)
particles_X, particles_Z = np.meshgrid(particles_x, particles_z)
particles = ax2.scatter([], [], c='k', s=20, alpha=0.7)

# Funkcja animacji
def update(t):
    # Aktualizacja powierzchni swobodnej
    line.set_ydata(eta(x_unique, t))
    
    # Aktualizacja pozycji cząstek
    displacement = 0.1 * np.sin(k*particles_X - omega*t) * np.exp(k*(particles_Z + h))
    particles.set_offsets(np.column_stack((particles_X + displacement, particles_Z)))
    
    return line, particles

# Tworzenie animacji
ani = FuncAnimation(fig, update, frames=np.linspace(0, 2*T, 60),
                    interval=100, blit=False)
ani.save('animacja.gif', writer=PillowWriter(fps=15), dpi=100)  # Zapis do GIF

plt.tight_layout()
plt.show()

