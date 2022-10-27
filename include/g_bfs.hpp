#pragma once

#include "g_concepts.hpp"
#include <bits/stdc++.h>

namespace graph {
namespace algorithms {
enum class VertexStatus {
  READY,
  WAITING,
  PROCESSED,
};

struct BFSVertex {
  VertexStatus status;
  int distance = NULL;
  int parent = NULL;
};

struct DFSVertex {
  VertexStatus status;
  int parent = NULL;
  int discovery_time = NULL;
  int finishing_time = NULL;
};

template <typename T> class TreeNode {
public:
  TreeNode() {}

  TreeNode(const T &_value) : value(_value) {}

  T value;
  std::vector<TreeNode<T>> children;

  void print(const int depth = 0) const {
    for (int i = 0; i < depth; ++i) {
      if (i != depth - 1)
        std::cout << "    ";
      else
        std::cout << "|-- ";
    }

    std::cout << this->value << std::endl;
    for (auto i = 0; i < children.size(); ++i) {
      this->children[i].print(depth + 1);
    }
  }
};

template <concepts::Graph G> class BFS {
  G &_graph;
  TreeNode<int> real_tree;

public:
  std::unordered_map<int, BFSVertex> bfs_tree;
  BFS(G &g) : _graph{g} {};
  void init();
  void visit(int source);
  void print_tree(int source);
  void make_tree(TreeNode<int> &tree);
};

template <concepts::Graph G> void BFS<G>::init() {
  for (auto vertex : _graph.vertices()) {
    bfs_tree[vertex.id] = BFSVertex{VertexStatus::READY};
  }
}

template <concepts::Graph G> void BFS<G>::visit(int source) {
  init();

  bfs_tree[source].status = VertexStatus::WAITING;
  bfs_tree[source].distance = 0;
  bfs_tree[source].parent = NULL;

  std::queue<int> queue;
  queue.push(source);

  while (!queue.empty()) {
    int vertex = queue.front();
    queue.pop();

    for (auto adjacent : _graph.out_edges({vertex})) {
      if (bfs_tree[adjacent].status == VertexStatus::READY) {
        bfs_tree[adjacent].status = VertexStatus::WAITING;
        bfs_tree[adjacent].distance = bfs_tree[vertex].distance + 1;
        bfs_tree[adjacent].parent = vertex;

        queue.push(adjacent);
      }
    }

    bfs_tree[vertex].status = VertexStatus::PROCESSED;
  }
}

template <concepts::Graph G> void BFS<G>::print_tree(int source) {
  TreeNode<int> root = TreeNode(source);
  make_tree(root);
  root.print();
}

template <concepts::Graph G> void BFS<G>::make_tree(TreeNode<int> &tree) {
  for (const auto &[key, vertex] : bfs_tree) {
    if (vertex.parent == tree.value && vertex.distance != 0) {
      TreeNode<int> children_tree = TreeNode<int>(key);
      tree.children.push_back(children_tree);
      make_tree(tree.children[tree.children.size() - 1]);
    }
  }
}
} // namespace algorithms
} // namespace graph