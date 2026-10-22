#include "analysis_RecoEnergyStudies_outputs.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
int main(int argc,char** argv) {
  try {
    if (argc!=3) throw std::runtime_error("Usage: analysis_RecoEnergyStudies_outputs INPUT.root OUTPUT_DIRECTORY");
    std::unique_ptr<TFile> input(TFile::Open(argv[1],"READ"));
    if (!input || input->IsZombie()) throw std::runtime_error("Cannot open input ROOT file");
    auto* tree=input->Get<TTree>("recoEnergy/WC");
    if (!tree) throw std::runtime_error("Missing TTree recoEnergy/WC");
    // MakeClass generates large fixed arrays: allocate the class on the heap.
    auto analysis=std::make_unique<analysis_RecoEnergyStudies_outputs>(tree);
    analysis->outputDirectory=std::filesystem::absolute(argv[2]).string();
    // ROOT's generated class destructor owns and deletes GetCurrentFile().
    input.release();
    analysis->Loop();
    return 0;
  } catch(const std::exception& e) {std::cerr<<"ERROR: "<<e.what()<<std::endl;return 1;}
}
