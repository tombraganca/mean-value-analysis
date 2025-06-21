#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Para formatar a saída (casas decimais, etc.)

int main() {
    // --- COLETA DE DADOS ---
    int N_customers; // Número total de clientes (N maiúsculo do problema)
    int K_resources; // Número de recursos (filas/aparelhos)

    std::cout << "===== ANÁLISE DE VALOR MÉDIO (MVA) =====\n";
    std::cout << "Entre com o numero total de clientes (N): ";
    std::cin >> N_customers;

    std::cout << "Entre com o numero de recursos/filas (K): ";
    std::cin >> K_resources;

    // Usamos vetores para armazenar os dados de cada recurso.
    // Assim, o programa funciona para qualquer número de recursos.
    std::vector<std::string> resource_names(K_resources);
    std::vector<double> S(K_resources); // Tempos de Serviço (S_i)
    std::vector<double> V(K_resources); // Número de Visitas (V_i)

    for (int i = 0; i < K_resources; ++i) {
        std::cout << "\n--- Recurso " << i + 1 << " ---\n";
        std::cout << "Nome do recurso (ex: CPU, Esteira): ";
        std::cin >> resource_names[i];
        std::cout << "Tempo de Servico (S) para " << resource_names[i] << ": ";
        std::cin >> S[i];
        std::cout << "Numero de Visitas (V) para " << resource_names[i] << ": ";
        std::cin >> V[i];
    }

    // --- ALGORITMO MVA ---
    
    // N_i é o número médio de clientes em cada recurso.
    // Este é o vetor mais importante, pois ele carrega o estado de uma iteração para a outra.
    // Inicializamos com 0 para a iteração n=0.
    std::vector<double> N_i(K_resources, 0.0);

    // Vetores para guardar os resultados finais da última iteração (n = N_customers)
    std::vector<double> R_i_final(K_resources);
    std::vector<double> U_i_final(K_resources);
    double R_0_final = 0.0;
    double X_0_final = 0.0;


    // Laço principal do MVA: itera de n=1 até N_customers
    for (int n = 1; n <= N_customers; ++n) {
        
        // Vetores para os cálculos desta iteração específica
        std::vector<double> R_i_current(K_resources);
        double R_0_current = 0.0;

        // Passo A: Calcular R_i(n) para cada recurso
        // A mágica está aqui: usamos N_i da iteração anterior (n-1)
        for (int k = 0; k < K_resources; ++k) {
            R_i_current[k] = S[k] * (1.0 + N_i[k]);
        }

        // Passo B: Calcular R_0(n) (Tempo de Resposta do Sistema)
        for (int k = 0; k < K_resources; ++k) {
            R_0_current += V[k] * R_i_current[k];
        }

        // Passo C: Calcular X_0(n) (Throughput do Sistema)
        double X_0_current = static_cast<double>(n) / R_0_current;

        // Passo F: Calcular o NOVO N_i(n) para a PRÓXIMA iteração
        // N_i(n) = R_i(n) * X_i(n)  e  X_i(n) = V_i * X_0(n)
        for (int k = 0; k < K_resources; ++k) {
            N_i[k] = R_i_current[k] * V[k] * X_0_current;
        }

        // Se esta for a última iteração, guardamos os resultados
        if (n == N_customers) {
            R_i_final = R_i_current;
            R_0_final = R_0_current;
            X_0_final = X_0_current;
            // Passo E: Calcular U_i(n) (Utilização) para o relatório final
            for (int k = 0; k < K_resources; ++k) {
                // U_i = S_i * X_i  e  X_i = V_i * X_0
                U_i_final[k] = S[k] * V[k] * X_0_final;
            }
        }
    } // Fim do laço principal do MVA

    // --- APRESENTAÇÃO DOS RESULTADOS ---
    std::cout << "\n\n===== RESULTADOS FINAIS PARA N = " << N_customers << " CLIENTES =====\n";

    // Define a formatação para números de ponto flutuante
    std::cout << std::fixed << std::setprecision(4);

    std::cout << "\n>> Metricas Gerais do Sistema:\n";
    std::cout << "Tempo de Resposta Total (R0): " << R_0_final << " minutos\n";
    std::cout << "Throughput Total (X0): " << X_0_final << " clientes/minuto\n";
    std::cout << "Throughput Total (X0): " << X_0_final * 60 << " clientes/hora\n";

    std::cout << "\n>> Metricas por Recurso:\n";
    std::cout << "---------------------------------------------------------------------------\n";
    std::cout << std::setw(15) << "Recurso"
              << std::setw(15) << "Utilizacao"
              << std::setw(20) << "Tempo Resposta (R)"
              << std::setw(25) << "Nro. Medio Clientes (L)" << std::endl;
    std::cout << "---------------------------------------------------------------------------\n";

    for (int k = 0; k < K_resources; ++k) {
        // L_i (número médio de clientes no recurso) é o valor final de N_i[k]
        std::cout << std::setw(15) << resource_names[k]
                  << std::setw(15) << U_i_final[k]
                  << std::setw(20) << R_i_final[k]
                  << std::setw(25) << N_i[k] << std::endl;
    }
    std::cout << "---------------------------------------------------------------------------\n";

    return 0;
}