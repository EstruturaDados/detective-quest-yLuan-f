#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// funcao criar de mapa, onde cada sala tem nome e ponteiros para salas adjacentes.
typedef struct Sala {
    char nome[50];
    struct Sala* direita;
    struct Sala* esquerda;
} Sala;

// funcao para criar nova sala, alocando memoria e copiando nome.
Sala* novaSala(char* nome) {
    Sala* sala = (Sala*)malloc(sizeof(Sala));
    strncpy(sala->nome, nome, 50);
    sala->direita = NULL;
    sala->esquerda = NULL;
    return sala;
}

// funcao para liberar memoria alocada para as salas, usando recursao.
void liberarSala(Sala* raiz) {
    if (raiz != NULL) {
        liberarSala(raiz->esquerda);
        liberarSala(raiz->direita);
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

// funcao de exploracao, onde o jogador pode escolher direcao para seguir, usando recursao para navegar pelas salas.
void exploracao (Sala* atual){
    if (atual == NULL) {
        printf("Caminho sem saida!\n");
        return;
    }

    printf("\n Voce esta na sala: %s \n", atual->nome);

    if (atual->esquerda) printf("\n - Esquerda: %s\n", atual->esquerda->nome);
    if (atual->direita) printf("\n - Direita: %s\n", atual->direita->nome);
    
    if (!atual->esquerda && !atual->direita) {
        printf("\n Voce chegou a uma sala sem saida, fim do caminho! \n");
        return;
    }

    printf("\n Escolha uma direcao para prosseguir (esquerda/direita) ou pressione s para sair: \n");
    char opcao[15];
    scanf("%s", opcao);

    if (opcao[0] == 'E' || opcao[0] == 'e' || strcmp(opcao, "Esquerda") == 0 || strcmp(opcao, "esquerda") == 0)
        exploracao(atual->esquerda);
    else if (opcao[0] == 'D' || opcao[0] == 'd' || strcmp(opcao, "Direita") == 0 || strcmp(opcao, "direita") == 0)
        exploracao(atual->direita);
    else if (opcao[0] == 'S' || opcao[0] == 's')
        printf("Saindo da exploracao...\n");
    else {
        printf("Opcao invalida, tente novamente.\n");
        exploracao(atual);
    }
}

// funcao principal, onde o mapa e exploracao sao iniciados, e memoria e liberada ao final.
int main() {
    
    struct Sala* raiz = NULL;
    criarMapa(&raiz);

    printf("\n Seja bem-vindo a mais um caso de investigacao, detetive! Prepare-se para explorar esta assombrosa mansao e desvendar seus segredos. \n");
    printf("Pressione enter para continuar...\n");
    getchar();

    exploracao(raiz);
    liberarSala(raiz);
    raiz = NULL;

    
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

