import pandas as pd
import matplotlib.pyplot as plt

#-------------------------------------------ALGORITMO SA-IS-------------------------------------------#

# Leer archivo CSV
df1 = pd.read_csv('SA-IS/resultados_SA-IS.csv')

# Configurar estilo
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (10, 6)
plt.rcParams['font.size'] = 12

# Gráfico de Tiempo
plt.figure()
plt.plot(df1['Tamaño'], df1['Tiempo(s)'], marker='o', color='steelblue')
plt.title('Tiempo de Ejecución vs Tamaño del Texto (SA-IS)')
plt.xlabel('Tamaño del Texto')
plt.ylabel('Tiempo (segundos)')
plt.savefig('Results/SA-IS/tiempo_vs_tamano_SA-IS.png')
plt.show()

# Gráfico de Memoria
plt.figure()
plt.plot(df1['Tamaño'], df1['Memoria Pico (KB)'], marker='o', color='firebrick')
plt.title('Memoria Pico vs Tamaño del Texto (SA-IS)')
plt.xlabel('Tamaño del Texto')
plt.ylabel('Memoria Pico (KB)')
plt.savefig('Results/SA-IS/memoria_vs_tamano_SA-IS.png')
plt.show()


#-------------------------------------------ALGORITMO PREFIX-DOUBLING-------------------------------------------#

df2 = pd.read_csv('Prefix-Doubling/resultados_prefix_doubling.csv')

# Configurar estilo
plt.style.use('seaborn-v0_8-darkgrid')
plt.rcParams['figure.figsize'] = (10, 6)
plt.rcParams['font.size'] = 12

# Gráfico de Tiempo
plt.figure()
plt.plot(df2['Tamaño'], df2['Tiempo(s)'], marker='o', color='steelblue')
plt.title('Tiempo de Ejecución vs Tamaño del Texto (Prefix Doubling)')
plt.xlabel('Tamaño del Texto')
plt.ylabel('Tiempo (segundos)')
plt.savefig('Results/Prefix-Doubling/tiempo_vs_tamano_prefix-doubling.png')
plt.show()

# Gráfico de Memoria
plt.figure()
plt.plot(df2['Tamaño'], df2['Memoria Pico (KB)'], marker='o', color='firebrick')
plt.title('Memoria Pico vs Tamaño del Texto (Prefix Doubling)')
plt.xlabel('Tamaño del Texto')
plt.ylabel('Memoria Pico (KB)')
plt.savefig('Results/Prefix-Doubling/memoria_vs_tamano_prefix-doubling.png')
plt.show()