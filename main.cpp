#include "iostream"
#include "vector"
#include "math.h"
#include "chrono"

template <typename T>
using matrix = std::vector<std::vector<T>>;

struct Node
{
  std::vector<Node *> edges;
  int index;

  Node(int i)
  {
    index = i;
  }

  void connect(Node *node)
  {
    edges.push_back(node);
  }
};

void generateVertexes(int graphSize, std::vector<Node> &graph)
{
  for (int i = 0; i < graphSize; i++)
  {
    Node node(i);
    graph.push_back(node);
  }
}

void generateRandomGraph(int graphSize, std::vector<Node> &graph)
{
  std::vector<int> list;
  int nEdges = 100 + round(rand() % (10 * graphSize - 100));
  int connections = 0;

  // std::cout << "nEdges: " << nEdges << std::endl
  //           << "graphSize: " << graphSize << std::endl
  //           << "Generating: ";

  std::chrono::time_point<std::chrono::high_resolution_clock> coutTimer;
  double couts = 0;

  for (int i = 0; i < graphSize; i++)
  {
    // int n = round((nEdges / graphSize));
    int n = 1 + round(rand() % (nEdges / graphSize));
    coutTimer = std::chrono::high_resolution_clock::now();
    // std::cout << (i == 0 ? "" : ", ") << n;
    couts += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - coutTimer).count();

    list.clear();
    for (int j = 0; j < n; j++)
    {
      int index;
    tryagain:
      index = rand() % graphSize;

      for (int x = 0; x < list.size(); x++)
        if (list[x] == index)
          goto tryagain;

      list.push_back(index);

      graph[i].connect(&graph[index]);
      graph[index].connect(&graph[i]);

      connections++;

      if (connections >= nEdges)
        break;
    }
  }
}

void printGraph(std::vector<Node> &graph)
{
  for (int i = 0; i < graph.size(); i++)
  {
    std::cout << "Item [" << i << "]:" << std::endl;

    std::cout << "(";
    for (int j = 0; j < graph[i].edges.size(); j++)
      std::cout << (j == 0 ? "" : ",") << graph[i].edges[j]->index;
    std::cout << ")" << std::endl;
  }
}

void printMatrix(matrix<bool> m)
{
  for (int i = 0; i < m.size(); i++)
  {
    for (int j = 0; j < m[i].size(); j++)
      std::cout << (j == 0 ? "| " : " ") << (m[i][j] == true ? 1 : 0) << (j == m[i].size() - 1 ? " |" : "");
    std::cout << std::endl;
  }
}

matrix<bool> binMatrix(std::vector<Node> &graph)
{
  matrix<bool> m;

  for (int i = 0; i < graph.size(); i++)
  {
    std::vector<bool> line;

    for (int j = 0; j < graph.size(); j++)
      line.push_back(false);

    for (int j = 0; j < graph[i].edges.size(); j++)
    {
      line[graph[i].edges[j]->index] = true;
    }

    m.push_back(line);
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

std::vector<bool> binVec(matrix<bool> m)
{
  std::vector<bool> vec;

  // std::cout << "matrix line size: " << m.size() << std::endl;
  // std::cout << "matrix col size: " << m[0].size() << std::endl;

  for (int i = 0; i < m.size(); i++)
    for (int j = i; j < m[i].size(); j++)
      vec.push_back(m[i][j]);

  // std::cout << "vec size: " << vec.size() << std::endl;

  return vec;
}

std::vector<int> compactVec(matrix<bool> m)
{
  std::vector<int> vec;

  for (int i = 0; i < m.size(); i++)
    for (int j = i; j < m[i].size(); j++)
      vec.push_back(m[i][j] == true ? j : -1);

  return vec;
}

matrix<bool> compactToMatrix(std::vector<int> vec, int nVertex)
{
  matrix<bool> m;
  std::vector<int> emps;

  for (int i = 0; i < nVertex; i++)
  {
    std::vector<bool> line;
    m.push_back(line);

    emps.push_back(i == 0 ? 0 : emps[i - 1] + i);

    for (int j = 0; j < nVertex; j++)
    {
      // std::cout << "DEBUG: "
      //           << "; i:" << i << "; j:" << j << "; " << (i * nVertex + j) - emps[i] << "; " << (j * nVertex + i) - emps[j] << std::endl;
      int value = i <= j ? vec[(i * nVertex + j) - emps[i]] : vec[(j * nVertex + i) - emps[j]];
      m[i].push_back(i <= j ? value == j : value == i);
    }
  }

  return m;
}

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

  std::vector<Node> graph;
  int graphSize = 100;

  // 1)
  generateVertexes(graphSize, graph);
  generateRandomGraph(graphSize, graph);
  // std::cout << "Graph:" << std::endl;
  // printGraph(graph);

  // 2)
  matrix<bool> m = binMatrix(graph);
  std::cout << "Binary matrix from graph:" << std::endl;
  printMatrix(m);

  // 3)
  std::vector<bool> bvec = binVec(m);
  // std::cout << "Binary vector from binary matrix:" << std::endl;
  // printVec(bvec);

  // 4)
  std::vector<int> cvec = compactVec(m);
  std::cout << "Compact vector from binary matrix:" << std::endl;
  printVec(cvec);

  // 5)
  matrix<bool> fromvec = compactToMatrix(cvec, graphSize);
  // std::cout << "Binary matrix from compact vector:" << std::endl;
  // printMatrix(fromvec);

  // 6)
  for (int x = 0; x < 20; x++)
  {
    int i = 20, j = x;
    int vecIndex = computeVecIndex(i, j, graphSize);
    // std::cout << "Compact vector on matrix[" << i << "][" << j << "]: " << std::endl
    //           << "Mapped index: " << vecIndex << std::endl
    //           << "Vector value: " << cvec[vecIndex] << std::endl;
  }

  // 7)

  std::cout << "Exit with status 0" << std::endl;
  return 0;
}