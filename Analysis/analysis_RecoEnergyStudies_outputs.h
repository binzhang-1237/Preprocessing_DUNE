//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Oct 21 16:56:56 2026 by ROOT version 6.28/12
// from TTree WC/WC
// found on file: /home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/RecoEnergyS.root
//////////////////////////////////////////////////////////

#ifndef analysis_RecoEnergyStudies_outputs_h
#define analysis_RecoEnergyStudies_outputs_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"

class analysis_RecoEnergyStudies_outputs {
public :
   // Match the producer buffers in RecoEnergyStudies/RecoEnergyS_module.cc.
   // These are format capacities, not maxima inferred from one input sample.
   static constexpr Int_t kMaxHits = 50000;
   static constexpr Int_t kMax = 3000;
   static constexpr Int_t kMaxClst = 100;

   // User addition: output directory used by Loop().
   std::string outputDirectory = "outputs";
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           ievent;
   Double_t        det_t0;
   Double_t        driftvelocity;
   Double_t        SampleRate;
   Int_t           NTPCs_evt;
   Int_t           NWires_P0;
   Int_t           NWires_P1;
   Int_t           NWires_P2;
   Int_t           NuTruthFid;
   Int_t           NuTruthN;
   Double_t        NuEngTruth[kMax];   //[NuTruthN]
   Int_t           NuPDGTruth[kMax];   //[NuTruthN]
   Double_t        NuVtxXTruth[kMax];   //[NuTruthN]
   Double_t        NuVtxYTruth[kMax];   //[NuTruthN]
   Double_t        NuVtxZTruth[kMax];   //[NuTruthN]
   Int_t           NuModeTruth[kMax];   //[NuTruthN]
   Int_t           NuCCNCTruth[kMax];   //[NuTruthN]
   Int_t           LepPDGTruth[kMax];   //[NuTruthN]
   Double_t        LepP0Truth[kMax];   //[NuTruthN]
   Double_t        LepP1Truth[kMax];   //[NuTruthN]
   Double_t        LepP2Truth[kMax];   //[NuTruthN]
   Double_t        LepEngTruth[kMax];   //[NuTruthN]
   Int_t           MuTrackCont;
   Int_t           PrimPDG;
   Double_t        TrueEnergy;
   Double_t        TrueVertexX;
   Double_t        TrueVertexY;
   Double_t        TrueVertexZ;
   Double_t        TrueEndX;
   Double_t        TrueEndY;
   Double_t        TrueEndZ;
   Double_t        TruePx;
   Double_t        TruePy;
   Double_t        TruePz;
   Int_t           TrueVtxin;
   Double_t        TrueVtxWire[3];
   Double_t        TrueVtxGlobalWire[3];
   Double_t        TrueVtxGlobalTick[3];
   Double_t        TrueVtxGlobalPlane[3];
   Double_t        TrueVtxTPC[3];
   Double_t        TrueVtxTime[3];
   Double_t        TrueVtxChan[3];
   Double_t        TrueEndWire[3];
   Double_t        TrueEndTime[3];
   Double_t        TrueMidWire[3];
   Double_t        TrueMidTime[3];
   Double_t        EndProngGlobalWire[3];
   Double_t        EndProngGlobalTick[3];
   Double_t        EndProngGlobalPlane[3];
   Int_t           EvthasPriPF;
   Double_t        RecoVertexX;
   Double_t        RecoVertexY;
   Double_t        RecoVertexZ;
   Double_t        RecoVtxWire[3];
   Double_t        RecoVtxTime[3];
   Double_t        DepositU;
   Double_t        DepositV;
   Double_t        DepositZ;
   Double_t        ChargeU;
   Double_t        ChargeV;
   Double_t        ChargeZ;
   Double_t        CorrectedChargeU;
   Double_t        CorrectedChargeV;
   Double_t        CorrectedChargeZ;
   Double_t        VertexDetectorDist;
   Double_t        EnergyU;
   Double_t        EnergyV;
   Double_t        EnergyZ;
   Double_t        CorrectedEnergyU;
   Double_t        CorrectedEnergyV;
   Double_t        CorrectedEnergyZ;
   Double_t        ErecoNu;
   Double_t        RecoLepEnNu;
   Double_t        RecoHadEnNu;
   Int_t           RecoMethodNu;
   Int_t           NHits;
   Int_t           NTPCs;
   Int_t           Hit_Cryst[kMaxHits];   //[NHits]
   Int_t           Hit_TPC[kMaxHits];   //[NHits]
   Int_t           Hit_Plane[kMaxHits];   //[NHits]
   Int_t           Hit_Wire[kMaxHits];   //[NHits]
   Int_t           Hit_Channel[kMaxHits];   //[NHits]
   Int_t           Hit_Multi[kMaxHits];   //[NHits]
   Float_t         Hit_PeakT[kMaxHits];   //[NHits]
   Float_t         Hit_Amp[kMaxHits];   //[NHits]
   Float_t         Hit_MinusT[kMaxHits];   //[NHits]
   Float_t         Hit_PlusT[kMaxHits];   //[NHits]
   Float_t         Hit_RMST[kMaxHits];   //[NHits]
   Float_t         Hit_StartT[kMaxHits];   //[NHits]
   Float_t         Hit_EndT[kMaxHits];   //[NHits]
   Float_t         Hit_Charge[kMaxHits];   //[NHits]
   Float_t         Hit_CorrCharge[kMaxHits];   //[NHits]
   Double_t        Hit_Energy[kMaxHits];   //[NHits]
   Int_t           Hit_TrueTrackID[kMaxHits];   //[NHits]
   Double_t        Hit_X[kMaxHits];   //[NHits]
   Double_t        Hit_Y[kMaxHits];   //[NHits]
   Double_t        Hit_Z[kMaxHits];   //[NHits]
   Double_t        Hit_Global_Wire[kMaxHits];   //[NHits]
   Double_t        Hit_Global_Tick[kMaxHits];   //[NHits]
   Double_t        Hit_Global_Plane[kMaxHits];   //[NHits]
   Float_t         Hit_Good[kMaxHits];   //[NHits]
   Int_t           Hit_Ndf[kMaxHits];   //[NHits]
   Int_t           Hit_Prong_Tag[kMaxHits];   //[NHits]
   Int_t           Hit_Prong_PDG[kMaxHits];   //[NHits]
   Int_t           Hit_Prong_PDG_Mom[kMaxHits];   //[NHits]
   Int_t           Hit_ShwID[kMaxHits];   //[NHits]
   Int_t           Hit_LedShwID[kMaxHits];   //[NHits]
   Double_t        Hit_ShwDirX[kMaxHits];   //[NHits]
   Double_t        Hit_ShwDirY[kMaxHits];   //[NHits]
   Double_t        Hit_ShwDirZ[kMaxHits];   //[NHits]
   Double_t        Hit_ShwCoth[kMaxHits];   //[NHits]
   Double_t        Hit_ShwPhi[kMaxHits];   //[NHits]
   Int_t           Hit_ShwStartT[kMaxHits];   //[NHits]
   Int_t           Hit_ShwEndT[kMaxHits];   //[NHits]
   Double_t        Hit_ShwEvtEng[kMaxHits];   //[NHits]
   Double_t        Hit_ShwEvtdEdx[kMaxHits];   //[NHits]
   Int_t           Hit_ntrackID[kMaxHits];   //[NHits]
   Double_t        Hit_trueE[kMaxHits];   //[NHits]
   Int_t           Hit_nID_Pri[kMaxHits];   //[NHits]
   Int_t           Hit_PDG[kMaxHits];   //[NHits]
   Int_t           Hit_Mo_PDG[kMaxHits];   //[NHits]
   Int_t           Hit_mu_flag[kMaxHits];   //[NHits]
   Int_t           Hit_ele_flag[kMaxHits];   //[NHits]
   Double_t        Hit_offset[3];
   Int_t           n_tracks_pad;
   Double_t        all_track_length[kMax];   //[n_tracks_pad]
   Double_t        all_track_mom[kMax];   //[n_tracks_pad]
   Double_t        all_track_calmom[kMax];   //[n_tracks_pad]
   Double_t        all_track_startx[kMax];   //[n_tracks_pad]
   Double_t        all_track_starty[kMax];   //[n_tracks_pad]
   Double_t        all_track_startz[kMax];   //[n_tracks_pad]
   Double_t        all_track_px[kMax];   //[n_tracks_pad]
   Double_t        all_track_py[kMax];   //[n_tracks_pad]
   Double_t        all_track_pz[kMax];   //[n_tracks_pad]
   Double_t        all_track_true_Efrac[kMax];   //[n_tracks_pad]
   Int_t           all_track_true_pdg[kMax];   //[n_tracks_pad]
   Int_t           all_track_true_pdg_mom[kMax];   //[n_tracks_pad]
   Double_t        all_track_dist_reco_vtx[kMax];   //[n_tracks_pad]
   Double_t        all_track_dist_true_vtx[kMax];   //[n_tracks_pad]
   Float_t         all_track_min_angle[kMax];   //[n_tracks_pad]
   Double_t        all_track_true_eng[kMax];   //[n_tracks_pad]
   Double_t        all_track_true_px[kMax];   //[n_tracks_pad]
   Double_t        all_track_true_py[kMax];   //[n_tracks_pad]
   Double_t        all_track_true_pz[kMax];   //[n_tracks_pad]
   Double_t        all_track_true_mom_px[kMax];   //[n_tracks_pad]
   Double_t        all_track_true_mom_py[kMax];   //[n_tracks_pad]
   Double_t        all_track_true_mom_pz[kMax];   //[n_tracks_pad]
   Double_t        all_track2_true_Efrac[kMax];   //[n_tracks_pad]
   Int_t           all_track2_true_pdg[kMax];   //[n_tracks_pad]
   Int_t           all_track2_true_pdg_mom[kMax];   //[n_tracks_pad]
   Double_t        all_track2_true_eng[kMax];   //[n_tracks_pad]
   Double_t        all_track2_true_px[kMax];   //[n_tracks_pad]
   Double_t        all_track2_true_py[kMax];   //[n_tracks_pad]
   Double_t        all_track2_true_pz[kMax];   //[n_tracks_pad]
   Double_t        all_track2_true_mom_px[kMax];   //[n_tracks_pad]
   Double_t        all_track2_true_mom_py[kMax];   //[n_tracks_pad]
   Double_t        all_track2_true_mom_pz[kMax];   //[n_tracks_pad]
   Int_t           n_showers;
   Float_t         shw_tot_chg;
   Double_t        shw_chg[kMax];   //[n_showers]
   Int_t           shw_nhits[kMax];   //[n_showers]
   Int_t           all_shw_idx[kMax];   //[n_showers]
   Double_t        all_shw_eng[kMax];   //[n_showers]
   Double_t        all_shw_dedx[kMax];   //[n_showers]
   Float_t         all_shw_length[kMax];   //[n_showers]
   Double_t        all_shw_startx[kMax];   //[n_showers]
   Double_t        all_shw_starty[kMax];   //[n_showers]
   Double_t        all_shw_startz[kMax];   //[n_showers]
   Double_t        all_shw_px[kMax];   //[n_showers]
   Double_t        all_shw_py[kMax];   //[n_showers]
   Double_t        all_shw_pz[kMax];   //[n_showers]
   Double_t        all_shw_true_eng[kMax];   //[n_showers]
   Double_t        all_shw_true_px[kMax];   //[n_showers]
   Double_t        all_shw_true_py[kMax];   //[n_showers]
   Double_t        all_shw_true_pz[kMax];   //[n_showers]
   Double_t        all_shw_true_mom_px[kMax];   //[n_showers]
   Double_t        all_shw_true_mom_py[kMax];   //[n_showers]
   Double_t        all_shw_true_mom_pz[kMax];   //[n_showers]
   Double_t        all_shw_costh[kMax];   //[n_showers]
   Double_t        all_shw_phi[kMax];   //[n_showers]
   Double_t        all_shw_true_Efrac[kMax];   //[n_showers]
   Int_t           all_shw_true_pdg[kMax];   //[n_showers]
   Int_t           all_shw_true_pdg_mom[kMax];   //[n_showers]
   Double_t        all_shw_dist_reco_vtx[kMax];   //[n_showers]
   Double_t        all_shw_dist_true_vtx[kMax];   //[n_showers]
   Float_t         all_shw_min_angle[kMax];   //[n_showers]
   Double_t        all_shw2_true_Efrac[kMax];   //[n_showers]
   Int_t           all_shw2_true_pdg[kMax];   //[n_showers]
   Int_t           all_shw2_true_pdg_mom[kMax];   //[n_showers]
   Double_t        all_shw2_true_eng[kMax];   //[n_showers]
   Double_t        all_shw2_true_px[kMax];   //[n_showers]
   Double_t        all_shw2_true_py[kMax];   //[n_showers]
   Double_t        all_shw2_true_pz[kMax];   //[n_showers]
   Double_t        all_shw2_true_mom_px[kMax];   //[n_showers]
   Double_t        all_shw2_true_mom_py[kMax];   //[n_showers]
   Double_t        all_shw2_true_mom_pz[kMax];   //[n_showers]
   Int_t           led_shw_idx;
   Double_t        led_shw_eng;
   Double_t        led_shw_dirx;
   Double_t        led_shw_diry;
   Double_t        led_shw_dirz;
   Double_t        led_shw_startx;
   Double_t        led_shw_starty;
   Double_t        led_shw_startz;
   Double_t        led_shw_costh;
   Double_t        ShwVtxTPC[3];
   Double_t        ShwVtxWire[3];
   Double_t        ShwVtxTime[3];
   Double_t        ShwVtxGlobalWire[3];
   Double_t        ShwVtxGlobalTick[3];
   Double_t        ShwVtxGlobalPlane[3];
   Double_t        led_shw_trueP0;
   Double_t        led_shw_trueP1;
   Double_t        led_shw_trueP2;
   Double_t        led_shw_trueP3;
   Double_t        led_shw_truedang;
   Int_t           led_shw_truePDG;
   Int_t           led_shw_truePDGMom;
   Double_t        led_shw_truecosth;
   Float_t         ShwFirstWire;
   Float_t         ShwFirstTime;
   Int_t           n_clusters;
   Int_t           Clst_Lead_Idx;
   Int_t           Clst_Plane[kMax];   //[n_clusters]
   Float_t         Clst_Integral[kMax];   //[n_clusters]
   Float_t         Clst_SumADC[kMax];   //[n_clusters]
   Float_t         Clst_Width[kMax];   //[n_clusters]
   Int_t           Clst_Nhits[kMax];   //[n_clusters]
   Float_t         Clst_StartWire[kMax];   //[n_clusters]
   Float_t         Clst_StartTick[kMax];   //[n_clusters]
   Float_t         Clst_EndWire[kMax];   //[n_clusters]
   Float_t         Clst_EndTick[kMax];   //[n_clusters]
   Double_t        Shw_ChargeU;
   Double_t        Shw_ChargeV;
   Double_t        Shw_ChargeZ;
   Double_t        Shw_CorrectedChargeU;
   Double_t        Shw_CorrectedChargeV;
   Double_t        Shw_CorrectedChargeZ;
   Double_t        Shw_EnergyU;
   Double_t        Shw_EnergyV;
   Double_t        Shw_EnergyZ;
   Double_t        Shw_CorrectedEnergyU;
   Double_t        Shw_CorrectedEnergyV;
   Double_t        Shw_CorrectedEnergyZ;
   Int_t           NHits_Shw;
   Int_t           Shw_Idx[kMaxHits];   //[NHits_Shw]
   Double_t        Shw_StartX[kMaxHits];   //[NHits_Shw]
   Double_t        Shw_StartY[kMaxHits];   //[NHits_Shw]
   Double_t        Shw_StartZ[kMaxHits];   //[NHits_Shw]
   Double_t        Shw_DirX[kMaxHits];   //[NHits_Shw]
   Double_t        Shw_DirY[kMaxHits];   //[NHits_Shw]
   Double_t        Shw_DirZ[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_TPC[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_Plane[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_Wire[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_Channel[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_Multi[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_TrueTrackID[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_ntrackID[kMaxHits];   //[NHits_Shw]
   Float_t         Hit_Shw_PeakT[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_StartT[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_EndT[kMaxHits];   //[NHits_Shw]
   Double_t        Hit_Shw_Charge[kMaxHits];   //[NHits_Shw]
   Double_t        Hit_Shw_CorrCharge[kMaxHits];   //[NHits_Shw]
   Double_t        Hit_Shw_Energy[kMaxHits];   //[NHits_Shw]
   Double_t        Hit_Shw_trueE[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_PDG[kMaxHits];   //[NHits_Shw]
   Int_t           Hit_Shw_PDG_Mom[kMaxHits];   //[NHits_Shw]
   Double_t        Hit_Shw_Global_Plane[kMaxHits];   //[NHits_Shw]
   Double_t        Hit_Shw_Global_Wire[kMaxHits];   //[NHits_Shw]
   Double_t        Hit_Shw_Global_Tick[kMaxHits];   //[NHits_Shw]
   Int_t           n_wires;
   Int_t           Hit_Nallraws[kMaxHits];   //[n_wires]
   Int_t           Hit_Nadcvec[kMaxHits];   //[n_wires]
   Int_t           Hit_raw_wireID[kMaxHits];   //[n_wires]
   Int_t           Hit_raw_chID[kMaxHits];   //[n_wires]
   std::vector<std::vector<int> > *Hit_rawadc;
   std::vector<std::vector<int> > *Hit_rawtick;
   Int_t           Wire_assn[kMaxHits];   //[n_wires]
   Int_t           Wire_min_tick[kMaxHits];   //[n_wires]
   Int_t           Wire_max_tick[kMaxHits];   //[n_wires]
   Float_t         Wire_max_rmsT[kMaxHits];   //[n_wires]
   Int_t           Wire_min_shwtick[kMaxHits][kMaxClst];   //[n_wires]
   Int_t           Wire_max_shwtick[kMaxHits][kMaxClst];   //[n_wires]
   Int_t           Wire_min_trktick[kMaxHits][kMaxClst];   //[n_wires]
   Int_t           Wire_max_trktick[kMaxHits][kMaxClst];   //[n_wires]
   Int_t           Wire_max_shwrms[kMaxHits][kMaxClst];   //[n_wires]
   Int_t           Wire_max_trkrms[kMaxHits][kMaxClst];   //[n_wires]
   Int_t           Wire_min_ledshwtick[kMaxHits];   //[n_wires]
   Int_t           Wire_max_ledshwtick[kMaxHits];   //[n_wires]
   std::vector<std::vector<double> > *Wire_adc;
   std::vector<std::vector<double> > *Wire_corradc;
   std::vector<std::vector<int> > *Wire_tick;
   std::vector<std::vector<int> > *Wire_shwflag;
   std::vector<std::vector<int> > *Wire_ledshwflag;
   std::vector<std::vector<double> > *Wire_x;
   std::vector<std::vector<int> > *Wire_pdg;
   std::vector<std::vector<int> > *Wire_pdg_mom;
   std::vector<std::vector<double> > *Wire_dr;
   std::vector<std::vector<double> > *Wire_dist;
   std::vector<std::vector<int> > *Wire_tag_pdg;
   std::vector<std::vector<int> > *Wire_tag_pdg_mom;
   std::vector<std::vector<int> > *Wire_tag_is_shw;
   std::vector<std::vector<int> > *Wire_tag_ith;
   std::vector<std::vector<double> > *Wire_tag_true_px;
   std::vector<std::vector<double> > *Wire_tag_true_py;
   std::vector<std::vector<double> > *Wire_tag_true_pz;
   std::vector<std::vector<double> > *Wire_tag_reco_px;
   std::vector<std::vector<double> > *Wire_tag_reco_py;
   std::vector<std::vector<double> > *Wire_tag_reco_pz;
   std::vector<std::vector<double> > *Wire_tag_reco_eng;
   std::vector<std::vector<double> > *Ch_charge;
   std::vector<std::vector<int> > *Ch_tick;
   Int_t           Wire_ROIfirst[kMaxHits];   //[n_wires]
   Int_t           Wire_ROIend[kMaxHits];   //[n_wires]
   Double_t        Sum_chchg[kMaxHits];   //[n_wires]
   Int_t           TrueParticle_size;
   std::vector<int>     *TruePart_PDG;
   std::vector<double>  *TruePart_E;
   std::vector<double>  *ChgU;
   std::vector<double>  *ChgV;
   std::vector<double>  *ChgZ;
   Int_t           n_tracks_pm;
   Int_t           n_pand_par;
   Int_t           n_prims;
   Double_t        m_pf_vtx;
   Double_t        m_pf_vtx_x;
   Double_t        m_pf_vtx_y;
   Double_t        m_pf_vtx_z;
   Double_t        PFVtxTPC[3];
   Double_t        PFVtxWire[3];
   Double_t        PFVtxTime[3];
   Double_t        PFVtxGlobalWire[3];
   Double_t        PFVtxGlobalTick[3];
   Double_t        PFVtxGlobalPlane[3];

   // List of branches
   TBranch        *b_ievent;   //!
   TBranch        *b_det_t0;   //!
   TBranch        *b_driftvelocity;   //!
   TBranch        *b_SampleRate;   //!
   TBranch        *b_NTPCs_evt;   //!
   TBranch        *b_NWires_P0;   //!
   TBranch        *b_NWires_P1;   //!
   TBranch        *b_NWires_P2;   //!
   TBranch        *b_NuTruthFid;   //!
   TBranch        *b_NuTruthN;   //!
   TBranch        *b_NuEngTruth;   //!
   TBranch        *b_NuPDGTruth;   //!
   TBranch        *b_NuVtxXTruth;   //!
   TBranch        *b_NuVtxYTruth;   //!
   TBranch        *b_NuVtxZTruth;   //!
   TBranch        *b_NuModeTruth;   //!
   TBranch        *b_NuCCNCTruth;   //!
   TBranch        *b_LepPDGTruth;   //!
   TBranch        *b_LepP0Truth;   //!
   TBranch        *b_LepP1Truth;   //!
   TBranch        *b_LepP2Truth;   //!
   TBranch        *b_LepEngTruth;   //!
   TBranch        *b_MuTrackCont;   //!
   TBranch        *b_PrimPDG;   //!
   TBranch        *b_TrueEnergy;   //!
   TBranch        *b_TrueVertexX;   //!
   TBranch        *b_TrueVertexY;   //!
   TBranch        *b_TrueVertexZ;   //!
   TBranch        *b_TrueEndX;   //!
   TBranch        *b_TrueEndY;   //!
   TBranch        *b_TrueEndZ;   //!
   TBranch        *b_TruePx;   //!
   TBranch        *b_TruePy;   //!
   TBranch        *b_TruePz;   //!
   TBranch        *b_TrueVtxin;   //!
   TBranch        *b_TrueVtxWire;   //!
   TBranch        *b_TrueVtxGlobalWire;   //!
   TBranch        *b_TrueVtxGlobalTick;   //!
   TBranch        *b_TrueVtxGlobalPlane;   //!
   TBranch        *b_TrueVtxTPC;   //!
   TBranch        *b_TrueVtxTime;   //!
   TBranch        *b_TrueVtxChan;   //!
   TBranch        *b_TrueEndWire;   //!
   TBranch        *b_TrueEndTime;   //!
   TBranch        *b_TrueMidWire;   //!
   TBranch        *b_TrueMidTime;   //!
   TBranch        *b_EndProngGlobalWire;   //!
   TBranch        *b_EndProngGlobalTick;   //!
   TBranch        *b_EndProngGlobalPlane;   //!
   TBranch        *b_EvthasPriPF;   //!
   TBranch        *b_RecoVertexX;   //!
   TBranch        *b_RecoVertexY;   //!
   TBranch        *b_RecoVertexZ;   //!
   TBranch        *b_RecoVtxWire;   //!
   TBranch        *b_RecoVtxTime;   //!
   TBranch        *b_DepositU;   //!
   TBranch        *b_DepositV;   //!
   TBranch        *b_DepositZ;   //!
   TBranch        *b_ChargeU;   //!
   TBranch        *b_ChargeV;   //!
   TBranch        *b_ChargeZ;   //!
   TBranch        *b_CorrectedChargeU;   //!
   TBranch        *b_CorrectedChargeV;   //!
   TBranch        *b_CorrectedChargeZ;   //!
   TBranch        *b_VertexDetectorDist;   //!
   TBranch        *b_EnergyU;   //!
   TBranch        *b_EnergyV;   //!
   TBranch        *b_EnergyZ;   //!
   TBranch        *b_CorrectedEnergyU;   //!
   TBranch        *b_CorrectedEnergyV;   //!
   TBranch        *b_CorrectedEnergyZ;   //!
   TBranch        *b_ErecoNu;   //!
   TBranch        *b_RecoLepEnNu;   //!
   TBranch        *b_RecoHadEnNu;   //!
   TBranch        *b_RecoMethodNu;   //!
   TBranch        *b_NHits;   //!
   TBranch        *b_NTPCs;   //!
   TBranch        *b_Hit_Cryst;   //!
   TBranch        *b_Hit_TPC;   //!
   TBranch        *b_Hit_Plane;   //!
   TBranch        *b_Hit_Wire;   //!
   TBranch        *b_Hit_Channel;   //!
   TBranch        *b_Hit_Multi;   //!
   TBranch        *b_Hit_PeakT;   //!
   TBranch        *b_Hit_Amp;   //!
   TBranch        *b_Hit_MinusT;   //!
   TBranch        *b_Hit_PlusT;   //!
   TBranch        *b_Hit_RMST;   //!
   TBranch        *b_Hit_StartT;   //!
   TBranch        *b_Hit_EndT;   //!
   TBranch        *b_Hit_Charge;   //!
   TBranch        *b_Hit_CorrCharge;   //!
   TBranch        *b_Hit_Energy;   //!
   TBranch        *b_Hit_TrueTrackID;   //!
   TBranch        *b_Hit_X;   //!
   TBranch        *b_Hit_Y;   //!
   TBranch        *b_Hit_Z;   //!
   TBranch        *b_Hit_Global_Wire;   //!
   TBranch        *b_Hit_Global_Tick;   //!
   TBranch        *b_Hit_Global_Plane;   //!
   TBranch        *b_Hit_Good;   //!
   TBranch        *b_Hit_Ndf;   //!
   TBranch        *b_Hit_Prong_Tag;   //!
   TBranch        *b_Hit_Prong_PDG;   //!
   TBranch        *b_Hit_Prong_PDG_Mom;   //!
   TBranch        *b_Hit_ShwID;   //!
   TBranch        *b_Hit_LedShwID;   //!
   TBranch        *b_Hit_ShwDirX;   //!
   TBranch        *b_Hit_ShwDirY;   //!
   TBranch        *b_Hit_ShwDirZ;   //!
   TBranch        *b_Hit_ShwCoth;   //!
   TBranch        *b_Hit_ShwPhi;   //!
   TBranch        *b_Hit_ShwStartT;   //!
   TBranch        *b_Hit_ShwEndT;   //!
   TBranch        *b_Hit_ShwEvtEng;   //!
   TBranch        *b_Hit_ShwEvtdEdx;   //!
   TBranch        *b_Hit_ntrackID;   //!
   TBranch        *b_Hit_trueE;   //!
   TBranch        *b_Hit_nID_Pri;   //!
   TBranch        *b_Hit_PDG;   //!
   TBranch        *b_Hit_Mo_PDG;   //!
   TBranch        *b_Hit_mu_flag;   //!
   TBranch        *b_Hit_ele_flag;   //!
   TBranch        *b_Hit_offset;   //!
   TBranch        *b_n_tracks_pad;   //!
   TBranch        *b_all_track_length;   //!
   TBranch        *b_all_track_mom;   //!
   TBranch        *b_all_track_calmom;   //!
   TBranch        *b_all_track_startx;   //!
   TBranch        *b_all_track_starty;   //!
   TBranch        *b_all_track_startz;   //!
   TBranch        *b_all_track_px;   //!
   TBranch        *b_all_track_py;   //!
   TBranch        *b_all_track_pz;   //!
   TBranch        *b_all_track_true_Efrac;   //!
   TBranch        *b_all_track_true_pdg;   //!
   TBranch        *b_all_track_true_pdg_mom;   //!
   TBranch        *b_all_track_dist_reco_vtx;   //!
   TBranch        *b_all_track_dist_true_vtx;   //!
   TBranch        *b_all_track_min_angle;   //!
   TBranch        *b_all_track_true_eng;   //!
   TBranch        *b_all_track_true_px;   //!
   TBranch        *b_all_track_true_py;   //!
   TBranch        *b_all_track_true_pz;   //!
   TBranch        *b_all_track_true_mom_px;   //!
   TBranch        *b_all_track_true_mom_py;   //!
   TBranch        *b_all_track_true_mom_pz;   //!
   TBranch        *b_all_track2_true_Efrac;   //!
   TBranch        *b_all_track2_true_pdg;   //!
   TBranch        *b_all_track2_true_pdg_mom;   //!
   TBranch        *b_all_track2_true_eng;   //!
   TBranch        *b_all_track2_true_px;   //!
   TBranch        *b_all_track2_true_py;   //!
   TBranch        *b_all_track2_true_pz;   //!
   TBranch        *b_all_track2_true_mom_px;   //!
   TBranch        *b_all_track2_true_mom_py;   //!
   TBranch        *b_all_track2_true_mom_pz;   //!
   TBranch        *b_n_showers;   //!
   TBranch        *b_shw_tot_chg;   //!
   TBranch        *b_shw_chg;   //!
   TBranch        *b_shw_nhits;   //!
   TBranch        *b_all_shw_idx;   //!
   TBranch        *b_all_shw_eng;   //!
   TBranch        *b_all_shw_dedx;   //!
   TBranch        *b_all_shw_length;   //!
   TBranch        *b_all_shw_startx;   //!
   TBranch        *b_all_shw_starty;   //!
   TBranch        *b_all_shw_startz;   //!
   TBranch        *b_all_shw_px;   //!
   TBranch        *b_all_shw_py;   //!
   TBranch        *b_all_shw_pz;   //!
   TBranch        *b_all_shw_true_eng;   //!
   TBranch        *b_all_shw_true_px;   //!
   TBranch        *b_all_shw_true_py;   //!
   TBranch        *b_all_shw_true_pz;   //!
   TBranch        *b_all_shw_true_mom_px;   //!
   TBranch        *b_all_shw_true_mom_py;   //!
   TBranch        *b_all_shw_true_mom_pz;   //!
   TBranch        *b_all_shw_costh;   //!
   TBranch        *b_all_shw_phi;   //!
   TBranch        *b_all_shw_true_Efrac;   //!
   TBranch        *b_all_shw_true_pdg;   //!
   TBranch        *b_all_shw_true_pdg_mom;   //!
   TBranch        *b_all_shw_dist_reco_vtx;   //!
   TBranch        *b_all_shw_dist_true_vtx;   //!
   TBranch        *b_all_shw_min_angle;   //!
   TBranch        *b_all_shw2_true_Efrac;   //!
   TBranch        *b_all_shw2_true_pdg;   //!
   TBranch        *b_all_shw2_true_pdg_mom;   //!
   TBranch        *b_all_shw2_true_eng;   //!
   TBranch        *b_all_shw2_true_px;   //!
   TBranch        *b_all_shw2_true_py;   //!
   TBranch        *b_all_shw2_true_pz;   //!
   TBranch        *b_all_shw2_true_mom_px;   //!
   TBranch        *b_all_shw2_true_mom_py;   //!
   TBranch        *b_all_shw2_true_mom_pz;   //!
   TBranch        *b_led_shw_idx;   //!
   TBranch        *b_led_shw_eng;   //!
   TBranch        *b_led_shw_dirx;   //!
   TBranch        *b_led_shw_diry;   //!
   TBranch        *b_led_shw_dirz;   //!
   TBranch        *b_led_shw_startx;   //!
   TBranch        *b_led_shw_starty;   //!
   TBranch        *b_led_shw_startz;   //!
   TBranch        *b_led_shw_costh;   //!
   TBranch        *b_ShwVtxTPC;   //!
   TBranch        *b_ShwVtxWire;   //!
   TBranch        *b_ShwVtxTime;   //!
   TBranch        *b_ShwVtxGlobalWire;   //!
   TBranch        *b_ShwVtxGlobalTick;   //!
   TBranch        *b_ShwVtxGlobalPlane;   //!
   TBranch        *b_led_shw_trueP0;   //!
   TBranch        *b_led_shw_trueP1;   //!
   TBranch        *b_led_shw_trueP2;   //!
   TBranch        *b_led_shw_trueP3;   //!
   TBranch        *b_led_shw_truedang;   //!
   TBranch        *b_led_shw_truePDG;   //!
   TBranch        *b_led_shw_truePDGMom;   //!
   TBranch        *b_led_shw_truecosth;   //!
   TBranch        *b_ShwFirstWire;   //!
   TBranch        *b_ShwFirstTime;   //!
   TBranch        *b_n_clusters;   //!
   TBranch        *b_Clst_Lead_idx;   //!
   TBranch        *b_Clst_Plane;   //!
   TBranch        *b_Clst_Integral;   //!
   TBranch        *b_Clst_SumADC;   //!
   TBranch        *b_Clst_Width;   //!
   TBranch        *b_Clst_Nhits;   //!
   TBranch        *b_Clst_StartWire;   //!
   TBranch        *b_Clst_StartTick;   //!
   TBranch        *b_Clst_EndWire;   //!
   TBranch        *b_Clst_EndTick;   //!
   TBranch        *b_Shw_ChargeU;   //!
   TBranch        *b_Shw_ChargeV;   //!
   TBranch        *b_Shw_ChargeZ;   //!
   TBranch        *b_Shw_CorrectedChargeU;   //!
   TBranch        *b_Shw_CorrectedChargeV;   //!
   TBranch        *b_Shw_CorrectedChargeZ;   //!
   TBranch        *b_Shw_EnergyU;   //!
   TBranch        *b_Shw_EnergyV;   //!
   TBranch        *b_Shw_EnergyZ;   //!
   TBranch        *b_Shw_CorrectedEnergyU;   //!
   TBranch        *b_Shw_CorrectedEnergyV;   //!
   TBranch        *b_Shw_CorrectedEnergyZ;   //!
   TBranch        *b_NHits_Shw;   //!
   TBranch        *b_Shw_Idx;   //!
   TBranch        *b_Shw_StartX;   //!
   TBranch        *b_Shw_StartY;   //!
   TBranch        *b_Shw_StartZ;   //!
   TBranch        *b_Shw_DirX;   //!
   TBranch        *b_Shw_DirY;   //!
   TBranch        *b_Shw_DirZ;   //!
   TBranch        *b_Hit_Shw_TPC;   //!
   TBranch        *b_Hit_Shw_Plane;   //!
   TBranch        *b_Hit_Shw_Wire;   //!
   TBranch        *b_Hit_Shw_Channel;   //!
   TBranch        *b_Hit_Shw_Multi;   //!
   TBranch        *b_Hit_Shw_TrueTrackID;   //!
   TBranch        *b_Hit_Shw_ntrackID;   //!
   TBranch        *b_Hit_Shw_PeakT;   //!
   TBranch        *b_Hit_Shw_StartT;   //!
   TBranch        *b_Hit_Shw_EndT;   //!
   TBranch        *b_Hit_Shw_Charge;   //!
   TBranch        *b_Hit_Shw_CorrCharge;   //!
   TBranch        *b_Hit_Shw_Energy;   //!
   TBranch        *b_Hit_Shw_trueE;   //!
   TBranch        *b_Hit_Shw_PDG;   //!
   TBranch        *b_Hit_Shw_PDG_Mom;   //!
   TBranch        *b_Hit_Shw_Global_Plane;   //!
   TBranch        *b_Hit_Shw_Global_Wire;   //!
   TBranch        *b_Hit_Shw_Global_Tick;   //!
   TBranch        *b_n_wires;   //!
   TBranch        *b_Hit_Nallraws;   //!
   TBranch        *b_Hit_Nadcvec;   //!
   TBranch        *b_Hit_raw_wireID;   //!
   TBranch        *b_Hit_raw_chID;   //!
   TBranch        *b_Hit_rawadc;   //!
   TBranch        *b_Hit_rawtick;   //!
   TBranch        *b_Wire_assn;   //!
   TBranch        *b_Wire_min_tick;   //!
   TBranch        *b_Wire_max_tick;   //!
   TBranch        *b_Wire_max_rmsT;   //!
   TBranch        *b_Wire_min_shwtick;   //!
   TBranch        *b_Wire_max_shwtick;   //!
   TBranch        *b_Wire_min_trktick;   //!
   TBranch        *b_Wire_max_trktick;   //!
   TBranch        *b_Wire_max_shwrms;   //!
   TBranch        *b_Wire_max_trkrms;   //!
   TBranch        *b_Wire_min_ledshwtick;   //!
   TBranch        *b_Wire_max_ledshwtick;   //!
   TBranch        *b_Wire_adc;   //!
   TBranch        *b_Wire_corradc;   //!
   TBranch        *b_Wire_tick;   //!
   TBranch        *b_Wire_shwflag;   //!
   TBranch        *b_Wire_ledshwflag;   //!
   TBranch        *b_Wire_x;   //!
   TBranch        *b_Wire_pdg;   //!
   TBranch        *b_Wire_pdg_mom;   //!
   TBranch        *b_Wire_dr;   //!
   TBranch        *b_Wire_dist;   //!
   TBranch        *b_Wire_tag_pdg;   //!
   TBranch        *b_Wire_tag_pdg_mom;   //!
   TBranch        *b_Wire_tag_is_shw;   //!
   TBranch        *b_Wire_tag_ith;   //!
   TBranch        *b_Wire_tag_true_px;   //!
   TBranch        *b_Wire_tag_true_py;   //!
   TBranch        *b_Wire_tag_true_pz;   //!
   TBranch        *b_Wire_tag_reco_px;   //!
   TBranch        *b_Wire_tag_reco_py;   //!
   TBranch        *b_Wire_tag_reco_pz;   //!
   TBranch        *b_Wire_tag_reco_eng;   //!
   TBranch        *b_Ch_charge;   //!
   TBranch        *b_Ch_tick;   //!
   TBranch        *b_Wire_ROIfirst;   //!
   TBranch        *b_Wire_ROIend;   //!
   TBranch        *b_Sum_chchg;   //!
   TBranch        *b_TrueParticle_size;   //!
   TBranch        *b_TruePart_PDG;   //!
   TBranch        *b_TruePart_E;   //!
   TBranch        *b_ChgU;   //!
   TBranch        *b_ChgV;   //!
   TBranch        *b_ChgZ;   //!
   TBranch        *b_n_tracks_pm;   //!
   TBranch        *b_n_pand_par;   //!
   TBranch        *b_n_prims;   //!
   TBranch        *b_m_pf_vtx;   //!
   TBranch        *b_m_pf_vtx_x;   //!
   TBranch        *b_m_pf_vtx_y;   //!
   TBranch        *b_m_pf_vtx_z;   //!
   TBranch        *b_PFVtxTPC;   //!
   TBranch        *b_PFVtxWire;   //!
   TBranch        *b_PFVtxTime;   //!
   TBranch        *b_PFVtxGlobalWire;   //!
   TBranch        *b_PFVtxGlobalTick;   //!
   TBranch        *b_PFVtxGlobalPlane;   //!

   analysis_RecoEnergyStudies_outputs(TTree *tree=0);
   virtual ~analysis_RecoEnergyStudies_outputs();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef analysis_RecoEnergyStudies_outputs_cxx
analysis_RecoEnergyStudies_outputs::analysis_RecoEnergyStudies_outputs(TTree *tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/RecoEnergyS.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/RecoEnergyS.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/RecoEnergyS.root:/recoEnergy");
      dir->GetObject("WC",tree);

   }
   Init(tree);
}

analysis_RecoEnergyStudies_outputs::~analysis_RecoEnergyStudies_outputs()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t analysis_RecoEnergyStudies_outputs::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t analysis_RecoEnergyStudies_outputs::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void analysis_RecoEnergyStudies_outputs::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   Hit_rawadc = 0;
   Hit_rawtick = 0;
   Wire_adc = 0;
   Wire_corradc = 0;
   Wire_tick = 0;
   Wire_shwflag = 0;
   Wire_ledshwflag = 0;
   Wire_x = 0;
   Wire_pdg = 0;
   Wire_pdg_mom = 0;
   Wire_dr = 0;
   Wire_dist = 0;
   Wire_tag_pdg = 0;
   Wire_tag_pdg_mom = 0;
   Wire_tag_is_shw = 0;
   Wire_tag_ith = 0;
   Wire_tag_true_px = 0;
   Wire_tag_true_py = 0;
   Wire_tag_true_pz = 0;
   Wire_tag_reco_px = 0;
   Wire_tag_reco_py = 0;
   Wire_tag_reco_pz = 0;
   Wire_tag_reco_eng = 0;
   Ch_charge = 0;
   Ch_tick = 0;
   TruePart_PDG = 0;
   TruePart_E = 0;
   ChgU = 0;
   ChgV = 0;
   ChgZ = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ievent", &ievent, &b_ievent);
   fChain->SetBranchAddress("det_t0", &det_t0, &b_det_t0);
   fChain->SetBranchAddress("driftvelocity", &driftvelocity, &b_driftvelocity);
   fChain->SetBranchAddress("SampleRate", &SampleRate, &b_SampleRate);
   fChain->SetBranchAddress("NTPCs_evt", &NTPCs_evt, &b_NTPCs_evt);
   fChain->SetBranchAddress("NWires_P0", &NWires_P0, &b_NWires_P0);
   fChain->SetBranchAddress("NWires_P1", &NWires_P1, &b_NWires_P1);
   fChain->SetBranchAddress("NWires_P2", &NWires_P2, &b_NWires_P2);
   fChain->SetBranchAddress("NuTruthFid", &NuTruthFid, &b_NuTruthFid);
   fChain->SetBranchAddress("NuTruthN", &NuTruthN, &b_NuTruthN);
   fChain->SetBranchAddress("NuEngTruth", NuEngTruth, &b_NuEngTruth);
   fChain->SetBranchAddress("NuPDGTruth", NuPDGTruth, &b_NuPDGTruth);
   fChain->SetBranchAddress("NuVtxXTruth", NuVtxXTruth, &b_NuVtxXTruth);
   fChain->SetBranchAddress("NuVtxYTruth", NuVtxYTruth, &b_NuVtxYTruth);
   fChain->SetBranchAddress("NuVtxZTruth", NuVtxZTruth, &b_NuVtxZTruth);
   fChain->SetBranchAddress("NuModeTruth", NuModeTruth, &b_NuModeTruth);
   fChain->SetBranchAddress("NuCCNCTruth", NuCCNCTruth, &b_NuCCNCTruth);
   fChain->SetBranchAddress("LepPDGTruth", LepPDGTruth, &b_LepPDGTruth);
   fChain->SetBranchAddress("LepP0Truth", LepP0Truth, &b_LepP0Truth);
   fChain->SetBranchAddress("LepP1Truth", LepP1Truth, &b_LepP1Truth);
   fChain->SetBranchAddress("LepP2Truth", LepP2Truth, &b_LepP2Truth);
   fChain->SetBranchAddress("LepEngTruth", LepEngTruth, &b_LepEngTruth);
   fChain->SetBranchAddress("MuTrackCont", &MuTrackCont, &b_MuTrackCont);
   fChain->SetBranchAddress("PrimPDG", &PrimPDG, &b_PrimPDG);
   fChain->SetBranchAddress("TrueEnergy", &TrueEnergy, &b_TrueEnergy);
   fChain->SetBranchAddress("TrueVertexX", &TrueVertexX, &b_TrueVertexX);
   fChain->SetBranchAddress("TrueVertexY", &TrueVertexY, &b_TrueVertexY);
   fChain->SetBranchAddress("TrueVertexZ", &TrueVertexZ, &b_TrueVertexZ);
   fChain->SetBranchAddress("TrueEndX", &TrueEndX, &b_TrueEndX);
   fChain->SetBranchAddress("TrueEndY", &TrueEndY, &b_TrueEndY);
   fChain->SetBranchAddress("TrueEndZ", &TrueEndZ, &b_TrueEndZ);
   fChain->SetBranchAddress("TruePx", &TruePx, &b_TruePx);
   fChain->SetBranchAddress("TruePy", &TruePy, &b_TruePy);
   fChain->SetBranchAddress("TruePz", &TruePz, &b_TruePz);
   fChain->SetBranchAddress("TrueVtxin", &TrueVtxin, &b_TrueVtxin);
   fChain->SetBranchAddress("TrueVtxWire", TrueVtxWire, &b_TrueVtxWire);
   fChain->SetBranchAddress("TrueVtxGlobalWire", TrueVtxGlobalWire, &b_TrueVtxGlobalWire);
   fChain->SetBranchAddress("TrueVtxGlobalTick", TrueVtxGlobalTick, &b_TrueVtxGlobalTick);
   fChain->SetBranchAddress("TrueVtxGlobalPlane", TrueVtxGlobalPlane, &b_TrueVtxGlobalPlane);
   fChain->SetBranchAddress("TrueVtxTPC", TrueVtxTPC, &b_TrueVtxTPC);
   fChain->SetBranchAddress("TrueVtxTime", TrueVtxTime, &b_TrueVtxTime);
   fChain->SetBranchAddress("TrueVtxChan", TrueVtxChan, &b_TrueVtxChan);
   fChain->SetBranchAddress("TrueEndWire", TrueEndWire, &b_TrueEndWire);
   fChain->SetBranchAddress("TrueEndTime", TrueEndTime, &b_TrueEndTime);
   fChain->SetBranchAddress("TrueMidWire", TrueMidWire, &b_TrueMidWire);
   fChain->SetBranchAddress("TrueMidTime", TrueMidTime, &b_TrueMidTime);
   fChain->SetBranchAddress("EndProngGlobalWire", EndProngGlobalWire, &b_EndProngGlobalWire);
   fChain->SetBranchAddress("EndProngGlobalTick", EndProngGlobalTick, &b_EndProngGlobalTick);
   fChain->SetBranchAddress("EndProngGlobalPlane", EndProngGlobalPlane, &b_EndProngGlobalPlane);
   fChain->SetBranchAddress("EvthasPriPF", &EvthasPriPF, &b_EvthasPriPF);
   fChain->SetBranchAddress("RecoVertexX", &RecoVertexX, &b_RecoVertexX);
   fChain->SetBranchAddress("RecoVertexY", &RecoVertexY, &b_RecoVertexY);
   fChain->SetBranchAddress("RecoVertexZ", &RecoVertexZ, &b_RecoVertexZ);
   fChain->SetBranchAddress("RecoVtxWire", RecoVtxWire, &b_RecoVtxWire);
   fChain->SetBranchAddress("RecoVtxTime", RecoVtxTime, &b_RecoVtxTime);
   fChain->SetBranchAddress("DepositU", &DepositU, &b_DepositU);
   fChain->SetBranchAddress("DepositV", &DepositV, &b_DepositV);
   fChain->SetBranchAddress("DepositZ", &DepositZ, &b_DepositZ);
   fChain->SetBranchAddress("ChargeU", &ChargeU, &b_ChargeU);
   fChain->SetBranchAddress("ChargeV", &ChargeV, &b_ChargeV);
   fChain->SetBranchAddress("ChargeZ", &ChargeZ, &b_ChargeZ);
   fChain->SetBranchAddress("CorrectedChargeU", &CorrectedChargeU, &b_CorrectedChargeU);
   fChain->SetBranchAddress("CorrectedChargeV", &CorrectedChargeV, &b_CorrectedChargeV);
   fChain->SetBranchAddress("CorrectedChargeZ", &CorrectedChargeZ, &b_CorrectedChargeZ);
   fChain->SetBranchAddress("VertexDetectorDist", &VertexDetectorDist, &b_VertexDetectorDist);
   fChain->SetBranchAddress("EnergyU", &EnergyU, &b_EnergyU);
   fChain->SetBranchAddress("EnergyV", &EnergyV, &b_EnergyV);
   fChain->SetBranchAddress("EnergyZ", &EnergyZ, &b_EnergyZ);
   fChain->SetBranchAddress("CorrectedEnergyU", &CorrectedEnergyU, &b_CorrectedEnergyU);
   fChain->SetBranchAddress("CorrectedEnergyV", &CorrectedEnergyV, &b_CorrectedEnergyV);
   fChain->SetBranchAddress("CorrectedEnergyZ", &CorrectedEnergyZ, &b_CorrectedEnergyZ);
   fChain->SetBranchAddress("ErecoNu", &ErecoNu, &b_ErecoNu);
   fChain->SetBranchAddress("RecoLepEnNu", &RecoLepEnNu, &b_RecoLepEnNu);
   fChain->SetBranchAddress("RecoHadEnNu", &RecoHadEnNu, &b_RecoHadEnNu);
   fChain->SetBranchAddress("RecoMethodNu", &RecoMethodNu, &b_RecoMethodNu);
   fChain->SetBranchAddress("NHits", &NHits, &b_NHits);
   fChain->SetBranchAddress("NTPCs", &NTPCs, &b_NTPCs);
   fChain->SetBranchAddress("Hit_Cryst", Hit_Cryst, &b_Hit_Cryst);
   fChain->SetBranchAddress("Hit_TPC", Hit_TPC, &b_Hit_TPC);
   fChain->SetBranchAddress("Hit_Plane", Hit_Plane, &b_Hit_Plane);
   fChain->SetBranchAddress("Hit_Wire", Hit_Wire, &b_Hit_Wire);
   fChain->SetBranchAddress("Hit_Channel", Hit_Channel, &b_Hit_Channel);
   fChain->SetBranchAddress("Hit_Multi", Hit_Multi, &b_Hit_Multi);
   fChain->SetBranchAddress("Hit_PeakT", Hit_PeakT, &b_Hit_PeakT);
   fChain->SetBranchAddress("Hit_Amp", Hit_Amp, &b_Hit_Amp);
   fChain->SetBranchAddress("Hit_MinusT", Hit_MinusT, &b_Hit_MinusT);
   fChain->SetBranchAddress("Hit_PlusT", Hit_PlusT, &b_Hit_PlusT);
   fChain->SetBranchAddress("Hit_RMST", Hit_RMST, &b_Hit_RMST);
   fChain->SetBranchAddress("Hit_StartT", Hit_StartT, &b_Hit_StartT);
   fChain->SetBranchAddress("Hit_EndT", Hit_EndT, &b_Hit_EndT);
   fChain->SetBranchAddress("Hit_Charge", Hit_Charge, &b_Hit_Charge);
   fChain->SetBranchAddress("Hit_CorrCharge", Hit_CorrCharge, &b_Hit_CorrCharge);
   fChain->SetBranchAddress("Hit_Energy", Hit_Energy, &b_Hit_Energy);
   fChain->SetBranchAddress("Hit_TrueTrackID", Hit_TrueTrackID, &b_Hit_TrueTrackID);
   fChain->SetBranchAddress("Hit_X", Hit_X, &b_Hit_X);
   fChain->SetBranchAddress("Hit_Y", Hit_Y, &b_Hit_Y);
   fChain->SetBranchAddress("Hit_Z", Hit_Z, &b_Hit_Z);
   fChain->SetBranchAddress("Hit_Global_Wire", Hit_Global_Wire, &b_Hit_Global_Wire);
   fChain->SetBranchAddress("Hit_Global_Tick", Hit_Global_Tick, &b_Hit_Global_Tick);
   fChain->SetBranchAddress("Hit_Global_Plane", Hit_Global_Plane, &b_Hit_Global_Plane);
   fChain->SetBranchAddress("Hit_Good", Hit_Good, &b_Hit_Good);
   fChain->SetBranchAddress("Hit_Ndf", Hit_Ndf, &b_Hit_Ndf);
   fChain->SetBranchAddress("Hit_Prong_Tag", Hit_Prong_Tag, &b_Hit_Prong_Tag);
   fChain->SetBranchAddress("Hit_Prong_PDG", Hit_Prong_PDG, &b_Hit_Prong_PDG);
   fChain->SetBranchAddress("Hit_Prong_PDG_Mom", Hit_Prong_PDG_Mom, &b_Hit_Prong_PDG_Mom);
   fChain->SetBranchAddress("Hit_ShwID", Hit_ShwID, &b_Hit_ShwID);
   fChain->SetBranchAddress("Hit_LedShwID", Hit_LedShwID, &b_Hit_LedShwID);
   fChain->SetBranchAddress("Hit_ShwDirX", Hit_ShwDirX, &b_Hit_ShwDirX);
   fChain->SetBranchAddress("Hit_ShwDirY", Hit_ShwDirY, &b_Hit_ShwDirY);
   fChain->SetBranchAddress("Hit_ShwDirZ", Hit_ShwDirZ, &b_Hit_ShwDirZ);
   fChain->SetBranchAddress("Hit_ShwCoth", Hit_ShwCoth, &b_Hit_ShwCoth);
   fChain->SetBranchAddress("Hit_ShwPhi", Hit_ShwPhi, &b_Hit_ShwPhi);
   fChain->SetBranchAddress("Hit_ShwStartT", Hit_ShwStartT, &b_Hit_ShwStartT);
   fChain->SetBranchAddress("Hit_ShwEndT", Hit_ShwEndT, &b_Hit_ShwEndT);
   fChain->SetBranchAddress("Hit_ShwEvtEng", Hit_ShwEvtEng, &b_Hit_ShwEvtEng);
   fChain->SetBranchAddress("Hit_ShwEvtdEdx", Hit_ShwEvtdEdx, &b_Hit_ShwEvtdEdx);
   fChain->SetBranchAddress("Hit_ntrackID", Hit_ntrackID, &b_Hit_ntrackID);
   fChain->SetBranchAddress("Hit_trueE", Hit_trueE, &b_Hit_trueE);
   fChain->SetBranchAddress("Hit_nID_Pri", Hit_nID_Pri, &b_Hit_nID_Pri);
   fChain->SetBranchAddress("Hit_PDG", Hit_PDG, &b_Hit_PDG);
   fChain->SetBranchAddress("Hit_Mo_PDG", Hit_Mo_PDG, &b_Hit_Mo_PDG);
   fChain->SetBranchAddress("Hit_mu_flag", Hit_mu_flag, &b_Hit_mu_flag);
   fChain->SetBranchAddress("Hit_ele_flag", Hit_ele_flag, &b_Hit_ele_flag);
   fChain->SetBranchAddress("Hit_offset", Hit_offset, &b_Hit_offset);
   fChain->SetBranchAddress("n_tracks_pad", &n_tracks_pad, &b_n_tracks_pad);
   fChain->SetBranchAddress("all_track_length", all_track_length, &b_all_track_length);
   fChain->SetBranchAddress("all_track_mom", all_track_mom, &b_all_track_mom);
   fChain->SetBranchAddress("all_track_calmom", all_track_calmom, &b_all_track_calmom);
   fChain->SetBranchAddress("all_track_startx", all_track_startx, &b_all_track_startx);
   fChain->SetBranchAddress("all_track_starty", all_track_starty, &b_all_track_starty);
   fChain->SetBranchAddress("all_track_startz", all_track_startz, &b_all_track_startz);
   fChain->SetBranchAddress("all_track_px", all_track_px, &b_all_track_px);
   fChain->SetBranchAddress("all_track_py", all_track_py, &b_all_track_py);
   fChain->SetBranchAddress("all_track_pz", all_track_pz, &b_all_track_pz);
   fChain->SetBranchAddress("all_track_true_Efrac", all_track_true_Efrac, &b_all_track_true_Efrac);
   fChain->SetBranchAddress("all_track_true_pdg", all_track_true_pdg, &b_all_track_true_pdg);
   fChain->SetBranchAddress("all_track_true_pdg_mom", all_track_true_pdg_mom, &b_all_track_true_pdg_mom);
   fChain->SetBranchAddress("all_track_dist_reco_vtx", all_track_dist_reco_vtx, &b_all_track_dist_reco_vtx);
   fChain->SetBranchAddress("all_track_dist_true_vtx", all_track_dist_true_vtx, &b_all_track_dist_true_vtx);
   fChain->SetBranchAddress("all_track_min_angle", all_track_min_angle, &b_all_track_min_angle);
   fChain->SetBranchAddress("all_track_true_eng", all_track_true_eng, &b_all_track_true_eng);
   fChain->SetBranchAddress("all_track_true_px", all_track_true_px, &b_all_track_true_px);
   fChain->SetBranchAddress("all_track_true_py", all_track_true_py, &b_all_track_true_py);
   fChain->SetBranchAddress("all_track_true_pz", all_track_true_pz, &b_all_track_true_pz);
   fChain->SetBranchAddress("all_track_true_mom_px", all_track_true_mom_px, &b_all_track_true_mom_px);
   fChain->SetBranchAddress("all_track_true_mom_py", all_track_true_mom_py, &b_all_track_true_mom_py);
   fChain->SetBranchAddress("all_track_true_mom_pz", all_track_true_mom_pz, &b_all_track_true_mom_pz);
   fChain->SetBranchAddress("all_track2_true_Efrac", all_track2_true_Efrac, &b_all_track2_true_Efrac);
   fChain->SetBranchAddress("all_track2_true_pdg", all_track2_true_pdg, &b_all_track2_true_pdg);
   fChain->SetBranchAddress("all_track2_true_pdg_mom", all_track2_true_pdg_mom, &b_all_track2_true_pdg_mom);
   fChain->SetBranchAddress("all_track2_true_eng", all_track2_true_eng, &b_all_track2_true_eng);
   fChain->SetBranchAddress("all_track2_true_px", all_track2_true_px, &b_all_track2_true_px);
   fChain->SetBranchAddress("all_track2_true_py", all_track2_true_py, &b_all_track2_true_py);
   fChain->SetBranchAddress("all_track2_true_pz", all_track2_true_pz, &b_all_track2_true_pz);
   fChain->SetBranchAddress("all_track2_true_mom_px", all_track2_true_mom_px, &b_all_track2_true_mom_px);
   fChain->SetBranchAddress("all_track2_true_mom_py", all_track2_true_mom_py, &b_all_track2_true_mom_py);
   fChain->SetBranchAddress("all_track2_true_mom_pz", all_track2_true_mom_pz, &b_all_track2_true_mom_pz);
   fChain->SetBranchAddress("n_showers", &n_showers, &b_n_showers);
   fChain->SetBranchAddress("shw_tot_chg", &shw_tot_chg, &b_shw_tot_chg);
   fChain->SetBranchAddress("shw_chg", shw_chg, &b_shw_chg);
   fChain->SetBranchAddress("shw_nhits", shw_nhits, &b_shw_nhits);
   fChain->SetBranchAddress("all_shw_idx", all_shw_idx, &b_all_shw_idx);
   fChain->SetBranchAddress("all_shw_eng", all_shw_eng, &b_all_shw_eng);
   fChain->SetBranchAddress("all_shw_dedx", all_shw_dedx, &b_all_shw_dedx);
   fChain->SetBranchAddress("all_shw_length", all_shw_length, &b_all_shw_length);
   fChain->SetBranchAddress("all_shw_startx", all_shw_startx, &b_all_shw_startx);
   fChain->SetBranchAddress("all_shw_starty", all_shw_starty, &b_all_shw_starty);
   fChain->SetBranchAddress("all_shw_startz", all_shw_startz, &b_all_shw_startz);
   fChain->SetBranchAddress("all_shw_px", all_shw_px, &b_all_shw_px);
   fChain->SetBranchAddress("all_shw_py", all_shw_py, &b_all_shw_py);
   fChain->SetBranchAddress("all_shw_pz", all_shw_pz, &b_all_shw_pz);
   fChain->SetBranchAddress("all_shw_true_eng", all_shw_true_eng, &b_all_shw_true_eng);
   fChain->SetBranchAddress("all_shw_true_px", all_shw_true_px, &b_all_shw_true_px);
   fChain->SetBranchAddress("all_shw_true_py", all_shw_true_py, &b_all_shw_true_py);
   fChain->SetBranchAddress("all_shw_true_pz", all_shw_true_pz, &b_all_shw_true_pz);
   fChain->SetBranchAddress("all_shw_true_mom_px", all_shw_true_mom_px, &b_all_shw_true_mom_px);
   fChain->SetBranchAddress("all_shw_true_mom_py", all_shw_true_mom_py, &b_all_shw_true_mom_py);
   fChain->SetBranchAddress("all_shw_true_mom_pz", all_shw_true_mom_pz, &b_all_shw_true_mom_pz);
   fChain->SetBranchAddress("all_shw_costh", all_shw_costh, &b_all_shw_costh);
   fChain->SetBranchAddress("all_shw_phi", all_shw_phi, &b_all_shw_phi);
   fChain->SetBranchAddress("all_shw_true_Efrac", all_shw_true_Efrac, &b_all_shw_true_Efrac);
   fChain->SetBranchAddress("all_shw_true_pdg", all_shw_true_pdg, &b_all_shw_true_pdg);
   fChain->SetBranchAddress("all_shw_true_pdg_mom", all_shw_true_pdg_mom, &b_all_shw_true_pdg_mom);
   fChain->SetBranchAddress("all_shw_dist_reco_vtx", all_shw_dist_reco_vtx, &b_all_shw_dist_reco_vtx);
   fChain->SetBranchAddress("all_shw_dist_true_vtx", all_shw_dist_true_vtx, &b_all_shw_dist_true_vtx);
   fChain->SetBranchAddress("all_shw_min_angle", all_shw_min_angle, &b_all_shw_min_angle);
   fChain->SetBranchAddress("all_shw2_true_Efrac", all_shw2_true_Efrac, &b_all_shw2_true_Efrac);
   fChain->SetBranchAddress("all_shw2_true_pdg", all_shw2_true_pdg, &b_all_shw2_true_pdg);
   fChain->SetBranchAddress("all_shw2_true_pdg_mom", all_shw2_true_pdg_mom, &b_all_shw2_true_pdg_mom);
   fChain->SetBranchAddress("all_shw2_true_eng", all_shw2_true_eng, &b_all_shw2_true_eng);
   fChain->SetBranchAddress("all_shw2_true_px", all_shw2_true_px, &b_all_shw2_true_px);
   fChain->SetBranchAddress("all_shw2_true_py", all_shw2_true_py, &b_all_shw2_true_py);
   fChain->SetBranchAddress("all_shw2_true_pz", all_shw2_true_pz, &b_all_shw2_true_pz);
   fChain->SetBranchAddress("all_shw2_true_mom_px", all_shw2_true_mom_px, &b_all_shw2_true_mom_px);
   fChain->SetBranchAddress("all_shw2_true_mom_py", all_shw2_true_mom_py, &b_all_shw2_true_mom_py);
   fChain->SetBranchAddress("all_shw2_true_mom_pz", all_shw2_true_mom_pz, &b_all_shw2_true_mom_pz);
   fChain->SetBranchAddress("led_shw_idx", &led_shw_idx, &b_led_shw_idx);
   fChain->SetBranchAddress("led_shw_eng", &led_shw_eng, &b_led_shw_eng);
   fChain->SetBranchAddress("led_shw_dirx", &led_shw_dirx, &b_led_shw_dirx);
   fChain->SetBranchAddress("led_shw_diry", &led_shw_diry, &b_led_shw_diry);
   fChain->SetBranchAddress("led_shw_dirz", &led_shw_dirz, &b_led_shw_dirz);
   fChain->SetBranchAddress("led_shw_startx", &led_shw_startx, &b_led_shw_startx);
   fChain->SetBranchAddress("led_shw_starty", &led_shw_starty, &b_led_shw_starty);
   fChain->SetBranchAddress("led_shw_startz", &led_shw_startz, &b_led_shw_startz);
   fChain->SetBranchAddress("led_shw_costh", &led_shw_costh, &b_led_shw_costh);
   fChain->SetBranchAddress("ShwVtxTPC", ShwVtxTPC, &b_ShwVtxTPC);
   fChain->SetBranchAddress("ShwVtxWire", ShwVtxWire, &b_ShwVtxWire);
   fChain->SetBranchAddress("ShwVtxTime", ShwVtxTime, &b_ShwVtxTime);
   fChain->SetBranchAddress("ShwVtxGlobalWire", ShwVtxGlobalWire, &b_ShwVtxGlobalWire);
   fChain->SetBranchAddress("ShwVtxGlobalTick", ShwVtxGlobalTick, &b_ShwVtxGlobalTick);
   fChain->SetBranchAddress("ShwVtxGlobalPlane", ShwVtxGlobalPlane, &b_ShwVtxGlobalPlane);
   fChain->SetBranchAddress("led_shw_trueP0", &led_shw_trueP0, &b_led_shw_trueP0);
   fChain->SetBranchAddress("led_shw_trueP1", &led_shw_trueP1, &b_led_shw_trueP1);
   fChain->SetBranchAddress("led_shw_trueP2", &led_shw_trueP2, &b_led_shw_trueP2);
   fChain->SetBranchAddress("led_shw_trueP3", &led_shw_trueP3, &b_led_shw_trueP3);
   fChain->SetBranchAddress("led_shw_truedang", &led_shw_truedang, &b_led_shw_truedang);
   fChain->SetBranchAddress("led_shw_truePDG", &led_shw_truePDG, &b_led_shw_truePDG);
   fChain->SetBranchAddress("led_shw_truePDGMom", &led_shw_truePDGMom, &b_led_shw_truePDGMom);
   fChain->SetBranchAddress("led_shw_truecosth", &led_shw_truecosth, &b_led_shw_truecosth);
   fChain->SetBranchAddress("ShwFirstWire", &ShwFirstWire, &b_ShwFirstWire);
   fChain->SetBranchAddress("ShwFirstTime", &ShwFirstTime, &b_ShwFirstTime);
   fChain->SetBranchAddress("n_clusters", &n_clusters, &b_n_clusters);
   fChain->SetBranchAddress("Clst_Lead_Idx", &Clst_Lead_Idx, &b_Clst_Lead_idx);
   fChain->SetBranchAddress("Clst_Plane", Clst_Plane, &b_Clst_Plane);
   fChain->SetBranchAddress("Clst_Integral", Clst_Integral, &b_Clst_Integral);
   fChain->SetBranchAddress("Clst_SumADC", Clst_SumADC, &b_Clst_SumADC);
   fChain->SetBranchAddress("Clst_Width", Clst_Width, &b_Clst_Width);
   fChain->SetBranchAddress("Clst_Nhits", Clst_Nhits, &b_Clst_Nhits);
   fChain->SetBranchAddress("Clst_StartWire", Clst_StartWire, &b_Clst_StartWire);
   fChain->SetBranchAddress("Clst_StartTick", Clst_StartTick, &b_Clst_StartTick);
   fChain->SetBranchAddress("Clst_EndWire", Clst_EndWire, &b_Clst_EndWire);
   fChain->SetBranchAddress("Clst_EndTick", Clst_EndTick, &b_Clst_EndTick);
   fChain->SetBranchAddress("Shw_ChargeU", &Shw_ChargeU, &b_Shw_ChargeU);
   fChain->SetBranchAddress("Shw_ChargeV", &Shw_ChargeV, &b_Shw_ChargeV);
   fChain->SetBranchAddress("Shw_ChargeZ", &Shw_ChargeZ, &b_Shw_ChargeZ);
   fChain->SetBranchAddress("Shw_CorrectedChargeU", &Shw_CorrectedChargeU, &b_Shw_CorrectedChargeU);
   fChain->SetBranchAddress("Shw_CorrectedChargeV", &Shw_CorrectedChargeV, &b_Shw_CorrectedChargeV);
   fChain->SetBranchAddress("Shw_CorrectedChargeZ", &Shw_CorrectedChargeZ, &b_Shw_CorrectedChargeZ);
   fChain->SetBranchAddress("Shw_EnergyU", &Shw_EnergyU, &b_Shw_EnergyU);
   fChain->SetBranchAddress("Shw_EnergyV", &Shw_EnergyV, &b_Shw_EnergyV);
   fChain->SetBranchAddress("Shw_EnergyZ", &Shw_EnergyZ, &b_Shw_EnergyZ);
   fChain->SetBranchAddress("Shw_CorrectedEnergyU", &Shw_CorrectedEnergyU, &b_Shw_CorrectedEnergyU);
   fChain->SetBranchAddress("Shw_CorrectedEnergyV", &Shw_CorrectedEnergyV, &b_Shw_CorrectedEnergyV);
   fChain->SetBranchAddress("Shw_CorrectedEnergyZ", &Shw_CorrectedEnergyZ, &b_Shw_CorrectedEnergyZ);
   fChain->SetBranchAddress("NHits_Shw", &NHits_Shw, &b_NHits_Shw);
   fChain->SetBranchAddress("Shw_Idx", Shw_Idx, &b_Shw_Idx);
   fChain->SetBranchAddress("Shw_StartX", Shw_StartX, &b_Shw_StartX);
   fChain->SetBranchAddress("Shw_StartY", Shw_StartY, &b_Shw_StartY);
   fChain->SetBranchAddress("Shw_StartZ", Shw_StartZ, &b_Shw_StartZ);
   fChain->SetBranchAddress("Shw_DirX", Shw_DirX, &b_Shw_DirX);
   fChain->SetBranchAddress("Shw_DirY", Shw_DirY, &b_Shw_DirY);
   fChain->SetBranchAddress("Shw_DirZ", Shw_DirZ, &b_Shw_DirZ);
   fChain->SetBranchAddress("Hit_Shw_TPC", Hit_Shw_TPC, &b_Hit_Shw_TPC);
   fChain->SetBranchAddress("Hit_Shw_Plane", Hit_Shw_Plane, &b_Hit_Shw_Plane);
   fChain->SetBranchAddress("Hit_Shw_Wire", Hit_Shw_Wire, &b_Hit_Shw_Wire);
   fChain->SetBranchAddress("Hit_Shw_Channel", Hit_Shw_Channel, &b_Hit_Shw_Channel);
   fChain->SetBranchAddress("Hit_Shw_Multi", Hit_Shw_Multi, &b_Hit_Shw_Multi);
   fChain->SetBranchAddress("Hit_Shw_TrueTrackID", Hit_Shw_TrueTrackID, &b_Hit_Shw_TrueTrackID);
   fChain->SetBranchAddress("Hit_Shw_ntrackID", Hit_Shw_ntrackID, &b_Hit_Shw_ntrackID);
   fChain->SetBranchAddress("Hit_Shw_PeakT", Hit_Shw_PeakT, &b_Hit_Shw_PeakT);
   fChain->SetBranchAddress("Hit_Shw_StartT", Hit_Shw_StartT, &b_Hit_Shw_StartT);
   fChain->SetBranchAddress("Hit_Shw_EndT", Hit_Shw_EndT, &b_Hit_Shw_EndT);
   fChain->SetBranchAddress("Hit_Shw_Charge", Hit_Shw_Charge, &b_Hit_Shw_Charge);
   fChain->SetBranchAddress("Hit_Shw_CorrCharge", Hit_Shw_CorrCharge, &b_Hit_Shw_CorrCharge);
   fChain->SetBranchAddress("Hit_Shw_Energy", Hit_Shw_Energy, &b_Hit_Shw_Energy);
   fChain->SetBranchAddress("Hit_Shw_trueE", Hit_Shw_trueE, &b_Hit_Shw_trueE);
   fChain->SetBranchAddress("Hit_Shw_PDG", Hit_Shw_PDG, &b_Hit_Shw_PDG);
   fChain->SetBranchAddress("Hit_Shw_PDG_Mom", Hit_Shw_PDG_Mom, &b_Hit_Shw_PDG_Mom);
   fChain->SetBranchAddress("Hit_Shw_Global_Plane", Hit_Shw_Global_Plane, &b_Hit_Shw_Global_Plane);
   fChain->SetBranchAddress("Hit_Shw_Global_Wire", Hit_Shw_Global_Wire, &b_Hit_Shw_Global_Wire);
   fChain->SetBranchAddress("Hit_Shw_Global_Tick", Hit_Shw_Global_Tick, &b_Hit_Shw_Global_Tick);
   fChain->SetBranchAddress("n_wires", &n_wires, &b_n_wires);
   fChain->SetBranchAddress("Hit_Nallraws", Hit_Nallraws, &b_Hit_Nallraws);
   fChain->SetBranchAddress("Hit_Nadcvec", Hit_Nadcvec, &b_Hit_Nadcvec);
   fChain->SetBranchAddress("Hit_raw_wireID", Hit_raw_wireID, &b_Hit_raw_wireID);
   fChain->SetBranchAddress("Hit_raw_chID", Hit_raw_chID, &b_Hit_raw_chID);
   fChain->SetBranchAddress("Hit_rawadc", &Hit_rawadc, &b_Hit_rawadc);
   fChain->SetBranchAddress("Hit_rawtick", &Hit_rawtick, &b_Hit_rawtick);
   fChain->SetBranchAddress("Wire_assn", Wire_assn, &b_Wire_assn);
   fChain->SetBranchAddress("Wire_min_tick", Wire_min_tick, &b_Wire_min_tick);
   fChain->SetBranchAddress("Wire_max_tick", Wire_max_tick, &b_Wire_max_tick);
   fChain->SetBranchAddress("Wire_max_rmsT", Wire_max_rmsT, &b_Wire_max_rmsT);
   fChain->SetBranchAddress("Wire_min_shwtick", Wire_min_shwtick, &b_Wire_min_shwtick);
   fChain->SetBranchAddress("Wire_max_shwtick", Wire_max_shwtick, &b_Wire_max_shwtick);
   fChain->SetBranchAddress("Wire_min_trktick", Wire_min_trktick, &b_Wire_min_trktick);
   fChain->SetBranchAddress("Wire_max_trktick", Wire_max_trktick, &b_Wire_max_trktick);
   fChain->SetBranchAddress("Wire_max_shwrms", Wire_max_shwrms, &b_Wire_max_shwrms);
   fChain->SetBranchAddress("Wire_max_trkrms", Wire_max_trkrms, &b_Wire_max_trkrms);
   fChain->SetBranchAddress("Wire_min_ledshwtick", Wire_min_ledshwtick, &b_Wire_min_ledshwtick);
   fChain->SetBranchAddress("Wire_max_ledshwtick", Wire_max_ledshwtick, &b_Wire_max_ledshwtick);
   fChain->SetBranchAddress("Wire_adc", &Wire_adc, &b_Wire_adc);
   fChain->SetBranchAddress("Wire_corradc", &Wire_corradc, &b_Wire_corradc);
   fChain->SetBranchAddress("Wire_tick", &Wire_tick, &b_Wire_tick);
   fChain->SetBranchAddress("Wire_shwflag", &Wire_shwflag, &b_Wire_shwflag);
   fChain->SetBranchAddress("Wire_ledshwflag", &Wire_ledshwflag, &b_Wire_ledshwflag);
   fChain->SetBranchAddress("Wire_x", &Wire_x, &b_Wire_x);
   fChain->SetBranchAddress("Wire_pdg", &Wire_pdg, &b_Wire_pdg);
   fChain->SetBranchAddress("Wire_pdg_mom", &Wire_pdg_mom, &b_Wire_pdg_mom);
   fChain->SetBranchAddress("Wire_dr", &Wire_dr, &b_Wire_dr);
   fChain->SetBranchAddress("Wire_dist", &Wire_dist, &b_Wire_dist);
   fChain->SetBranchAddress("Wire_tag_pdg", &Wire_tag_pdg, &b_Wire_tag_pdg);
   fChain->SetBranchAddress("Wire_tag_pdg_mom", &Wire_tag_pdg_mom, &b_Wire_tag_pdg_mom);
   fChain->SetBranchAddress("Wire_tag_is_shw", &Wire_tag_is_shw, &b_Wire_tag_is_shw);
   fChain->SetBranchAddress("Wire_tag_ith", &Wire_tag_ith, &b_Wire_tag_ith);
   fChain->SetBranchAddress("Wire_tag_true_px", &Wire_tag_true_px, &b_Wire_tag_true_px);
   fChain->SetBranchAddress("Wire_tag_true_py", &Wire_tag_true_py, &b_Wire_tag_true_py);
   fChain->SetBranchAddress("Wire_tag_true_pz", &Wire_tag_true_pz, &b_Wire_tag_true_pz);
   fChain->SetBranchAddress("Wire_tag_reco_px", &Wire_tag_reco_px, &b_Wire_tag_reco_px);
   fChain->SetBranchAddress("Wire_tag_reco_py", &Wire_tag_reco_py, &b_Wire_tag_reco_py);
   fChain->SetBranchAddress("Wire_tag_reco_pz", &Wire_tag_reco_pz, &b_Wire_tag_reco_pz);
   fChain->SetBranchAddress("Wire_tag_reco_eng", &Wire_tag_reco_eng, &b_Wire_tag_reco_eng);
   fChain->SetBranchAddress("Ch_charge", &Ch_charge, &b_Ch_charge);
   fChain->SetBranchAddress("Ch_tick", &Ch_tick, &b_Ch_tick);
   fChain->SetBranchAddress("Wire_ROIfirst", Wire_ROIfirst, &b_Wire_ROIfirst);
   fChain->SetBranchAddress("Wire_ROIend", Wire_ROIend, &b_Wire_ROIend);
   fChain->SetBranchAddress("Sum_chchg", Sum_chchg, &b_Sum_chchg);
   fChain->SetBranchAddress("TrueParticle_size", &TrueParticle_size, &b_TrueParticle_size);
   fChain->SetBranchAddress("TruePart_PDG", &TruePart_PDG, &b_TruePart_PDG);
   fChain->SetBranchAddress("TruePart_E", &TruePart_E, &b_TruePart_E);
   fChain->SetBranchAddress("ChgU", &ChgU, &b_ChgU);
   fChain->SetBranchAddress("ChgV", &ChgV, &b_ChgV);
   fChain->SetBranchAddress("ChgZ", &ChgZ, &b_ChgZ);
   fChain->SetBranchAddress("n_tracks_pm", &n_tracks_pm, &b_n_tracks_pm);
   fChain->SetBranchAddress("n_pand_par", &n_pand_par, &b_n_pand_par);
   fChain->SetBranchAddress("n_prims", &n_prims, &b_n_prims);
   fChain->SetBranchAddress("m_pf_vtx", &m_pf_vtx, &b_m_pf_vtx);
   fChain->SetBranchAddress("m_pf_vtx_x", &m_pf_vtx_x, &b_m_pf_vtx_x);
   fChain->SetBranchAddress("m_pf_vtx_y", &m_pf_vtx_y, &b_m_pf_vtx_y);
   fChain->SetBranchAddress("m_pf_vtx_z", &m_pf_vtx_z, &b_m_pf_vtx_z);
   fChain->SetBranchAddress("PFVtxTPC", PFVtxTPC, &b_PFVtxTPC);
   fChain->SetBranchAddress("PFVtxWire", PFVtxWire, &b_PFVtxWire);
   fChain->SetBranchAddress("PFVtxTime", PFVtxTime, &b_PFVtxTime);
   fChain->SetBranchAddress("PFVtxGlobalWire", PFVtxGlobalWire, &b_PFVtxGlobalWire);
   fChain->SetBranchAddress("PFVtxGlobalTick", PFVtxGlobalTick, &b_PFVtxGlobalTick);
   fChain->SetBranchAddress("PFVtxGlobalPlane", PFVtxGlobalPlane, &b_PFVtxGlobalPlane);
   Notify();
}

Bool_t analysis_RecoEnergyStudies_outputs::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void analysis_RecoEnergyStudies_outputs::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t analysis_RecoEnergyStudies_outputs::Cut(Long64_t /*entry*/)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef analysis_RecoEnergyStudies_outputs_cxx
