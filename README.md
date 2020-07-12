# struct_array
Wrapper object for "structure of arrays" coding style

Small example
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

    std::cout << "sf:\n";
    for(const auto& [x] : sf)
        std::cout << x << '\n';
    std::cout << '\n';

    std::transform(std::begin(sf), std::end(sf), std::back_inserter(sb0),
        [](const auto& v) noexcept 
        {
            const auto& [x] = v;
            return std::make_tuple(x, x * x);
        });

    std::cout << "sb0 unsorted:\n";
    for(const auto& [x, y] : sb0)
        std::cout << x << ' ' << y << '\n';
    std::cout << '\n';

    std::sort(std::begin(sb0), std::end(sb0), [](const auto& lhs, const auto& rhs) noexcept {
        const auto& [x0, y0] = lhs;
        const auto& [x1, y1] = rhs;
        return x0 > x1 || (x0 == x1 && y0 > y1);
    });

    std::cout << "sb0 sorted:\n";
    for(const auto& [x, y] : sb0)
        std::cout << x << ' ' << y << '\n';
    std::cout << '\n';

    using std::swap;
    swap(sb0, sb1);

    std::cout << "sb0 swapped:\n";
    for(const auto& [x, y] : sb0)
        std::cout << x << ' ' << y << '\n';
    std::cout << '\n';

    std::cout << "sb1:\n";
    for(const auto& [x, y] : sb1)
        std::cout << x << ' ' << y << '\n';
    std::cout << '\n';
}
```

`struct_array` treats any POD-type `foo`

```c++
struct foo {
  T0 x;
  T1 y;
};
```

internally as `std::tuple<std::vector<T0>, std::vector<T1>...>` and provides the interface of std::vector.
