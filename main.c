#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//-------------------------------------------TAD EVENTO-----------------------------------------------------
typedef struct evento {
    char nome[50];
    float avaliacao;
} TEvento;

//-------------------------------------------TAD CIDADE-----------------------------------------------------
typedef struct cidade {
    char nome[50];
    TEvento *eventos;
    int numEventos;
    float notaMedia;
} TCidade;

//-------------------------------------------TAD ITEM-----------------------------------------------------
typedef struct item {
    int chave;
    TCidade cidade;
} TItem;

//-------------------------------------------TAD CELULA-----------------------------------------------------
typedef struct celula {
    TItem item;
    struct celula *pai;
    struct celula *esq;
    struct celula *dir;
} TCelula;

//-------------------------------------------TAD ARVORE-----------------------------------------------------
typedef struct arvore {
    TCelula *raiz;
} TArvore;

//-------------------------------------------OPERACOES DE MEDIA-----------------------------------------------------
void CalculaMediaCidade(TCidade *cidade) {
    if (cidade->numEventos == 0) {
        cidade->notaMedia = 0.0;
        return;
    }

    float soma = 0.0;
    for (int i = 0; i < cidade->numEventos; i++) {
        soma += cidade->eventos[i].avaliacao;
    }
    cidade->notaMedia = soma / cidade->numEventos;
}

void CalculaMediaTodasCidades(TCelula *x) {
    if (x == NULL) {
        return;
    }

    CalculaMediaCidade(&(x->item.cidade));

    if (x->esq != NULL) {
        CalculaMediaTodasCidades(x->esq);
    }

    if (x->dir != NULL) {
        CalculaMediaTodasCidades(x->dir);
    }
}

//-------------------------------------------COLETA DE DADOS-----------------------------------------------------
int ColetaCidades(TCelula *raiz, TCidade *cidades, int index) {
    if (raiz == NULL) {
        return index;
    }

    index = ColetaCidades(raiz->esq, cidades, index);
    cidades[index++] = raiz->item.cidade;
    index = ColetaCidades(raiz->dir, cidades, index);

    return index;
}

//-------------------------------------------INICIALIZACAO-----------------------------------------------------
void InicializaArvore(TArvore *arvore) {
    arvore->raiz = NULL;
}

TCelula *CriaCelula(TItem item) {
    TCelula *celula = (TCelula *)malloc(sizeof(TCelula));
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

//-------------------------------------------GERACAO DE DADOS-----------------------------------------------------
void GeraNomeCidade(char *nome) {
    static char *prefixos[] = {"Nova", "São", "Santa", "Santo", "Porto", "Vila", "Belo", "Bela"};
    static char *sufixosMasc[] = {"Horizonte", "Alegre", "Campo", "Jardim", "Monte", "Recanto", "Vale"};
    static char *sufixosFem[] = {"Cruz", "Terra", "Vista", "Praia", "Serra", "Lagoa", "Floresta"};
    static char *meio[] = {"do", "da", "dos", "das", "de"};

    int tipo = rand() % 3;

    if (tipo == 0) {
        int prefixoIdx = rand() % 8;
        char *sufixo;

        if (prefixoIdx == 2 || prefixoIdx == 7)
            sufixo = sufixosFem[rand() % 7];
        else if (prefixoIdx == 3 || prefixoIdx == 6)
            sufixo = sufixosMasc[rand() % 7];
        else {
            if (rand() % 2)
                sufixo = sufixosFem[rand() % 7];
            else
                sufixo = sufixosMasc[rand() % 7];
        }
        sprintf(nome, "%s %s", prefixos[prefixoIdx], sufixo);
    }
    else if (tipo == 1) {
        static char *nomes[] = {"Campinas", "Ouro Preto", "Diamantina", "Petrópolis", "Gramado", "Curitiba", "Florianópolis", "Búzios", "Paraty", "Olinda"};
        int nomeIdx = rand() % 10;
        strcpy(nome, nomes[nomeIdx]);
    }
    else {
        static char *primeiros[] = {"Monte", "Serra", "Campo", "Morro", "Rio", "Lagoa", "Recanto"};
        int primeiroIdx = rand() % 7;
        int meioIdx = rand() % 5;
        char *sufixo;

        if (primeiroIdx == 5)
            sufixo = sufixosFem[rand() % 7];
        else
            sufixo = sufixosMasc[rand() % 7];

        sprintf(nome, "%s %s %s", primeiros[primeiroIdx], meio[meioIdx], sufixo);
    }
}

void GeraNomeEvento(char *nome) {
    static char *tipos[] = {"Festival de", "Feira de", "Mostra de", "Tour por", "Visita ao", "Passeio de", "Espetaculo de", "Exposicao de"};
    static char *temas[] = {"Arte", "Musica", "Gastronomia", "Historia", "Cultura", "Natureza", "Artesanato", "Cinema", "Teatro", "Danca"};

    int tipoIdx = rand() % 8;
    int temaIdx = rand() % 10;

    sprintf(nome, "%s %s", tipos[tipoIdx], temas[temaIdx]);
}

void GeraEventos(TCidade *cidade, int numEventos) {
    cidade->numEventos = numEventos;
    cidade->eventos = (TEvento *)malloc(numEventos * sizeof(TEvento));

    if (cidade->eventos == NULL) {
        printf("Erro: Falha na alocacao de memoria para eventos\n");
        exit(1);
    }

    for (int i = 0; i < numEventos; i++) {
        GeraNomeEvento(cidade->eventos[i].nome);
        cidade->eventos[i].avaliacao = (rand() % 100) / 10.0;
    }
}

TCidade GeraCidadeAleatoria() {
    TCidade cidade;
    GeraNomeCidade(cidade.nome);

    int numEventos = 5 + (rand() % 6);
    GeraEventos(&cidade, numEventos);

    return cidade;
}

//-------------------------------------------OPERACOES DE ARVORE-----------------------------------------------------
void InsereNo(TArvore *arvore, TItem item) {
    TCelula *z = CriaCelula(item);
    TCelula *y = NULL;
    TCelula *x = arvore->raiz;

    while (x != NULL) {
        y = x;
        if (z->item.chave < x->item.chave) {
            x = x->esq;
        }
        else {
            x = x->dir;
        }
    }

    z->pai = y;

    if (y == NULL) {
        arvore->raiz = z;
    }
    else if (z->item.chave < y->item.chave) {
        y->esq = z;
    }
    else {
        y->dir = z;
    }
}

void GeraInsereCidades(TArvore *arvore, int numCidades) {
    for (int i = 0; i < numCidades; i++) {
        TItem item;
        item.chave = rand() % 1000;
        item.cidade = GeraCidadeAleatoria();
        InsereNo(arvore, item);
    }
}

void GeraCidadesEventosPersonalizados(TArvore *arvore, int numCidades) {
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

    for (int i = 0; i < numCidades; i++) {
        TItem item;
        item.chave = 100 + i * 100;

        GeraNomeCidade(item.cidade.nome);

        int numEventos = 5 + (rand() % 6);
        item.cidade.numEventos = numEventos;
        item.cidade.eventos = (TEvento *)malloc(numEventos * sizeof(TEvento));

        if (item.cidade.eventos == NULL) {
            printf("Erro: Falha na alocacao de memoria para eventos\n");
            exit(1);
        }

        int eventosDisponiveis[20];
        for (int j = 0; j < 20; j++) {
            eventosDisponiveis[j] = j;
        }

        for (int j = 14; j > 0; j--) {
            int k = rand() % (j + 1);
            int temp = eventosDisponiveis[j];
            eventosDisponiveis[j] = eventosDisponiveis[k];
            eventosDisponiveis[k] = temp;
        }

        for (int j = 0; j < numEventos; j++) {
            int idx = eventosDisponiveis[j];
            strcpy(item.cidade.eventos[j].nome, eventosNomes[idx]);
            item.cidade.eventos[j].avaliacao = avaliacoes[idx];
        }

        InsereNo(arvore, item);
    }

    printf("Geradas %d cidades usando o algoritmo original com eventos personalizados.\n", numCidades);
}

//-------------------------------------------CAMINHAMENTOS-----------------------------------------------------
void CaminhamentoCentral(TCelula *x) {
    if (x != NULL) {
        CaminhamentoCentral(x->esq);
        printf("Chave: %d, Cidade: %s, Eventos: %d, Nota media do Evento: %.2f\n",
               x->item.chave,
               x->item.cidade.nome,
               x->item.cidade.numEventos,
               x->item.cidade.notaMedia);
        CaminhamentoCentral(x->dir);
    }
}

void CaminhamentoPreOrdem(TCelula *x) {
    if (x != NULL) {
        printf("Chave: %d, Cidade: %s, Eventos: %d, Nota media do Evento: %.2f\n",
               x->item.chave,
               x->item.cidade.nome,
               x->item.cidade.numEventos,
               x->item.cidade.notaMedia);
        CaminhamentoPreOrdem(x->esq);
        CaminhamentoPreOrdem(x->dir);
    }
}

void CaminhamentoPosOrdem(TCelula *x) {
    if (x != NULL) {
        CaminhamentoPosOrdem(x->esq);
        CaminhamentoPosOrdem(x->dir);
        printf("Chave: %d, Cidade: %s, Eventos: %d, Nota media do Evento: %.2f\n",
               x->item.chave,
               x->item.cidade.nome,
               x->item.cidade.numEventos,
               x->item.cidade.notaMedia);
    }
}

//-------------------------------------------BUSCAS-----------------------------------------------------
TCelula *Busca(TCelula *x, char *nome) {
    if (x == NULL || strcmp(x->item.cidade.nome, nome) == 0) {
        return x;
    }

    if (strcmp(nome, x->item.cidade.nome) < 0) {
        return Busca(x->esq, nome);
    }
    else {
        return Busca(x->dir, nome);
    }
}

TCelula *Minimo(TCelula *x) {
    if (x == NULL) {
        return NULL;
    }

    TCelula *menorCidade = x;
    TCelula *atual = x;

    while (atual != NULL) {
        if (atual->item.cidade.notaMedia < menorCidade->item.cidade.notaMedia) {
            menorCidade = atual;
        }

        if (atual->esq != NULL) {
            TCelula *esquerdoMin = Minimo(atual->esq);
            if (esquerdoMin != NULL && esquerdoMin->item.cidade.notaMedia < menorCidade->item.cidade.notaMedia) {
                menorCidade = esquerdoMin;
            }
        }

        if (atual->dir != NULL) {
            TCelula *direitoMin = Minimo(atual->dir);
            if (direitoMin != NULL && direitoMin->item.cidade.notaMedia < menorCidade->item.cidade.notaMedia) {
                menorCidade = direitoMin;
            }
        }

        atual = atual->dir;
    }

    return menorCidade;
}

TCelula *Maximo(TCelula *x) {
    if (x == NULL) {
        return NULL;
    }

    TCelula *maiorCidade = x;
    TCelula *atual = x;

    while (atual != NULL) {
        if (atual->item.cidade.notaMedia > maiorCidade->item.cidade.notaMedia) {
            maiorCidade = atual;
        }

        if (atual->esq != NULL) {
            TCelula *esquerdoMax = Maximo(atual->esq);
            if (esquerdoMax != NULL && esquerdoMax->item.cidade.notaMedia > maiorCidade->item.cidade.notaMedia) {
                maiorCidade = esquerdoMax;
            }
        }

        if (atual->dir != NULL) {
            TCelula *direitoMax = Maximo(atual->dir);
            if (direitoMax != NULL && direitoMax->item.cidade.notaMedia > maiorCidade->item.cidade.notaMedia) {
                maiorCidade = direitoMax;
            }
        }

        atual = atual->dir;
    }

    return maiorCidade;
}

TCelula *Sucessor(TCelula *x) {
    if (x == NULL) {
        return NULL;
    }

    if (x->dir != NULL) {
        return Minimo(x->dir);
    }

    TCelula *y = x->pai;
    while (y != NULL && x == y->dir) {
        x = y;
        y = y->pai;
    }

    return y;
}

TCelula *Predecessor(TCelula *x) {
    if (x == NULL) {
        return NULL;
    }

    if (x->esq != NULL) {
        return Maximo(x->esq);
    }

    TCelula *y = x->pai;
    while (y != NULL && x == y->esq) {
        x = y;
        y = y->pai;
    }

    return y;
}

//-------------------------------------------OPERACOES DE EVENTOS-----------------------------------------------------
void MostraEventos(TCidade cidade) {
    printf("Eventos em %s:\n", cidade.nome);
    for (int i = 0; i < cidade.numEventos; i++) {
        printf("  %d. %s - Avaliacao: %.1f\n",
               i + 1,
               cidade.eventos[i].nome,
               cidade.eventos[i].avaliacao);
    }
}

void BuscarEventoNaCidadePorNome(TCidade *cidade, char *nome) {
    if (cidade == NULL) {
        printf("Cidade não encontrada.\n");
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < cidade->numEventos; i++) {
        if (_stricmp(cidade->eventos[i].nome, nome) == 0) {
            printf("Cidade: %s\nNome do evento: %s\nAvaliação: %.1f\n",
                   cidade->nome, cidade->eventos[i].nome, cidade->eventos[i].avaliacao);
            encontrado = 1;
        }
    }

    if (!encontrado) {
        printf("Evento '%s' não encontrado na cidade %s.\n", nome, cidade->nome);
    }
}

void BuscarEventoNaCidadePorAvaliacao(TCidade *cidade, float avaliacao) {
    if (cidade == NULL) {
        printf("Cidade não encontrada.\n");
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < cidade->numEventos; i++) {
        if (cidade->eventos[i].avaliacao == avaliacao) {
            printf("Cidade: %s\nNome do evento: %s\nAvaliação: %.1f\n",
                   cidade->nome, cidade->eventos[i].nome, cidade->eventos[i].avaliacao);
            encontrado = 1;
        }
    }

    if (!encontrado) {
        float menorDiferenca = fabs(avaliacao - cidade->eventos[0].avaliacao);
        int indiceProximo = 0;

        for (int i = 1; i < cidade->numEventos; i++) {
            float diferenca = fabs(avaliacao - cidade->eventos[i].avaliacao);
            if (diferenca < menorDiferenca) {
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

void MostrarTodosEventosDeTodasCidades(TCelula *raiz) {
    if (raiz == NULL) {
        return;
    }

    for (int i = 0; i < raiz->item.cidade.numEventos; i++) {
        printf("  %s - Avaliação: %.1f - Cidade: %s\n",
               raiz->item.cidade.eventos[i].nome,
               raiz->item.cidade.eventos[i].avaliacao,
               raiz->item.cidade.nome);
    }

    MostrarTodosEventosDeTodasCidades(raiz->esq);
    MostrarTodosEventosDeTodasCidades(raiz->dir);
}

TCidade *buscarCidade(TCelula *raiz, char *nome) {
    if (raiz == NULL) {
        return NULL;
    }

    if (_stricmp(raiz->item.cidade.nome, nome) == 0) {
        return &(raiz->item.cidade);
    }

    TCidade *resultado = buscarCidade(raiz->esq, nome);
    if (resultado != NULL) {
        return resultado;
    }

    return buscarCidade(raiz->dir, nome);
}

char *mostraMelhorEvento(TCidade *cidade) {
    if (cidade == NULL || cidade->numEventos == 0) {
        return NULL;
    }

    float maior = cidade->eventos[0].avaliacao;
    int loc = 0;

    for (int i = 1; i < cidade->numEventos; i++) {
        if (cidade->eventos[i].avaliacao > maior) {
            maior = cidade->eventos[i].avaliacao;
            loc = i;
        }
    }

    return cidade->eventos[loc].nome;
}

//-------------------------------------------ROTEIRO-----------------------------------------------------
void criarRoteiro(TCelula *raiz) {
    char nomeCidade[3][50];

    printf("\nVamos criar um roteiro com 3 cidades!\n");

    getchar();

    for (int i = 0; i < 3; i++) {
        printf("Digite o nome da cidade %d: ", i + 1);
        fgets(nomeCidade[i], 50, stdin);

        size_t len = strlen(nomeCidade[i]);
        if (len > 0 && nomeCidade[i][len - 1] == '\n') {
            nomeCidade[i][len - 1] = '\0';
        }
    }

    printf("\n================================================================= SEU ROTEIRO DE VIAGEM ================================================================\n");
    printf("\n");

    for (int i = 0; i < 3; i++) {
        TCidade *cidade = buscarCidade(raiz, nomeCidade[i]);

        if (cidade == NULL) {
            printf("%d. %s: Cidade não encontrada no sistema\n", i + 1, nomeCidade[i]);
            continue;
        }

        char *melhorEvento = mostraMelhorEvento(cidade);

        if (melhorEvento == NULL) {
            printf("%d. %s: Não há eventos disponíveis\n", i + 1, cidade->nome);
        }
        else {
            printf("%d. %s: %s (Avaliação: %.1f)",
                   i + 1,
                   cidade->nome,
                   melhorEvento,
                   cidade->eventos[0].avaliacao);
        }

        if (i < 2) {
            printf(" -> ");
        }
    }
    printf("\n");
    printf("\n========================================================================================================================================================\n");
}

void BuscarCidadePorEvento(char *nomeDoEvento, TCelula *raiz) {
    if (raiz == NULL) {
        return;
    }

    int encontrou = 0;

    for (int i = 0; i < raiz->item.cidade.numEventos; i++) {
        if (_stricmp(nomeDoEvento, raiz->item.cidade.eventos[i].nome) == 0) {
            printf("\nCidade: %s\n", raiz->item.cidade.nome);
            encontrou = 1;
            break;
        }
    }

    BuscarCidadePorEvento(nomeDoEvento, raiz->esq);
    BuscarCidadePorEvento(nomeDoEvento, raiz->dir);
}

//-------------------------------------------LIBERACAO DE MEMORIA-----------------------------------------------------
void LiberaEventos(TCidade *cidade) {
    if (cidade->eventos != NULL) {
        free(cidade->eventos);
        cidade->eventos = NULL;
    }
}

void LiberaNo(TCelula *x) {
    if (x != NULL) {
        LiberaNo(x->esq);
        LiberaNo(x->dir);
        LiberaEventos(&(x->item.cidade));
        free(x);
    }
}

void LiberaArvore(TArvore *arvore) {
    LiberaNo(arvore->raiz);
    arvore->raiz = NULL;
}

//-------------------------------------------FUNCOES AUXILIARES-----------------------------------------------------
void mensagemAleatoria() {
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

void EventosAleatorios(TCelula *raiz) {
    int probabilidade = rand() % 2 + 1;
    if (probabilidade == 1) {
        return;
    }

    if (raiz == NULL) {
        return;
    }

    static int removido = 0;
    static char nomeEventoRemovido[50];

    int numAleatorios[] = {2, 5, 7, 9, 4, 6, 3, 8, 1, 0};
    int indice = rand() % 10;
    int escolhido = numAleatorios[indice];

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

    if (opcao == 10) {
        if (raiz->item.chave == escolhido) {
            if (raiz->item.cidade.numEventos > 0) {
                strcpy(nomeEventoRemovido, raiz->item.cidade.eventos[escolhido % raiz->item.cidade.numEventos].nome);
                printf("O evento '%s' foi cancelado!\n", nomeEventoRemovido);
                removido = 1;
            }
        }

        if (removido == 1) {
            printf("Evento %s adicionado de volta!\n", nomeEventoRemovido);
            removido = 0;
        }
    }
    else if (opcao == 20) {
        int indiceEventoTemp = rand() % 10;
        char *eventoEscolhido = eventosTemporarios[indiceEventoTemp];

        printf("EVENTO ESPECIAL TEMPORÁRIO na cidade %s: %s - Evento dura só hoje!!\n",
               raiz->item.cidade.nome, eventoEscolhido);
    }

    EventosAleatorios(raiz->esq);
    EventosAleatorios(raiz->dir);
}

//-------------------------------------------ALGORITMOS DE ORDENAÇÃO-----------------------------------------------------
void Merge(void *x, int l, int m, int r, int tipo, int ordem) {
    if (tipo == 0) {
        TCidade *base = (TCidade*)x;
        int n1 = m - l + 1;
        int n2 = r - m;

        TCidade *L = malloc(n1 * sizeof(TCidade));
        TCidade *R = malloc(n2 * sizeof(TCidade));

        for (int i = 0; i < n1; i++)
            L[i] = base[l + i];
        for (int j = 0; j < n2; j++)
            R[j] = base[m + 1 + j];

        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            int compare = (ordem == 0) ?
                (L[i].notaMedia <= R[j].notaMedia) :
                (L[i].notaMedia >= R[j].notaMedia);

            if (compare) {
                base[k] = L[i];
                i++;
            } else {
                base[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            base[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            base[k] = R[j];
            j++;
            k++;
        }

        free(L);
        free(R);
    }
    else if (tipo == 1) {
        TEvento *base = (TEvento*)x;
        int n1 = m - l + 1;
        int n2 = r - m;

        TEvento *L = malloc(n1 * sizeof(TEvento));
        TEvento *R = malloc(n2 * sizeof(TEvento));

        for (int i = 0; i < n1; i++)
            L[i] = base[l + i];
        for (int j = 0; j < n2; j++)
            R[j] = base[m + 1 + j];

        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            int compare = (ordem == 0) ?
                (L[i].avaliacao <= R[j].avaliacao) :
                (L[i].avaliacao >= R[j].avaliacao);

            if (compare) {
                base[k] = L[i];
                i++;
            } else {
                base[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            base[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            base[k] = R[j];
            j++;
            k++;
        }

        free(L);
        free(R);
    }
}

void MergeSort(void *x, int l, int r, int tipo, int ordem) {
    if (l >= r) {
        return;
    }

    int m = l + (r - l) / 2;

    MergeSort(x, l, m, tipo, ordem);
    MergeSort(x, m + 1, r, tipo, ordem);
    Merge(x, l, m, r, tipo, ordem);
}

void MergeSortCidades(TCidade *cidades, int n, int ordem) {
    MergeSort(cidades, 0, n - 1, 0, ordem);
}

void MergeSortEventos(TEvento *eventos, int n, int ordem) {
    MergeSort(eventos, 0, n - 1, 1, ordem);
}

// Funções de troca genéricas
void trocar(void *a, void *b, size_t size) {
    char temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

// Selection Sort genérico
void SelectionSort(void *x, int n, int tipo, int ordem) {
    if (tipo == 0) {
        TCidade *cidades = (TCidade*)x;
        for (int i = 0; i < n - 1; i++) {
            int comp_idx = i;
            for (int j = i + 1; j < n; j++) {
                int compare = (ordem == 0) ?
                    (cidades[j].notaMedia < cidades[comp_idx].notaMedia) :
                    (cidades[j].notaMedia > cidades[comp_idx].notaMedia);

                if (compare) {
                    comp_idx = j;
                }
            }
            if (comp_idx != i) {
                trocar(&cidades[i], &cidades[comp_idx], sizeof(TCidade));
            }
        }
    }
    else if (tipo == 1) {
        TEvento *eventos = (TEvento*)x;
        for (int i = 0; i < n - 1; i++) {
            int comp_idx = i;
            for (int j = i + 1; j < n; j++) {
                int compare = (ordem == 0) ?
                    (eventos[j].avaliacao < eventos[comp_idx].avaliacao) :
                    (eventos[j].avaliacao > eventos[comp_idx].avaliacao);

                if (compare) {
                    comp_idx = j;
                }
            }
            if (comp_idx != i) {
                trocar(&eventos[i], &eventos[comp_idx], sizeof(TEvento));
            }
        }
    }
}

void InsertionSort(void *x, int n, int tipo, int ordem) {
    if (tipo == 0) {
        TCidade *cidades = (TCidade*)x;
        for (int i = 1; i < n; i++) {
            TCidade chave = cidades[i];
            int j = i - 1;

            while (j >= 0) {
                int compare = (ordem == 0) ?
                    (cidades[j].notaMedia > chave.notaMedia) :
                    (cidades[j].notaMedia < chave.notaMedia);

                if (compare) {
                    cidades[j + 1] = cidades[j];
                    j = j - 1;
                } else {
                    break;
                }
            }
            cidades[j + 1] = chave;
        }
    }
    else if (tipo == 1) {
        TEvento *eventos = (TEvento*)x;
        for (int i = 1; i < n; i++) {
            TEvento chave = eventos[i];
            int j = i - 1;

            while (j >= 0) {
                int compare = (ordem == 0) ?
                    (eventos[j].avaliacao > chave.avaliacao) :
                    (eventos[j].avaliacao < chave.avaliacao);

                if (compare) {
                    eventos[j + 1] = eventos[j];
                    j = j - 1;
                } else {
                    break;
                }
            }
            eventos[j + 1] = chave;
        }
    }
}

void ShellSort(void *x, int n, int tipo, int ordem) {
    if (tipo == 0) {
        TCidade *cidades = (TCidade*)x;
        for (int gap = n/2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; i++) {
                TCidade temp = cidades[i];
                int j;

                for (j = i; j >= gap; j -= gap) {
                    int compare = (ordem == 0) ?
                        (cidades[j - gap].notaMedia > temp.notaMedia) :
                        (cidades[j - gap].notaMedia < temp.notaMedia);

                    if (compare) {
                        cidades[j] = cidades[j - gap];
                    } else {
                        break;
                    }
                }

                cidades[j] = temp;
            }
        }
    }
    else if (tipo == 1) {
        TEvento *eventos = (TEvento*)x;
        for (int gap = n/2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; i++) {
                TEvento temp = eventos[i];
                int j;

                for (j = i; j >= gap; j -= gap) {
                    int compare = (ordem == 0) ?
                        (eventos[j - gap].avaliacao > temp.avaliacao) :
                        (eventos[j - gap].avaliacao < temp.avaliacao);

                    if (compare) {
                        eventos[j] = eventos[j - gap];
                    } else {
                        break;
                    }
                }

                eventos[j] = temp;
            }
        }
    }
}

void BubbleSort(void *x, int n, int tipo, int ordem) {
    if (tipo == 0) {
        TCidade *cidades = (TCidade*)x;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                int compare = (ordem == 0) ?
                    (cidades[j].notaMedia > cidades[j + 1].notaMedia) :
                    (cidades[j].notaMedia < cidades[j + 1].notaMedia);

                if (compare) {
                    trocar(&cidades[j], &cidades[j + 1], sizeof(TCidade));
                }
            }
        }
    }
    else if (tipo == 1) {
        TEvento *eventos = (TEvento*)x;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                int compare = (ordem == 0) ?
                    (eventos[j].avaliacao > eventos[j + 1].avaliacao) :
                    (eventos[j].avaliacao < eventos[j + 1].avaliacao);

                if (compare) {
                    trocar(&eventos[j], &eventos[j + 1], sizeof(TEvento));
                }
            }
        }
    }
}

void heapify(void *x, int n, int i, int tipo, int ordem) {
    if (tipo == 0) {
        TCidade *cidades = (TCidade*)x;
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int compareLeft = (ordem == 0) ?
            (left < n && cidades[left].notaMedia > cidades[largest].notaMedia) :
            (left < n && cidades[left].notaMedia < cidades[largest].notaMedia);

        int compareRight = (ordem == 0) ?
            (right < n && cidades[right].notaMedia > cidades[largest].notaMedia) :
            (right < n && cidades[right].notaMedia < cidades[largest].notaMedia);

        if (compareLeft)
            largest = left;

        if (compareRight)
            largest = right;

        if (largest != i) {
            trocar(&cidades[i], &cidades[largest], sizeof(TCidade));
            heapify(x, n, largest, tipo, ordem);
        }
    }
    else if (tipo == 1) {
        TEvento *eventos = (TEvento*)x;
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int compareLeft = (ordem == 0) ?
            (left < n && eventos[left].avaliacao > eventos[largest].avaliacao) :
            (left < n && eventos[left].avaliacao < eventos[largest].avaliacao);

        int compareRight = (ordem == 0) ?
            (right < n && eventos[right].avaliacao > eventos[largest].avaliacao) :
            (right < n && eventos[right].avaliacao < eventos[largest].avaliacao);

        if (compareLeft)
            largest = left;

        if (compareRight)
            largest = right;

        if (largest != i) {
            trocar(&eventos[i], &eventos[largest], sizeof(TEvento));
            heapify(x, n, largest, tipo, ordem);
        }
    }
}

void HeapSort(void *x, int n, int tipo, int ordem) {
    // Construir heap (reorganizar array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(x, n, i, tipo, ordem);

    // Extrair elementos do heap um por um
    for (int i = n - 1; i > 0; i--) {
        if (tipo == 0) { // Cidades
            trocar(&((TCidade*)x)[0], &((TCidade*)x)[i], sizeof(TCidade));
        } else { // Eventos
            trocar(&((TEvento*)x)[0], &((TEvento*)x)[i], sizeof(TEvento));
        }
        heapify(x, i, 0, tipo, ordem);
    }
}

int particionar(void *x, int low, int high, int tipo, int ordem) {
    if (tipo == 0) {
        TCidade *cidades = (TCidade*)x;
        float pivot = cidades[high].notaMedia;
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            int compare = (ordem == 0) ?
                (cidades[j].notaMedia <= pivot) :
                (cidades[j].notaMedia >= pivot);

            if (compare) {
                i++;
                trocar(&cidades[i], &cidades[j], sizeof(TCidade));
            }
        }
        trocar(&cidades[i + 1], &cidades[high], sizeof(TCidade));
        return (i + 1);
    }
    else {
        TEvento *eventos = (TEvento*)x;
        float pivot = eventos[high].avaliacao;
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            int compare = (ordem == 0) ?
                (eventos[j].avaliacao <= pivot) :
                (eventos[j].avaliacao >= pivot);

            if (compare) {
                i++;
                trocar(&eventos[i], &eventos[j], sizeof(TEvento));
            }
        }
        trocar(&eventos[i + 1], &eventos[high], sizeof(TEvento));
        return (i + 1);
    }
}

void QuickSortRecursivo(void *x, int low, int high, int tipo, int ordem) {
    if (low < high) {
        int pi = particionar(x, low, high, tipo, ordem);
        QuickSortRecursivo(x, low, pi - 1, tipo, ordem);
        QuickSortRecursivo(x, pi + 1, high, tipo, ordem);
    }
}

void QuickSort(void *x, int n, int tipo, int ordem) {
    QuickSortRecursivo(x, 0, n - 1, tipo, ordem);
}

// Funções de conveniência para cada tipo (0 = crescente, 1 = decrescente)
void SelectionSortCidades(TCidade *cidades, int n, int ordem) {
    SelectionSort(cidades, n, 0, ordem);
}

void InsertionSortCidades(TCidade *cidades, int n, int ordem) {
    InsertionSort(cidades, n, 0, ordem);
}

void ShellSortCidades(TCidade *cidades, int n, int ordem) {
    ShellSort(cidades, n, 0, ordem);
}

void BubbleSortCidades(TCidade *cidades, int n, int ordem) {
    BubbleSort(cidades, n, 0, ordem);
}

void HeapSortCidades(TCidade *cidades, int n, int ordem) {
    HeapSort(cidades, n, 0, ordem);
}

void QuickSortCidades(TCidade *cidades, int n, int ordem) {
    QuickSort(cidades, n, 0, ordem);
}

void SelectionSortEventos(TEvento *eventos, int n, int ordem) {
    SelectionSort(eventos, n, 1, ordem);
}

void InsertionSortEventos(TEvento *eventos, int n, int ordem) {
    InsertionSort(eventos, n, 1, ordem);
}

void ShellSortEventos(TEvento *eventos, int n, int ordem) {
    ShellSort(eventos, n, 1, ordem);
}

void BubbleSortEventos(TEvento *eventos, int n, int ordem) {
    BubbleSort(eventos, n, 1, ordem);
}

void HeapSortEventos(TEvento *eventos, int n, int ordem) {
    HeapSort(eventos, n, 1, ordem);
}

void QuickSortEventos(TEvento *eventos, int n, int ordem) {
    QuickSort(eventos, n, 1, ordem);
}

//-------------------------------------------FUNÇÕES AUXILIARES-----------------------------------------------------
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

    CalculaMediaTodasCidades(arvore.raiz);

    int opcao;
    do
    {
        printf("\nMenu:\n");
        printf("1 - Mostrar eventos\n");
        printf("2 - Caminhamento central\n");
        printf("3 - Caminhamento pré-ordem\n");
        printf("4 - Caminhamento pós-ordem\n");
        printf("5 - Buscar cidade por evento\n");
        printf("6 - Buscar evento na cidade\n");
        printf("7 - Buscar cidades com determinado evento\n");
        printf("8 - Cidade com a menor nota média\n");
        printf("9 - Cidade com a maior nota média\n");
        printf("10 - Criar roteiro\n");
        printf("11 - Ordenar cidades\n");
        printf("12 - Ordenar eventos\n");
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
                CaminhamentoPreOrdem(arvore.raiz);
                mensagemAleatoria();
                printf("\n");
                EventosAleatorios(arvore.raiz);
                break;
            case 4:
                CaminhamentoPosOrdem(arvore.raiz);
                mensagemAleatoria();
                printf("\n");
                EventosAleatorios(arvore.raiz);
                break;
            case 5:
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
            case 6: {
                char nomeCidade[100];
                int tipoSelecao;

                while (getchar() != '\n');

                printf("Em qual cidade deseja procurar?\n");
                fgets(nomeCidade, sizeof(nomeCidade), stdin);

                nomeCidade[strcspn(nomeCidade, "\n")] = '\0';

                TCidade *cidadeEncontrada = buscarCidade(arvore.raiz, nomeCidade);
                if (cidadeEncontrada == NULL) {
                    printf("Cidade '%s' não encontrada.\n", nomeCidade);
                    break;
                }

                printf("Deseja procurar o evento por:\n");
                printf("1 - Nome do Evento\n");
                printf("2 - Avaliação\n");
                printf("Escolha (1/2):\n");
                scanf("%d", &tipoSelecao);
                getchar(); // Clear input buffer

                switch (tipoSelecao) {
                    case 1: {
                        char nomeEvento[100];
                        printf("Qual o nome do evento?\n");
                        fgets(nomeEvento, sizeof(nomeEvento), stdin);

                        nomeEvento[strcspn(nomeEvento, "\n")] = '\0';

                        BuscarEventoNaCidadePorNome(cidadeEncontrada, nomeEvento);
                        break;
                    }
                    case 2: {
                        float nota;
                        printf("Qual é a avaliação desejada? (Digite separando o decimal com ,):\n");
                        scanf("%f", &nota);
                        getchar(); // Clear input buffer

                        BuscarEventoNaCidadePorAvaliacao(cidadeEncontrada, nota);
                        break;
                    }
                    default: {
                        printf("Opção inválida.\n");
                        break;
                    }
                }

                mensagemAleatoria();
                printf("\n");
                EventosAleatorios(arvore.raiz);
                break;
            }
            case 7: {
                char nomeEvento[100];
                printf("Digite o nome do evento para buscar nas cidades:\n");

                // Clear input buffer
                while (getchar() != '\n');

                fgets(nomeEvento, sizeof(nomeEvento), stdin);

                // Remove newline character
                nomeEvento[strcspn(nomeEvento, "\n")] = '\0';

                BuscarCidadePorEvento(nomeEvento, arvore.raiz);

                mensagemAleatoria();
                printf("\n");
                EventosAleatorios(arvore.raiz);
                break;
            }
            case 8:
                TCidade cidades[10];
                int numCidades = ColetaCidades(arvore.raiz, cidades, 0);

                if (numCidades == 0) {
                    printf("Nenhuma cidade encontrada.\n");
                    break;
                }

                for (int i = 0; i < numCidades; i++) {
                    CalculaMediaCidade(&cidades[i]);
                }

                int indiceMenor = 0;
                for (int i = 1; i < numCidades; i++) {
                    if (cidades[i].notaMedia < cidades[indiceMenor].notaMedia) {
                        indiceMenor = i;
                    }
                }

                printf("%s - %.2f\n",cidades[indiceMenor].nome, cidades[indiceMenor].notaMedia);
                mensagemAleatoria();
                printf("\n");
                EventosAleatorios(arvore.raiz);
                break;
            case 9:
            {
                TCidade cidades[10];
                int numCidades = ColetaCidades(arvore.raiz, cidades, 0);

                if (numCidades == 0) {
                    printf("Nenhuma cidade encontrada.\n");
                    break;
                }

                // Calcular médias
                for (int i = 0; i < numCidades; i++) {
                    CalculaMediaCidade(&cidades[i]);
                }

                // Encontrar cidade com maior nota média
                int indiceMaior = 0;
                for (int i = 1; i < numCidades; i++) {
                    if (cidades[i].notaMedia > cidades[indiceMaior].notaMedia) {
                        indiceMaior = i;
                    }
                }

                printf("%s - %.2f\n", cidades[indiceMaior].nome, cidades[indiceMaior].notaMedia);
                mensagemAleatoria();
                printf("\n");
                EventosAleatorios(arvore.raiz);
                break;
            }
            case 10:
                criarRoteiro(arvore.raiz);
                mensagemAleatoria();
                printf("\n");
                EventosAleatorios(arvore.raiz);
                break;
            case 11:
            {
                printf("1 - Ordenação por Bubble Sort\n");
                printf("2 - Ordenação por Selection Sort\n");
                printf("3 - Ordenação por Quick Sort\n");
                printf("4 - Ordenação por Heap Sort\n");
                printf("5 - Ordenação por Insertion Sort\n");
                printf("6 - Ordenação por Shell Sort\n");
                printf("7 - Ordenação por Merge Sort\n");
                printf("Escolha uma opção: ");

                int sort;
                scanf("%d", &sort);

                int ordem;
                printf("Escolha a ordem: \n Crescente = 0 ou Decrescente = 1\n");
                scanf("%d", &ordem);

                TCidade cidades2[10];
                int numCidades2 = ColetaCidades(arvore.raiz, cidades2, 0);

                // Calcular médias antes de ordenar
                for (int i = 0; i < numCidades2; i++) {
                    CalculaMediaCidade(&cidades2[i]);
                }

                switch (sort) {
                    case 1:
                        BubbleSortCidades(cidades2, numCidades2, ordem);
                    break;
                    case 2:
                        SelectionSortCidades(cidades2, numCidades2, ordem);
                    break;
                    case 3:
                        QuickSortCidades(cidades2, numCidades2, ordem);
                    break;
                    case 4:
                        HeapSortCidades(cidades2, numCidades2, ordem);
                    break;
                    case 5:
                        InsertionSortCidades(cidades2, numCidades2, ordem);
                    break;
                    case 6:
                        ShellSortCidades(cidades2, numCidades2, ordem);
                    break;
                    case 7:
                        MergeSortCidades(cidades2, numCidades2, ordem);
                    break;
                }

                for (int i = 0; i < numCidades2; i++) {
                    printf("%s - Nota média: %.2f\n", cidades2[i].nome, cidades2[i].notaMedia);
                }

                mensagemAleatoria();
                printf("\n");
                EventosAleatorios(arvore.raiz);
                break;
            }

            case 12:
            {
                char nomeCidades[50];
                printf("Digite o nome da cidade que deseja ordenar os eventos: ");
                getchar();
                fgets(nomeCidades, 100, stdin);
                size_t len2 = strlen(nomeCidades);
                if (len2 > 0 && nomeCidades[len2 - 1] == '\n') {
                    nomeCidades[len2 - 1] = '\0';
                }

                TCidade *cidade = buscarCidade(arvore.raiz, nomeCidades);
                if (cidade == NULL) {
                    printf("Cidade não encontrada\n");
                    break;
                }

                printf("1 - Ordenação por Bubble Sort\n");
                printf("2 - Ordenação por Selection Sort\n");
                printf("3 - Ordenação por Quick Sort\n");
                printf("4 - Ordenação por Heap Sort\n");
                printf("5 - Ordenação por Insertion Sort\n");
                printf("6 - Ordenação por Shell Sort\n");
                printf("7 - Ordenação por Merge Sort\n");
                printf("Escolha uma opção: ");

                int sort2;
                scanf("%d", &sort2);

                int ordem;
                printf("Escolha a ordem: \n Crescente = 0 ou Decrescente = 1\n");
                scanf("%d", &ordem);

                switch (sort2) {
                    case 1:
                        BubbleSortEventos(cidade->eventos, cidade->numEventos, ordem);
                    break;
                    case 2:
                        SelectionSortEventos(cidade->eventos, cidade->numEventos, ordem);
                    break;
                    case 3:
                        QuickSortEventos(cidade->eventos, cidade->numEventos, ordem);
                    break;
                    case 4:
                        HeapSortEventos(cidade->eventos, cidade->numEventos, ordem);
                    break;
                    case 5:
                        InsertionSortEventos(cidade->eventos, cidade->numEventos, ordem);
                    break;
                    case 6:
                        ShellSortEventos(cidade->eventos, cidade->numEventos, ordem);
                    break;
                    case 7:
                        MergeSortEventos(cidade->eventos, cidade->numEventos, ordem);
                    break;
                }

                for (int i = 0; i < cidade->numEventos; i++) {
                    printf("%s - Avaliação: %.2f\n", cidade->eventos[i].nome, cidade->eventos[i].avaliacao);
                }

                mensagemAleatoria();
                printf("\n");
                EventosAleatorios(arvore.raiz);
                break;
            }

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
