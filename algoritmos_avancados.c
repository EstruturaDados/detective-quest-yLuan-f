#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define MAX_QUADRO 10
#define MAX_SUSPEITOS 3
 
// ========== STRUCTS ==========
 
// estrutura para representar as salas da mansao.
typedef struct Sala {
    char nome[50];
    struct Pista* pista;
    struct Sala* direita;
    struct Sala* esquerda;
} Sala;
 
// estrutura para representar as pistas encontradas.
typedef struct Pista {
    char texto[50];
    struct Pista* direita;
    struct Pista* esquerda;
} Pista;
 
// estrutura para representar o quadro de suspeitos e suas pistas associadas.
typedef struct Quadro {
    char descricao[50];
    struct Quadro* proximo;
} Quadro;
 
// ========== SUSPEITOS E HASH ==========
 
Quadro* quadroHash[MAX_QUADRO];
 
const char* suspeitos[] = {"Jardineiro", "Filho", "Mae"};
 
// contador de pistas por suspeito para calcular o mais citado
int contadorSuspeitos[MAX_SUSPEITOS] = {0, 0, 0};
 
// funcao para inicializar o quadro, definindo todos os ponteiros como NULL.
void inicializarQuadro() {
    for (int i = 0; i < MAX_QUADRO; i++) {
        quadroHash[i] = NULL;
    }
}
 
// funcao hash ponderada: multiplica cada caractere pela sua posicao + 1.
int funcaoHash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i] * (i + 1);
    }
    return soma % MAX_QUADRO;
}
 
// insere uma pista na tabela hash associada a um suspeito.
// em caso de colisao, usa encadeamento (lista ligada).
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = funcaoHash(suspeito);
 
    Quadro* nova = (Quadro*)malloc(sizeof(Quadro));
    strncpy(nova->descricao, pista, 49);
    nova->descricao[49] = '\0';
 
    // insere no inicio da lista encadeada daquele indice
    nova->proximo = quadroHash[indice];
    quadroHash[indice] = nova;
}
 
// busca o indice do suspeito no array de suspeitos pelo nome.
// retorna -1 se nao encontrado.
int buscarSuspeito(const char* nome) {
    for (int i = 0; i < MAX_SUSPEITOS; i++) {
        if (strcmp(suspeitos[i], nome) == 0)
            return i;
    }
    return -1;
}
 
// exibe todas as pistas associadas a cada suspeito.
void listarAssociacoes() {
    separador();
    printf("\n====== QUADRO DE SUSPEITOS ======\n");
    for (int i = 0; i < MAX_SUSPEITOS; i++) {
        int indice = funcaoHash(suspeitos[i]);
        Quadro* atual = quadroHash[indice];
 
        printf("\n>>> %s (%d pista(s)):\n", suspeitos[i], contadorSuspeitos[i]);
        if (atual == NULL) {
            printf("   Nenhuma pista associada.\n");
        }
        while (atual != NULL) {
            printf("   - %s\n", atual->descricao);
            atual = atual->proximo;
        }
    }
    separador();
}
 
// exibe o suspeito mais citado com base no contador de pistas.
void exibirSuspeitoMaisCitado() {
    int maior = 0;
    int indiceMaior = 0;
 
    for (int i = 1; i < MAX_SUSPEITOS; i++) {
        if (contadorSuspeitos[i] > contadorSuspeitos[maior]) {
            maior = i;
            indiceMaior = i;
        }
    }
 
    separador();
    if (contadorSuspeitos[maior] == 0) {
        printf("\nNenhuma pista foi associada. Caso encerrado sem conclusao.\n");
    } else {
        printf("\n=== VEREDITO FINAL ===\n");
        printf("\nCom base nas pistas coletadas, o suspeito mais provavel e:\n");
        printf("\n   >> %s << \n", suspeitos[maior]);
        printf("\ncom %d pista(s) apontando para ele.\n", contadorSuspeitos[maior]);
    }
    separador();
}
 
// libera toda a memoria alocada na tabela hash.
void liberarQuadro() {
    for (int i = 0; i < MAX_QUADRO; i++) {
        Quadro* atual = quadroHash[i];
        while (atual != NULL) {
            Quadro* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        quadroHash[i] = NULL;
    }
}
 
// fase de acusacao: o jogador associa cada pista coletada a um suspeito.
void associarPistasASuspeitos(Pista** coletadas, int total) {
    if (total == 0) {
        printf("\nNenhuma pista para associar.\n");
        return;
    }
 
    separador();
    printf("\n=== FASE DE ACUSACAO ===\n");
    printf("Associe cada pista encontrada a um suspeito:\n");
 
    for (int i = 0; i < total; i++) {
        separador();
        printf("\nPista: \"%s\"\n", coletadas[i]->texto);
        printf("Qual suspeito voce associa a esta pista?\n");
        for (int j = 0; j < MAX_SUSPEITOS; j++) {
            printf("  %d) %s\n", j + 1, suspeitos[j]);
        }
        printf("  0) Ignorar pista\n");
 
        int escolha = -1;
        printf("Escolha: ");
        scanf("%d", &escolha);
 
        if (escolha >= 1 && escolha <= MAX_SUSPEITOS) {
            int idx = escolha - 1;
            inserirNaHash(coletadas[i]->texto, suspeitos[idx]);
            contadorSuspeitos[idx]++;
            printf("Pista associada a %s!\n", suspeitos[idx]);
        } else {
            printf("Pista ignorada.\n");
        }
    }
}
 
// ========== FUNCOES DE MANIPULACAO DE MAPA, PISTA E QUADRO ==========
 
void separador() {
    printf("\n\n========================================\n\n");
}
 
Sala* novaSala(char* nome) {
    Sala* sala = (Sala*)malloc(sizeof(Sala));
    strncpy(sala->nome, nome, 50);
    sala->pista = NULL;
    sala->direita = NULL;
    sala->esquerda = NULL;
    return sala;
}
 
Pista* novaPista(char* texto) {
    Pista* pista = (Pista*)malloc(sizeof(Pista));
    strncpy(pista->texto, texto, 50);
    pista->direita = NULL;
    pista->esquerda = NULL;
    return pista;
}
 
void liberarSala(Sala* raiz) {
    if (raiz != NULL) {
        liberarSala(raiz->esquerda);
        liberarSala(raiz->direita);
        free(raiz);
    }
}
 
void liberarPista(Pista* raiz) {
    if (raiz != NULL) {
        liberarPista(raiz->esquerda);
        liberarPista(raiz->direita);
        free(raiz);
    }
}
 
void criarMapa(Sala** raiz) {
    *raiz = novaSala("Hall de entrada");
    (*raiz)->esquerda = novaSala("Biblioteca");
    (*raiz)->direita = novaSala("Cozinha");
    (*raiz)->esquerda->esquerda = novaSala("Quarto de servico");
    (*raiz)->esquerda->direita = novaSala("Estudio de arte");
    (*raiz)->direita->direita = novaSala("Jardim");
}
 
void criarPistas(Pista** raiz) {
    *raiz = novaPista("Casaco sujo encontrado no mancebo");
    (*raiz)->esquerda = novaPista("Bilhete encontrado na mesa");
    (*raiz)->direita = novaPista("Taca de vinho quebrada no chao");
    (*raiz)->esquerda->esquerda = novaPista("Bota suja de lama");
    (*raiz)->direita->direita = novaPista("Buraco recem cavado no jardim");
}
 
void associarPistas(Sala* sala, Pista* pista) {
    if (sala == NULL) return;
    sala->pista = pista;
    associarPistas(sala->esquerda, pista ? pista->esquerda : NULL);
    associarPistas(sala->direita, pista ? pista->direita : NULL);
}
 
Pista* inserirABB(Pista* raiz, Pista* nova) {
    if (raiz == NULL) return nova;
    if (strcmp(nova->texto, raiz->texto) < 0)
        raiz->esquerda = inserirABB(raiz->esquerda, nova);
    else
        raiz->direita = inserirABB(raiz->direita, nova);
    return raiz;
}
 
void emOrdem(Pista* raiz, int* contador) {
    if (raiz == NULL) return;
    emOrdem(raiz->esquerda, contador);
    printf(" %d. %s\n", ++(*contador), raiz->texto);
    emOrdem(raiz->direita, contador);
}
 
void exibirPistasOrdenadas(Pista** coletadas, int total) {
    if (total == 0) {
        printf("\nNenhuma pista foi coletada.\n");
        return;
    }
 
    Pista* abb = NULL;
    for (int i = 0; i < total; i++) {
        Pista* copia = novaPista(coletadas[i]->texto);
        abb = inserirABB(abb, copia);
    }
 
    separador();
    printf("\n=== Pistas em ordem alfabetica: ===\n");
    int contador = 0;
    emOrdem(abb, &contador);
    separador();
 
    liberarPista(abb);
}
 
void exploracao(Sala* atual, Pista** coletadas, int* total) {
    if (atual == NULL) {
        printf("Caminho sem saida!\n");
        return;
    }
 
    separador();
    printf("\nVoce esta na sala: %s\n", atual->nome);
 
    if (atual->pista) {
        printf("Voce encontrou a seguinte pista: %s\n", atual->pista->texto);
        coletadas[*total] = atual->pista;
        (*total)++;
    }
 
    separador();
 
    if (atual->esquerda) printf("\n > Esquerda: %s\n", atual->esquerda->nome);
    if (atual->direita)  printf("\n > Direita:  %s\n", atual->direita->nome);
 
    if (!atual->esquerda && !atual->direita) {
        printf("\nVoce chegou ao fim do caminho!\n");
        return;
    }
 
    separador();
    printf("\nEscolha uma direcao (esquerda/direita) ou 's' para sair:\n");
    char opcao[15];
    scanf("%s", opcao);
 
    if (opcao[0] == 'E' || opcao[0] == 'e')
        exploracao(atual->esquerda, coletadas, total);
    else if (opcao[0] == 'D' || opcao[0] == 'd')
        exploracao(atual->direita, coletadas, total);
    else if (opcao[0] == 'S' || opcao[0] == 's')
        printf("Saindo da exploracao...\n");
    else {
        printf("Opcao invalida, tente novamente.\n");
        exploracao(atual, coletadas, total);
    }
}
 
// ========== MAIN ==========
 
int main() {
    struct Sala* raiz = NULL;
    struct Pista* pistaRaiz = NULL;
 
    inicializarQuadro();
    criarMapa(&raiz);
    criarPistas(&pistaRaiz);
    associarPistas(raiz, pistaRaiz);
 
    Pista* coletadas[10];
    int total = 0;
 
    separador();
    printf("\n Seja bem-vindo a mais um caso de investigacao, detetive!\n");
    printf(" Prepare-se para explorar esta assombrosa mansao e desvendar seus segredos.\n");
    printf("\n Pressione enter para continuar...\n");
    separador();
    getchar();
 
    // 1. explorar salas e coletar pistas
    exploracao(raiz, coletadas, &total);
 
    // 2. exibir pistas coletadas em ordem alfabetica (ABB)
    exibirPistasOrdenadas(coletadas, total);
 
    // 3. fase de acusacao: associar pistas a suspeitos (hash)
    associarPistasASuspeitos(coletadas, total);
 
    // 4. exibir quadro completo: suspeito -> pistas
    listarAssociacoes();
 
    // 5. exibir veredicto: suspeito mais citado
    exibirSuspeitoMaisCitado();
 
    // liberar toda a memoria
    liberarSala(raiz);
    liberarPista(pistaRaiz);
    liberarQuadro();
 
    raiz = NULL;
    pistaRaiz = NULL;
 
    return 0;
}