#pragma once
#include "base.hpp"

#include <unordered_map>

namespace graph{
    template <typename T> class VertexPropertyMap {
        protected:       
            std::unordered_map<Id, T> _vertex_with_prop;
        public:
            T& operator [] (const Vertex& v){
                return _vertex_with_prop[v.id];
            };

            T& operator [] (Vertex&& v){
                return _vertex_with_prop[v.id];
            };

    };
}
