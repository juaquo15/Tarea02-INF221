import subprocess
import os
import time
import sys
import platform

def ejecutar_comando(comando, descripcion):
    print(f"\n{'='*50}")
    print(f"Ejecutando {descripcion}...")
    print(f"{'='*50}")
    
    try:
        # En Windows, necesitamos usar la ruta completa o el directorio actual
        if platform.system() == 'Windows':
            comando = comando.replace('./', '')  # Eliminar ./ si existe
            
        proceso = subprocess.run(comando, 
                               stdout=subprocess.PIPE, 
                               stderr=subprocess.PIPE,
                               text=True,
                               shell=True)
        
        if proceso.returncode == 0:
            print(f"✓ {descripcion} completado exitosamente")
        else:
            print(f"✗ Error en {descripcion}")
            print("Error:", proceso.stderr)
            sys.exit(1)
            
    except Exception as e:
        print(f"✗ Error al ejecutar {descripcion}:")
        print(str(e))
        sys.exit(1)

def main():
    # Verificar que existan los archivos necesarios
    archivos_requeridos = [
        "generador.py",
        "fuerzaDinamica.cpp",
        "analisis.py"
    ]
    
    for archivo in archivos_requeridos:
        if not os.path.exists(archivo):
            print(f"Error: No se encuentra el archivo {archivo}")
            sys.exit(1)

    # 1. Compilar el programa C++
    print("\nCompilando programa C++...")
    ejecutar_comando("g++ fuerzaDinamica.cpp -o fuerzaDinamica.exe", "compilación de C++")

    # 2. Ejecutar el generador de archivos
    print("\nGenerando archivos de prueba...")
    ejecutar_comando("python generador.py", "generador de archivos")

    # Pequeña pausa para asegurar que los archivos se hayan escrito
    time.sleep(1)

    # 3. Ejecutar el programa C++
    print("\nEjecutando programa C++...")
    ejecutar_comando("fuerzaDinamica.exe", "programa C++")

    # Pequeña pausa para asegurar que los resultados se hayan escrito
    time.sleep(1)

    # 4. Ejecutar el análisis
    print("\nRealizando análisis...")
    ejecutar_comando("python analisis.py", "análisis de resultados")

    print("\n✓ Proceso completo finalizado exitosamente")
    print("\nArchivos generados:")
    print("- Imágenes PNG y SVG de las gráficas")
    print("- Archivos de resultados y estadísticas")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\nProceso interrumpido por el usuario")
        sys.exit(1)
    except Exception as e:
        print("\n\nError inesperado:", str(e))
        sys.exit(1)