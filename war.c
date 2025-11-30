// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define NUM_MISSOES 2
#define TAM_STRING 50

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(int missaoID);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* mapa, int atacante, int defensor);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int missaoID, const char* corJogador);

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));
    
    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa!\n");
        return 1;
    }
    
    inicializarTerritorios(mapa);
    
    char corJogador[TAM_STRING] = "Azul";
    int missaoID = sortearMissao();
    
    // 2. Laço Principal do Jogo (Game Loop):
    int opcao;
    int venceu = 0;
    
    do {
        printf("\n========================================\n");
        exibirMapa(mapa);
        printf("\n");
        exibirMissao(missaoID);
        printf("\n");
        exibirMenuPrincipal();
        
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        
        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                if (verificarVitoria(mapa, missaoID, corJogador)) {
                    printf("\nVocê completou sua missão e venceu o jogo!\n");
                    venceu = 1;
                } else {
                    printf("\nVocê ainda não completou sua missão. Continue jogando!\n");
                }
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
        
        if (opcao != 0 && !venceu) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (opcao != 0 && !venceu);
    
    // 3. Limpeza:
    liberarMemoria(mapa);
    
    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa() {
    return (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio* mapa) {
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, "Azul");
    mapa[0].tropas = 5;
    
    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].cor, "Verde");
    mapa[1].tropas = 3;
    
    strcpy(mapa[2].nome, "Chile");
    strcpy(mapa[2].cor, "Vermelho");
    mapa[2].tropas = 4;
    
    strcpy(mapa[3].nome, "Peru");
    strcpy(mapa[3].cor, "Verde");
    mapa[3].tropas = 2;
    
    strcpy(mapa[4].nome, "Colombia");
    strcpy(mapa[4].cor, "Amarelo");
    mapa[4].tropas = 3;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("========================================\n");
    printf("           MENU PRINCIPAL\n");
    printf("========================================\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
    printf("========================================\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa) {
    printf("========================================\n");
    printf("        ESTADO ATUAL DO MAPA\n");
    printf("========================================\n");
    printf("%-3s %-15s %-12s %s\n", "ID", "Território", "Exército", "Tropas");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-3d %-15s %-12s %d\n", 
               i + 1, 
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
    printf("========================================\n");
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(int missaoID) {
    printf("========================================\n");
    printf("         SUA MISSÃO SECRETA\n");
    printf("========================================\n");
    
    if (missaoID == 0) {
        printf("Destruir completamente o exército VERDE\n");
    } else if (missaoID == 1) {
        printf("Conquistar 3 territórios\n");
    }
    
    printf("========================================\n");
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio* mapa, const char* corJogador) {
    int atacante, defensor;
    
    printf("\n========================================\n");
    printf("           FASE DE ATAQUE\n");
    printf("========================================\n");
    
    printf("Escolha o território atacante (1-%d): ", NUM_TERRITORIOS);
    scanf("%d", &atacante);
    limparBufferEntrada();
    
    printf("Escolha o território defensor (1-%d): ", NUM_TERRITORIOS);
    scanf("%d", &defensor);
    limparBufferEntrada();
    
    // Ajustar para índice zero-based
    atacante--;
    defensor--;
    
    // Validações básicas
    if (atacante < 0 || atacante >= NUM_TERRITORIOS || 
        defensor < 0 || defensor >= NUM_TERRITORIOS) {
        printf("\nTerritórios inválidos!\n");
        return;
    }
    
    if (atacante == defensor) {
        printf("\nVocê não pode atacar o mesmo território!\n");
        return;
    }
    
    if (strcmp(mapa[atacante].cor, corJogador) != 0) {
        printf("\nVocê só pode atacar com seus próprios territórios!\n");
        return;
    }
    
    if (mapa[atacante].tropas < 2) {
        printf("\nVocê precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }
    
    simularAtaque(mapa, atacante, defensor);
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio* mapa, int atacante, int defensor) {
    printf("\nBATALHA: %s (%s) vs %s (%s)\n", 
           mapa[atacante].nome, mapa[atacante].cor,
           mapa[defensor].nome, mapa[defensor].cor);
    
    // Rolar dados
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;
    
    printf("Dado do Atacante: %d\n", dadoAtaque);
    printf("Dado do Defensor: %d\n", dadoDefesa);
    
    // Determinar vencedor
    if (dadoAtaque > dadoDefesa) {
        printf("Atacante venceu!\n");
        mapa[defensor].tropas--;
        
        if (mapa[defensor].tropas == 0) {
            printf("Território %s foi conquistado!\n", mapa[defensor].nome);
            strcpy(mapa[defensor].cor, mapa[atacante].cor);
            mapa[defensor].tropas = 1;
            mapa[atacante].tropas--;
        }
    } else {
        printf("Defensor resistiu ao ataque!\n");
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao() {
    return rand() % NUM_MISSOES;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(const Territorio* mapa, int missaoID, const char* corJogador) {
    if (missaoID == 0) {
        // Missão: Destruir o exército Verde
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) {
                return 0; // Ainda existe território verde
            }
        }
        return 1; // Todos os territórios verdes foram eliminados
    } else if (missaoID == 1) {
        // Missão: Conquistar 3 territórios
        int territoriosConquistados = 0;
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                territoriosConquistados++;
            }
        }
        return territoriosConquistados >= 3;
    }
    
    return 0;
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
