#pragma once
#include "g_base.hpp"

#include <unordered_map>

namespace graph{
    template <typename T> class EdgePropertyMap {
        protected:       
            std::unordered_map<Id, T> _edge_with_prop;
        public:
            T& operator [] (const Edge& e){
                return _edge_with_prop[e.id];
            };

            T& operator [] (Edge&& e){
                return _edge_with_prop[e.id];
            };


    };
}