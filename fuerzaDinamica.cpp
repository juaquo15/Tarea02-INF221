#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
#include <climits>
#include <numeric>
#include <algorithm>
#include <utility>
#include <iomanip>

using namespace std;

// Variables globales para las tablas de costos
vector<int> cost_insert(26);
vector<int> cost_delete(26);
vector<vector<int>> cost_replace(26, vector<int>(26));
vector<vector<int>> cost_transpose(26, vector<int>(26));

// Funciones para cargar las tablas de costos
bool loadCostTable(const string& filename, vector<int>& costs) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error al abrir " << filename << endl;
        return false;
    }
    
    for (int i = 0; i < 26; i++) {
        if (!(file >> costs[i])) {
            cerr << "Error al leer costos de " << filename << endl;
            return false;
        }
    }
    return true;
}

bool loadCostMatrix(const string& filename, vector<vector<int>>& costs) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error al abrir " << filename << endl;
        return false;
    }
    
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            if (!(file >> costs[i][j])) {
                cerr << "Error al leer costos de " << filename << endl;
                return false;
            }
        }
    }
    return true;
}

// Funciones de costos
int costo_sub(char a, char b) {
    if (a == b) return 0;
    int i = a - 'a';
    int j = b - 'a';
    if (i < 0 || i >= 26 || j < 0 || j >= 26) return 2;
    return cost_replace[i][j];
}

int costo_ins(char b) {
    int j = b - 'a';
    if (j < 0 || j >= 26) return 1;
    return cost_insert[j];
}

int costo_del(char a) {
    int i = a - 'a';
    if (i < 0 || i >= 26) return 1;
    return cost_delete[i];
}

int costo_trans(char a, char b) {
    int i = a - 'a';
    int j = b - 'a';
    if (i < 0 || i >= 26 || j < 0 || j >= 26) return 1;
    return cost_transpose[i][j];
}

// Función de fuerza bruta
int editDistanceBruteForce(const string& s1, const string& s2, int i, int j) {
    // Si primera cadena está vacía, insertar todos los caracteres restantes de s2
    if (i == s1.length()) {
        int cost = 0;
        for(int k = j; k < s2.length(); k++) {
            cost += costo_ins(s2[k]);
        }
        return cost;
    }
    // Si segunda cadena está vacía, eliminar todos los caracteres restantes de s1
    if (j == s2.length()) {
        int cost = 0;
        for(int k = i; k < s1.length(); k++) {
            cost += costo_del(s1[k]);
        }
        return cost;
    }

    // Si los caracteres son iguales, continuar sin costo
    if (s1[i] == s2[j]) {
        return editDistanceBruteForce(s1, s2, i + 1, j + 1);
    }

    int minCost = INT_MAX;

    // Sustitución
    int cost_subst = costo_sub(s1[i], s2[j]) + editDistanceBruteForce(s1, s2, i + 1, j + 1);
    minCost = min(minCost, cost_subst);

    // Inserción
    int cost_ins = costo_ins(s2[j]) + editDistanceBruteForce(s1, s2, i, j + 1);
    minCost = min(minCost, cost_ins);

    // Eliminación
    int cost_del = costo_del(s1[i]) + editDistanceBruteForce(s1, s2, i + 1, j);
    minCost = min(minCost, cost_del);

    // Transposición
    if (i + 1 < s1.length() && j + 1 < s2.length() && 
        s1[i] == s2[j + 1] && s1[i + 1] == s2[j]) {
        int cost_trans = costo_trans(s1[i], s1[i + 1]) + 
                        editDistanceBruteForce(s1, s2, i + 2, j + 2);
        minCost = min(minCost, cost_trans);
    }

    return minCost;
}

// Función de programación dinámica
int editDistanceDynamic(const string& s1, const string& s2) {
    vector<vector<int>> dp(s1.length() + 1, vector<int>(s2.length() + 1, INT_MAX));

    // Caso base: cadena vacía
    dp[0][0] = 0;
    
    // Primera columna: costo acumulado de eliminar caracteres de s1
    for(size_t i = 1; i <= s1.length(); ++i) {
        dp[i][0] = dp[i-1][0] + costo_del(s1[i-1]);
    }
    
    // Primera fila: costo acumulado de insertar caracteres de s2
    for(size_t j = 1; j <= s2.length(); ++j) {
        dp[0][j] = dp[0][j-1] + costo_ins(s2[j-1]);
    }

    // Rellenar el resto de la matriz
    for(size_t i = 1; i <= s1.length(); ++i) {
        for(size_t j = 1; j <= s2.length(); ++j) {
            // Si los caracteres son iguales
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
                continue;
            }

            // Calcular costos de operaciones
            int sustitucion = dp[i-1][j-1] + costo_sub(s1[i-1], s2[j-1]);
            int insercion = dp[i][j-1] + costo_ins(s2[j-1]);
            int eliminacion = dp[i-1][j] + costo_del(s1[i-1]);

            dp[i][j] = min({sustitucion, insercion, eliminacion});

            // Verificar transposición
            if (i > 1 && j > 1 && 
                s1[i-2] == s2[j-1] && s1[i-1] == s2[j-2]) {
                int transposicion = dp[i-2][j-2] + costo_trans(s1[i-2], s1[i-1]);
                dp[i][j] = min(dp[i][j], transposicion);
            }
        }
    }

    return dp[s1.length()][s2.length()];
}

// Estructura para almacenar resultados
struct ResultadosLongitud {
    vector<double> tiempos_fb;
    vector<double> tiempos_dp;
    vector<int> costos;
};

// Función para procesar entrada manual
void procesarEntradaManual() {
    string s1, s2;
    cout << "\nIngrese la primera cadena: ";
    getline(cin, s1);
    cout << "Ingrese la segunda cadena: ";
    getline(cin, s2);

    size_t longitud = max(s1.length(), s2.length());
    int costo_dp = 0, costo_fb = 0;
    double tiempo_fb = 0.0, tiempo_dp = 0.0;

    // Ejecutar y medir DP
    auto inicio_dp = chrono::high_resolution_clock::now();
    costo_dp = editDistanceDynamic(s1, s2);
    auto fin_dp = chrono::high_resolution_clock::now();
    tiempo_dp = chrono::duration<double, milli>(fin_dp - inicio_dp).count();

    cout << "\nResultados:\n";
    cout << "Longitud máxima: " << longitud << " caracteres\n";
    cout << "Programación Dinámica:\n";
    cout << "- Costo: " << costo_dp << "\n";
    cout << "- Tiempo: " << fixed << setprecision(3) << tiempo_dp << " ms\n";

    // Ejecutar FB solo si la longitud es menor a 14
    if (longitud < 14) {
        auto inicio_fb = chrono::high_resolution_clock::now();
        costo_fb = editDistanceBruteForce(s1, s2, 0, 0);
        auto fin_fb = chrono::high_resolution_clock::now();
        tiempo_fb = chrono::duration<double, milli>(fin_fb - inicio_fb).count();

        cout << "Fuerza Bruta:\n";
        cout << "- Costo: " << costo_fb << "\n";
        cout << "- Tiempo: " << fixed << setprecision(3) << tiempo_fb << " ms\n";
    }

    // Generar archivos con valor 0
    ofstream resultado("resultados.txt");
    resultado << "Tipo: Manual\n";
    resultado << "Longitud: 0\n";
    resultado << "Costo FB: 0\n";
    resultado << "Tiempo FB: 0.000\n";
    resultado << "Costo DP: 0\n";
    resultado << "Tiempo DP: 0.000\n";
    resultado.close();

    ofstream promedios("promedios_tiempos.txt");
    promedios << "Longitud: 0\n";
    promedios << "Tiempo promedio FB: 0.000\n";
    promedios << "Tiempo promedio DP: 0.000\n";
    promedios.close();
}

// Función para procesar archivo
void procesarArchivo(const string& nombre_archivo, bool incluir_fb, 
                    map<size_t, ResultadosLongitud>& resultados_longitud, 
                    ofstream& archivo_resultados, const string& tipo_caso) {
    ifstream archivo(nombre_archivo);
    string s1, s2;
    
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombre_archivo << endl;
        return;
    }

    while (getline(archivo, s1) && getline(archivo, s2)) {
        size_t longitud = max(s1.length(), s2.length());
        int costo_dp = 0, costo_fb = 0;
        double tiempo_fb = 0.0;

        auto inicio_dp = chrono::high_resolution_clock::now();
        costo_dp = editDistanceDynamic(s1, s2);
        auto fin_dp = chrono::high_resolution_clock::now();
        double tiempo_dp = chrono::duration<double, milli>(fin_dp - inicio_dp).count();

        if (incluir_fb && longitud < 14) {
            auto inicio_fb = chrono::high_resolution_clock::now();
            costo_fb = editDistanceBruteForce(s1, s2, 0, 0);
            auto fin_fb = chrono::high_resolution_clock::now();
            tiempo_fb = chrono::duration<double, milli>(fin_fb - inicio_fb).count();
        }

        archivo_resultados << "Tipo: " << tipo_caso << "\n"
                         << "Longitud: " << longitud << " caracteres\n"
                         << "S1: " << s1 << "\n"
                         << "S2: " << s2 << "\n";
        
        if (incluir_fb && longitud < 14) {
            archivo_resultados << "Costo FB: " << costo_fb << "\n"
                             << "Tiempo FB: " << fixed << setprecision(3) << tiempo_fb << " ms\n";
            resultados_longitud[longitud].tiempos_fb.push_back(tiempo_fb);
            resultados_longitud[longitud].costos.push_back(costo_fb);
        }
        
        archivo_resultados << "Costo DP: " << costo_dp << "\n"
                         << "Tiempo DP: " << fixed << setprecision(3) << tiempo_dp << " ms\n"
                         << "----------------------------------------\n";
        
        resultados_longitud[longitud].tiempos_dp.push_back(tiempo_dp);
    }
    archivo.close();
}

// Función para procesar datasets
void procesarDatasets() {
    vector<pair<string, string>> archivos = {
        {"casos_identicos_fb_dp.txt", "Cadenas idénticas"},
        {"casos_sustitucion_fb_dp.txt", "Cadenas con sustitución"},
        {"casos_insercion_fb_dp.txt", "Cadenas con inserción"},
        {"casos_eliminacion_fb_dp.txt", "Cadenas con eliminación"},
        {"casos_diferentes_fb_dp.txt", "Cadenas diferentes"},
        {"casos_repetitivos_fb_dp.txt", "Cadenas repetitivas"},
        {"casos_desplazados_fb_dp.txt", "Cadenas desplazadas"},
        {"casos_identicos_dp.txt", "Cadenas idénticas (DP)"},
        {"casos_sustitucion_dp.txt", "Cadenas con sustitución (DP)"},
        {"casos_insercion_dp.txt", "Cadenas con inserción (DP)"},
        {"casos_eliminacion_dp.txt", "Cadenas con eliminación (DP)"},
        {"casos_diferentes_dp.txt", "Cadenas diferentes (DP)"},
        {"casos_repetitivos_dp.txt", "Cadenas repetitivas (DP)"},
        {"casos_desplazados_dp.txt", "Cadenas desplazadas (DP)"}
    };

    ofstream archivo_resultados("resultados.txt");
    ofstream archivo_promedios("promedios_tiempos.txt");
    map<size_t, ResultadosLongitud> resultados_longitud;

    cout << "\nProcesando datasets...\n";

    for (const auto& par : archivos) {
        const string& archivo = par.first;
        const string& tipo = par.second;
        bool incluir_fb = archivo.find("_fb_dp") != string::npos;
        procesarArchivo(archivo, incluir_fb, resultados_longitud, archivo_resultados, tipo);
    }

    archivo_promedios << "PROMEDIOS DE TIEMPO POR LONGITUD DE CADENA\n"
                     << "==========================================\n\n";
    
    for (const auto& par : resultados_longitud) {
        size_t longitud = par.first;
        const ResultadosLongitud& resultados = par.second;
        
        archivo_promedios << "Longitud: " << longitud << " caracteres\n";
        
        if (!resultados.tiempos_fb.empty()) {
            double promedio_fb = accumulate(resultados.tiempos_fb.begin(), 
                                         resultados.tiempos_fb.end(), 0.0) / 
                               resultados.tiempos_fb.size();
            archivo_promedios << "Tiempo promedio FB: " << fixed << setprecision(3) 
                            << promedio_fb << " ms\n";
        }
        
        double promedio_dp = accumulate(resultados.tiempos_dp.begin(), 
                                      resultados.tiempos_dp.end(), 0.0) / 
                            resultados.tiempos_dp.size();
        archivo_promedios << "Tiempo promedio DP: " << fixed << setprecision(3) 
                         << promedio_dp << " ms\n"
                         << "----------------------------------------\n";
    }

    archivo_resultados.close();
    archivo_promedios.close();
    cout << "Procesamiento completado. Revise 'resultados.txt' y 'promedios_tiempos.txt'\n";
}

int main() {
    // Cargar tablas de costos
    if (!loadCostTable("cost_insert.txt", cost_insert) ||
        !loadCostTable("cost_delete.txt", cost_delete) ||
        !loadCostMatrix("cost_replace.txt", cost_replace) ||
        !loadCostMatrix("cost_transpose.txt", cost_transpose)) {
        cerr << "Error al cargar las tablas de costos" << endl;
        return 1;
    }

    cout << "Seleccione el modo de operación:\n";
    cout << "1. Entrada manual\n";
    cout << "2. Procesar datasets\n";
    cout << "Opción: ";

    int opcion;
    cin >> opcion;
    cin.ignore(); // Limpiar el buffer

    switch(opcion) {
        case 1:
            procesarEntradaManual();
            break;
        case 2:
            procesarDatasets();
            break;
        default:
            cout << "Opción inválida\n";
            return 1;
    }

    return 0;
}