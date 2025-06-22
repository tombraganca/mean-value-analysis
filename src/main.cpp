#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <filesystem>

// cSpell: disable

class Recurso {
public:
    std::string nome;
    double tempo_servico;
    double visitas;
    Recurso(const std::string& n, double s, double v)
        : nome(n), tempo_servico(s), visitas(v) {}
};

struct ResultadoMVA {
    std::vector<double> tempos_resposta;
    std::vector<double> utilizacoes;
    std::vector<double> clientes_medios;
    double tempo_resposta_total;
    double vazao;
};

class MotorMVA {
private:
    int num_clientes;
    std::vector<Recurso> recursos;
public:
    MotorMVA(int n, const std::vector<Recurso>& res)
        : num_clientes(n), recursos(res) {}
    ResultadoMVA executar();
};

class GerenciadorArquivos {
public:
    static bool gravar_resposta_vs_clientes(const std::string& arquivo, const std::vector<std::string>& nomes_recursos, const std::vector<std::vector<double>>& dados);
    static bool gravar_utilizacao_vs_clientes(const std::string& arquivo, const std::vector<std::string>& nomes_recursos, const std::vector<std::vector<double>>& dados);
    static bool gravar_resultado_final(const std::string& arquivo, int N, const ResultadoMVA& resultado, const std::vector<Recurso>& recursos);
};

class GerenciadorEntradaSaida {
public:
    static bool ler_entrada_arquivo(const std::string& arquivo, int& N_clientes, std::vector<Recurso>& recursos) {
        std::ifstream fin(arquivo);
        if (!fin) return false;
        int K_recursos;
        if (!(fin >> N_clientes)) return false;
        std::cout << "Número de clientes definido pelo arquivo: " << N_clientes << std::endl;
        if (!(fin >> K_recursos)) return false;
        std::cout << "Número de recursos definido pelo arquivo: " << K_recursos << std::endl;
        recursos.clear();
        for (int i = 0; i < K_recursos; ++i) {
            std::string nome;
            double S, V;
            if (!(fin >> nome >> S >> V)) return false;
            recursos.emplace_back(nome, S, V);
            std::cout << "Recurso " << (i+1) << ": nome='" << nome << "', tempo_servico=" << S << ", visitas=" << V << std::endl;
        }
        return true;
    }

    static void ler_entrada_terminal(int& N_clientes, std::vector<Recurso>& recursos) {
        int K_recursos;
        std::cout << "Entre com o numero total de clientes (N): ";
        std::cin >> N_clientes;
        std::cout << "Entre com o numero de recursos/filas (K): ";
        std::cin >> K_recursos;
        recursos.clear();
        for (int i = 0; i < K_recursos; ++i) {
            std::string nome;
            double S, V;
            std::cout << "\n--- Recurso " << i + 1 << " ---\n";
            std::cout << "Nome do recurso (ex: CPU, Esteira): ";
            std::cin >> nome;
            std::cout << "Tempo de Servico (S) para " << nome << ": ";
            std::cin >> S;
            std::cout << "Numero de Visitas (V) para " << nome << ": ";
            std::cin >> V;
            recursos.emplace_back(nome, S, V);
        }
    }
};

bool ler_entrada_arquivo(const std::string& arquivo, int& N_clientes, std::vector<Recurso>& recursos) {
    return GerenciadorEntradaSaida::ler_entrada_arquivo(arquivo, N_clientes, recursos);
}

int main(int argc, char* argv[]) {
    std::cout << "===== ANÁLISE DE VALOR MÉDIO (MVA) =====\n";

    // --- CRIA DIRETÓRIO DE SAÍDA src/output SE NÃO EXISTIR ---
    std::filesystem::create_directories("output");

    int N_clientes = 0;
    std::vector<Recurso> recursos;
    std::string arquivo_entrada = "entrada.txt";
    if (argc > 1) {
        arquivo_entrada = argv[1];
    }
    bool arquivo_ok = GerenciadorEntradaSaida::ler_entrada_arquivo(arquivo_entrada, N_clientes, recursos);
    if (arquivo_ok) {
        std::cout << "Dados carregados de '" << arquivo_entrada << "'.\n";
    } else {
        GerenciadorEntradaSaida::ler_entrada_terminal(N_clientes, recursos);
    }
    MotorMVA motor(N_clientes, recursos);
    ResultadoMVA resultado = motor.executar();

    if (!GerenciadorArquivos::gravar_resultado_final(
            "output/output.txt", N_clientes, resultado, recursos)) {
        std::cerr << "Erro ao criar output/output.txt\n";
        return 1;
    }
    std::cout << "Resultados gravados em output/output.txt\n";
    return 0;
}

ResultadoMVA MotorMVA::executar() {
    int K = recursos.size();
    std::vector<double> N_i(K, 0.0);
    std::vector<std::vector<double>> resposta_por_iter, utilizacao_por_iter, espera_por_iter;
    ResultadoMVA resultado_final;
    for (int n = 1; n <= num_clientes; ++n) {
        std::vector<double> R_i(K), U_i(K), X_i(K), E_i(K);
        double R_0 = 0.0;
        for (int k = 0; k < K; ++k) {
            R_i[k] = recursos[k].tempo_servico * (1.0 + N_i[k]);
        }
        for (int k = 0; k < K; ++k) {
            R_0 += recursos[k].visitas * R_i[k];
        }
        double X_0 = static_cast<double>(n) / R_0;
        for (int k = 0; k < K; ++k) {
            N_i[k] = R_i[k] * recursos[k].visitas * X_0;
        }
        for (int k = 0; k < K; ++k) {
            U_i[k] = recursos[k].tempo_servico * recursos[k].visitas * X_0;
            X_i[k] = recursos[k].visitas * X_0;
            E_i[k] = R_i[k] - recursos[k].tempo_servico; // Tempo de espera = Tempo resposta - tempo serviço
        }
        resposta_por_iter.push_back(R_i);
        utilizacao_por_iter.push_back(U_i);
        espera_por_iter.push_back(E_i);
        if (n == num_clientes) {
            resultado_final.tempos_resposta = R_i;
            resultado_final.utilizacoes = U_i;
            resultado_final.clientes_medios = N_i;
            resultado_final.tempo_resposta_total = R_0;
            resultado_final.vazao = X_0;
        }
    }
    // garante que src/output exista
    std::filesystem::create_directories("output");

    std::vector<std::string> nomes;
    for (const auto& r : recursos) nomes.push_back(r.nome);

    GerenciadorArquivos::gravar_resposta_vs_clientes(
        "output/resposta_vs_clientes.txt", nomes, resposta_por_iter);
    GerenciadorArquivos::gravar_utilizacao_vs_clientes(
        "output/utilizacao_vs_clientes.txt", nomes, utilizacao_por_iter);
    GerenciadorArquivos::gravar_resposta_vs_clientes(
        "output/espera_vs_clientes.txt", nomes, espera_por_iter);

    return resultado_final;
}

bool GerenciadorArquivos::gravar_resposta_vs_clientes(const std::string& arquivo, const std::vector<std::string>& nomes_recursos, const std::vector<std::vector<double>>& dados) {
    std::ofstream f(arquivo);
    if (!f) return false;
    f << "N";
    for (const auto& nome : nomes_recursos) f << " " << nome;
    f << "\n";
    for (size_t n = 0; n < dados.size(); ++n) {
        f << (n + 1);
        for (double v : dados[n]) f << " " << std::fixed << std::setprecision(6) << v;
        f << "\n";
    }
    return true;
}

bool GerenciadorArquivos::gravar_utilizacao_vs_clientes(const std::string& arquivo, const std::vector<std::string>& nomes_recursos, const std::vector<std::vector<double>>& dados) {
    return gravar_resposta_vs_clientes(arquivo, nomes_recursos, dados);
}

bool GerenciadorArquivos::gravar_resultado_final(const std::string& arquivo, int N, const ResultadoMVA& resultado, const std::vector<Recurso>& recursos) {
    std::ostringstream buffer;
    buffer << std::fixed << std::setprecision(4);
    buffer << "===== RESULTADOS FINAIS PARA N = " << N << " CLIENTES =====\n";
    buffer << "\n>> Metricas Gerais do Sistema:\n";
    buffer << "Tempo de Resposta Total (R0): " << resultado.tempo_resposta_total << " minutos\n";
    buffer << "Throughput Total (X0): " << resultado.vazao << " clientes/minuto\n";
    buffer << "Throughput Total (X0): " << resultado.vazao * 60 << " clientes/hora\n";
    buffer << "\n>> Metricas por Recurso:\n";
    buffer << "---------------------------------------------------------------------------------------------\n";
    buffer << std::setw(15) << "Recurso"
           << std::setw(15) << "Utilizacao"
           << std::setw(20) << "Tempo Resposta (R)"
           << std::setw(25) << "Nro. Medio Clientes (L)"
           << std::setw(20) << "Tempo de Espera (W)" << std::endl;
    buffer << "---------------------------------------------------------------------------------------------\n";
    for (size_t k = 0; k < recursos.size(); ++k) {
        double tempo_espera = resultado.clientes_medios[k] * recursos[k].tempo_servico;
        buffer << std::setw(15) << recursos[k].nome
               << std::setw(15) << resultado.utilizacoes[k]
               << std::setw(20) << resultado.tempos_resposta[k]
               << std::setw(25) << resultado.clientes_medios[k]
               << std::setw(20) << tempo_espera << std::endl;
    }
    buffer << "---------------------------------------------------------------------------------------------\n";
    std::ofstream fout(arquivo);
    if (!fout) return false;
    fout << buffer.str();
    std::cout << buffer.str();
    return true;
}