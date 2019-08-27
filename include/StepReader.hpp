#pragma once

#include <string>

#include <TDF_LabelSequence.hxx>
#include <TDocStd_Document.hxx>
#include <STEPCAFControl_Reader.hxx>

namespace Ge {

class StepReader {
 public:
  StepReader(const std::string& file_name);
  ~StepReader();

  bool succeeded() { return succeeded_; }
  TDF_LabelSequence GetLabels() const;

 private:
  void TransferDoc(STEPCAFControl_Reader &reader);
  static constexpr const char *OCAF_MAGIC_CODE = "MDTV-XCAF";
  std::string file_name_;
  bool succeeded_ = false;

  Handle(TDocStd_Document) doc_;
};

} // namespace Ge

