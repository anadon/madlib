#pragma once

#include <unordered_map>

namespace std{
    template<>
    struct hash< std::pair<double, size_t> > {
        std::size_t
        operator()(
          std::pair<double, size_t> const& s
        ) const noexcept {
            std::size_t const h1 ( std::hash< double >{}(s.first) );
            std::size_t const h2 ( std::hash< std::size_t >{}(s.second) );
            return h1 ^ ( h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2) );
        }
    };
}
