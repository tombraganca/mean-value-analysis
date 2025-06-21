#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Para formatar a saída (casas decimais, etc.)
#include <fstream>   // Para ler arquivos e gravar arquivos

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


    // --- PREPARA ARQUIVO PARA GRAVAR TEMPO DE RESPOSTA POR ITERAÇÃO ---
    std::ofstream fresp("resposta_vs_clientes.txt");
    if (!fresp) {
        std::cerr << "Erro ao criar resposta_vs_clientes.txt\n";
        return 1;
    }

    // Laço principal do MVA: itera de n=1 até N_customers
    for (int n = 1; n <= N_customers; ++n) {
        std::vector<double> R_i_current(K_resources);
        double R_0_current = 0.0;

        // Passo A: Calcular R_i(n) para cada recurso
        for (int k = 0; k < K_resources; ++k) {
            R_i_current[k] = S[k] * (1.0 + N_i[k]);
        }

        // Passo B: Calcular R_0(n)
        for (int k = 0; k < K_resources; ++k) {
            R_0_current += V[k] * R_i_current[k];
        }

        // Passo C: Calcular X_0(n)
        double X_0_current = static_cast<double>(n) / R_0_current;

        // Passo F: Calcular o NOVO N_i(n) para a PRÓXIMA iteração
        for (int k = 0; k < K_resources; ++k) {
            N_i[k] = R_i_current[k] * V[k] * X_0_current;
        }

        // --- GRAVA TEMPO DE RESPOSTA DE CADA RECURSO PARA ESTA ITERAÇÃO ---
        fresp << n;
        for (int k = 0; k < K_resources; ++k) {
            fresp << " " << std::fixed << std::setprecision(6) << R_i_current[k];
        }
        fresp << "\n";

        // Se esta for a última iteração, guarda os resultados finais
        if (n == N_customers) {
            R_i_final = R_i_current;
            R_0_final = R_0_current;
            X_0_final = X_0_current;
            for (int k = 0; k < K_resources; ++k) {
                U_i_final[k] = S[k] * V[k] * X_0_final;
            }
        }
    } // Fim do laço principal do MVA

    fresp.close();

    // --- APRESENTAÇÃO DOS RESULTADOS ---
    std::ofstream fout("output.txt");
    if (!fout) {
        std::cerr << "Erro ao criar output.txt\n";
        return 1;
    }

    fout << std::fixed << std::setprecision(4);

    fout << "===== RESULTADOS FINAIS PARA N = " << N_customers << " CLIENTES =====\n";
    fout << "\n>> Metricas Gerais do Sistema:\n";
    fout << "Tempo de Resposta Total (R0): " << R_0_final << " minutos\n";
    fout << "Throughput Total (X0): " << X_0_final << " clientes/minuto\n";
    fout << "Throughput Total (X0): " << X_0_final * 60 << " clientes/hora\n";

    fout << "\n>> Metricas por Recurso:\n";
    fout << "---------------------------------------------------------------------------\n";
    fout << std::setw(15) << "Recurso"
         << std::setw(15) << "Utilizacao"
         << std::setw(20) << "Tempo Resposta (R)"
         << std::setw(25) << "Nro. Medio Clientes (L)" << std::endl;
    fout << "---------------------------------------------------------------------------\n";

    for (int k = 0; k < K_resources; ++k) {
        fout << std::setw(15) << resource_names[k]
             << std::setw(15) << U_i_final[k]
             << std::setw(20) << R_i_final[k]
             << std::setw(25) << N_i[k] << std::endl;
    }
    fout << "---------------------------------------------------------------------------\n";

    fout.close();

    std::cout << "Resultados gravados em output.txt\n";
    return 0;
}