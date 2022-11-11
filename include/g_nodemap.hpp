#pragma once
#include "g_base.hpp"

#include <unordered_map>

namespace graph{
    template <typename T> class NodeMap {
        protected:       
            std::unordered_map<Id, T> _node_with_prop;
        public:
            T& operator [] (const Vertex &v){
                return _node_with_prop.at(v.id);
            };

            void setProperty(const Vertex &v, const T &value){
                _node_with_prop[v.id] = value;
            }


    };
}