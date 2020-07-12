# struct_array
Wrapper object for "structure of arrays" coding style

### Small example
```c++
#include <algorithm>
#include <iostream>
#include <iterator>
#include <tuple>

#include "struct_array.h"

struct foo {
    int x;
};

struct bar {
    int x, y;
};

int main() {
    soa::struct_array<foo> sf;
    soa::struct_array<bar> sb0, sb1;

    for(auto i = 0; i < 10; ++i)
        sf.emplace_back(std::make_tuple(i));

	for(auto i = 0; i < 10; ++i)
		sb1.emplace(sb1.end(), std::make_tuple(i * i), std::make_tuple(i));

    std::cout << "sf:\n{ ";
    for(const auto& [x] : sf)
        std::cout << x << ' ';
    std::cout << "}\n";

	std::cout << "sb1:\n{ ";
    for(const auto& [x, y] : sb1)
        std::cout << '(' << x << ',' << y << ')' << ' ';
    std::cout << "}\n";

    std::transform(std::begin(sf), std::end(sf), std::back_inserter(sb0),
        [](const auto& v) noexcept 
        {
            const auto& [x] = v;
            return std::make_tuple(x, x * x);
        });

    std::cout << "sb0 unsorted:\n{ ";
    for(const auto& [x, y] : sb0)
        std::cout << '(' << x << ',' << y << ')' << ' ';
    std::cout << "}\n";

    std::sort(std::begin(sb0), std::end(sb0), 
        [](const auto& lhs, const auto& rhs) noexcept {
            const auto& [x0, y0] = lhs;
            const auto& [x1, y1] = rhs;
            return x0 > x1 || (x0 == x1 && y0 > y1);
        });

    std::cout << "sb0 sorted:\n{ ";
    for(const auto& [x, y] : sb0)
        std::cout << '(' << x << ',' << y << ')' << ' ';
    std::cout << "}\n";

    using std::swap;
    swap(sb0, sb1);

    std::cout << "sb swapped:\n{ ";
    for(const auto& [x, y] : sb0)
        std::cout << '(' << x << ',' << y << ')' << ' ';
    std::cout << "}\n";

    std::cout << "sb1 swapped:\n{ ";
    for(const auto& [x, y] : sb1)
        std::cout << '(' << x << ',' << y << ')' << ' ';
    std::cout << "}\n";
}
```
#### out
```
sf:
{ 0 1 2 3 4 5 6 7 8 9 }
sb1:
{ (0,0) (1,1) (4,2) (9,3) (16,4) (25,5) (36,6) (49,7) (64,8) (81,9) }
sb0 unsorted:
{ (0,0) (1,1) (2,4) (3,9) (4,16) (5,25) (6,36) (7,49) (8,64) (9,81) }
sb0 sorted:
{ (9,81) (8,64) (7,49) (6,36) (5,25) (4,16) (3,9) (2,4) (1,1) (0,0) }
sb swapped:
{ (0,0) (1,1) (4,2) (9,3) (16,4) (25,5) (36,6) (49,7) (64,8) (81,9) }
sb1 swapped:
{ (9,81) (8,64) (7,49) (6,36) (5,25) (4,16) (3,9) (2,4) (1,1) (0,0) }
```

`struct_array` treats any POD-type of size N, e.g. `foo`

```c++
struct foo {
  T0 _0;
  T1 _1;
  ...
  TN-1 _N-1
};
```

internally as `std::tuple<std::vector<T0>, std::vector<T1>, ... , std::vector<TN-1>>` 
and provides the interface of `std::vector`.
Currently supports PODs of up to size 8, but that can be extended easily by providing 
the appropriate specializations for `to_tuple` and `max_bind` and updating the value 
of max_bindings to N + 1. (look at `to_tuple.h` and `bind.h`)
