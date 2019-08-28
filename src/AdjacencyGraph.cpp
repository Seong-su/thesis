#include <AdjacencyGraph.hpp>

#include <TopoDS.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepBndLib.hxx>

namespace Ge {

Connection::Connection(const Part &left, const Part &right) : left_{left}, right_{right} {
  BRepAlgoAPI_Common cmn_builder(left.Shape(), right.Shape());
  cmn_builder.Check();
  if (cmn_builder.IsDone() && !cmn_builder.HasErrors() && cmn_builder.HasGenerated()) {
    Part intersection(cmn_builder.Shape(), "");
    length_ = intersection.Length();
  }
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
