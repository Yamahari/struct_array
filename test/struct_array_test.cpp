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
    soa::struct_array<bar> sb;

    for(auto i = 0; i < 10; ++i)
        sf.emplace_back(std::make_tuple(i));

    std::transform(std::begin(sf), std::end(sf), std::back_inserter(sb),
        [](const std::tuple<int&>& foo) noexcept {
            const auto& [x] = foo;
            return std::make_tuple(x, x * x);
        });

    for(const auto& [x, y] : sb)
        std::cout << x << ' ' << y << '\n';
}