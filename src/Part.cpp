#include <Part.hpp>

#include <iostream>
#include <algorithm>
#include <tuple>
#include <cassert>

#include <StepReader.hpp>

#include <BRepBndLib.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Surface.hxx>
#include <GeomLProp_SLProps.hxx>

#include <TDataStd_Name.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TCollection_AsciiString.hxx>

namespace Ge{

Part::Part(const TopoDS_Shape &shape, const std::string &name) : shape_{shape}, name_(name) {
  box_ = MakeOBB(shape);
  std::tie(length_, breadth_, thickness_) = SortDimension(box_);
  elongation_ = breadth_ / length_;
  std::tie(volume_, center_) = MakeVolumnProperties(shape);
  std::tie(area_, normal_) = MakeAreaProperties(shape);
  metric_ = area_ * elongation_;
}

Part::~Part() {
}

std::tuple<double, double, double> Part::SortDimension(const Bnd_OBB &box) const {
  constexpr int SIZE = 3;
  double xyz[SIZE] { 2 * box.XHSize(), 2 * box.YHSize(), 2 * box.ZHSize() };
  std::sort(xyz, xyz + SIZE, std::greater<double>());
  return std::make_tuple(xyz[0], xyz[1], xyz[2]);
}

Bnd_OBB Part::MakeOBB(const TopoDS_Shape &shape) const {
  Bnd_OBB box;
  BRepBndLib bndlib;
  bndlib.AddOBB(shape, box);
  return std::move(box);
}

std::tuple<double, gp_Pnt> Part::MakeVolumnProperties(const TopoDS_Shape &shape) const {
  GProp_GProps volume_properties;
  BRepGProp::VolumePropertiesGK(shape, volume_properties);
  return std::make_tuple(volume_properties.Mass(), volume_properties.CentreOfMass());
}

std::tuple<double, gp_Dir> Part::MakeAreaProperties(const TopoDS_Shape &shape) const {
  double area = 0.0;
  gp_Dir normal;
  GProp_GProps area_properties;
  TopTools_IndexedMapOfShape face_map;
  TopExp::MapShapes(shape, TopAbs_FACE, face_map);
  for(int i = 1; i <= face_map.Extent(); ++i)
  {
    const TopoDS_Face& face = TopoDS::Face(face_map(i));
    BRepGProp::SurfaceProperties(face, area_properties);
    if (area < area_properties.Mass()) {
      area = area_properties.Mass();

      Standard_Real umin, umax, vmin, vmax;
      BRepTools::UVBounds(face,umin, umax, vmin, vmax);
      Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
      GeomLProp_SLProps props(surface, umin, vmin, 1, 0.01);
      gp_Dir normal = props.Normal();
    }
  }

  return std::make_tuple(area, normal);
}

void Part::Print() const {
  std::cout << "Part: " << name_ << "\n\t"
            << "L:" << length_ << "\t"
            << "B:" << breadth_ << "\t"
            << "T:" << thickness_ << "\t"
            << "E:" << elongation_ << "\n\t"
            << "M:" << metric_ << "\t"
            << "A:" << area_ << "\t"
            << "V:" << volume_ << std::endl;
}

std::string Part::GetType() const {
  switch (type_)
  {
  case Type::PLATE:
    return "PLATE";
    break;
  case Type::STIFF:
    return "STIFF";
    break;
  default:
    return "NONE";
    break;
  }
}

PartBuilder::PartBuilder(const std::string &filename) {
  StepReader reader(filename);
  TDF_LabelSequence labels = reader.GetLabels();
  ClearPartMap();
  
  for (int i = 1; i <= labels.Length(); i++)
	{
		const TDF_Label& lbl = labels.Value(i);

		if (!XCAFDoc_ShapeTool::IsFree(lbl))
			continue;
		TopoDS_Shape shape = XCAFDoc_ShapeTool::GetShape(lbl);
    
    TCollection_AsciiString name;
    Handle(TDataStd_Name) name_att;
    Handle_TDF_Attribute att;
    if (lbl.FindAttribute(TDataStd_Name::GetID(), att)) {
      name_att = Handle(TDataStd_Name)::DownCast(att);
      name = name_att->Get();
      Part part(shape, name.ToCString());
      AddPart(std::move(part));
    }
	}

  ClassifyPartType();
}

PartBuilder::~PartBuilder() {

}

void PartBuilder::PrintPartAll() const {
  std::for_each(part_map_.begin(), part_map_.end(), [](const PartPair &pair) {
    pair.second.Print();
  });
}

void PartBuilder::ClassifyPartType() {
  double ratio_max = 0.0;
  double prev_part_metric = part_map_.begin()->second.Metric();
  PartMap::iterator threshold_iter = part_map_.end();
  for (PartMap::iterator iter = part_map_.begin(); iter != part_map_.end(); ++iter) {
    const Part &part = iter->second;
    double steepness = part.Metric() - prev_part_metric;
    double increment_ratio = steepness / part.Metric();
    if (ratio_max < increment_ratio) {
      ratio_max = increment_ratio;
      threshold_iter = iter;
    }
    prev_part_metric = part.Metric();
  }
  
  for (PartMap::iterator iter = part_map_.begin(); iter != threshold_iter; ++iter) {
    Part &part = iter->second;
    part.SetType(Part::Type::STIFF);
  }

  for (PartMap::iterator iter = threshold_iter; iter != part_map_.end(); ++iter) {
    Part &part = iter->second;
    part.SetType(Part::Type::PLATE);
  }
}

void PartBuilder::ClearPartMap() {
  part_map_.swap(PartMap());
}

void PartBuilder::AddPart(Part &&part) {
  part_map_.emplace(Key{ part.Name(), part.Metric() }, std::move(part));
}

} // namespace Geometry
