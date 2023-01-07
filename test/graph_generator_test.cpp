#include "base.hpp"
#include "catch.hpp"
#include "list_graph.hpp"
#include "utils/graph_generator.hpp"

namespace graph_generator_test {
using namespace graph;

TEST_CASE("Graph generator tests", "[GRAPH_GEN]") {
  AdjacencyListGraph<Directedness::DIRECTED> undirected_list_graph{};
  GraphGenerator generator;

  const int TOTAL_VERTICES = 15;
  const int TOTAL_EDGES = 20;

  SECTION("generates input limited nodes") {
    generator.generate_random_graph(undirected_list_graph, TOTAL_VERTICES,
                                    TOTAL_EDGES);

    int num_vertices = 0;
    for (auto vertex : undirected_list_graph.vertices()) {
      num_vertices++;
    }

    REQUIRE(num_vertices == TOTAL_VERTICES);
  }

  SECTION("generates input limited edges") {
    generator.generate_random_graph(undirected_list_graph, TOTAL_VERTICES,
                                    TOTAL_EDGES);

    int num_edges = 0;
    for (auto edge : undirected_list_graph.edges()) {
      num_edges++;
    }

    REQUIRE(num_edges == TOTAL_EDGES);
  }

  SECTION("early returns if exceeds max out degree") {
    generator.generate_random_graph(undirected_list_graph, TOTAL_VERTICES,
                                    TOTAL_EDGES, 1);

    int num_edges = 0;
    for (auto edge : undirected_list_graph.edges()) {
      num_edges++;
    }

    REQUIRE(num_edges == TOTAL_VERTICES);
  }

  SECTION("early returns if there is not other candidate for a new edge when "
          "using no self edges mode") {
    generator.generate_random_graph(undirected_list_graph, 1, TOTAL_EDGES, -1,
                                    false);

    int num_edges = 0;
    for (auto edge : undirected_list_graph.edges()) {
      num_edges++;
    }

    REQUIRE(num_edges == 0);
  }

  SECTION("chooses another target id with no self edges mode") {
    generator.generate_random_graph(undirected_list_graph, 2, 100, -1, false);

    for (auto edge : undirected_list_graph.edges()) {
      REQUIRE(edge.u != edge.v);
    }
  }

  const int MAX_WEIGHT = 10;
  const int MIN_WEIGHT = 0;

  SECTION("generates weights within bounds") {
    generator.generate_random_graph(undirected_list_graph, TOTAL_VERTICES,
                                    TOTAL_EDGES);
    auto weights = generator.generate_random_weights(undirected_list_graph,
                                                     MIN_WEIGHT, MAX_WEIGHT);

    for (auto [id, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION("swaps min and max weight if in bad order") {
    generator.generate_random_graph(undirected_list_graph, TOTAL_VERTICES,
                                    TOTAL_EDGES);
    auto weights = generator.generate_random_weights(undirected_list_graph,
                                                     MAX_WEIGHT, MIN_WEIGHT);

    for (auto [id, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION(
      "swaps min and max weight if in bad order with fixed seed generator") {
    GraphGenerator fixed_seed_gen{0xdeadbeef};

    fixed_seed_gen.generate_random_graph(undirected_list_graph, TOTAL_VERTICES,
                                         TOTAL_EDGES);
    auto weights = fixed_seed_gen.generate_random_weights(
        undirected_list_graph, MAX_WEIGHT, MIN_WEIGHT);

    for (auto [id, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }
}
} // namespace graph_generator_test