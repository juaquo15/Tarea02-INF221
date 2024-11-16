import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def analizar_tiempos():
    # Leer el archivo de promedios
    tiempos = []
    current_length = None
    
    with open('promedios_tiempos.txt', 'r') as file:
        for line in file:
            line = line.strip()
            if line.startswith('Longitud:'):
                current_length = int(line.split()[1])
            elif line.startswith('Tiempo promedio FB:'):
                fb_time = float(line.split()[3])
                tiempos.append({
                    'longitud': current_length,
                    'tiempo': fb_time,
                    'enfoque': 'Fuerza Bruta'
                })
            elif line.startswith('Tiempo promedio DP:'):
                dp_time = float(line.split()[3])
                tiempos.append({
                    'longitud': current_length,
                    'tiempo': dp_time,
                    'enfoque': 'Programación Dinámica'
                })

    # Crear DataFrame
    df = pd.DataFrame(tiempos)

    # Configurar el estilo de las gráficas
    plt.style.use('default')
    sns.set_style("whitegrid")
    
    # Crear la figura con dos subplots
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))
    plt.subplots_adjust(top=0.9)
    fig.suptitle('Análisis de Tiempos de Ejecución', fontsize=16)

    # Gráfica para Fuerza Bruta
    fb_data = df[df['enfoque'] == 'Fuerza Bruta']
    if not fb_data.empty:
        sns.barplot(data=fb_data, x='longitud', y='tiempo', ax=ax1, color='skyblue')
        ax1.set_title('Tiempo Promedio - Fuerza Bruta')
        ax1.set_xlabel('Longitud de las cadenas')
        ax1.set_ylabel('Tiempo (ms)')

    # Gráfica para Programación Dinámica
    dp_data = df[df['enfoque'] == 'Programación Dinámica']
    sns.barplot(data=dp_data, x='longitud', y='tiempo', ax=ax2, color='lightgreen')
    ax2.set_title('Tiempo Promedio - Programación Dinámica')
    ax2.set_xlabel('Longitud de las cadenas')
    ax2.set_ylabel('Tiempo (ms)')

    # Rotar las etiquetas del eje x para mejor legibilidad
    ax1.tick_params(axis='x', rotation=45)
    ax2.tick_params(axis='x', rotation=45)

    # Ajustar el layout
    plt.tight_layout()

    # Guardar la figura en ambos formatos
    plt.savefig('analisis_tiempos.png', dpi=300, bbox_inches='tight')
    plt.savefig('analisis_tiempos.svg', format='svg', bbox_inches='tight')
    
    # Mostrar la primera figura
    plt.show()
    
    # Crear nueva figura para la gráfica de líneas comparativa
    plt.figure(figsize=(12, 6))
    sns.lineplot(data=df, x='longitud', y='tiempo', hue='enfoque', marker='o')
    plt.title('Comparación de Tiempos de Ejecución')
    plt.xlabel('Longitud de las cadenas')
    plt.ylabel('Tiempo (ms)')
    plt.xticks(rotation=45)
    plt.legend(title='Enfoque')
    plt.grid(True, linestyle='--', alpha=0.7)
    
    # Guardar la segunda figura en ambos formatos
    plt.savefig('comparacion_tiempos.png', dpi=300, bbox_inches='tight')
    plt.savefig('comparacion_tiempos.svg', format='svg', bbox_inches='tight')
    
    # Mostrar la segunda figura
    plt.show()

    # Generar estadísticas básicas
    with open('estadisticas_tiempos.txt', 'w') as f:
        f.write("Estadísticas de tiempos de ejecución\n")
        f.write("====================================\n\n")
        
        f.write("Fuerza Bruta:\n")
        f.write("--------------\n")
        if not fb_data.empty:
            f.write(f"Tiempo mínimo: {fb_data['tiempo'].min():.3f} ms\n")
            f.write(f"Tiempo máximo: {fb_data['tiempo'].max():.3f} ms\n")
            f.write(f"Tiempo promedio: {fb_data['tiempo'].mean():.3f} ms\n")
            f.write(f"Desviación estándar: {fb_data['tiempo'].std():.3f} ms\n\n")
        else:
            f.write("No hay datos disponibles para Fuerza Bruta\n\n")
        
        f.write("Programación Dinámica:\n")
        f.write("----------------------\n")
        f.write(f"Tiempo mínimo: {dp_data['tiempo'].min():.3f} ms\n")
        f.write(f"Tiempo máximo: {dp_data['tiempo'].max():.3f} ms\n")
        f.write(f"Tiempo promedio: {dp_data['tiempo'].mean():.3f} ms\n")
        f.write(f"Desviación estándar: {dp_data['tiempo'].std():.3f} ms\n")

if __name__ == "__main__":
    try:
        analizar_tiempos()
        print("Análisis completado. Se han generado los archivos:")
        print("- analisis_tiempos.png y analisis_tiempos.svg (gráficas de barras)")
        print("- comparacion_tiempos.png y comparacion_tiempos.svg (gráfica de líneas)")
        print("- estadisticas_tiempos.txt (estadísticas descriptivas)")
    except Exception as e:
        print(f"Error durante el análisis: {e}")