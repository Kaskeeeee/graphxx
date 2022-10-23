#pragma once

#include "g_base.hpp"
#include "g_concepts.hpp"

namespace graph {
namespace walker {
template <graph::concepts::Graph GraphType, typename Output,
          typename ItConstructorParamType>
class Walker {
  struct Manager {
    static void next(GraphType &, ItConstructorParamType &);
    static Output get_output(GraphType &, ItConstructorParamType &);
    static ItConstructorParamType get_begin(GraphType &);
    static ItConstructorParamType get_end(GraphType &);
  }

  struct It {
    using value_type = Output;
    using reference = Output &;
    using pointer = Output *;

    It(ItConstructorParamType it);

    Output operator*();
    It &operator++();
    It operator++(int);
    bool operator==(const It &);
    bool operator!=(const It &);

  private:
    ItConstructorParamType _it;
  };

  ItConstructorParamType *_graph;

public:
  Walker(GraphType *graph);
  It begin();
  It end();
};
} // namespace walker
} // namespace graph