# Trabalho de APA

## Alunos
Thiago de Oliveira Abreu

Matheus Reis

## Estruturas de Dados

**Node**

A estrutura do nó guarda o seu index, se está fechado ou aberto e seus adjacentes

```
struct Node
{
  std::vector<Node *> edges;
  int index;
  bool opened = true;

  Node(int i);
  void connect(Node *node);
};
```

**Grafo**

A estrutura do grafo, guarda todos os nós, os fechados e os abertos, além de contar com algumas funções auxiliares para iniciar o grafo e conectar dois nós.

```
struct Graph
{
  std::vector<Node *> nodes;
  std::vector<Node *> openedNodes;
  std::vector<Node *> closedNodes;

  Graph(int size);
  void connectNodes(Node *srcNode, Node *dstNode);
};
```

## Funções

**generateRandomGraph** –

Gera as adjacencias de um grafo, calculando primeiramente a quantidade total de ligações que o grafo terá e, com base nisso, calculando para cada nó, um número aleatório de ligações, respeitando o limite para que cada nó possua pelo menos uma ligação.


**binMatrix** – Dado o grafo, transforma em uma matriz adjacência binária.

**binVec** – Dada a matriz adjacência binária, transforma em um vetor binário.

**compactVec** – Dada a matriz adjacência binária, transforma em um vetor compacto com endereçamento indireto.

**compactToMatrix** – Dado o vetor compacto, transforma em uma matriz adjacência binária.

**computeVecIndex** – Computa o index do vetor, dado i e j da matriz adjacência binária.

**printGraph** – Printa os nós e suas conexões.

**printMatrix** – Printa uma matriz qualquer.

**printVec** – Printa um vetor qualquer.

## Compilação e Execução

```bash
g++ *.cpp -o app
app > output
```