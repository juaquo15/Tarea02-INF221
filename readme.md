# Análisis de Algoritmos de Edición de Cadenas



## Descripción General

El proyecto consta de cuatro scripts principales:

### 1. generador.py
Script en Python que genera diversos casos de prueba para evaluar los algoritmos. Genera dos conjuntos de datos:
- **Casos FB y DP**: Cadenas de longitud 1-13 caracteres
- **Casos solo DP**: Cadenas de longitud hasta 2500 caracteres
Tambien genera los archivos de los costos, los cuales tienen las siguientes particularidades:
- **cost_delete**: costo fijo de 1.
- **cost_insert**: costo de 2 si el caracter esta en posicion par y 1 si esta en una posicion impar [a,b,c,d,...,z].
- **cost_replace**: costo aleatorio entre 1 y 2.
- **cost_transpose**: costo aleatorio entre 1 y 2.

#### Tipos de Cadenas Generadas:
1. **Cadenas idénticas**: Pares de cadenas iguales (ej: "abc" - "abc")
2. **Cadenas con sustitución**: Una letra diferente en el medio (ej: "abc" - "axc")
3. **Cadenas con inserción**: Una letra adicional en el medio (ej: "abc" - "abxc")
4. **Cadenas con eliminación**: Una letra menos en el medio (ej: "abc" - "ac")
5. **Cadenas completamente diferentes**: Pares de cadenas aleatorias
6. **Cadenas repetitivas**: Patrones que se repiten con una variación
7. **Cadenas con desplazamiento**: Una cadena rotada respecto a la otra

### 2. fuerzaDinamica.cpp
Implementación en C++ de los dos algoritmos de distancia de edición:
- **Fuerza Bruta**: Implementación recursiva (para cadenas cortas)
- **Programación Dinámica**: Implementación iterativa (para todas las longitudes)


### 3. analisis.py
Script de visualización que genera múltiples gráficos y análisis estadísticos:
1. **Costos por tipo de cadena**: Gráfico de barras del costo promedio
2. **Longitud vs Costo**: Gráfico de dispersión
3. **Distribución de costos**: Gráfico de violín
4. **Correlación de variables**: Heatmap
5. **Proporción de tipos**: Gráfico de barras apiladas

Genera archivos en formatos:
- PNG para uso general
- SVG para edición posterior
- TXT con estadísticas detalladas

### 4. Ejecutar.py
Script maestro que orquesta la ejecución de todo el proceso:
1. Compila el código C++
2. Ejecuta el generador de casos
3. Ejecuta los algoritmos de edición
4. Realiza el análisis y genera visualizaciones


## Uso

Para ejecutar todo el proceso:
```bash
python Ejecutar.py
```

Para ejecutar scripts individuales:
```bash
python generador.py
g++ fuerzaDinamica.cpp -o fuerzaDinamica.exe
./fuerzaDinamica.exe
python analisis.py
```

## Archivos Generados

### Archivos de Entrada
- `casos_*_fb_dp.txt`: Casos para ambos algoritmos
- `casos_*_dp.txt`: Casos solo para DP
- `cost_*.txt`: Archivos de costos de operaciones

### Archivos de Salida
- `resultados.txt`: Resultados detallados de cada caso
- `promedios_tiempos.txt`: Promedios de tiempo por longitud
- Gráficos en PNG y SVG:
  - `analisis_tiempos.png/svg`
  - `comparacion_tiempos.png/svg`
  - `costos_por_tipo.png/svg`
  - `longitud_vs_costo.png/svg`
  - `distribucion_costos.png/svg`
  - `correlacion_variables.png/svg`
  - `proporcion_tipos.png/svg`
- `analisis_por_tipo.txt`: Estadísticas por tipo de cadena

## Notas sobre los Casos de Prueba

- **Cadenas Cortas (FB y DP)**:
  - Longitud: 1-13 caracteres
  - Usadas para comparar ambos enfoques
  - Permiten validar la exactitud de los resultados

- **Cadenas Largas (solo DP)**:
  - Longitud: hasta 2500 caracteres
  - Usadas para evaluar el rendimiento de DP
  - Generadas en incrementos de 250 caracteres
