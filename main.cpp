#include"Vector.h"
#include"Allocate.h"
using namespace std;
using namespace tinystl;

int main() {
	vector<int>test;
	test.push_back(10);
	test.insert(test.begin(), 3);
	for (auto& x : test) {
		cout << x << endl;
	}
	return 0;
}
