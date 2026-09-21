# Laboratório Pthreads - Computação Paralela - Pedro de Souza Zequi

Este repositório contém as implementações desenvolvidas para o laboratório de Programação Concorrente em Memória Compartilhada utilizando a biblioteca POSIX Threads (Pthreads).

O trabalho foi dividido em três partes:

1. Análise de condição de corrida e sincronização com mutex.
2. Implementação de três problemas utilizando Pthreads.
3. Análise do Global Interpreter Lock (GIL) do Python.

---

## Estrutura do Projeto

```text
CompParalela/
│
├── parte1/
│   ├── contacorrente_seq.c
│   ├── contacorrente_f1.c
│   └── contacorrente_f2.c
│
├── parte2/
│   ├── primos.c
│   ├── monte_carlo_pi.c
│   └── soma_vetores.c
│
├── parte3/
│   ├── seq.py
│   └── threads.py
│
├── resultados/
│
├── Makefile
├── README.md
└── .gitignore
```

---

## Parte 1 - Conta Corrente

A primeira parte demonstra os efeitos da concorrência sobre uma variável compartilhada.

### `contacorrente_seq.c`

Implementação sequencial utilizada como referência.

As operações de depósito e saque são realizadas em série pela thread principal.

### `contacorrente_f1.c`

Implementação utilizando duas threads sem mecanismo de sincronização.

Essa versão demonstra a ocorrência de condição de corrida (`race condition`) e perda de atualizações sobre a variável compartilhada `saldo`.

### `contacorrente_f2.c`

Implementação concorrente utilizando `pthread_mutex_t`.

O mutex protege a região crítica responsável pela alteração do saldo, garantindo exclusão mútua e resultado determinístico.

---

## Parte 2 - Problemas com Pthreads

Foram selecionados três problemas para implementação paralela.

### A - Contagem de Números Primos

Arquivo:

```text
parte2/primos.c
```

O programa conta a quantidade de números primos existentes entre 1 e K.

Foi utilizada uma distribuição cíclica dos candidatos ímpares entre as threads para melhorar o balanceamento de carga.

Cada thread mantém uma contagem local de números primos e atualiza o total global ao final do processamento.

Execução:

```bash
./parte2/primos <K> <num_threads>
```

Exemplo:

```bash
./parte2/primos 5000000 4
```

---

### B - Cálculo de Pi pelo Método de Monte Carlo

Arquivo:

```text
parte2/monte_carlo_pi.c
```

O valor de Pi é estimado a partir da geração de pontos aleatórios dentro de um quadrado.

Cada thread utiliza sua própria semente e a função `rand_r()`, evitando o compartilhamento do estado do gerador pseudoaleatório.

Execução:

```bash
./parte2/monte_carlo_pi <N> <num_threads>
```

Exemplo:

```bash
./parte2/monte_carlo_pi 100000000 4
```

---

### C - Soma de Vetores

Arquivo:

```text
parte2/soma_vetores.c
```

O programa soma dois vetores A e B, produzindo o vetor C.

O vetor é dividido em blocos contíguos e cada thread processa uma região exclusiva.

Como cada thread escreve em posições diferentes do vetor resultante, não é necessário utilizar mutex.

Execução:

```bash
./parte2/soma_vetores <N> <num_threads>
```

Exemplo:

```bash
./parte2/soma_vetores 50000000 4
```

---

## Parte 3 - Python e GIL

A terceira parte compara uma tarefa CPU-bound executada sequencialmente e utilizando duas threads em Python.

### Versão sequencial

Arquivo:

```text
parte3/seq.py
```

Execução:

```bash
python3 parte3/seq.py
```

### Versão com threads

Arquivo:

```text
parte3/threads.py
```

Execução:

```bash
python3 parte3/threads.py
```

Os testes foram realizados com o Global Interpreter Lock (GIL) habilitado.

---

## Compilação

Todos os programas em C podem ser compilados utilizando o `Makefile`.

Na raiz do projeto, execute:

```bash
make
```

As flags utilizadas são:

```text
-O2 -Wall -pthread
```

Para remover os executáveis gerados:

```bash
make clean
```

Os executáveis gerados localmente não são versionados no repositório.

---

## Testes de Desempenho

Os programas da Parte 2 foram executados utilizando:

```text
1 thread
2 threads
4 threads
8 threads
```

Cada configuração foi executada cinco vezes para permitir o cálculo do tempo médio.

Foram analisadas as seguintes métricas:

- Tempo médio de execução
- Speedup
- Eficiência

O Speedup foi calculado por:

```text
Speedup = T1 / Tp
```

onde:

- `T1` é o tempo utilizando uma thread.
- `Tp` é o tempo utilizando p threads.

A eficiência foi calculada por:

```text
Eficiência = Speedup / p
```

---

## Ambiente Experimental

Os experimentos foram realizados no GitHub Codespaces.

### Sistema

```text
Sistema operacional: Ubuntu 24.04.5 LTS
Kernel: Linux 6.8.0-1064-azure
Arquitetura: x86_64
```

### Processador

```text
AMD EPYC 7763 64-Core Processor
CPUs lógicas disponíveis: 2
Núcleos expostos pelo ambiente: 1
Threads por núcleo: 2
Frequência observada: aproximadamente 3,24 GHz
```

O ambiente é virtualizado e utiliza hypervisor Microsoft.

### Cache

```text
L1 Data: 32 KiB
L1 Instruction: 32 KiB
L2: 512 KiB
L3: 32 MiB
```

### Memória

```text
RAM: 7,8 GiB
Swap: 0 B
```

### Ferramentas

```text
GCC: 13.3.0
glibc: 2.39
Python: 3.14.2
```

O GIL estava habilitado durante os experimentos em Python.

---

## Resultados

A pasta:

```text
resultados/
```

contém as saídas coletadas durante os experimentos.

Entre os arquivos estão os resultados dos testes de:

- Contagem de números primos
- Cálculo de Pi por Monte Carlo
- Soma de vetores
- Comparação entre Python sequencial e Python com threads

Os resultados completos, tabelas de Speedup e eficiência e a análise dos experimentos são apresentados no relatório da atividade.

---

## Observação

Os arquivos executáveis gerados durante a compilação não são enviados ao repositório.

O repositório contém apenas os códigos-fonte, scripts, arquivos de resultados, documentação e arquivos necessários para reprodução dos experimentos.