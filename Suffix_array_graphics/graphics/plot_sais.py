import pandas as pd
import matplotlib.pyplot as plt
import os
import sys
import numpy as np

# === Configuración de rutas ===
csv_path = "data/resultados_sais.csv"
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

# === Notaciones asintóticas ===
o1 = np.ones_like(n_teorico)
logn = np.log2(n_teorico)
n_lineal = n_teorico
nlogn = n_teorico * np.log2(n_teorico)
n2 = n_teorico ** 2

# === Constantes de escala uniformes para todos ===
escala_tiempo = t_ref / n_ref
escala_memoria = m_ref / n_ref

# === Escalamiento proporcional
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

# === Cortar curvas si se exceden del valor máximo real
def cortar(curva, ymax):
    return np.where(curva <= ymax, curva, np.nan)

o1_t = cortar(o1_t, t_ref)
logn_t = cortar(logn_t, t_ref)
n_t = cortar(n_t, t_ref)
nlogn_t = cortar(nlogn_t, t_ref)
n2_t = cortar(n2_t, t_ref)

o1_m = cortar(o1_m, m_ref)
logn_m = cortar(logn_m, m_ref)
n_m = cortar(n_m, m_ref)
nlogn_m = cortar(nlogn_m, m_ref)
n2_m = cortar(n2_m, m_ref)

# === Gráfica 1: Tiempo de ejecución ===
plt.figure(figsize=(12, 6))
plt.plot(n_real, tiempo_real, 'o-', linewidth=2, label="Tiempo Ejecución: SA-IS")

plt.plot(n_teorico, o1_t, '--', label="O(1)")
plt.plot(n_teorico, logn_t, '--', label="O(log n)")
plt.plot(n_teorico, n_t, '--', label="O(n)")
plt.plot(n_teorico, nlogn_t, '--', label="O(n log n)")
plt.plot(n_teorico, n2_t, '--', label="O(n²)")

plt.title("Tiempo de Ejecución - SA-IS")
plt.xlabel("Tamaño del String (n)")
plt.ylabel("Tiempo (segundos)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(output_folder, "tiempo_vs_tamaño_sais_escalado_proporcional.png"))
plt.show()

# === Gráfica 2: Memoria Pico ===
plt.figure(figsize=(12, 6))
plt.plot(n_real, memoria_real, 'o-', color='red', linewidth=2, label="Memoria real SA-IS")

plt.plot(n_teorico, o1_m, '--', label="O(1)")
plt.plot(n_teorico, logn_m, '--', label="O(log n)")
plt.plot(n_teorico, n_m, '--', label="O(n)")
plt.plot(n_teorico, nlogn_m, '--', label="O(n log n)")
plt.plot(n_teorico, n2_m, '--', label="O(n²)")

plt.title("Memoria Pico - SA-IS")
plt.xlabel("Tamaño del String (n)")
plt.ylabel("Memoria Pico (KB)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(output_folder, "memoria_vs_tamaño_sais_escalado_proporcional.png"))
plt.show()
