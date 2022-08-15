#include "iostream"
#include "vector"
#include "math.h"
#include "chrono"
#include "algorithm"

// A estrutura do par serve para armazenar o par de index i e j da matriz
struct Pair
{
  int i;
  int j;

  pair()
  {
    i = 0;
    j = 0;
  }
};

// A estrutura da matriz serve para guardar um vetor de vetores de tipo genérico e suporta o get com a estrutura de dados pair
template <typename T>
struct Matrix
{
  std::vector<std::vector<T>> matrix;

  T get(int i, int j)
  {
    return matrix[i][j];
  }

  T get(pair &p)
  {
    return matrix[p.i][p.j];
  }

  int lines()
  {
    return matrix.size();
  }

  int cols()
  {
    return matrix[0].size();
  }

  int size()
  {
    return lines() * cols();
  }

  void push(std::vector<T> &line)
  {
    matrix.push_back(line);
  }
};

// A estrutura do nó serve para guardar e conectar os nós  do grafo
struct Node
{
  std::vector<Node *> edges;
  int index;
  bool opened = true;

  Node(int i)
  {
    index = i;
  }

  void connect(Node *node)
  {
    edges.push_back(node);
  }
};

// A estrutura do grafo armazena todos os nós e também os separa em lista de abertos e fechados
struct Graph
{
  std::vector<Node *> nodes;
  std::vector<Node *> openedNodes;
  std::vector<Node *> closedNodes;

  Graph(int size)
  {
    for (int i = 0; i < size; i++)
    {
      Node *newNode = new Node(i);
      nodes.push_back(newNode);
      openedNodes.push_back(newNode);
    }
  }

  void connectNodes(Node *srcNode, Node *dstNode)
  {
    srcNode->connect(dstNode);
    dstNode->connect(srcNode);
  }

  void closeNode(Node *dstNode)
  {
    int pos;

    for (pos = 0; pos < openedNodes.size(); pos++)
      if (openedNodes[pos]->index == dstNode->index)
        break;

    closedNodes.push_back(dstNode);
    openedNodes.erase(openedNodes.begin() + pos);

    dstNode->opened = false;
  }
};

/**
 * @brief Conecta aleatoriamente o grafo de maneira que todos os vértices possuam pelo menos uma adjacencia.
 *
 * @param graphSize Tamanho do grafo
 * @param graph Estrutura do grafo
 */
void generateRandomGraph(int graphSize, Graph &graph)
{
  // Número total de conexões do grafo
  int nEdges = (rand() % 10 + 1) * graphSize;

  // Número acumulativo de conexões gastas
  int spentConnections = 0;

  std::vector<Node *> openedNodesClone;

  for (int i = 0; i < graphSize; i++)
  {
    // Fecha o nó atual
    graph.closeNode(graph.nodes[i]);

    // Calcula o número de conexões disponíveis de até nEdges, com folga, para que todos os vértices se conectem pelo menos 1 vez
    int availableConnections = nEdges - spentConnections - graph.openedNodes.size();
    availableConnections = std::max(availableConnections, 1);

    // Dado o número de conexões disponíveis, usa como limitador para um número aleatório de conexões a serem feitas
    int nConnections = rand() % availableConnections + 1;
    nConnections = std::min(nConnections, (int)graph.openedNodes.size());
    spentConnections += nConnections;

    // Gera uma lista de nós abertos temporária para evitar colisões ao escolher um nó para se conectar
    openedNodesClone.clear();
    for (int j = 0; j < graph.openedNodes.size(); j++)
      openedNodesClone.push_back(graph.openedNodes[j]);

    // Conecta o nó com nConnections adjacentes de maneira aleatória
    for (int j = 0; j < nConnections; j++)
    {
      int randomNodeIndex = rand() % openedNodesClone.size();
      graph.connectNodes(graph.nodes[i], openedNodesClone[randomNodeIndex]);

      openedNodesClone.erase(openedNodesClone.begin() + randomNodeIndex);
    }
  }
}

void printGraph(Graph &graph)
{
  for (int i = 0; i < graph.nodes.size(); i++)
  {
    std::cout << "Item [" << i << "]:" << std::endl;

    std::cout << "(";
    for (int j = 0; j < graph.nodes[i]->edges.size(); j++)
      std::cout << (j == 0 ? "" : ",") << graph.nodes[i]->edges[j]->index;
    std::cout << ")" << std::endl;
  }
}

void printMatrix(Matrix<bool> m)
{
  for (int i = 0; i < m.lines(); i++)
  {
    for (int j = 0; j < m.cols(); j++)
      std::cout << (j == 0 ? "| " : " ") << (m.get(i, j) == true ? 1 : 0) << (j == m.cols() - 1 ? " |" : "");
    std::cout << std::endl;
  }
}

/**
 * @brief Dado o grafo, retorna a matriz binária de conexões
 *
 * @param graph
 * @return matrix<bool>
 */
Matrix<bool> binMatrix(Graph &graph)
{
  matrix<bool> m;

  for (int i = 0; i < graph.nodes.size(); i++)
  {
    std::vector<bool> line;

    for (int j = 0; j < graph.nodes.size(); j++)
      line.push_back(false);

    line[graph.nodes[i]->index] = true;

    for (int j = 0; j < graph.nodes[i]->edges.size(); j++)
    {
      line[graph.nodes[i]->edges[j]->index] = true;
    }

    m.push(line);
  }

  return m;
}

template <typename T>
void printVec(std::vector<T> v)
{
  std::cout << "Vector size: " << v.size() << std::endl;
  for (int i = 0; i < v.size(); i++)
    std::cout << (i == 0 ? "[ " : ", ") << v[i] << (i == v.size() - 1 ? " ]" : "");
  std::cout << std::endl;
}

/**
 * @brief Retorna o vetor binário correspondente à matriz triangular superior
 *
 * @param m
 * @return std::vector<bool>
 */
std::vector<bool> binVec(Matrix<bool> m)
{
  std::vector<bool> vec;

  for (int i = 0; i < m.lines(); i++)
    for (int j = i + 1; j < m.cols(); j++)
      vec.push_back(m.get(i, j));

  return vec;
}

/**
 * @brief Retorna o vetor compacto da matriz triangular superior
 * contendo o índice ao qual se conecta, ou -1 caso não faça conexão
 *
 * @param m
 * @return std::vector<int>
 */
std::vector<int> compactVec(Matrix<bool> m)
{
  std::vector<int> vec;

  for (int i = 0; i < m.lines(); i++)
    for (int j = i + 1; j < m.cols(); j++)
      vec.push_back(m.get(i, j) == true ? j : -1);

  return vec;
}

/**
 * @brief Dados {i} e {j} da matriz de tamanho {size}, computa o indice do vetor compactado do elemento.
 *
 * @param i
 * @param j
 * @param size Tamanho da matriz quadrada
 * @param pa Caso verdadeiro, usa o método analítico da Progressão Aritimética, caso contrário usa da iteratividade.
 * @return Indíce do vetor
 */
int computeVecIndex(int i, int j, int size, bool pa = true)
{
  if (i == j)
    return -1;

  // Converte para i < j
  if (i > j)
  {
    int aux = j;
    j = i;
    i = aux;
  }

  if (pa)
    return (i * size) - (i * (i + 1) / 2) + j - (i + 1);
  else
  {
    int index = 0;

    for (int k = 0; k < i; k++)
      index += size - (k + 1);

    index += j - (i + 1);

    return index;
  }
}

/**
 * @brief Dados {i} index do vetor e tamanho {size} da matriz, computa o par i, j da matriz
 *
 * @param i
 * @param size Tamanho da matriz quadrada
 * @param pa Caso verdadeiro, usa o método analítico da Progressão Aritimética, caso contrário usa da iteratividade.
 * @return Indíce do vetor
 */
Pair computeMatrixPair(int i, int size, bool pa = true)
{
  pair matrixPair;

  int offset = i + 1;

  do
  {
    // Decrementa do número de indices restantes a cada linha, retirando também os elementos da triangular inferior
    matrixPair.i += 1;
    if (offset > size - matrixPair.i)
    {
      offset -= size - matrixPair.i;
      continue;
    }

    // Acrescenta à coluna, os indices restantes + o número de elementos da triangular inferior
    matrixPair.i -= 1;
    matrixPair.j += offset + matrixPair.i;
    offset = 0;
  } while (offset > 0);

  return matrixPair;
}

/**
 * @brief  Retorna a matriz do vetor compacto
 *
 * Calcula o número de elementos da triangular inferior referente
 * à matriz quadrada de índice anterior ao atual
 *
 * Subtrai o valor encontrado do número total de elementos até
 * a linha anterior
 *
 * Soma com o indice da coluna subtraido do offset, dado que
 * offset é o número de elementos na triangular inferior
 * da linha
 *
 * Em sumo:
 * b a a a
 * b b x a
 * b b b a
 * b b b b
 *
 * Para encontrar {x}, soma-se o número de {a} da linha anterior
 * com a coluna atual menos o número de {b} na linha atual.
 *
 * @param vec
 * @param nodeCount
 * @return matrix<bool>
 */
Matrix<bool> compactToMatrix(std::vector<int> vec, int nodeCount)
{
  matrix<bool> m;

  for (int i = 0; i < nodeCount; i++)
  {
    std::vector<bool> line;

    for (int j = 0; j < nodeCount; j++)
      if (i == j)
        line.push_back(true);
      else
        line.push_back(vec[computeVecIndex(i, j, nodeCount)] != -1);

    m.push(line);
  }

  return m;
}

int main()
{
  srand(time(NULL));

  int graphSize = 100;
  Graph graph(graphSize);

  // QUESTÃO 1)
  generateRandomGraph(graphSize, graph);
  std::cout << "1) Randomly generated graph:" << std::endl;
  printGraph(graph);

  // QUESTÃO 2)
  Matrix<bool> m = binMatrix(graph);
  std::cout << "\n\n2) Binary matrix from graph:" << std::endl;
  printMatrix(m);

  // QUESTÃO 3)
  std::vector<bool> bvec = binVec(m);
  std::cout << "\n\n3) Binary vector from binary matrix:" << std::endl;
  printVec(bvec);

  // QUESTÃO 4)
  std::vector<int> cvec = compactVec(m);
  std::cout << "\n\n4) Compact vector from binary matrix:" << std::endl;
  printVec(cvec);

  // QUESTÃO 5)
  Matrix<bool> fromvec = compactToMatrix(cvec, graphSize);
  std::cout << "\n\n5) Binary matrix from compact vector:" << std::endl;
  printMatrix(fromvec);

  int randomComputes = rand() % 20 + 5;

  // QUESTÃO 6)
  std::cout << "\n\n6) Mapping " << randomComputes << " random pairs from matrix to vector:" << std::endl;
  for (int k = 0; k < randomComputes; k++)
  {
    int i = rand() % graphSize;
    int j = rand() % graphSize;

    int compIndex = computeVecIndex(i, j, graphSize);

    std::cout << "Mapping matrix pair [" << i << "][" << j << "] to vector: " << std::endl
              << "Real matrix value: " << m.get(i, j) << std::endl
              << "Correspondant vector index: " << compIndex << std::endl
              << "Correspondant vector value: " << cvec[compIndex] << std::endl;
  }

  // QUESTÃO 7)
  std::cout << "\n\n7) Mapping " << randomComputes << " random indexes from vector to matrix:" << std::endl;
  for (int k = 0; k < randomComputes; k++)
  {
    int i = rand() % cvec.size();

    Pair compPair = computeMatrixPair(i, graphSize);

    std::cout << "Mapping vector index " << i << " to matrix: " << std::endl
              << "Real vector value: " << cvec[i] << std::endl
              << "Correspondant matrix pair: [" << compPair.i << "][" << compPair.j << "]" << std::endl
              << "Correspondant matrix value: " << (m.get(compPair) ? 1 : 0) << std::endl;
  }

  std::cout << "Exit with status 0" << std::endl;
  return 0;
}