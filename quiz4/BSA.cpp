#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

int binarySearch(const vector<int>& A, int target) {
  int low = 0;
  int hi = A.size() - 1;
  while (hi - low > 1) {
    int mid = hi + ((hi - low) >> 1);
    if (A[mid] < target)
      low = mid;
    else 
      hi = mid;
    }
  return low;
}


int main() {

  const int size = 100000;
  vector<int> A(size);
    
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dist(1, size*10);
    
  for (int i = 0; i < size; ++i)
    A[i] = dist(gen);
    
  sort(A.begin(), A.end());
    
    
  int target = A[dist(gen)%size];
    
  auto start = chrono::high_resolution_clock::now();
  int result = binarySearch(A, target);
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

  cout << "BSA: Elemento " << target << ": " << result << endl;
  cout << "Tiempo de ejecucion: " << duration.count() << " μs" << endl;
    
  return 0;

}
