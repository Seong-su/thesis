#pragma once

#include <string>
#include <set>
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

 private:
  double length_;
  const Part &left_, &right_;
};

class AdjacencyGraph {
 public:
  struct Comparator {
    bool operator() (const Connection &lhs, const Connection &rhs) const {
        return lhs.Length() < rhs.Length();
    }
  };

  using ConnectionSet = std::set<Connection, Comparator>;

  AdjacencyGraph(const PartBuilder &part_builder);
  ~AdjacencyGraph();

  const ConnectionSet &GetConnectionSet() const { return connection_set_; }

 private:
  const PartBuilder &part_builder_;
  ConnectionSet connection_set_;
};

} // namespace Ge
