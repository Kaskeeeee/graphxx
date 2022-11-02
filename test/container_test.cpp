#include "catch2/catch.hpp"
#include "g_container.hpp"
#include "g_iterator.hpp"
#include <vector>

TEST_CASE("Template container methods test", "[Container]") {
  using namespace graph;
  std::vector<int> V;

  struct VectorMapIt
      : public it::MapIterator<VectorMapIt, int, std::vector<int>::iterator> {
  public:
    void next() { ++_it; };
    int value() { return *_it; };
    VectorMapIt(std::vector<int>::iterator it) : _Super(it){};
  };

  container::C<VectorMapIt, std::vector<int>> container(V, false);

  SECTION("Container retrieves the same values as the wrapped one") {
    for (int i = 0; i < 10; i++) {
      V.push_back(i);
    }

    int i = 0;
    for (auto elem : container) {
      REQUIRE(elem == V[i++]);
    }
  }

  SECTION("Container gives the same values as the wrapped one") {
    for (int i = 0; i < 10; i++) {
      V.push_back(i);
    }

    REQUIRE(container.size() == V.size());
    REQUIRE(container.empty() == V.empty());
  }
}