#include "iostream"
#include "vector"
#include "math.h"
#include "algorithm"
#include "chrono"
#include "./structs.cpp"
#include "./functions.cpp"

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
  Matrix<bool> fromvec = compactToBinMatrix(cvec, graphSize);
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

  // QUESTÃO 8)
  int mSizes = 4;
  std::cout << "\n\n8) Generating two 4x4 random matrix" << std::endl;
  std::vector<int> vec1;
  std::vector<int> vec2;

  for (int i = 0; i < mSizes * (mSizes - 1) / 2; i++)
  {
    vec1.push_back(rand() % 21);
    vec2.push_back(rand() % 21);
  }

  std::cout << "Matrix A:" << std::endl;
  printMatrix(compactToMatrix(vec1, mSizes));
  std::cout << "\nMatrix B:" << std::endl;
  printMatrix(compactToMatrix(vec2, mSizes));

  std::cout << "\nCalculating union:" << std::endl;
  printMatrix(sumMatrix(vec1, vec2, mSizes));

  std::cout << "\nCalculating intersection:" << std::endl;
  printMatrix(mulMatrix(vec1, vec2, mSizes));

  std::cout << "Exit with status 0" << std::endl;
  return 0;
}