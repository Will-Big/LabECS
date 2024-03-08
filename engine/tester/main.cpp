#include <algorithm>
#include <cassert>

#include "List.h"

class MyClass
{
public:
	MyClass(int a, int b) : _a(a), _b(b) {}

	int _a;
	int _b;
};


int main()
{
	List<MyClass> myList;

	auto& t1 = myList.emplace_back(3, 4);

	t1._a++;

	return 0;
}