#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <omp.h>

using namespace std;

int binarySearch(const vector<int>& A, int target) {
  int n = A.size();
  int ans = -1;
  #pragma omp parallel
  {
    int thread_id = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
    int chunk_size = n / num_threads;
    int low = thread_id * chunk_size;
    int hi = (thread_id == num_threads - 1) ? n - 1 : low + chunk_size - 1;

    while (low <= hi) {
      int mid = low + (hi - low) / 2;
      if (A[mid] == target){
        #pragma omp critical
        {
          ans = mid;
        }
        break;
      }
      else if (A[mid] < target){
        low = mid + 1;
      }
      else{ 
        hi = mid - 1;
      }
    }
  }
  return ans;
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
    
  omp_set_num_threads(omp_get_max_threads());
    
  int target = A[dist(gen)%size];
    
  auto start = chrono::high_resolution_clock::now();
  int result = binarySearch(A, target);
  auto end = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

  cout << "P-BSA: Elemento " << target << ": " << result << endl;
  cout << "Tiempo de ejecucion: " << duration.count() << " μs" << endl;
    
  return 0;

}
