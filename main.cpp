#include "iostream"
#include "vector"
#include "math.h"
#include "chrono"
#include "algorithm"

template <typename T>
using matrix = std::vector<std::vector<T>>;

// A estrutura do nó serve para guardar e conectar
// os nós  do grafo
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

// A estrutura do grafo armazena todos os nós
// e também os separa em lista de abertos e fechados
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

// Gera um grafo aleatório
void generateRandomGraph(int graphSize, Graph &graph)
{
  int nEdges = (rand() % 10 + 1) * graphSize;
  // std::cout << "Number of edges in the graph: " << nEdges << std::endl;

  int spentConnections = 0;

  std::vector<Node *> openedNodesClone;

  for (int i = 0; i < graphSize; i++)
  {
    graph.closeNode(graph.nodes[i]);

    // Gera um número de conexões de até nEdges, que permita
    // que todos os vértices se conectem pelo menos 1 vez
    int availableConnections = nEdges - spentConnections - graph.openedNodes.size();
    availableConnections = std::max(availableConnections, 1);
    // std::cout << "Number of available connections: " << availableConnections << std::endl;

    int nConnections = rand() % availableConnections + 1;
    nConnections = std::min(nConnections, (int)graph.openedNodes.size());
    spentConnections += nConnections;
    // std::cout << "Number of connections in node [" << i << "]: " << nConnections << std::endl;

    // Clona a lista de nós abertos
    openedNodesClone.clear();
    for (int j = 0; j < graph.openedNodes.size(); j++)
      openedNodesClone.push_back(graph.openedNodes[j]);

    // Conecta o nó com todos os adjacentes de maneira
    // aleatória
    for (int j = 0; j < nConnections; j++)
    {
      int randomNodeIndex = rand() % openedNodesClone.size();
      // std::cout << "Selected node [" << openedNodesClone[randomNodeIndex]->index << "] to connect with [" << i << "]" << std::endl;
      graph.connectNodes(graph.nodes[i], openedNodesClone[randomNodeIndex]);

      openedNodesClone.erase(openedNodesClone.begin() + randomNodeIndex);
      // std::cout << "Removed from opened nodes clone, which now has size of: " << openedNodesClone.size() << std::endl;
    }
  }

  // std::cout << "Number of connections made" << spentConnections << std::endl;
}

// Printa o grafo
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

// Printa uma matriz qualquer
void printMatrix(matrix<bool> m)
{
  for (int i = 0; i < m.size(); i++)
  {
    for (int j = 0; j < m[i].size(); j++)
      std::cout << (j == 0 ? "| " : " ") << (m[i][j] == true ? 1 : 0) << (j == m[i].size() - 1 ? " |" : "");
    std::cout << std::endl;
  }
}

// Retorna a matriz binária do vetor
matrix<bool> binMatrix(Graph &graph)
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

    m.push_back(line);
  }

  return m;
}

// Printa um vetor aleatório
template <typename T>
void printVec(std::vector<T> v)
{
  std::cout << "Vector size: " << v.size() << std::endl;
  for (int i = 0; i < v.size(); i++)
    std::cout << (i == 0 ? "[ " : ", ") << v[i] << (i == v.size() - 1 ? " ]" : "");
  std::cout << std::endl;
}

// Retorna o vetor da triangular superior
// da matriz binária
std::vector<bool> binVec(matrix<bool> m)
{
  std::vector<bool> vec;

  for (int i = 0; i < m.size(); i++)
    for (int j = i + 1; j < m[i].size(); j++)
      vec.push_back(m[i][j]);

  return vec;
}

/**
 * @brief Retorna o vetor compacto da matriz triangular superior
 * contendo o índice ao qual se conecta, ou -1 caso não faça conexão
 *
 * @param m
 * @return std::vector<int>
 */
std::vector<int> compactVec(matrix<bool> m)
{
  std::vector<int> vec;

  for (int i = 0; i < m.size(); i++)
    for (int j = i + 1; j < m[i].size(); j++)
      vec.push_back(m[i][j] == true ? j : -1);

  return vec;
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
matrix<bool> compactToMatrix(std::vector<int> vec, int nodeCount)
{
  matrix<bool> m;

  for (int i = 0; i < nodeCount; i++)
  {
    std::vector<bool> line;

    for (int j = 0; j < nodeCount; j++)
      if (i == j)
        line.push_back(true);
      else
        line.push_back(vec[i < j ? (i * nodeCount) - (i * (i + 1) / 2) + j - (i + 1) : (j * nodeCount) - (j * (j + 1) / 2) + i - (j + 1)] != -1);

    m.push_back(line);
  }

  return m;
}

// Retorna o index computado da matriz em vetor
int computeVecIndex(int i, int j, int size)
{
  std::vector<int> jumps;

  for (int i = 0; i < size; i++)
    jumps.push_back(i == 0 ? 0 : jumps[i - 1] + i);

  return i <= j ? (i * size + j) - jumps[i] : (j * size + i) - jumps[j];
}

int main()
{
  srand(time(NULL));

  // std::vector<Node> graph;
  int graphSize = 100;
  Graph graph(graphSize);

  // 1)
  // generateVertexes(graphSize, graph);
  generateRandomGraph(graphSize, graph);
  // std::cout << "Graph:" << std::endl;
  // printGraph(graph);

  // 2)
  matrix<bool> m = binMatrix(graph);
  std::cout << "Binary matrix from graph:" << std::endl;
  printMatrix(m);

  // 3)
  std::vector<bool> bvec = binVec(m);
  std::cout << "Binary vector from binary matrix:" << std::endl;
  printVec(bvec);

  // 4)
  std::vector<int> cvec = compactVec(m);
  std::cout << "Compact vector from binary matrix:" << std::endl;
  printVec(cvec);

  // 5)
  matrix<bool> fromvec = compactToMatrix(cvec, graphSize);
  std::cout << "Binary matrix from compact vector:" << std::endl;
  printMatrix(fromvec);

  // 6)
  // for (int x = 0; x < 20; x++)
  // {
  //   int i = 20, j = x;
  //   int vecIndex = computeVecIndex(i, j, graphSize);
  // std::cout << "Compact vector on matrix[" << i << "][" << j << "]: " << std::endl
  //           << "Mapped index: " << vecIndex << std::endl
  //           << "Vector value: " << cvec[vecIndex] << std::endl;
  // }

  // 7)

  std::cout << "Exit with status 0" << std::endl;
  return 0;
}