#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definicao da estrutura para os eventos turisticos
typedef struct evento
{
    char nome[50];
    float avaliacao;
} TEvento;

// Definicao da estrutura para as cidades
typedef struct cidade
{
    char nome[50];
    TEvento *eventos;
    int numEventos;
} TCidade;

// Definicao da estrutura para os itens da arvore
typedef struct item
{
    int chave;
    TCidade cidade;
} TItem;

// Definicao da estrutura para as celulas da arvore
typedef struct celula
{
    TItem item;
    struct celula *pai;
    struct celula *esq;
    struct celula *dir;
} TCelula;

// Definicao da estrutura para a arvore
typedef struct arvore
{
    TCelula *raiz;
} TArvore;

// Funcao para inicializar a arvore
void InicializaArvore(TArvore *arvore)
{
    arvore->raiz = NULL;
}

// Funcao para criar uma nova celula
TCelula *CriaCelula(TItem item)
{
    TCelula *celula = (TCelula *)malloc(sizeof(TCelula));
    if (celula == NULL)
    {
        printf("Erro: Falha na alocacao de memoria\n");
        exit(1);
    }

    celula->item = item;
    celula->pai = NULL;
    celula->esq = NULL;
    celula->dir = NULL;

    return celula;
}

// Funcao para gerar um nome de cidade aleatorio
void GeraNomeCidade(char *nome)
{
    static char *prefixos[] = {"Nova", "Sao", "Santa", "Santo", "Porto", "Vila", "Belo", "Bela"};
    static char *sufixos[] = {"Horizonte", "Alegre", "Cruz", "Mar", "Terra", "Vista", "Campo", "Jardim", "Praia", "Montanha"};
    static char *meio[] = {"do", "da", "dos", "das", "de"};

    int tipo = rand() % 3;

    if (tipo == 0)
    {
        // Prefixo + Sufixo
        int prefixoIdx = rand() % 8;
        int sufixoIdx = rand() % 10;
        sprintf(nome, "%s %s", prefixos[prefixoIdx], sufixos[sufixoIdx]);
    }
    else if (tipo == 1)
    {
        // Apenas um nome
        static char *nomes[] = {"Campinas", "Ouro Preto", "Diamantina", "Petropolis", "Gramado", "Curitiba", "Florianopolis", "Buzios", "Paraty", "Olinda"};
        int nomeIdx = rand() % 10;
        strcpy(nome, nomes[nomeIdx]);
    }
    else
    {
        // Nome composto
        static char *primeiros[] = {"Ribeirao", "Monte", "Vale", "Serra", "Campo", "Morro", "Rio", "Lagoa", "Recanto"};
        int primeiroIdx = rand() % 9;
        int meioIdx = rand() % 5;
        int sufixoIdx = rand() % 10;
        sprintf(nome, "%s %s %s", primeiros[primeiroIdx], meio[meioIdx], sufixos[sufixoIdx]);
    }
}

// Funcao para gerar um nome de evento aleatorio
void GeraNomeEvento(char *nome)
{
    static char *tipos[] = {"Festival de", "Feira de", "Mostra de", "Tour por", "Visita ao", "Passeio de", "Espetaculo de", "Exposicao de"};
    static char *temas[] = {"Arte", "Musica", "Gastronomia", "Historia", "Cultura", "Natureza", "Artesanato", "Cinema", "Teatro", "Danca"};

    int tipoIdx = rand() % 8;
    int temaIdx = rand() % 10;

    sprintf(nome, "%s %s", tipos[tipoIdx], temas[temaIdx]);
}

// Funcao para gerar eventos aleatorios para uma cidade
void GeraEventos(TCidade *cidade, int numEventos)
{
    cidade->numEventos = numEventos;
    cidade->eventos = (TEvento *)malloc(numEventos * sizeof(TEvento));

    if (cidade->eventos == NULL)
    {
        printf("Erro: Falha na alocacao de memoria para eventos\n");
        exit(1);
    }

    for (int i = 0; i < numEventos; i++)
    {
        GeraNomeEvento(cidade->eventos[i].nome);
        cidade->eventos[i].avaliacao = (rand() % 100) / 10.0; // Gera avaliacoes entre 0.0 e 10.0
    }
}

// Funcao para gerar uma cidade aleatoria
TCidade GeraCidadeAleatoria()
{
    TCidade cidade;
    GeraNomeCidade(cidade.nome);

    int numEventos = 5 + (rand() % 6);
    GeraEventos(&cidade, numEventos);

    return cidade;
}

// Funcao para inserir um no na arvore, conforme a estrutura da ABB do documento
void InsereNo(TArvore *arvore, TItem item)
{
    TCelula *z = CriaCelula(item);
    TCelula *y = NULL;
    TCelula *x = arvore->raiz;

    // Encontra a posicao para inserir
    while (x != NULL)
    {
        y = x;
        if (z->item.chave < x->item.chave)
        {
            x = x->esq;
        }
        else
        {
            x = x->dir;
        }
    }

    z->pai = y;

    if (y == NULL)
    {
        // Arvore vazia, z se torna a raiz
        arvore->raiz = z;
    }
    else if (z->item.chave < y->item.chave)
    {
        // z vai para o lado esquerdo de y
        y->esq = z;
    }
    else
    {
        // z vai para o lado direito de y
        y->dir = z;
    }
}

// Funcao para gerar e inserir cidades aleatorias na arvore
void GeraInsereCidades(TArvore *arvore, int numCidades)
{
    for (int i = 0; i < numCidades; i++)
    {
        TItem item;
        item.chave = rand() % 1000; // Chave aleatoria entre 0 e 999
        item.cidade = GeraCidadeAleatoria();

        InsereNo(arvore, item);
    }
}

// Nova funcao para gerar cidades e eventos personalizados usando o algoritmo original
void GeraCidadesEventosPersonalizados(TArvore *arvore, int numCidades)
{
    // Estruturas para eventos personalizados
    char *eventosNomes[] = {
        "Festival de Musica", "Feira de Arte", "Mostra de Cinema",
        "Tour pelos Pontos Turisticos", "Visita ao Museu",
        "Passeio pela Praia", "Espetaculo de Teatro",
        "Exposicao de Artesanato", "Festival Gastronomico",
        "Tour Historico", "Passeio de Barco", "Visita a Mercados",
        "Festival Cultural", "Tour Noturno", "Passeio Ecologico"};

    float avaliacoes[] = {
        9.8, 8.5, 9.0, 9.5, 7.9,
        9.2, 8.0, 8.3, 9.7, 8.6,
        9.3, 8.9, 9.6, 8.2, 9.4};

    for (int i = 0; i < numCidades; i++)
    {
        TItem item;
        item.chave = 100 + i * 100; // Chaves 100, 200, 300, etc.

        // Usar o algoritmo original para gerar o nome da cidade
        GeraNomeCidade(item.cidade.nome);

        // Configurar eventos personalizados
        int numEventos = 5 + (rand() % 6); // Entre 5 e 10 eventos
        item.cidade.numEventos = numEventos;
        item.cidade.eventos = (TEvento *)malloc(numEventos * sizeof(TEvento));

        if (item.cidade.eventos == NULL)
        {
            printf("Erro: Falha na alocacao de memoria para eventos\n");
            exit(1);
        }

        // Criar eventos personalizados sem repetir
        int eventosDisponiveis[15];
        for (int j = 0; j < 15; j++)
        {
            eventosDisponiveis[j] = j;
        }

        // Embaralhar os eventos disponíveis (algoritmo Fisher-Yates)
        for (int j = 14; j > 0; j--)
        {
            int k = rand() % (j + 1);
            int temp = eventosDisponiveis[j];
            eventosDisponiveis[j] = eventosDisponiveis[k];
            eventosDisponiveis[k] = temp;
        }

        // Configurar eventos únicos para esta cidade
        for (int j = 0; j < numEventos; j++)
        {
            int idx = eventosDisponiveis[j];
            strcpy(item.cidade.eventos[j].nome, eventosNomes[idx]);
            item.cidade.eventos[j].avaliacao = avaliacoes[idx];
        }

        // Inserir na árvore
        InsereNo(arvore, item);
    }

    printf("Geradas %d cidades usando o algoritmo original com eventos personalizados.\n", numCidades);
}

// Percurso in-ordem (caminhamento central)
void CaminhamentoCentral(TCelula *x)
{
    if (x != NULL)
    {
        CaminhamentoCentral(x->esq);
        printf("Chave: %d, Cidade: %s, Eventos: %d\n",
               x->item.chave,
               x->item.cidade.nome,
               x->item.cidade.numEventos);
        CaminhamentoCentral(x->dir);
    }
}

// Percurso pre-ordem
void CaminhamentoPreOrdem(TCelula *x)
{
    if (x != NULL)
    {
        printf("Chave: %d, Cidade: %s, Eventos: %d\n",
               x->item.chave,
               x->item.cidade.nome,
               x->item.cidade.numEventos);
        CaminhamentoPreOrdem(x->esq);
        CaminhamentoPreOrdem(x->dir);
    }
}

// Percurso pos-ordem
void CaminhamentoPosOrdem(TCelula *x)
{
    if (x != NULL)
    {
        CaminhamentoPosOrdem(x->esq);
        CaminhamentoPosOrdem(x->dir);
        printf("Chave: %d, Cidade: %s, Eventos: %d\n",
               x->item.chave,
               x->item.cidade.nome,
               x->item.cidade.numEventos);
    }
}

// Funcao para buscar um no com uma determinada chave na arvore

TCelula *Busca(TCelula *x, char *nome)
{
    if (x == NULL || strcmp(x->item.cidade.nome, nome) == 0)
    {
        return x;
    }

    if (strcmp(nome, x->item.cidade.nome) < 0)
    {
        return Busca(x->esq, nome);
    }
    else
    {
        return Busca(x->dir, nome);
    }
}

void PesquisaEvento(char nomeDoEvento, TArvore arvore)
{
}

// Funcao para encontrar o no com a chave minima na arvore
TCelula *Minimo(TCelula *x)
{
    if (x == NULL)
    {
        return NULL;
    }

    while (x->esq != NULL)
    {
        x = x->esq;
    }

    return x;
}

// Funcao para encontrar o no com a chave maxima na arvore
TCelula *Maximo(TCelula *x)
{
    if (x == NULL)
    {
        return NULL;
    }

    while (x->dir != NULL)
    {
        x = x->dir;
    }

    return x;
}

// Funcao para encontrar o sucessor de um no x
TCelula *Sucessor(TCelula *x)
{
    if (x == NULL)
    {
        return NULL;
    }

    // Se x tem um filho a direita, o sucessor e o minimo da subarvore direita
    if (x->dir != NULL)
    {
        return Minimo(x->dir);
    }

    // Senao, e o ancestral mais baixo cujo filho esquerdo tambem e ancestral de x
    TCelula *y = x->pai;
    while (y != NULL && x == y->dir)
    {
        x = y;
        y = y->pai;
    }

    return y;
}

// Funcao para encontrar o predecessor de um no x
TCelula *Predecessor(TCelula *x)
{
    if (x == NULL)
    {
        return NULL;
    }

    // Se x tem um filho a esquerda, o predecessor e o maximo da subarvore esquerda
    if (x->esq != NULL)
    {
        return Maximo(x->esq);
    }

    // Senao, e o ancestral mais baixo cujo filho direito tambem e ancestral de x
    TCelula *y = x->pai;
    while (y != NULL && x == y->esq)
    {
        x = y;
        y = y->pai;
    }

    return y;
}

// Funcao para mostrar os eventos de uma cidade
void MostraEventos(TCidade cidade)
{
    printf("Eventos em %s:\n", cidade.nome);
    for (int i = 0; i < cidade.numEventos; i++)
    {
        printf("  %d. %s - Avaliacao: %.1f\n",
               i + 1,
               cidade.eventos[i].nome,
               cidade.eventos[i].avaliacao);
    }
}

void MostrarTodosEventosDeTodasCidades(TCelula *raiz)
{
    if (raiz == NULL)
    {
        return;
    }

    // Percorre os eventos da cidade atual
    for (int i = 0; i < raiz->item.cidade.numEventos; i++)
    {
        printf("  %s - Avaliação: %.1f - Cidade: %s\n",
               raiz->item.cidade.eventos[i].nome,
               raiz->item.cidade.eventos[i].avaliacao,
               raiz->item.cidade.nome);
    }

    // Chama recursivamente para os filhos da árvore
    MostrarTodosEventosDeTodasCidades(raiz->esq);
    MostrarTodosEventosDeTodasCidades(raiz->dir);
}

void BuscarCidadePorEvento(char *nomeDoEvento, TCelula *raiz)
{
    if (raiz == NULL)
    {
        return;
    }

    // Verifica se algum evento da cidade atual corresponde ao nome buscado
    int encontrou = 0;
    for (int i = 0; i < raiz->item.cidade.numEventos; i++)
    {
        if (strcmp(nomeDoEvento, raiz->item.cidade.eventos[i].nome) == 0)
        {
            printf("Cidade: %s\n", raiz->item.cidade.nome);
            encontrou = 1;
            break; // Sai do loop depois de encontrar o primeiro evento correspondente
        }
    }

    // Busca recursivamente nas subárvores
    BuscarCidadePorEvento(nomeDoEvento, raiz->esq);
    BuscarCidadePorEvento(nomeDoEvento, raiz->dir);
}

// Funcao para liberar a memoria alocada para os eventos de uma cidade
void LiberaEventos(TCidade *cidade)
{
    if (cidade->eventos != NULL)
    {
        free(cidade->eventos);
        cidade->eventos = NULL;
    }
}

// Funcao para liberar a memoria alocada para um no e seus descendentes
void LiberaNo(TCelula *x)
{
    if (x != NULL)
    {
        LiberaNo(x->esq);
        LiberaNo(x->dir);
        LiberaEventos(&(x->item.cidade));
        free(x);
    }
}

// Funcao para liberar toda a memoria alocada para a arvore
void LiberaArvore(TArvore *arvore)
{
    LiberaNo(arvore->raiz);
    arvore->raiz = NULL;
}

int main()
{
    // Inicializa o gerador de numeros aleatorios
    srand(time(NULL));

    // Cria e inicializa a arvore
    TArvore arvore;
    InicializaArvore(&arvore);

    // Gera e insere 5 cidades com o algoritmo original mas eventos personalizados
    printf("Gerando e inserindo cidades com o algoritmo original e eventos personalizados...\n");
    GeraCidadesEventosPersonalizados(&arvore, 5);

    // Gera e insere 5 cidades totalmente aleatorias na arvore
    int numCidadesAleatorias = 5;
    printf("Gerando e inserindo %d cidades aleatorias na arvore...\n", numCidadesAleatorias);
    GeraInsereCidades(&arvore, numCidadesAleatorias);

    int opcao;
    do
    {
        printf("\nMenu:\n");
        printf("1 - Mostrar eventos\n");
        printf("2 - Buscar cidade por evento\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            MostrarTodosEventosDeTodasCidades(arvore.raiz);
            break;
        case 2:
            char nomeDoEvento[100]; 
            printf("Digite o nome do evento que procura: ");
            // Limpar o buffer de entrada
            getchar();
            // Usar fgets para ler a linha completa, incluindo espaços
            fgets(nomeDoEvento, 100, stdin);
            // Remover o caractere de nova linha no final, se existir
            size_t len = strlen(nomeDoEvento);
            if (len > 0 && nomeDoEvento[len-1] == '\n') {
                nomeDoEvento[len-1] = '\0';
            }
            BuscarCidadePorEvento(nomeDoEvento, arvore.raiz);
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;

    LiberaArvore(&arvore);

    return 0;
}