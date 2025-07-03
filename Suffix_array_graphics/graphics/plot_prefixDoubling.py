import pandas as pd
import matplotlib.pyplot as plt
import os
import sys
import numpy as np

# === Configuración de rutas ===
csv_path = "data/resultados_prefix_doubling.csv"
output_folder = "data"

if not os.path.isfile(csv_path):
    print(f"Error: No se encontró el archivo '{csv_path}'.")
    sys.exit(1)

# === Leer datos reales ===
df = pd.read_csv(csv_path)
n_real = df["Tamaño"]
tiempo_real = df["Tiempo(s)"]
memoria_real = df["Memoria Pico (KB)"]

# === Parámetros base ===
n_ref = n_real.iloc[-1]
t_ref = tiempo_real.iloc[-1]
m_ref = memoria_real.iloc[-1]

# === Dominio teórico ===
n_teorico = np.arange(1, n_ref + 1)

# === Notaciones teóricas ===
o1 = np.ones_like(n_teorico)
logn = np.log2(n_teorico)
n_lineal = n_teorico
nlogn = n_teorico * np.log2(n_teorico)
n2 = n_teorico**2

# === Escalamiento proporcional respecto al último punto
escala_tiempo = t_ref / n_ref
escala_memoria = m_ref / n_ref

# === Escaladas
o1_t = o1 * escala_tiempo
logn_t = logn * escala_tiempo
n_t = n_lineal * escala_tiempo
nlogn_t = nlogn * escala_tiempo
n2_t = n2 * escala_tiempo

o1_m = o1 * escala_memoria
logn_m = logn * escala_memoria
n_m = n_lineal * escala_memoria
nlogn_m = nlogn * escala_memoria
n2_m = n2 * escala_memoria

# === Función para cortar curvas que exceden el valor máximo real
def cortar_curva(curva, y_max):
    return np.where(curva <= y_max, curva, np.nan)

o1_t = cortar_curva(o1_t, t_ref)
logn_t = cortar_curva(logn_t, t_ref)
n_t = cortar_curva(n_t, t_ref)
nlogn_t = cortar_curva(nlogn_t, t_ref)
n2_t = cortar_curva(n2_t, t_ref)

o1_m = cortar_curva(o1_m, m_ref)
logn_m = cortar_curva(logn_m, m_ref)
n_m = cortar_curva(n_m, m_ref)
nlogn_m = cortar_curva(nlogn_m, m_ref)
n2_m = cortar_curva(n2_m, m_ref)

# === Gráfica 1: Tiempo
plt.figure(figsize=(12, 6))
plt.plot(n_real, tiempo_real, 'o-', linewidth=2, label="Tiempo real")

plt.plot(n_teorico, o1_t, '--', label="O(1)")
plt.plot(n_teorico, logn_t, '--', label="O(log n)")
plt.plot(n_teorico, n_t, '--', label="O(n)")
plt.plot(n_teorico, nlogn_t, '--', label="O(n log n)")
plt.plot(n_teorico, n2_t, '--', label="O(n²)")

plt.title("Tiempo de Ejecución - Prefix Doubling")
plt.xlabel("Tamaño del String (n)")
plt.ylabel("Tiempo (segundos)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(output_folder, "tiempo_vs_tamaño_prefix_doubling_escalado_proporcional.png"))
plt.show()

# === Gráfica 2: Memoria
plt.figure(figsize=(12, 6))
plt.plot(n_real, memoria_real, 'o-', color='red', linewidth=2, label="Memoria real")

plt.plot(n_teorico, o1_m, '--', label="O(1)")
plt.plot(n_teorico, logn_m, '--', label="O(log n)")
plt.plot(n_teorico, n_m, '--', label="O(n)")
plt.plot(n_teorico, nlogn_m, '--', label="O(n log n)")
plt.plot(n_teorico, n2_m, '--', label="O(n²)")

plt.title("Memoria Pico - Prefix Doubling")
plt.xlabel("Tamaño del String (n)")
plt.ylabel("Memoria Pico (KB)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(output_folder, "memoria_vs_tamaño_prefix_doubling_escalado_proporcional.png"))
plt.show()
