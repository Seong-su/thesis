#include <AdjacencyGraph.hpp>

#include <BRepAlgoAPI_Section.hxx>
//#include <BRepAlgoAPI_Common.hxx>

#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepExtrema_ShapeProximity.hxx>
#include <BRepExtrema_MapOfIntegerPackedMapOfInteger.hxx>

namespace Ge {

Connection::Connection(const Part &left, const Part &right, double length)
    : left_{left}, right_{right}, length_{length} {

}

Connection::~Connection() {
}

std::tuple<bool, double> Connection::Connected(const Part &left, const Part &right) {
  static constexpr Standard_Real linear_deflection = 1.0e-2;
  static constexpr Standard_Real angular_deflection = 1.0e-2;
  static constexpr Standard_Real tolerance = 1.0e-1;

  bool result = false;
  double length = 0.0;

  BRepMesh_IncrementalMesh mesh1(left.Shape(), linear_deflection);
  BRepMesh_IncrementalMesh mesh2(right.Shape(), linear_deflection);
  BRepExtrema_ShapeProximity proximity(mesh1.Shape(), mesh2.Shape(), tolerance);
  proximity.Perform();
  auto overlap = proximity.OverlapSubShapes1();
  if (!overlap.IsEmpty()) {
    BRepAlgoAPI_Section builder(left.Shape(), right.Shape());
    builder.Build();
    TopoDS_Shape shape = builder.Shape();
    Part intersection(std::move(shape), "");
    intersection.Print();
    length = intersection.Length();
    result = true;
  }
  
  return std::make_tuple(result, length);
}

bool Connection::Contains(const std::string &name) const{
  return std::strcmp(left_.Name().c_str(), name.c_str())
      || std::strcmp(right_.Name().c_str(), name.c_str());
}

void Connection::Print() const {
  std::cout << left_.Name() << "\t" << right_.Name() << "\t" << length_ << std::endl;
}

AdjacencyGraph::AdjacencyGraph(const PartBuilder &part_builder) : part_builder_{part_builder} {
  const PartBuilder::PartMap &part_map = part_builder.GetPartMap();
  for (PartBuilder::PartMap::const_iterator iter1 = part_map.cbegin(); iter1 != std::prev(part_map.cend()); ++iter1) {
    const Part &part1 = iter1->second;
    for (PartBuilder::PartMap::const_iterator iter2 = std::next(iter1); iter2 != part_map.cend(); ++iter2) {
      const Part &part2 = iter2->second;
      bool is_connected;
      double length;
      std::tie(is_connected, length) = Connection::Connected(part1, part2);
      if (is_connected) {
        connection_list_.emplace_back(part1, part2, length);
      }
    }
  }
  
}

AdjacencyGraph::~AdjacencyGraph() {
}

} // namespace Ge
