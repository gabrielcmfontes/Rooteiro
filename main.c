#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

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
    // float notaMediaDosEventos;
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
        "Festival Cultural", "Tour Noturno", "Passeio Ecologico",
        "Tour pelos Pontos Turisticos", "Visita ao Museu", "Corrida de bicicleta na lagoa",
        "Passeio pela Praia", "Espetaculo de Teatro", "Jogo de futebol do time da cidade",
        "Exposicao de Artesanato", "Festival Gastronomico", "Gastronomia na praça"
                                                            "Tour Historico",
        "Passeio de Barco", "Visita a Mercados", "Museu da prefeitura"
                                                 "Festival Cultural",
        "Tour Noturno", "Passeio Ecologico", "Exposição de carros antigos"};

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
        int eventosDisponiveis[20];
        for (int j = 0; j < 20; j++)
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

// Percurso in-ordem (caminhamento central) -> Só Printa a arvore por caminhamento central
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

// Percurso pre-ordem -> Só Printa a arvore por caminhamento pre ordem
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

// Percurso pos-ordem -> Só Printa a arvore por caminhamento pos ordem
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

// Funcao para encontrar o no com a chave minima na arvore // BAIANOOOOOOO ai em vez de ser a chave vai ser a nota media de eventos
// Deve mostrar a cidade com menor nota media
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
// Deve mostrar a cidade com maior nota media
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
// nao entendi funcionalidade direito
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
// nao entendi funcionalidade direito
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

// Busca algum evento numa cidade por nome do evento
void BuscarEventoNaCidadePorNome(TCidade *cidade, char *nome)
{
    if (cidade == NULL)
    {
        printf("Cidade não encontrada.\n");
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < cidade->numEventos; i++)
    {
        // Usar _stricmp para comparação case-insensitive
        if (_stricmp(cidade->eventos[i].nome, nome) == 0)
        {
            printf("Cidade: %s\nNome do evento: %s\nAvaliação: %.1f\n",
                   cidade->nome, cidade->eventos[i].nome, cidade->eventos[i].avaliacao);
            encontrado = 1;
        }
    }

    if (!encontrado)
    {
        printf("Evento '%s' não encontrado na cidade %s.\n", nome, cidade->nome);
    }
}

// Busca algum evento na cidade por avaliação do evento
void BuscarEventoNaCidadePorAvaliacao(TCidade *cidade, float avaliacao)
{
    if (cidade == NULL)
    {
        printf("Cidade não encontrada.\n");
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < cidade->numEventos; i++)
    {
        if (cidade->eventos[i].avaliacao == avaliacao)
        {
            printf("Cidade: %s\nNome do evento: %s\nAvaliação: %.1f\n",
                   cidade->nome, cidade->eventos[i].nome, cidade->eventos[i].avaliacao);
            encontrado = 1;
        }
    }

    if (!encontrado)
    {
        // Inicializar com o primeiro evento
        float menorDiferenca = fabs(avaliacao - cidade->eventos[0].avaliacao);
        int indiceProximo = 0;

        // Procurar pelo evento com menor diferença de avaliação
        for (int i = 1; i < cidade->numEventos; i++)
        {
            float diferenca = fabs(avaliacao - cidade->eventos[i].avaliacao);
            if (diferenca < menorDiferenca)
            {
                menorDiferenca = diferenca;
                indiceProximo = i;
            }
        }

        printf("Não encontramos evento com avaliação %.1f.\n", avaliacao);
        printf("O evento com avaliação mais próxima é:\n");
        printf("Cidade: %s\nNome do evento: %s\nAvaliação: %.1f\n",
               cidade->nome, cidade->eventos[indiceProximo].nome,
               cidade->eventos[indiceProximo].avaliacao);
    }
}

// Mostra todos os eventos de todas as cidades da arvore
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

// Busca cidade na arvore
// Criei e usei para criara roteiro
// Tentei mudar pela funcao *Busca depois que vi que existia, mas deu erro, mantive essa
TCidade *buscarCidade(TCelula *raiz, char *nome)
{
    if (raiz == NULL)
    {
        return NULL;
    }

    // Usar _stricmp para comparação case-insensitive
    if (_stricmp(raiz->item.cidade.nome, nome) == 0)
    {
        return &(raiz->item.cidade);
    }

    TCidade *resultado = buscarCidade(raiz->esq, nome);
    if (resultado != NULL)
    {
        return resultado;
    }

    return buscarCidade(raiz->dir, nome);
}

// Mostra o melhor evento de uma cidade, usando para criar roteiro
char *mostraMelhorEvento(TCidade *cidade)
{
    if (cidade == NULL || cidade->numEventos == 0)
    {
        return NULL;
    }

    float maior = cidade->eventos[0].avaliacao;
    int loc = 0;

    for (int i = 1; i < cidade->numEventos; i++)
    {
        if (cidade->eventos[i].avaliacao > maior)
        {
            maior = cidade->eventos[i].avaliacao;
            loc = i;
        }
    }

    return cidade->eventos[loc].nome;
}

// Cria o roteiro
void criarRoteiro(TCelula *raiz)
{
    char nomeCidade[3][50];

    printf("\nVamos criar um roteiro com 3 cidades!\n");

    getchar();

    for (int i = 0; i < 3; i++)
    {
        printf("Digite o nome da cidade %d: ", i + 1);
        fgets(nomeCidade[i], 50, stdin);

        size_t len = strlen(nomeCidade[i]);
        if (len > 0 && nomeCidade[i][len - 1] == '\n')
        {
            nomeCidade[i][len - 1] = '\0';
        }
    }

    printf("\n================================================================= SEU ROTEIRO DE VIAGEM ================================================================\n");
    printf("\n");

    for (int i = 0; i < 3; i++)
    {
        TCidade *cidade = buscarCidade(raiz, nomeCidade[i]);

        if (cidade == NULL)
        {
            printf("%d. %s: Cidade não encontrada no sistema\n", i + 1, nomeCidade[i]);
            continue;
        }

        char *melhorEvento = mostraMelhorEvento(cidade);

        if (melhorEvento == NULL)
        {
            printf("%d. %s: Não há eventos disponíveis\n", i + 1, cidade->nome);
        }
        else
        {
            printf("%d. %s: %s (Avaliação: %.1f)",
                   i + 1,
                   cidade->nome,
                   melhorEvento,
                   cidade->eventos[0].avaliacao);
        }

        if (i < 2)
        {
            printf(" -> ");
        }
    }
    printf("\n");
    printf("\n========================================================================================================================================================\n");
}

// O usuario digita um evento e mostra qual cidade esta tendo ele
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
        // Usar _stricmp para comparação case-insensitive
        if (_stricmp(nomeDoEvento, raiz->item.cidade.eventos[i].nome) == 0)
        {
            printf("\nCidade: %s\n", raiz->item.cidade.nome);
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

// Gera mensagens aleatorias a cada ordem
void mensagemAleatoria()
{

    const char *mensagens[] = {
        "ATENÇÃO! EVENTOS ESPORTIVOS TERÃO PARTICIPAÇÕES DE ATLETAS PROFISSIONAIS",
        "DICA: PRIORIZE OS EVENTOS COM AVALIAÇÕES ACIMA DE 9.0!",
        "PROMOÇÃO ESPECIAL: EVENTOS MUSICAIS TERÃO COMIDA DE GRAÇA!",
        "LEMBRETE: CONFIRME SUA PRESENÇA COM ANTECEDÊNCIA.",
        "EVENTOS GASTRONÔMICOS ESTÃO NA METADE DO PREÇO!",
        "NOVIDADE! O FESTIVAL DE MÚSICA CONTARÁ COM ARTISTAS INTERNACIONAIS",
        "AVISO: TOUR NOTURNO AGORA INCLUI JANTAR COMPLETO!",
        "ÚLTIMA HORA: INGRESSOS PARA A EXPOSIÇÃO DE CARROS ANTIGOS QUASE ESGOTADOS",
        "PROMOÇÃO: LEVE 3 AMIGOS PARA O FESTIVAL GASTRONÔMICO E GANHE DESCONTO!",
        "DICA DE VIAGEM: O PASSEIO DE BARCO OFERECE AS MELHORES VISTAS DA CIDADE",
        "DESTAQUE: O ESPETÁCULO DE TEATRO RECEBEU CRÍTICAS EXCELENTES!",
        "ALERTA DE CLIMA: EVENTOS AO AR LIVRE PODEM SER REMARCADOS EM CASO DE CHUVA",
        "DICA CULTURAL: NÃO PERCA A EXPOSIÇÃO DE ARTESANATO COM ARTISTAS LOCAIS",
        "EMERGÊNCIA: JOGO DE FUTEBOL DO TIME DA CIDADE MUDOU DE HORÁRIO"};

    int totalMensagens = sizeof(mensagens) / sizeof(mensagens[0]);
    srand(time(NULL));
    int indice = rand() % totalMensagens;
    printf("\n\n=====================================================================================\n\n%s\n\n=====================================================================================\n", mensagens[indice]);
}

// Remove e adiciona eventos aleatoriamente
// 50% de funcionar 50% de nao fazer nada
void EventosAleatorios(TCelula *raiz) {
    // Rand para ver se roda ou nao
    int probabilidade = rand() % 2 + 1; 
    if (probabilidade == 1) {
        return; 
    }

    if (raiz == NULL) {
        return;
    }

    static int removido = 0;
    static char nomeEventoRemovido[50];

    // Vetor de números aleatórios para seleção
    int numAleatorios[] = {2, 5, 7, 9, 4, 6, 3, 8, 1, 0};
    int indice = rand() % 10;
    int escolhido = numAleatorios[indice];

    // Opções de eventos temporários
    char *eventosTemporarios[] = {
        "Teatro Cultural na Praça", 
        "Jogo de Futebol Beneficente", 
        "Aniversário da Cidade", 
        "Amostra Grátis de Torta", 
        "Festival de Rua de Música", 
        "Corrida Beneficente", 
        "Feira de Artesanato Local", 
        "Show de Talentos Amadores", 
        "Inauguração de Espaço Cultural", 
        "Dia de Conscientização Ambiental"
    };

    int escolha[] = {10, 20};
    int indice2 = rand() % 2;
    int opcao = escolha[indice2];

    // Remover evento existente
    if (opcao == 10) {
        if (raiz->item.chave == escolhido) {
            // Verificar se há eventos para remover
            if (raiz->item.cidade.numEventos > 0) {
                // Salvar o nome do evento removido
                strcpy(nomeEventoRemovido, raiz->item.cidade.eventos[escolhido % raiz->item.cidade.numEventos].nome);

                printf("O evento '%s' foi cancelado!\n", nomeEventoRemovido);
                removido = 1;
            }
        }

        // Se já foi removido, adicionar de volta
        if (removido == 1) {
            printf("Evento %s adicionado de volta!\n", nomeEventoRemovido);
            removido = 0;
        }
    } 
    // Criar evento temporário
    else if (opcao == 20) {
        int indiceEventoTemp = rand() % 10;
        char *eventoEscolhido = eventosTemporarios[indiceEventoTemp];
        
        printf("EVENTO ESPECIAL TEMPORÁRIO na cidade %s: %s - Evento dura só hoje!!\n", 
               raiz->item.cidade.nome, eventoEscolhido);
    }

    // Continuar a traversal na árvore
    EventosAleatorios(raiz->esq);
    EventosAleatorios(raiz->dir);
}

// Funcao para liberar toda a memoria alocada para a arvore
void LiberaArvore(TArvore *arvore)
{
    LiberaNo(arvore->raiz);
    arvore->raiz = NULL;
}

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
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
        printf("2 - Caminhamente central\n");
        printf("3 - Buscar cidade por evento\n");
        printf("4 - Buscar evento na cidade\n");
        printf("5 - Criar roteiro\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            MostrarTodosEventosDeTodasCidades(arvore.raiz);
            mensagemAleatoria();
            printf("\n");
            EventosAleatorios(arvore.raiz);
            break;
        case 2:
            CaminhamentoCentral(arvore.raiz);
            mensagemAleatoria();
            printf("\n");
            EventosAleatorios(arvore.raiz);
            break;
        case 3:
            char nomeDoEvento[100];
            printf("Digite o nome do evento que procura: ");
            // Limpar o buffer de entrada
            getchar();
            // Usar fgets para ler a linha completa, incluindo espaços
            fgets(nomeDoEvento, 100, stdin);
            // Remover o caractere de nova linha no final, se existir
            size_t len = strlen(nomeDoEvento);
            if (len > 0 && nomeDoEvento[len - 1] == '\n')
            {
                nomeDoEvento[len - 1] = '\0';
            }
            BuscarCidadePorEvento(nomeDoEvento, arvore.raiz);
            mensagemAleatoria();
            printf("\n");
            EventosAleatorios(arvore.raiz);
            break;
        case 4:
        {
            char resposta[100];
            char nomeCidade[100];

            printf("Em qual cidade deseja procurar? ");
            // Limpar o buffer de entrada
            getchar();
            // Usar fgets para ler a linha completa, incluindo espaços
            fgets(nomeCidade, 100, stdin);
            // Remover o caractere de nova linha no final
            size_t len = strlen(nomeCidade);
            if (len > 0 && nomeCidade[len - 1] == '\n')
            {
                nomeCidade[len - 1] = '\0';
            }

            printf("Deseja procurar o evento por nome ou avaliação? ");
            fgets(resposta, 100, stdin);
            // Remover o caractere de nova linha
            len = strlen(resposta);
            if (len > 0 && resposta[len - 1] == '\n')
            {
                resposta[len - 1] = '\0';
            }

            TCidade *cidadeEncontrada = buscarCidade(arvore.raiz, nomeCidade);

            if (cidadeEncontrada == NULL)
            {
                printf("Cidade '%s' não encontrada.\n", nomeCidade);
                break;
            }

            if (_stricmp(resposta, "Nome") == 0)
            {
                char nomeEvento[100];
                printf("Qual o nome do evento? ");
                fgets(nomeEvento, 100, stdin);
                // Remover o caractere de nova linha
                len = strlen(nomeEvento);
                if (len > 0 && nomeEvento[len - 1] == '\n')
                {
                    nomeEvento[len - 1] = '\0';
                }

                BuscarEventoNaCidadePorNome(cidadeEncontrada, nomeEvento);
            }
            else if (strcasecmp(resposta, "Avaliação") == 0 ||
                     strcasecmp(resposta, "Avaliacao") == 0)
            {
                float nota;
                printf("Qual é avaliação desejada? Digite separando o decimal com .\n");
                scanf("%f", &nota);
                getchar();
                BuscarEventoNaCidadePorAvaliacao(cidadeEncontrada, nota);
            }
            else
            {
                printf("Opção inválida.\n");
            }
        }
            mensagemAleatoria();
            printf("\n");
            EventosAleatorios(arvore.raiz);
            break;
        case 5:
            criarRoteiro(arvore.raiz);
            mensagemAleatoria();
            printf("\n");
            EventosAleatorios(arvore.raiz);
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