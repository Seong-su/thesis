#pragma once

#include <string>
#include <list>
#include <tuple>

#include <Part.hpp>

namespace Ge {

class Connection {
 public:
  Connection(const Part &left, const Part &right, double length);
  ~Connection();
  static std::tuple<bool, double> Connected(const Part &left, const Part &right);
  bool Contains(const std::string &name) const;
  void Print() const;

  double Length() const { return length_; }
  const Part &Left() const { return left_; }
  const Part &Right() const { return right_; }

 private:
  double length_;
  const Part &left_, &right_;
};

class AdjacencyGraph {
 public:
  using ConnectionList = std::list<Connection>;

  AdjacencyGraph(const PartBuilder &part_builder);
  ~AdjacencyGraph();

  const ConnectionList &GetConnectionList() const { return connection_list_; }

 private:
  const PartBuilder &part_builder_;
  ConnectionList connection_list_;
};

} // namespace Ge
