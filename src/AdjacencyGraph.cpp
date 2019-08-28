#include <AdjacencyGraph.hpp>


namespace Ge {

Connection::Connection(const Part &left, const Part &right) : left_{left}, right_{right} {
}

Connection::~Connection() {
}

bool Connection::Contains(const std::string &name) const{
  return std::strcmp(left_.Name().c_str(), name.c_str())
      || std::strcmp(right_.Name().c_str(), name.c_str());
}

AdjacencyGraph::AdjacencyGraph(const PartBuilder &part_builder) : part_builder_{part_builder} {
}

AdjacencyGraph::~AdjacencyGraph() {
}

} // namespace Ge
