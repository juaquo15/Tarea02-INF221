#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <climits>
#include <algorithm>

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

void imprimirMatriz(const vector<vector<int>>& dp, const string& s1, const string& s2) {
    cout << "\n   ";
    for(char c : s2) cout << setw(4) << c << " ";
    cout << endl;
    
    for(size_t i = 0; i <= s1.length(); ++i) {
        if(i == 0) cout << "-";
        else cout << s1[i-1];
        cout << " ";
        for(size_t j = 0; j <= s2.length(); ++j) {
            if(dp[i][j] == INT_MAX)
                cout << setw(4) << "INF" << " ";
            else
                cout << setw(4) << dp[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void imprimirMatrizCostos(const string& nombre, const vector<vector<int>>& matriz) {
    cout << "\nMatriz de " << nombre << " (primeros 5x5):\n";
    cout << "     ";
    for(int i = 0; i < 5; i++) cout << setw(4) << char('a' + i) << " ";
    cout << endl;
    
    for(int i = 0; i < 5; i++) {
        cout << setw(4) << char('a' + i) << " ";
        for(int j = 0; j < 5; j++) {
            cout << setw(4) << matriz[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int editDistanceDynamic(const string& s1, const string& s2, bool debug = false) {
    vector<vector<int>> dp(s1.length() + 1, vector<int>(s2.length() + 1, INT_MAX));

    // Inicialización
    dp[0][0] = 0;
    
    // Primera columna - eliminaciones
    for(size_t i = 1; i <= s1.length(); ++i) 
        dp[i][0] = dp[i-1][0] + cost_delete[s1[i-1] - 'a'];
    
    // Primera fila - inserciones
    for(size_t j = 1; j <= s2.length(); ++j) 
        dp[0][j] = dp[0][j-1] + cost_insert[s2[j-1] - 'a'];

    // Rellenar el resto de la matriz
    for(size_t i = 1; i <= s1.length(); ++i) {
        for(size_t j = 1; j <= s2.length(); ++j) {
            if(s1[i-1] == s2[j-1]) {
                // Si los caracteres son iguales, no hay costo
                dp[i][j] = dp[i-1][j-1];
            } else {
                // Primero intentamos la sustitución directa
                dp[i][j] = dp[i-1][j-1] + cost_replace[s1[i-1] - 'a'][s2[j-1] - 'a'];
                
                // Solo consideramos inserción/eliminación si resultan en un costo menor
                if (dp[i][j-1] + cost_insert[s2[j-1] - 'a'] < dp[i][j])
                    dp[i][j] = dp[i][j-1] + cost_insert[s2[j-1] - 'a'];
                
                if (dp[i-1][j] + cost_delete[s1[i-1] - 'a'] < dp[i][j])
                    dp[i][j] = dp[i-1][j] + cost_delete[s1[i-1] - 'a'];
            }

            // Verificar transposición si es posible
            if(i > 1 && j > 1 && s1[i-2] == s2[j-1] && s1[i-1] == s2[j-2]) {
                int costo_trans = cost_transpose[s1[i-2] - 'a'][s1[i-1] - 'a'];
                if (dp[i-2][j-2] + costo_trans < dp[i][j])
                    dp[i][j] = dp[i-2][j-2] + costo_trans;
            }
        }
    }

    return dp[s1.length()][s2.length()];
}

void probarCaso(const string& s1, const string& s2) {
    cout << "\n====================================\n";
    cout << "Probando transformación de '" << s1 << "' a '" << s2 << "':\n";
    int costo = editDistanceDynamic(s1, s2, true); // Activamos el modo debug
    cout << "\nCosto total final: " << costo << endl;
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

    // Imprimir las matrices de costo para verificar
    imprimirMatrizCostos("cost_replace", cost_replace);
    imprimirMatrizCostos("cost_transpose", cost_transpose);

    // Probar el caso problemático
    probarCaso("pac", "pbc");

    return 0;
}