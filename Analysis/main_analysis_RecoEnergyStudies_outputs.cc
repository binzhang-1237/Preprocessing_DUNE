#include "analysis_RecoEnergyStudies_outputs.h"
#include <TChain.h>
#include <TLeaf.h>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>

namespace {
// Validate every file before binding MakeClass buffers or creating outputs.
std::map<std::string,std::string> schema(TTree& tree) {
  std::map<std::string,std::string> result;
  for(auto* obj:*tree.GetListOfLeaves()) {
    auto* leaf=dynamic_cast<TLeaf*>(obj);
    if(!leaf) continue;
    result.emplace(leaf->GetName(),std::string(leaf->GetTypeName())+"|"+
        leaf->GetTitle()+"|"+std::to_string(leaf->GetLenStatic())+"|"+leaf->GetBranch()->GetName());
  }
  return result;
}
}

int main(int argc,char** argv) {
  try {
    if(argc<3) throw std::runtime_error(
        "Usage: analysis_RecoEnergyStudies_outputs INPUT.root [INPUT2.root ...] OUTPUT_DIRECTORY");
    TChain chain("recoEnergy/WC");
    std::map<std::string,std::string> reference;
    std::set<std::filesystem::path> seen;
    Long64_t expectedEntries=0;
    for(int i=1;i<argc-1;++i) {
      const auto path=std::filesystem::canonical(argv[i]);
      if(!seen.insert(path).second) throw std::runtime_error("Duplicate input file: "+path.string());
      std::unique_ptr<TFile> input(TFile::Open(path.c_str(),"READ"));
      if(!input || input->IsZombie()) throw std::runtime_error("Cannot open input: "+path.string());
      auto* tree=input->Get<TTree>("recoEnergy/WC");
      if(!tree) throw std::runtime_error("Missing recoEnergy/WC in "+path.string());
      const auto current=schema(*tree);
      if(i==1) reference=current;
      else if(current!=reference) throw std::runtime_error("Tree schema differs in "+path.string());
      const Long64_t entries=tree->GetEntries();
      expectedEntries+=entries;
      if(chain.AddFile(path.c_str(),entries,"recoEnergy/WC")!=1)
        throw std::runtime_error("Cannot add input to TChain: "+path.string());
      std::cout<<"Input "<<i<<": "<<path<<" (entries="<<entries<<")"<<std::endl;
    }
    if(chain.GetEntries()!=expectedEntries) throw std::runtime_error("TChain entry count mismatch");
    if(expectedEntries==0) throw std::runtime_error("Input trees contain no entries");
    if(chain.LoadTree(0)<0) throw std::runtime_error("Cannot load first input tree");
    std::cout<<"Input files: "<<seen.size()<<"\nTotal entries: "<<expectedEntries<<std::endl;
    // MakeClass's arrays live on the heap; TChain owns its input files.
    auto analysis=std::make_unique<analysis_RecoEnergyStudies_outputs>(&chain);
    analysis->outputDirectory=std::filesystem::absolute(argv[argc-1]).string();
    analysis->Loop();
    return 0;
  } catch(const std::exception& e) {std::cerr<<"ERROR: "<<e.what()<<std::endl;return 1;}
}
