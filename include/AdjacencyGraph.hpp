#pragma once

#include <Part.hpp>

namespace Ge {

class Connection {
 public:
  Connection(/* args */);
  ~Connection();

 private:
  /* data */
};

class AdjacencyGraph {
 public:
  AdjacencyGraph(const PartBuilder &part_builder);
  ~AdjacencyGraph();

 private:
  const PartBuilder &part_builder_;
};

} // namespace Ge
