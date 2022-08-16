#ifndef STRUCTS_CPP
#define STRUCTS_CPP

#include "iostream"
#include "vector"
#include "math.h"
#include "algorithm"

// A estrutura do par serve para armazenar o par de index i e j da matriz
struct Pair
{
  int i;
  int j;

  Pair()
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

  T get(Pair &p)
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

#endif