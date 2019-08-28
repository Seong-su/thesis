#pragma once

#include <string>
#include <map>
#include <functional>

#include <gp_Pnt.hxx>
#include <Bnd_OBB.hxx>
#include <TopoDS_Shape.hxx>

namespace Ge {

class Part {
 public:
  enum class Type {
    PLATE,
    STIFF,
  };

  Part(TopoDS_Shape &&shape, const std::string &name);
  ~Part();
  void Print() const;

  const TopoDS_Shape &Shape() const { return shape_; }
  const std::string &Name() const { return name_; }
  double Length() const { return length_; }
  double Area() const { return area_; }
  double Elongation() const { return elongation_; }
  double Metric() const { return metric_; }
  
  std::string GetType() const;
  void SetType(Type type) { type_ = type; }

 private:
  std::tuple<double, double, double> SortDimension(const Bnd_OBB &box) const;
  Bnd_OBB MakeOBB(const TopoDS_Shape &shape) const;
  std::tuple<double, gp_Pnt> MakeVolumnProperties(const TopoDS_Shape &shape) const;
  std::tuple<double, gp_Dir> MakeAreaProperties(const TopoDS_Shape &shape) const;

  TopoDS_Shape shape_;
  Type type_;

  std::string name_;

  double volume_;
  double area_;
  double length_;
  double breadth_;
  double thickness_;
  double elongation_;
  double metric_;

  Bnd_OBB box_;
  gp_Pnt center_;
  gp_Dir normal_;
};

class PartBuilder {
 public:
  struct Key {
    std::string name;
    double metric;
  };
  
  struct Comparator {
    bool operator()(const Key &a, const Key &b) const {
      if (a.metric == b.metric)
        return a.name < b.name;
      return a.metric < b.metric;
    }
  };

  using PartMap = std::map<Key, Part, Comparator>;
  using PartPair = std::pair<Key, Part>;
  
  PartBuilder(const std::string &filename);
  ~PartBuilder();
  void PrintPartAll() const;

  const PartMap &GetPartMap() const {return part_map_; }

 private:
  void ClearPartMap();
  void AddPart(Part &&part);
  void ClassifyPartType();


  PartMap part_map_;
};

} // namespace Geometry
