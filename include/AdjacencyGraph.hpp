#pragma once

#include <string>

#include <Part.hpp>

namespace Ge {

class Connection {
 public:
  Connection(const Part &left, const Part &right);
  ~Connection();

  bool Contains(const std::string &name) const;

 private:
  const Part &left_, &right_;
};

class AdjacencyGraph {
 public:
  AdjacencyGraph(const PartBuilder &part_builder);
  ~AdjacencyGraph();

 private:
  const PartBuilder &part_builder_;
};

} // namespace Ge
