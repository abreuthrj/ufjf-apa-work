#ifndef FUNCTIONS_CPP
#define FUNCTIONS_CPP

#include "./structs.cpp"
#include "iostream"
#include "vector"
#include "math.h"
#include "algorithm"

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

template <typename T>
void printMatrix(Matrix<T> m)
{
  for (int i = 0; i < m.lines(); i++)
  {
    for (int j = 0; j < m.cols(); j++)
      std::cout << (j == 0 ? "| " : " ") << (int)m.get(i, j) << (j == m.cols() - 1 ? " |" : "");
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
  Matrix<bool> m;

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
  Pair matrixPair;

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
Matrix<bool> compactToBinMatrix(std::vector<int> vec, int nodeCount)
{
  Matrix<bool> m;

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

Matrix<int> compactToMatrix(std::vector<int> vec, int nodeCount)
{
  Matrix<int> m;

  for (int i = 0; i < nodeCount; i++)
  {
    std::vector<int> line;

    for (int j = 0; j < nodeCount; j++)
      if (i == j)
        line.push_back(0);
      else
        line.push_back(vec[computeVecIndex(i, j, nodeCount)]);

    m.push(line);
  }

  return m;
}

Matrix<int> sumMatrix(std::vector<int> vec1, std::vector<int> vec2, int size)
{
  std::vector<int> vec;

  for (int i = 0; i < vec1.size(); i++)
    vec.push_back(vec1[i] + vec2[i]);

  return compactToMatrix(vec, size);
}

Matrix<int> mulMatrix(std::vector<int> vec1, std::vector<int> vec2, int size)
{
  std::vector<int> vec;

  for (int i = 0; i < size; i++)
  {
    for (int j = i + 1; j < size; j++)
    {
      int r = 0;

      std::cout << "Index [" << i << "][" << j << "]: ";

      for (int k = 0; k < size; k++)
      {
        int vec1Index = computeVecIndex(i, k, size);
        int vec2Index = computeVecIndex(k, j, size);

        int vec1Value = (vec1Index == -1 ? 0 : vec1[vec1Index]);
        int vec2Value = (vec2Index == -1 ? 0 : vec2[vec2Index]);

        r += vec1Value * vec2Value;

        std::cout << (k != 0 ? " + " : "") << vec1Value << " * " << vec2Value;
      }

      std::cout << " = " << r << std::endl;

      vec.push_back(r);
    }
  }

  std::cout << std::endl;

  return compactToMatrix(vec, size);
}

#endif