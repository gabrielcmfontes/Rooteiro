#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definicao da estrutura para os eventos turisticos
typedef struct evento {
    char nome[50];
    float avaliacao;
} TEvento;

// Definicao da estrutura para as cidades
typedef struct cidade {
    char nome[50];
    TEvento *eventos;
    int numEventos;
} TCidade;

// Definicao da estrutura para os itens da arvore
typedef struct item {
    int chave;
    TCidade cidade;
} TItem;

// Definicao da estrutura para as celulas da arvore
typedef struct celula {
    TItem item;
    struct celula *pai;
    struct celula *esq;
    struct celula *dir;
} TCelula;

// Definicao da estrutura para a arvore
typedef struct arvore {
    TCelula *raiz;
} TArvore;

// Funcao para inicializar a arvore
void InicializaArvore(TArvore *arvore) {
    arvore->raiz = NULL;
}

// Funcao para criar uma nova celula
TCelula* CriaCelula(TItem item) {
    TCelula *celula = (TCelula*) malloc(sizeof(TCelula));
    if (celula == NULL) {
        printf("Erro: Falha na alocacao de memoria\n");
        exit(1);
    }

    celula->item = item;
    celula->pai = NULL;
    celula->esq = NULL;
    celula->dir = NULL;

    return celula;
}

int CidadeExiste(TCelula *raiz, const char *nomeCidade) {
    if (raiz == NULL) {
        return 0;
    }

    if (strcmp(raiz->item.cidade.nome, nomeCidade) == 0) {
        return 1;
    }

    // Busca recursiva nos dois lados
    return CidadeExiste(raiz->esq, nomeCidade) || CidadeExiste(raiz->dir, nomeCidade);
}


// Funcao para gerar um nome de cidade aleatorio
void GeraNomeCidade(char *nome, TArvore *arvore) {
    static char *prefixos[] = {"Nova", "Sao", "Santa", "Santo", "Porto", "Vila", "Belo", "Bela"};
    static char *sufixos[] = {"Horizonte", "Alegre", "Cruz", "Mar", "Terra", "Vista", "Campo", "Jardim", "Praia", "Montanha"};
    static char *meio[] = {"do", "da", "de"};

    int nomeUnico = 0;

    while ((!nomeUnico)) {
        int tipo = rand() % 3;

        if (tipo == 0) {
            // Prefixo + Sufixo
            int prefixoIdx = rand() % 8;
            int sufixoIdx = rand() % 10;
            sprintf(nome, "%s %s", prefixos[prefixoIdx], sufixos[sufixoIdx]);
        } else if (tipo == 1) {
            // Apenas um nome
            static char *nomes[] = {"Campinas", "Ouro Preto", "Diamantina", "Petropolis", "Gramado", "Curitiba", "Florianopolis", "Buzios", "Paraty", "Olinda"};
            int nomeIdx = rand() % 10;
            strcpy(nome, nomes[nomeIdx]);
        } else {
            // Nome composto
            static char *primeiros[] = {"Ribeirao", "Monte", "Vale", "Serra", "Campo", "Morro", "Rio", "Lagoa", "Recanto"};
            int primeiroIdx = rand() % 9;
            int meioIdx = rand() % 3;
            int sufixoIdx = rand() % 10;
            sprintf(nome, "%s %s %s", primeiros[primeiroIdx], meio[meioIdx], sufixos[sufixoIdx]);
        }

        nomeUnico = !CidadeExiste(arvore->raiz, nome);
    }
}

// Funcao para gerar um nome de evento aleatorio
void GeraNomeEvento(char *nome) {
    static char *tipos[] = {"Festival de", "Feira de", "Mostra de", "Tour por", "Visita ao", "Passeio de", "Espetaculo de", "Exposicao de"};
    static char *temas[] = {"Arte", "Musica", "Gastronomia", "Historia", "Cultura", "Natureza", "Artesanato", "Cinema", "Teatro", "Danca"};

    int tipoIdx = rand() % 8;
    int temaIdx = rand() % 10;


    sprintf(nome, "%s %s", tipos[tipoIdx], temas[temaIdx]);
}

// Funcao para gerar eventos aleatorios para uma cidade
void GeraEventos(TCidade *cidade, int numEventos) {
    cidade->numEventos = numEventos;
    cidade->eventos = (TEvento*) malloc(numEventos * sizeof(TEvento));

    if (cidade->eventos == NULL) {
        printf("Erro: Falha na alocacao de memoria para eventos\n");
        exit(1);
    }

    for (int i = 0; i < numEventos; i++) {
        GeraNomeEvento(cidade->eventos[i].nome);
        cidade->eventos[i].avaliacao = (rand() % 100) / 10.0; // Gera avaliacoes entre 0.0 e 10.0
    }
}

// Funcao para gerar uma cidade aleatoria
TCidade GeraCidadeAleatoria(TArvore *arvore) {
    TCidade cidade;
    GeraNomeCidade(cidade.nome, arvore);

    int numEventos = 5 + (rand() % 6);
    GeraEventos(&cidade, numEventos);

    return cidade;
}

// Funcao para inserir um no na arvore, conforme a estrutura da ABB do documento
void InsereNo(TArvore *arvore, TItem item) {
    TCelula *z = CriaCelula(item);
    TCelula *y = NULL;
    TCelula *x = arvore->raiz;

    // Encontra a posicao para inserir
    while (x != NULL) {
        y = x;
        if (z->item.chave < x->item.chave) {
            x = x->esq;
        } else {
            x = x->dir;
        }
    }

    z->pai = y;

    if (y == NULL) {
        // Arvore vazia, z se torna a raiz
        arvore->raiz = z;
    } else if (z->item.chave < y->item.chave) {
        // z vai para o lado esquerdo de y
        y->esq = z;
    } else {
        // z vai para o lado direito de y
        y->dir = z;
    }
}

// Funcao para gerar e inserir cidades aleatorias na arvore
void GeraInsereCidades(TArvore *arvore, int numCidades) {
    for (int i = 0; i < numCidades; i++) {
        TItem item;
        item.chave = rand() % 1000; // Chave aleatoria entre 0 e 999
        item.cidade = GeraCidadeAleatoria(arvore);

        InsereNo(arvore, item);
    }
}

void GeraCidadesEventosPersonalizados(TArvore *arvore, int numCidades) {
    // Estruturas para eventos personalizados
    char *eventosNomes[] = {
        "Festival de Musica", "Feira de Arte", "Mostra de Cinema",
        "Tour pelos Pontos Turisticos", "Visita ao Museu",
        "Passeio pela Praia", "Espetaculo de Teatro",
        "Exposicao de Artesanato", "Festival Gastronomico",
        "Tour Historico", "Passeio de Barco", "Visita a Mercados",
        "Festival Cultural", "Tour Noturno", "Passeio Ecologico"
    };

    for (int i = 0; i < numCidades; i++) {
        TItem item;
        item.chave = 100 + i * 100; // Chaves 100, 200, 300, etc.

        // Usar o algoritmo original para gerar o nome da cidade
        GeraNomeCidade(item.cidade.nome, arvore);

        // Configurar eventos personalizados
        int numEventos = 5 + (rand() % 6); // Entre 5 e 10 eventos
        item.cidade.numEventos = numEventos;
        item.cidade.eventos = (TEvento*) malloc(numEventos * sizeof(TEvento));

        if (item.cidade.eventos == NULL) {
            printf("Erro: Falha na alocacao de memoria para eventos\n");
            exit(1);
        }

        // Criar eventos personalizados sem repetir
        int eventosDisponiveis[15];
        for (int j = 0; j < 15; j++) {
            eventosDisponiveis[j] = j;
        }

        // Embaralhar os eventos disponíveis (algoritmo Fisher-Yates)
        for (int j = 14; j > 0; j--) {
            int k = rand() % (j + 1);
            int temp = eventosDisponiveis[j];
            eventosDisponiveis[j] = eventosDisponiveis[k];
            eventosDisponiveis[k] = temp;
        }

        // Configurar eventos únicos para esta cidade
        for (int j = 0; j < numEventos; j++) {
            int idx = eventosDisponiveis[j];
            strcpy(item.cidade.eventos[j].nome, eventosNomes[idx]);
            // Gera valores aleatórios entre 0.0 e 10.0 com incrementos de 0.1
            item.cidade.eventos[j].avaliacao = (rand() % 101) / 10.0;
        }

        // Inserir na árvore
        InsereNo(arvore, item);
    }

    printf("Geradas %d cidades usando o algoritmo original com eventos personalizados.\n", numCidades);
}

// Percurso in-ordem (caminhamento central)
void CaminhamentoCentral(TCelula *x) {
    if (x != NULL) {
        CaminhamentoCentral(x->esq);
        printf("Chave: %d, Cidade: %s, Eventos: %d\n",
               x->item.chave,
               x->item.cidade.nome,
               x->item.cidade.numEventos);
        CaminhamentoCentral(x->dir);
    }
}

// Percurso pre-ordem
void CaminhamentoPreOrdem(TCelula *x) {
    if (x != NULL) {
        printf("Chave: %d, Cidade: %s, Eventos: %d\n",
               x->item.chave,
               x->item.cidade.nome,
               x->item.cidade.numEventos);
        CaminhamentoPreOrdem(x->esq);
        CaminhamentoPreOrdem(x->dir);
    }
}

// Percurso pos-ordem
void CaminhamentoPosOrdem(TCelula *x) {
    if (x != NULL) {
        CaminhamentoPosOrdem(x->esq);
        CaminhamentoPosOrdem(x->dir);
        printf("Chave: %d, Cidade: %s, Eventos: %d\n",
               x->item.chave,
               x->item.cidade.nome,
               x->item.cidade.numEventos);
    }
}

// Funcao para buscar um no com uma determinada chave na arvore
TCelula* Busca(TCelula *x, int chave) {
    if (x == NULL || x->item.chave == chave) {
        return x;
    }

    if (chave < x->item.chave) {
        return Busca(x->esq, chave);
    } else {
        return Busca(x->dir, chave);
    }
}

// Funcao para encontrar o no com a chave minima na arvore
TCelula* Minimo(TCelula *x) {
    if (x == NULL) {
        return NULL;
    }

    while (x->esq != NULL) {
        x = x->esq;
    }

    return x;
}

// Funcao para encontrar o no com a chave maxima na arvore
TCelula* Maximo(TCelula *x) {
    if (x == NULL) {
        return NULL;
    }

    while (x->dir != NULL) {
        x = x->dir;
    }

    return x;
}

// Funcao para encontrar o sucessor de um no x
TCelula* Sucessor(TCelula *x) {
    if (x == NULL) {
        return NULL;
    }

    // Se x tem um filho a direita, o sucessor e o minimo da subarvore direita
    if (x->dir != NULL) {
        return Minimo(x->dir);
    }

    // Senao, e o ancestral mais baixo cujo filho esquerdo tambem e ancestral de x
    TCelula *y = x->pai;
    while (y != NULL && x == y->dir) {
        x = y;
        y = y->pai;
    }

    return y;
}

// Funcao para encontrar o predecessor de um no x
TCelula* Predecessor(TCelula *x) {
    if (x == NULL) {
        return NULL;
    }

    // Se x tem um filho a esquerda, o predecessor e o maximo da subarvore esquerda
    if (x->esq != NULL) {
        return Maximo(x->esq);
    }

    // Senao, e o ancestral mais baixo cujo filho direito tambem e ancestral de x
    TCelula *y = x->pai;
    while (y != NULL && x == y->esq) {
        x = y;
        y = y->pai;
    }

    return y;
}

// Funcao para mostrar os eventos de uma cidade
void MostraEventos(TCidade cidade) {
    printf("Eventos em %s:\n", cidade.nome);
    for (int i = 0; i < cidade.numEventos; i++) {
        printf("  %d. %s - Avaliacao: %.1f\n",
               i+1,
               cidade.eventos[i].nome,
               cidade.eventos[i].avaliacao);
    }
}

// Funcao para liberar a memoria alocada para os eventos de uma cidade
void LiberaEventos(TCidade *cidade) {
    if (cidade->eventos != NULL) {
        free(cidade->eventos);
        cidade->eventos = NULL;
    }
}

// Funcao para liberar a memoria alocada para um no e seus descendentes
void LiberaNo(TCelula *x) {
    if (x != NULL) {
        LiberaNo(x->esq);
        LiberaNo(x->dir);
        LiberaEventos(&(x->item.cidade));
        free(x);
    }
}

// Funcao para liberar toda a memoria alocada para a arvore
void LiberaArvore(TArvore *arvore) {
    LiberaNo(arvore->raiz);
    arvore->raiz = NULL;
}


int main() {
    // Inicializa o gerador de numeros aleatorios
    srand(time(NULL));

    TArvore arvore;
    InicializaArvore(&arvore);

    int numCidades = 10;
    printf("Gerando %d cidades com verificação de nomes únicos...\n", numCidades);

    GeraInsereCidades(&arvore, numCidades);

    printf("\n=== LISTA DE CIDADES GERADAS (em ordem pela chave) ===\n");
    CaminhamentoCentral(arvore.raiz);

    printf("\n=== VERIFICACAO DE NOMES ===\n");

    // Função simples que varre a árvore e mostra todas as cidades para verificação
    void VerificaNomes(TCelula *no) {
        if (no != NULL) {
            VerificaNomes(no->esq);
            printf("Cidade: %-25s | Chave: %d\n", no->item.cidade.nome, no->item.chave);
            VerificaNomes(no->dir);
        }
    }

    VerificaNomes(arvore.raiz);

    // Libera a memória no final
    LiberaArvore(&arvore);

    printf("\nMemória liberada com sucesso!\n");

    return 0;
}
