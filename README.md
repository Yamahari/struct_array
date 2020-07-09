# struct_array
Wrapper object for "structure of arrays" coding style

Small example
```c++
#include <algorithm>
#include <iostream>

#include "struct_array.h"

using namespace std;

struct foo {
	int x;
};

struct bar {
	int x, y;
};

int main()
{
	soa::struct_array<foo> sf;
	soa::struct_array<bar> sb;

	for (auto i = 0; i < 10; ++i)
		sf.emplace_back(std::make_tuple(i));

	std::transform(sf.begin(), sf.end(), std::back_inserter(sb),
		[](auto&& foo) {
			const auto& [x] = foo;
			return std::make_tuple(x, x * x);
		});

	for (const auto& [x, y] : sb)
		std::cout << x << ' ' << y << '\n';
}
```

`struct_array` treats any POD-type `Foo`

```c++
struct Foo {
  T0 x;
  T1 y;
};
```

internally as `std::tuple<std::vector<T0>, std::vector<T1>...>`.
