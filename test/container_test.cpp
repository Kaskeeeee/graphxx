#include "catch2/catch.hpp"
#include "g_container.hpp"
#include "g_iterator.hpp"
#include <vector>

TEST_CASE("template container wraps a STL container", "[container]") {
  using namespace graph;
  std::vector<int> V;

  struct VectorMapIt
      : public it::MapIterator<VectorMapIt, int, std::vector<int>::iterator> {
  public:
    void next() { ++_it; };
    int value() { return *_it; };
    VectorMapIt(std::vector<int>::iterator it) : _Super(it){};
  };

  container::C<VectorMapIt, std::vector<int>> container(V);

  SECTION("container retrieves the same values as the wrapped one") {
    for (int i = 0; i < 10; i++) {
      V.push_back(i);
    }

    int i = 0;
    for (auto elem : container) {
      REQUIRE(elem == V[i++]);
    }
  }

  SECTION("container gives the same size and empty values as the wrapped one") {
    for (int i = 0; i < 10; i++) {
      V.push_back(i);
    }

    REQUIRE(container.size() == V.size());
    REQUIRE(container.empty() == V.empty());
  }
}