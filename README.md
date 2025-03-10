Rooteiro - Sistema de Roteiro Turístico
Descrição
Este projeto implementa um sistema de decisão para o planejamento de um rooteiro turístico inteligente (trocadilho com "root" de árvore e "roteiro"). O sistema modela uma rede de cidades em uma árvore binária de busca, onde cada nó representa uma cidade. Para cada cidade, um conjunto de eventos (atrações) é gerado automaticamente, contendo um nome e uma avaliação (valor entre 0.0 e 10.0).
Estrutura do Projeto
O sistema é baseado em uma estrutura de árvore binária de busca com as seguintes características:

Cada nó da árvore representa uma cidade com um nome gerado aleatoriamente
Cada cidade contém uma lista de eventos turísticos com avaliações
A árvore suporta as operações padrão: inserção, busca, caminhamentos, etc.

Funcionalidades Implementadas
Gerenciamento de Cidades

Geração aleatória de cidades com nomes variados
Inserção de cidades na árvore binária de busca
Busca de cidades por chave

Gerenciamento de Eventos

Geração aleatória de eventos turísticos para cada cidade
Cada evento possui um nome e uma avaliação entre 0.0 e 10.0

Operações da Árvore

Caminhamento central (in-ordem)
Caminhamento pré-ordem
Caminhamento pós-ordem
Busca de nós
Identificação de mínimo e máximo
Busca de sucessor e predecessor

Como Compilar e Executar
Para compilar o programa, utilize o compilador gcc:
bashCopygcc main.c -o rooteiro
Para executar:
bashCopy./rooteiro
Exemplo de Saída
Ao executar o programa, você verá:

Lista das cidades geradas em ordem crescente de chave (caminhamento central)
Lista das cidades em percurso pré-ordem
Lista das cidades em percurso pós-ordem
Detalhes da cidade com menor chave e seus eventos
Detalhes da cidade com maior chave e seus eventos

Próximos Passos

Implementar algoritmos de ordenação (Bubble Sort, Selection Sort, Insertion Sort, Shellsort, Quicksort, Mergesort e Heapsort) para ordenar os eventos de cada cidade
Desenvolver funcionalidade para escolha de roteiros entre cidades
Adicionar interface para interação do usuário

Autores
Gabriel Fontes
João Pedro Lobo
Gabriel Calili Nunes

Licença
Este projeto foi desenvolvido como parte de um trabalho acadêmico.
