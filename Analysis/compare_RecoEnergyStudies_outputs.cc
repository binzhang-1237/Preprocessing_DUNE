// Overlay corresponding 2018/2026 histograms without normalization or rebinning.
#include <TCanvas.h>
#include <TClass.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TKey.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TROOT.h>
#include <TStyle.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>

namespace {
std::unique_ptr<TFile> openInput(const char* path) {
  std::unique_ptr<TFile> f(TFile::Open(path,"READ"));
  if(!f || f->IsZombie()) throw std::runtime_error(std::string("Cannot open ")+path);
  return f;
}
std::set<std::string> histogramNames(TFile& f) {
  std::set<std::string> names;
  for(auto* obj:*f.GetListOfKeys()) {
    auto* key=dynamic_cast<TKey*>(obj);
    auto* cls=TClass::GetClass(key->GetClassName());
    if(cls && cls->InheritsFrom(TH1::Class())) names.insert(key->GetName());
  }
  return names;
}
bool sameBinning(const TH1& a,const TH1& b) {
  if(a.GetNbinsX()!=b.GetNbinsX()) return false;
  for(int i=1;i<=a.GetNbinsX()+1;++i)
    if(a.GetXaxis()->GetBinLowEdge(i)!=b.GetXaxis()->GetBinLowEdge(i)) return false;
  return true;
}
std::string imageName(std::string name) {
  for(char& ch:name)
    if(!((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch>='0'&&ch<='9')||ch=='_'||ch=='-')) ch='_';
  return name;
}
}
int main(int argc,char** argv) {
  try {
    if(argc!=4) throw std::runtime_error("Usage: compare_RecoEnergyStudies_outputs 2018.root 2026.root OUTPUT_DIRECTORY");
    gROOT->SetBatch(kTRUE);
    TH1::AddDirectory(kFALSE);
    gStyle->SetOptStat(0);
    gStyle->SetPaperSize(27.94,19.05);
    auto f18=openInput(argv[1]);
    auto f26=openInput(argv[2]);
    const auto names=histogramNames(*f18);
    const auto names26=histogramNames(*f26);
    if(names.empty()) throw std::runtime_error("No histograms in 2018 file");
    if(names!=names26) {
      for(const auto& n:names) if(!names26.count(n)) std::cerr<<"Missing in 2026: "<<n<<'\n';
      for(const auto& n:names26) if(!names.count(n)) std::cerr<<"Missing in 2018: "<<n<<'\n';
      throw std::runtime_error("Histogram names do not match");
    }
    // Validate before creating any output files.
    for(const auto& n:names) {
      if(f18->Get<TH1>(n.c_str())->GetDimension()!=1 || f26->Get<TH1>(n.c_str())->GetDimension()!=1)
        throw std::runtime_error("Expected one-dimensional histograms: "+n);
    }
    const auto outDir=std::filesystem::absolute(argv[3]);
    std::filesystem::create_directories(outDir/"plots");
    const auto pdf=outDir/"RecoEnergyStudies_2018_vs_2026.pdf";
    TFile output((outDir/"RecoEnergyStudies_2018_vs_2026.root").c_str(),"RECREATE");
    if(output.IsZombie()) throw std::runtime_error("Cannot create output ROOT file");
    auto* originals18=output.mkdir("histograms_2018");
    auto* originals26=output.mkdir("histograms_2026");
    auto* canvases=output.mkdir("canvases");
    std::ofstream summary(outDir/"comparison_summary.csv");
    if(!summary) throw std::runtime_error("Cannot create comparison_summary.csv");
    summary<<"histogram,entries_2018,entries_2026,integral_2018,integral_2026,same_binning,xmin_2018,xmax_2018,xmin_2026,xmax_2026\n";
    summary.precision(17);
    TCanvas canvas("comparison_canvas","2018 vs 2026",1100,750);
    canvas.SetCanvasSize(1100,750);
    canvas.SetLeftMargin(0.12); canvas.SetBottomMargin(0.12); canvas.SetTopMargin(0.12);
    canvas.Modified(); canvas.Update();
    canvas.Print((pdf.string()+"[").c_str());
    int completed=0,different=0;
    for(const auto& name:names) {
      // Clone to leave the input files and their stored histogram contents untouched.
      std::unique_ptr<TH1> h18(static_cast<TH1*>(f18->Get<TH1>(name.c_str())->Clone()));
      std::unique_ptr<TH1> h26(static_cast<TH1*>(f26->Get<TH1>(name.c_str())->Clone()));
      const bool same=sameBinning(*h18,*h26);
      if(!same) ++different;
      originals18->cd(); h18->Write(name.c_str());
      originals26->cd(); h26->Write(name.c_str());
      const double xmin=std::min(h18->GetXaxis()->GetXmin(),h26->GetXaxis()->GetXmin());
      const double xmax=std::max(h18->GetXaxis()->GetXmax(),h26->GetXaxis()->GetXmax());
      const double ymax=std::max(h18->GetMaximum(),h26->GetMaximum());
      std::string xlabel=h18->GetXaxis()->GetTitle();
      if(xlabel.empty()) xlabel=name;
      TH1D frame("comparison_axis",(name+";"+xlabel+";Counts / original bin").c_str(),1,xmin,xmax);
      frame.SetStats(false); frame.SetMinimum(0); frame.SetMaximum(ymax>0?ymax*1.30:1);
      h18->SetStats(false); h26->SetStats(false);
      h18->SetLineColor(kBlue+1); h18->SetLineStyle(1); h18->SetLineWidth(2); h18->SetFillStyle(0);
      h26->SetLineColor(kRed+1); h26->SetLineStyle(2); h26->SetLineWidth(2); h26->SetFillStyle(0);
      canvas.cd(); frame.Draw("AXIS");
      h18->Draw("HIST SAME"); h26->Draw("HIST SAME");
      TLegend legend(0.57,0.74,0.88,0.87);
      legend.SetBorderSize(0); legend.SetFillStyle(0);
      const auto label18="2018 (entries = "+std::to_string(static_cast<long long>(h18->GetEntries()))+")";
      const auto label26="2026 (entries = "+std::to_string(static_cast<long long>(h26->GetEntries()))+")";
      legend.AddEntry(h18.get(),label18.c_str(),"l");
      legend.AddEntry(h26.get(),label26.c_str(),"l");
      legend.Draw();
      TLatex note; note.SetNDC(); note.SetTextSize(0.026);
      if(!same) note.DrawLatex(0.12,0.91,"Original binning differs; no rebinning or normalization");
      if(h18->Integral()==0 && h26->Integral()==0) note.DrawLatex(0.16,0.64,"Both histograms are empty");
      canvas.RedrawAxis(); canvas.Modified(); canvas.Update();
      canvas.Print(pdf.c_str(),"pdf");
      canvas.Print((outDir/"plots"/(imageName(name)+".png")).c_str(),"png");
      canvases->cd(); canvas.Write(name.c_str());
      summary<<name<<','<<h18->GetEntries()<<','<<h26->GetEntries()<<','<<h18->Integral()<<','<<h26->Integral()<<','<<(same?"true":"false")<<','
             <<h18->GetXaxis()->GetXmin()<<','<<h18->GetXaxis()->GetXmax()<<','<<h26->GetXaxis()->GetXmin()<<','<<h26->GetXaxis()->GetXmax()<<'\n';
      canvas.Clear();
      if(++completed%25==0) std::cout<<"Compared "<<completed<<" / "<<names.size()<<std::endl;
    }
    canvas.Print((pdf.string()+"]").c_str());
    output.Close();
    std::cout<<"Compared "<<completed<<" histogram pairs; "<<different<<" pairs have different original binning.\n"
             <<"No scaling, normalization, rebinning or ratio calculation.\nOutput: "<<outDir<<std::endl;
    return 0;
  } catch(const std::exception& e) {std::cerr<<"ERROR: "<<e.what()<<std::endl;return 1;}
}
