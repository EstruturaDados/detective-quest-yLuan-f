#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUADRO 10
#define MAX_SUSPEITOS 3

// ========== STRUCTS ==========

// funcao criar de mapa, onde cada sala tem nome e ponteiros para salas adjacentes.
typedef struct Sala {
    char nome[50];
    struct Pista* pista; // permitindo associar uma pista a cada sala.
    struct Sala* direita;
    struct Sala* esquerda;
} Sala;

// funcao para criar as pistas.
typedef struct Pista {
    char texto[50];
    struct Pista* direita;
    struct Pista* esquerda;
} Pista;

//funcao para criar quadro de pistas.
typedef struct Quadro {
    char descricao[50];
    struct Quadro* proximo;
} Quadro; 

 // ========== SUSPEITOS E HASH ==========

Quadro* quadroHash[MAX_QUADRO]; // tabela hash para associar pistas a suspeitos

// suspeitos
const char* suspeitos[] = {"Jardineiro", "Filho", "Mae"};

// funcao para inicializar o quadro.
void inicializarQuadro() {
    for (int i = 0; i < MAX_QUADRO; i++) {
        quadroHash[i] = NULL;
    }
}
 
// funcao para iniciar a tabela hash, alocando memoria e definindo ponteiros como NULL.
int funcaoHash(const char* chave) {
    int soma = 0;
    for(int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i] * (i + 1);
    }
    return soma % MAX_QUADRO;
}

// inserir pista no quadro hash.
void inserirPista 
// funcao para separacao visual entre etapas do jogo.
void separador() {
    printf("\n\n========================================\n\n");
}

// ==========  FUNCOES DE MANIPULACAO DE MAPA. PISTA E QUADRO ==========

// funcao para criar nova sala, alocando memoria e copiando nome.
Sala* novaSala(char* nome) {
    Sala* sala = (Sala*)malloc(sizeof(Sala));
    strncpy(sala->nome, nome, 50);
    sala->pista = NULL; 
    sala->direita = NULL;
    sala->esquerda = NULL;
    return sala;
}

// funcao para criar nova pista, alocando memoria e copiando texto.
Pista* novaPista(char* texto) {
    Pista* pista = (Pista*)malloc(sizeof(Pista));
    strncpy(pista->texto, texto, 50);
    pista->direita = NULL;
    pista->esquerda = NULL;
    return pista;
}
// funcao para liberar memoria alocada para as salas.
void liberarSala(Sala* raiz) {
    if (raiz != NULL) {
        liberarSala(raiz->esquerda);
        liberarSala(raiz->direita);
        free(raiz);
    }
}

// fumca para liberar memoria alocada para as pistas.
void liberarPista(Pista* raiz) {
    if (raiz != NULL) {
        liberarPista(raiz->esquerda);
        liberarPista(raiz->direita);
        free(raiz);
    }
}

// funcao para criar o mapa da mansao, excluindo necessidade de insercao manual.
void criarMapa (Sala** raiz) {
        *raiz = novaSala("Hall de entrada");
        (*raiz)->esquerda = novaSala("Biblioteca");
        (*raiz)->direita = novaSala("Cozinha");
        (*raiz)->esquerda->esquerda = novaSala("Quarto de servico");
        (*raiz)->esquerda->direita = novaSala("Estudio de arte");
        (*raiz)->direita->direita = novaSala("Jardim");

}

// funcao para criar as pistas.
void criarPistas (Pista** raiz) {
    *raiz = novaPista("Casaco sujo encontrado no mancebo");
    (*raiz)->esquerda = novaPista("Bilhete encontrado na mesa");
    (*raiz)->direita = novaPista("Taca de vinho quebrada no chao");
    (*raiz)->esquerda->esquerda = novaPista("Bota suja de lama");
    (*raiz)->direita->direita = novaPista("Buraco recem cavado no jardim");
}



// funcao para associar pistas as salas, permitindo o jogador coletar automaticamente as pistas ao entrar nas salas.
void associarPistas (Sala* sala, Pista* pista) {
    if (sala == NULL) return;

    sala->pista = pista;

    associarPistas(sala->esquerda, pista ? pista->esquerda : NULL);
    associarPistas(sala->direita, pista ? pista->direita : NULL);
}
// funcao para inserir pistas na arvore binaria de busca, organizando as pistas em ordem alfabetica.
Pista* inserirABB(Pista* raiz, Pista* nova) {
    if (raiz == NULL) return nova;

    if (strcmp(nova->texto, raiz->texto) < 0)
        raiz->esquerda = inserirABB(raiz->esquerda, nova);
    else
        raiz->direita = inserirABB(raiz->direita, nova);

    return raiz;
}

// funcao para percorrer a ABB em ordem, exibindo as pistas em ordem alfabetica.
void emOrdem(Pista* raiz, int* contador) {
    if (raiz == NULL) return;
    emOrdem(raiz->esquerda, contador);
    printf(" %d. %s\n", ++(*contador), raiz->texto);
    emOrdem(raiz->direita, contador);
}

// funcao para exibir as pistas em ordem alfabetica, usando percurso em ordem na ABB.
void exibirPistasOrdenadas(Pista** coletadas, int total) {
    if (total == 0) {
        printf("\nNenhuma pista foi coletada.\n");
        return;
    }

    // cria novos nos para a ABB, sem tocar nos originais
    Pista* abb = NULL;
    for (int i = 0; i < total; i++) {
        Pista* copia = novaPista(coletadas[i]->texto);  // novo no independente
        abb = inserirABB(abb, copia);
    }

    separador();
    printf("\n=== Pistas em ordem alfabetica: ===\n");
    int contador = 0;
    emOrdem(abb, &contador);
    separador();

    liberarPista(abb);  // agora e seguro liberar pois sao nos proprios da ABB
}

// funcao de exploracao, onde o jogador pode escolher direcao para seguir, usando recursao para navegar pelas salas.
void exploracao (Sala* atual, Pista** coletadas, int* total) {
    if (atual == NULL) {
        printf("Caminho sem saida!\n");
        return;
    }

    separador();
    printf("\n");
    printf("\nVoce esta na sala: %s \n", atual->nome); 
    printf("\n");

    if (atual->pista) {
        printf("Voce encontrou a seguinte pista: %s \n", atual->pista->texto);
        coletadas[*total] = atual->pista;
        (*total)++;
    }

    separador();

    if (atual->esquerda) printf("\n > Esquerda: %s\n", atual->esquerda->nome);
    if (atual->direita) printf("\n > Direita: %s\n", atual->direita->nome);
    
    if (!atual->esquerda && !atual->direita) {

        printf("\n Voce chegou ao fim do caminho!\n");
        return;
    }

    separador();

    printf("\n Escolha uma direcao para prosseguir (esquerda/direita) ou pressione s para sair: \n");
    char opcao[15];
    scanf("%s", opcao);

    if (opcao[0] == 'E' || opcao[0] == 'e' || strcmp(opcao, "Esquerda") == 0 || strcmp(opcao, "esquerda") == 0)
        exploracao(atual->esquerda, coletadas, total);
    else if (opcao[0] == 'D' || opcao[0] == 'd' || strcmp(opcao, "Direita") == 0 || strcmp(opcao, "direita") == 0)
        exploracao(atual->direita, coletadas, total);
    else if (opcao[0] == 'S' || opcao[0] == 's')
        printf("Saindo da exploracao...\n");
    else {
        printf("Opcao invalida, tente novamente.\n");
        exploracao(atual, coletadas, total);
    }
}

// funcao principal, onde o mapa e exploracao sao iniciados, e memoria e liberada ao final.
int main() {
    
    struct Sala* raiz = NULL;
    struct Pista* pistaRaiz = NULL;

    criarMapa(&raiz);
    criarPistas(&pistaRaiz);
    associarPistas(raiz, pistaRaiz);

    Pista* coletadas[10]; 
    int total = 0;

    separador();
    printf("\n Seja bem-vindo a mais um caso de investigacao, detetive! Prepare-se para explorar esta assombrosa mansao e desvendar seus segredos. \n");
    printf("\n Pressione enter para continuar...\n");
    separador();
    getchar();

    exploracao(raiz, coletadas, &total);
    exibirPistasOrdenadas(coletadas, total);

    liberarSala(raiz);
    liberarPista(pistaRaiz);

    raiz = NULL;
    pistaRaiz = NULL;

    
    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}

