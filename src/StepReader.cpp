#include <StepReader.hpp>

#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

namespace Ge {

class FileDialog {
 public:
  FileDialog(/* args */) {}
  ~FileDialog() {}

 private:
  /* data */
};

StepReader::StepReader(const std::string& file_name) : file_name_(file_name) {
  STEPCAFControl_Reader reader;
  IFSelect_ReturnStatus stat = reader.ReadFile(file_name_.c_str());
  switch (stat)
  {
  case IFSelect_RetVoid:
  case IFSelect_RetDone:
    succeeded_ = true;
    TransferDoc(reader);
    break;
  
  default:
    succeeded_ = false;
    break;
  }
}

StepReader::~StepReader() {}

void StepReader::TransferDoc(STEPCAFControl_Reader &reader) {
  if (!doc_.IsNull())
    doc_.Nullify();
  
	doc_ = new TDocStd_Document(OCAF_MAGIC_CODE);
  reader.Transfer(doc_);
}

TDF_LabelSequence StepReader::GetLabels() const {
  TDF_Label lblMain = doc_->Main();
	Handle(XCAFDoc_ShapeTool) myAssembly = XCAFDoc_DocumentTool::ShapeTool(lblMain);
  TDF_LabelSequence labels;
	myAssembly->GetShapes(labels);
  return std::move(labels);
}

} // namespace Ge
