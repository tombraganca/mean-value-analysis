# Implementação do Algoritmo de Análise do Valor Médio (MVA)

**Autor(a):** Gleydiston Bragança
**Disciplina:** Modelagem e Análise de Desempenho
**Professora:** Marta Noronha

## 1. Introdução

[cite_start]A Análise do Valor Médio (MVA - Mean Value Analysis) é uma técnica analítica fundamental na teoria das filas, amplamente utilizada para avaliar o desempenho de sistemas computacionais.  [cite_start]Diferentemente de abordagens baseadas em simulação, que exigem múltiplos experimentos, o MVA oferece um método determinístico e direto para o cálculo de métricas de desempenho. 

[cite_start]A principal vantagem do MVA é sua capacidade de resolver modelos de redes de filas de forma eficiente, evitando o problema da "explosão do número de estados" que ocorre ao usar Modelos de Markov, especialmente em sistemas com muitos clientes ou recursos.  Este trabalho se dedica a implementar o algoritmo MVA para analisar um sistema de filas e validar seu funcionamento.

## 2. Objetivo

[cite_start]O objetivo central deste trabalho é implementar o algoritmo de Análise do Valor Médio (MVA) para um sistema de filas fechado.  [cite_start]A implementação desenvolvida deverá ser capaz de calcular e exibir as principais métricas de desempenho do sistema, permitindo a análise de diferentes cenários de carga e configuração. 

As métricas a serem calculadas para cada fila (recurso) do sistema são:
* [cite_start]Tempo médio de resposta (R) 
* [cite_start]Tempo médio de espera (W) 
* Utilização do servidor (U)
* Número médio de clientes no sistema (L)

## 3. Descrição do Algoritmo MVA

[cite_start]O MVA é um algoritmo recursivo que calcula as métricas de desempenho para uma população de `n` clientes com base nos resultados conhecidos para uma população de `n-1` clientes.  [cite_start]Isso é feito sem a necessidade de gerar e resolver o grande conjunto de equações lineares de um Modelo de Markov. 

O algoritmo opera com os seguintes parâmetros de entrada:
* `N`: O número total de clientes no sistema.
* `K`: O número total de recursos (filas).
* `S_i`: O tempo de serviço médio do recurso `i`.
* `V_i`: O número médio de visitas que um cliente faz ao recurso `i` para completar seu ciclo.

A lógica recursiva segue as seguintes fórmulas:

**1. Inicialização (Base da Recursão):**
O número médio de clientes em qualquer recurso para um sistema com 0 clientes é 0.
[cite_start]$$N_i(0) = 0, \quad \text{para todo recurso } i$$ 

**2. Laço Principal (de n = 1 até N):**
Para cada cliente `n` adicionado ao sistema:

* **Tempo de Resposta por Recurso `i`:**
    $$R_i(n) = S_i \times [1 + N_i(n-1)]$$ 

* **Tempo de Resposta Total do Sistema:**
    $$R_0(n) = \sum_{i=1}^{K} V_i \times R_i(n)$$ 

* **Throughput (Vazão) do Sistema:**
    $$X_0(n) = \frac{n}{R_0(n)}$$ 

* **Throughput (Vazão) por Recurso `i`:**
    $$X_i(n) = V_i \times X_0(n)$$ 

* **Utilização por Recurso `i`:**
    $$U_i(n) = S_i \times X_i(n)$$ 

* **Número Médio de Clientes por Recurso `i`:**
    $$N_i(n) = R_i(n) \times X_i(n)$$ 

O valor de `$N_i(n)$` calculado ao final de uma iteração é a entrada para o cálculo de `$R_i(n+1)$` na próxima iteração.

## 4. Metodologia de Implementação

Para atender aos objetivos, foi desenvolvido um programa em **[Escolha aqui: C, C++ ou Java]**. A implementação segue os passos lógicos do algoritmo MVA.

1.  **Estruturas de Dados:** Foram utilizados arrays (vetores) para armazenar os parâmetros de cada um dos `K` recursos, como Tempo de Serviço (`S_i`), Visitas (`V_i`), e as métricas calculadas em cada iteração (`R_i`, `U_i`, `N_i`, etc.).

2.  [cite_start]**Entrada de Parâmetros:** O programa solicita ao usuário os seguintes dados: 
    * [cite_start]Número de clientes no sistema (`N`). 
    * [cite_start]Número de filas/recursos (`K`). 
    * Para cada recurso `i`:
        * O tempo de serviço (`S_i`).
        * O número de visitas (`V_i`).

3.  **Laço de Execução:** O núcleo do programa é um laço `for` que itera de `n = 1` até `N`. Em cada iteração, as fórmulas descritas na Seção 3 são aplicadas sequencialmente para calcular as métricas para a população `n`.

4.  **Apresentação dos Resultados:** Ao final do laço, quando `n = N`, o programa exibe uma tabela formatada com os resultados finais para cada recurso, contendo todas as métricas de desempenho solicitadas.

## 5. Cenários de Teste e Análise de Resultados

[cite_start]Conforme solicitado, foram executados três cenários de teste para validar a implementação e analisar o comportamento do sistema sob diferentes condições. 

### Cenário 1: Carga Baixa

Este cenário testa o sistema com uma população pequena de clientes para estabelecer uma linha de base de desempenho.

* **Parâmetros de Entrada:**
    * N = [Ex: 2]
    * Recursos: [Descrever os recursos, S_i e V_i]
* **Resultados Obtidos:**
| Recurso | Utilização (U) | Tempo de Resposta (R) | Nro. Médio Clientes (L) |
| :--- | :---: | :---: | :---: |
| CPU | [Valor] | [Valor] | [Valor] |
| Disco 1 | [Valor] | [Valor] | [Valor] |
* **Análise:**
    [Aqui você analisa os resultados. Ex: "Com poucos clientes, a utilização de todos os recursos é baixa, e o tempo de resposta é próximo ao tempo de serviço, indicando pouca ou nenhuma fila."]

### Cenário 2: Carga Alta

Aumentamos o número de clientes para observar como o sistema se comporta perto de seu limite e identificar possíveis gargalos.

* **Parâmetros de Entrada:**
    * N = [Ex: 10]
    * Recursos: [Mesmos recursos do Cenário 1]
* **Resultados Obtidos:**
| Recurso | Utilização (U) | Tempo de Resposta (R) | Nro. Médio Clientes (L) |
| :--- | :---: | :---: | :---: |
| CPU | [Valor] | [Valor] | [Valor] |
| Disco 1 | [Valor] | [Valor] | [Valor] |
* **Análise:**
    [Analise o aumento da utilização, o crescimento do tempo de resposta devido à formação de filas, e qual recurso se torna o gargalo (maior utilização).]

### Cenário 3: Sistema com Gargalo Explícito

Neste cenário, um dos recursos é configurado para ser significativamente mais lento que os outros, forçando-o a ser o gargalo do sistema.

* **Parâmetros de Entrada:**
    * N = [Ex: 5]
    * Recursos: [Descrever os recursos, com um S_i bem maior que os outros]
* **Resultados Obtidos:**
| Recurso | Utilização (U) | Tempo de Resposta (R) | Nro. Médio Clientes (L) |
| :--- | :---: | :---: | :---: |
| CPU | [Valor] | [Valor] | [Valor] |
| Disco Lento | [Valor] | [Valor] | [Valor] |
* **Análise:**
    [Mostre como a utilização do recurso lento se aproxima de 100% rapidamente, enquanto os outros permanecem com baixa utilização, e como isso impacta o tempo de resposta geral do sistema.]

## 6. Conclusões

Este trabalho permitiu a implementação prática e o estudo do algoritmo de Análise do Valor Médio. O programa desenvolvido foi capaz de calcular corretamente as métricas de desempenho para sistemas de filas fechados, conforme validado pelos cenários de teste.

Observou-se que o MVA é uma ferramenta poderosa para *capacity planning*, permitindo identificar gargalos e prever o comportamento de um sistema com o aumento da carga.

**Melhorias Futuras:**
Como possíveis melhorias para a implementação, poderiam ser adicionadas:
* Suporte a múltiplas classes de clientes.
* Implementação de algoritmos variantes do MVA para sistemas com recursos dependentes da carga.
* Criação de uma interface gráfica para facilitar a entrada de dados e a visualização dos resultados.