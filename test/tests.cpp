#include <map>
#include <algorithm>

#include <StepReader.hpp>
#include <Part.hpp>
#include <AdjacencyGraph.hpp>

#include <gtest/gtest.h>

const std::string model_file("models/B226.step");

TEST(ImportStep, StepReader) {
  Ge::StepReader reader(model_file.c_str()); 
  EXPECT_TRUE(reader.succeeded());
}

TEST(ImportStep, PartBuilder) {
  Ge::PartBuilder builder(model_file.c_str());
  builder.PrintPartAll();
}

TEST(GetPartInfo, Area) {
  Ge::PartBuilder builder(model_file.c_str());
  // std::map<double, int> area_histogram;
  const Ge::PartBuilder::PartMap &part_map = builder.GetPartMap();
  for (const auto &pair : part_map) {
    std::cout << pair.first.name << "\t" << pair.second.Area() << std::endl;

    // if (area_histogram.find(pair.second.Area()) != area_histogram.end())
    //   ++(area_histogram.at(pair.second.Area()));
    // else
    //   area_histogram.emplace(pair.second.Area(), 1);
  }
  // int total_parts = 0;
  // std::for_each(area_histogram.begin(), area_histogram.end(), [&total_parts](const std::pair<double, int> &pair){
  //   std::cout << "Area: " << pair.first << "\t#" << pair.second << std::endl;
  //   total_parts += pair.second;
  // });
  // std::cout << "Parts#: " << total_parts << std::endl;
}

TEST(GetPartInfo, Elogation) {
  Ge::PartBuilder builder(model_file.c_str());
  const Ge::PartBuilder::PartMap &part_map = builder.GetPartMap();
  std::for_each(part_map.begin(), part_map.end(), [](const Ge::PartBuilder::PartPair &pair){
    std::cout << pair.first.name << "\t" << pair.second.Elongation() << std::endl;
  });
}

TEST(GetPartInfo, Metric) {
  Ge::PartBuilder builder(model_file.c_str());
  const Ge::PartBuilder::PartMap &part_map = builder.GetPartMap();
  std::for_each(part_map.begin(), part_map.end(), [](const Ge::PartBuilder::PartPair &pair){
    std::cout << pair.first.name << "\t" << pair.second.Metric() << std::endl;
  });
}

TEST(GetPartInfo, Steepness) {
  Ge::PartBuilder builder(model_file.c_str());
  const Ge::PartBuilder::PartMap &part_map = builder.GetPartMap();

  double prev_part_metric = part_map.begin()->second.Metric();
  std::for_each(part_map.begin(), part_map.end(), [&prev_part_metric](const Ge::PartBuilder::PartPair &pair){
    double steepness = pair.second.Metric() - prev_part_metric;
    std::cout << pair.first.name << "\t" << steepness << std::endl;
    prev_part_metric = pair.second.Metric();
  });
}

TEST(GetPartInfo, PartClassification) {
  Ge::PartBuilder builder(model_file.c_str());

  const Ge::PartBuilder::PartMap &part_map = builder.GetPartMap();
  std::for_each(part_map.begin(), part_map.end(), [](const Ge::PartBuilder::PartPair &pair){
    std::cout << pair.first.name << "\t" << pair.second.GetType() << std::endl;
  });
}

TEST(Graph, Constructor) {
  const Ge::PartBuilder builder(model_file.c_str());
  Ge::AdjacencyGraph graph(builder);
}