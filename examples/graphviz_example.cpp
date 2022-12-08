#include <bits/stdc++.h>
#include "list_graph.hpp"
#include "io/graphviz.hpp"
#include "io/graphml.hpp"
#include <fstream>

using namespace graph;
using namespace std;

int main() {

    AdjacencyListGraph<Directedness::UNDIRECTED> g{};

    std::unordered_map<int, std::string> vertex_map;
    
    Vertex v1 = g.add_vertex(); //A
    vertex_map.insert({v1, "A"});

    Vertex v2 = g.add_vertex();
    vertex_map.insert({v2, "B"});

    Vertex v3 = g.add_vertex();
    vertex_map.insert({v3, "C"});

    Vertex v4 = g.add_vertex();
    vertex_map.insert({v4, "D"});

    Vertex v5 = g.add_vertex();
    vertex_map.insert({v5, "E"});

    g.add_edge(v1, v2);
    g.add_edge(v2, v2);
    g.add_edge(v1, v3);
    g.add_edge(v3, v4);
    g.add_edge(v5, v3);
    g.add_edge(v4, v5);
    g.add_edge(v5, v4);
    g.add_edge(v4, v2);
    g.add_edge(v4, v4);

    io::graphviz::serialize(cout, g, [&](Vertex v) {return io::graphviz::GraphvizProperties{{"label", vertex_map[v.id]}}; });

    cout << "\n\n";

    io::graphml::serialize(cout, g, [&](Vertex v) {return io::graphviz::GraphvizProperties{{"label", vertex_map[v.id]}}; });

    return 0;
}