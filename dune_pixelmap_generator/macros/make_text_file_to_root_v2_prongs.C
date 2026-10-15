// add ievent log output
// change adc charge cut from 0 to -500

const int kMaxClst = 100;
const int nParticleClasses = 11;

void make_text_file_to_root_v2_prongs(TString input, TString outtext, string type="nue", bool hit_based_pixelmap = true){
    bool cutNC = true;
    cout << "Start Making Text File" << endl;
	TStopwatch gtimer;
	gtimer.Start();


	TString name_tree;
	if (input.Contains(".skim.")) name_tree = "WC";
	else name_tree = "recoEnergy/WC";

	TChain *ch = new TChain("recoEnergy/WC");
	ch->Add(input);
        cout << ">>>" << input.Data() << endl;
	ch->SetMakeClass(1);

	TFile *output = new TFile(outtext.Data(), "recreate");
	TTree *newtree = new TTree("pixelmap", "pixelmap");
	output->SetCompressionLevel(9);
	cout << output->GetCompressionAlgorithm() << endl;

	int f_ievt, f_tpc_id, f_iplane, f_shw_id, f_unique_id;
	float f_shw_tot_energy;
	int f_nwires, f_nticks;
	int f_wcoarse, f_tcoarse;
	int f_rel_wire, f_rel_tick, f_mean_wire, f_mean_tick;
	int f_mean_tpc;
	float f_wire_charge, f_wire_corrcharge, f_hit_tot_energy;
	float f_trueEnergy, f_trueVertex_X, f_trueVertex_Y, f_trueVertex_Z, f_truePx, f_truePy, f_truePz;
	int f_trueVtxWire, f_trueVtxTick;
	int f_trueVtxGlobalWire, f_trueVtxGlobalTick;
	int f_pfVtxGlobalWire, f_pfVtxGlobalTick;
	int f_truemode, f_trueccnc, f_truenupdg;

	int f_trueleppdg, f_nshowers, f_shwflag;
	float f_truelepeng, f_trueleppx, f_trueleppy, f_trueleppz;
	float f_shweng, f_shwpx, f_shwpy, f_shwpz;
	int f_offsetU, f_offsetV, f_offsetZ;
	float f_recoNu_E, f_recoLep_E, f_recoHad_E, f_recoVertex_X, f_recoVertex_Y, f_recoVertex_Z;
	int f_trueTPCin, f_trueDETin, f_trackCont;
	int f_prong_tag;
        float f_prong_eng, f_prong_length, f_prong_startx, f_prong_starty, f_prong_startz;
        float f_prong_px, f_prong_py, f_prong_pz;
        int f_prong_true_pdg, f_prong_true_pdg_mom;
        float f_prong_true_Efrac, f_prong_true_eng;
        float f_prong_true_px, f_prong_true_py, f_prong_true_pz;
        float f_prong_true_mom_px, f_prong_true_mom_py, f_prong_true_mom_pz;
        int f_prong2_true_pdg, f_prong2_true_pdg_mom;
        float f_prong2_true_Efrac, f_prong2_true_eng;
        float f_prong2_true_px, f_prong2_true_py, f_prong2_true_pz;
        float f_prong2_true_mom_px, f_prong2_true_mom_py, f_prong2_true_mom_pz;

        int f_is_leading_shower;

        int maxNumberOfDaughters = 20;
        int numberOfDaugters;
        int daughterPDG[maxNumberOfDaughters];
        float daughterE[maxNumberOfDaughters];
        float daughterDepoE[maxNumberOfDaughters];
        float daughterPx[maxNumberOfDaughters];
        float daughterPy[maxNumberOfDaughters];
        float daughterPz[maxNumberOfDaughters];


	newtree->Branch("ievent", &f_ievt, "ievent/I");
	newtree->Branch("tpc_id", &f_tpc_id, "tpc_id/I");
	newtree->Branch("iplane", &f_iplane, "iplane/I");
	newtree->Branch("shw_id", &f_shw_id, "shw_id/I");


	newtree->Branch("nwires", &f_nwires, "nwires/I");
	newtree->Branch("nticks", &f_nticks, "nticks/I");
	newtree->Branch("wcoarse", &f_wcoarse, "wcoarse/I");
	newtree->Branch("tcoarse", &f_tcoarse, "tcoarse/I");

	newtree->Branch("rel_wire", &f_rel_wire, "rel_wire/I");
	newtree->Branch("rel_tick", &f_rel_tick, "rel_tick/I");
	newtree->Branch("mean_wire", &f_mean_wire, "mean_wire/I");
	newtree->Branch("mean_tick", &f_mean_tick, "mean_tick/I");
	newtree->Branch("mean_tpc",  &f_mean_tpc, "mean_tpc/I");
	newtree->Branch("trueVtxWire", &f_trueVtxWire, "trueVtxWire/I");
	newtree->Branch("trueVtxTick", &f_trueVtxTick, "trueVtxTick/I");
	newtree->Branch("trueVtxGlobalWire", &f_trueVtxGlobalWire, "trueVtxGlobalWire/I");
	newtree->Branch("trueVtxGlobalTick", &f_trueVtxGlobalTick, "trueVtxGlobalTick/I");
	newtree->Branch("pfVtxGlobalWire", &f_pfVtxGlobalWire, "pfVtxGlobalWire/I");
	newtree->Branch("pfVtxGlobalTick", &f_pfVtxGlobalTick, "pfVtxGlobalTick/I");
	newtree->Branch("shw_tot_energy", &f_shw_tot_energy, "shw_tot_energy/F");
	newtree->Branch("wire_charge", &f_wire_charge, "wire_charge/F");
	newtree->Branch("wire_corrcharge", &f_wire_corrcharge, "wire_corrcharge/F");
	newtree->Branch("hit_tot_energy", &f_hit_tot_energy, "hit_tot_energy/F");
	newtree->Branch("trueEnergy", &f_trueEnergy, "trueEnergy/F");
	newtree->Branch("trueVertex_X", &f_trueVertex_X, "trueVertex_X/F");
	newtree->Branch("trueVertex_Y", &f_trueVertex_Y, "trueVertex_Y/F");
	newtree->Branch("trueVertex_Z", &f_trueVertex_Z, "trueVertex_Z/F");
	newtree->Branch("truePx", &f_truePx, "truePx/F");
	newtree->Branch("truePy", &f_truePy, "truePy/F");
	newtree->Branch("truePz", &f_truePz, "truePz/F");
	newtree->Branch("trueMode",  &f_truemode,  "trueMode/I");
	newtree->Branch("trueCCNC",  &f_trueccnc,  "trueCCNC/I");
	newtree->Branch("trueNuPDG", &f_truenupdg, "trueNuPDG/I");

	newtree->Branch("trueLepPDG", &f_trueleppdg, "trueLepPDG/I");
	newtree->Branch("trueLepEng", &f_truelepeng, "trueLepEng/F");
	newtree->Branch("trueLepPx", &f_trueleppx, "trueLepPx/F");
	newtree->Branch("trueLepPy", &f_trueleppy, "trueLepPy/F");
	newtree->Branch("trueLepPz", &f_trueleppz, "trueLepPz/F");
	newtree->Branch("NShw",      &f_nshowers, "NShw/I");
	newtree->Branch("ShwFlag",   &f_shwflag,  "ShwFlag/I");
	newtree->Branch("ShwEng",    &f_shweng,   "ShwEng/F");
	newtree->Branch("ShwPx",     &f_shwpx,   "ShwPx/F");
	newtree->Branch("ShwPy",     &f_shwpy,   "ShwPy/F");
	newtree->Branch("ShwPz",     &f_shwpz,   "ShwPz/F");

	newtree->Branch("OffsetU",   &f_offsetU, "OffsetU/I");
	newtree->Branch("OffsetV",   &f_offsetV, "OffsetV/I");
	newtree->Branch("OffsetZ",   &f_offsetZ, "OffsetZ/I");

	newtree->Branch("recoNu_E",     &f_recoNu_E,     "recoNu_E/F");
	newtree->Branch("recoVertex_X", &f_recoVertex_X, "recoVertex_X/F");
	newtree->Branch("recoVertex_Y", &f_recoVertex_Y, "recoVertex_Y/F");
	newtree->Branch("recoVertex_Z", &f_recoVertex_Z, "recoVertex_Z/F");

	newtree->Branch("trueTPCin",    &f_trueTPCin,     "trueTPCin/I");
	newtree->Branch("trueDETin",    &f_trueDETin,     "trueDETin/I");
	newtree->Branch("trackCont",    &f_trackCont,      "trackCont/I");

	newtree->Branch("prong_tag",          &f_prong_tag,          "prong_tag/I");
	newtree->Branch("prong_eng",          &f_prong_eng,          "prong_eng/F");
	newtree->Branch("prong_length",       &f_prong_length,       "prong_length/F");
	newtree->Branch("prong_startx",       &f_prong_startx,       "prong_startx/F");
	newtree->Branch("prong_starty",       &f_prong_starty,       "prong_starty/F");
	newtree->Branch("prong_startz",       &f_prong_startz,       "prong_startz/F");
	newtree->Branch("prong_px",           &f_prong_px,           "prong_px/F");
	newtree->Branch("prong_py",           &f_prong_py,           "prong_py/F");
	newtree->Branch("prong_pz",           &f_prong_pz,           "prong_pz/F");
	newtree->Branch("prong_true_Efrac",   &f_prong_true_Efrac,   "prong_true_Efrac/F");
	newtree->Branch("prong_true_pdg",     &f_prong_true_pdg,     "prong_true_pdg/I");
	newtree->Branch("prong_true_pdg_mom", &f_prong_true_pdg_mom, "prong_true_pdg_mom/I");
	newtree->Branch("prong_true_eng",     &f_prong_true_eng,     "prong_true_eng/F");
	newtree->Branch("prong_true_px",      &f_prong_true_px,      "prong_true_px/F");
	newtree->Branch("prong_true_py",      &f_prong_true_py,      "prong_true_py/F");
	newtree->Branch("prong_true_pz",      &f_prong_true_pz,      "prong_true_pz/F");
	newtree->Branch("prong_true_mom_px",  &f_prong_true_mom_px,  "prong_true_mom_px/F");
	newtree->Branch("prong_true_mom_py",  &f_prong_true_mom_py,  "prong_true_mom_py/F");
	newtree->Branch("prong_true_mom_pz",  &f_prong_true_mom_pz,  "prong_true_mom_pz/F");
	newtree->Branch("prong2_true_Efrac",   &f_prong2_true_Efrac,   "prong2_true_Efrac/F");
	newtree->Branch("prong2_true_pdg",     &f_prong2_true_pdg,     "prong2_true_pdg/I");
	newtree->Branch("prong2_true_pdg_mom", &f_prong2_true_pdg_mom, "prong2_true_pdg_mom/I");
	newtree->Branch("prong2_true_eng",     &f_prong2_true_eng,     "prong2_true_eng/F");
	newtree->Branch("prong2_true_px",      &f_prong2_true_px,      "prong2_true_px/F");
	newtree->Branch("prong2_true_py",      &f_prong2_true_py,      "prong2_true_py/F");
	newtree->Branch("prong2_true_pz",      &f_prong2_true_pz,      "prong2_true_pz/F");
	newtree->Branch("prong2_true_mom_px",  &f_prong2_true_mom_px,  "prong2_true_mom_px/F");
	newtree->Branch("prong2_true_mom_py",  &f_prong2_true_mom_py,  "prong2_true_mom_py/F");
	newtree->Branch("prong2_true_mom_pz",  &f_prong2_true_mom_pz,  "prong2_true_mom_pz/F");

    newtree->Branch("unique_id", &f_unique_id, "unique_id/I");

    newtree->Branch("recoLep_E",     &f_recoLep_E,     "recoNu_E/F");
    newtree->Branch("recoHad_E",     &f_recoHad_E,     "recoNu_E/F");

    newtree->Branch("is_leading_shower", &f_is_leading_shower, "is_leading_shower/I");

    newtree->Branch("numberOfDaugters", &numberOfDaugters, "numberOfDaugters/I");
    newtree->Branch("daughterPDG", &daughterPDG, "daughterPDG[numberOfDaugters]/I");
    newtree->Branch("daughterE", &daughterE, "daughterE[numberOfDaugters]/F");
    newtree->Branch("daughterDepoE", &daughterDepoE, "daughterDepoE[numberOfDaugters]/F");
    newtree->Branch("daughterPx", &daughterPx, "daughterPx[numberOfDaugters]/F");
    newtree->Branch("daughterPy", &daughterPy, "daughterPy[numberOfDaugters]/F");
    newtree->Branch("daughterPz", &daughterPz, "daughterPz[numberOfDaugters]/F");


	const int kMaxHits = 50000;
	const int kMax = 3000;

        int ievt;
        int n_tpcs_evt;
        int nhits;


        double ChargeU;
        double ChargeV;
        double ChargeZ;
        double correctedChargeU;
        double correctedChargeV;
        double correctedChargeZ;
        double EnergyU;
        double EnergyV;
        double EnergyZ;
        double correctedEnergyU;
        double correctedEnergyV;
        double correctedEnergyZ;

        double shw_ChargeU;
        double shw_ChargeV;
        double shw_ChargeZ;
        double shw_correctedChargeU;
        double shw_correctedChargeV;
        double shw_correctedChargeZ;
        double shw_EnergyU;
        double shw_EnergyV;
        double shw_EnergyZ;
        double shw_correctedEnergyU;
        double shw_correctedEnergyV;
        double shw_correctedEnergyZ;

	int nu_truth_N;
	double nueng_truth[10000];
	int numode_truth[10000];
	int nuccnc_truth[10000];
	int nupdg_truth[10000];
	int leppdg_truth[10000];
	double lepp0_truth[10000];
	double lepp1_truth[10000];
	double lepp2_truth[10000];
	double lepeng_truth[10000];

        double trueEnergy;
        double trueVertex_X;
        double trueVertex_Y;
        double trueVertex_Z;
        double trueEnd_X;
        double trueEnd_Y;
        double trueEnd_Z;
        double truePx;
        double truePy;
        double truePz;

	int vtx_in_tpc;
	int nutrue_fid;
	int mutrack_cont;
        double trueVtxWire[3];
        double trueVtxTPC[3];
        double trueVtxTime[3];
	double vtxdetdist;


        double ErecoNu, RecoLepEnNu, RecoHadEnNu;
        double recoVertex_X;
        double recoVertex_Y;
        double recoVertex_Z;


        int hit_tpc[kMaxHits];
        int hit_plane[kMaxHits];
        int hit_wire[kMaxHits];
        int hit_channel[kMaxHits];
        float hit_peakT[kMaxHits];
        float hit_charge[kMaxHits];
        float hit_corrcharge[kMaxHits];
        double hit_energy[kMaxHits];
        double hit_trueE[kMaxHits];
        float hit_startT[kMaxHits];
        float hit_endT[kMaxHits];
        int    hit_prong_tag[kMaxHits];
        float hit_rmsT[kMaxHits];

	int led_shw_idx;
	double led_shw_eng;
	double led_shw_dirx;
	double led_shw_diry;
	double led_shw_dirz;

        int n_wires;
        int wire_assn[kMaxHits];
        std::vector<std::vector<double> > *wire_corradc;
        std::vector<std::vector<double> > *wire_adc;
        std::vector<std::vector<int> > *wire_tick;
        std::vector<std::vector<double> > *wire_x;
        std::vector<std::vector<int> > *wire_shwflag;
        std::vector<std::vector<int> > *wire_ledshwflag;
        std::vector<std::vector<int> > *wire_tag_is_shw;
        std::vector<std::vector<int> > *wire_tag_ith;


	double hit_global_wire[kMaxHits];
	double hit_global_tick[kMaxHits];
	double hit_global_plane[kMaxHits];
	double trueVtxGlobalWire[3];
	double trueVtxGlobalTick[3];
	double trueVtxGlobalPlane[3];
	int    pfVtxTPC[3];
	double pfVtxGlobalWire[3];
	double pfVtxGlobalTick[3];
	double pfVtxGlobalPlane[3];
	double hit_offset[3];

	int    n_showers;
        double all_shw_eng[kMax];
        float all_shw_length[kMax];
        double all_shw_startx[kMax];
        double all_shw_starty[kMax];
        double all_shw_startz[kMax];
        double all_shw_px[kMax];
        double all_shw_py[kMax];
        double all_shw_pz[kMax];
        double all_shw_dist_reco_vtx[kMax];
        double all_shw_dist_true_vtx[kMax];

        double all_shw_true_Efrac[kMax];
        int all_shw_true_pdg[kMax];
        int all_shw_true_pdg_mom[kMax];
        double all_shw_true_eng[kMax];
        double all_shw_true_px[kMax];
        double all_shw_true_py[kMax];
        double all_shw_true_pz[kMax];
        double all_shw_true_mom_px[kMax];
        double all_shw_true_mom_py[kMax];
        double all_shw_true_mom_pz[kMax];

        double all_shw2_true_Efrac[kMax];
        int all_shw2_true_pdg[kMax];
        int all_shw2_true_pdg_mom[kMax];
        double all_shw2_true_eng[kMax];
        double all_shw2_true_px[kMax];
        double all_shw2_true_py[kMax];
        double all_shw2_true_pz[kMax];
        double all_shw2_true_mom_px[kMax];
        double all_shw2_true_mom_py[kMax];
        double all_shw2_true_mom_pz[kMax];

        double all_trk_calmom[kMax];
        float all_trk_length[kMax];
        double all_trk_startx[kMax];
        double all_trk_starty[kMax];
        double all_trk_startz[kMax];
        double all_trk_px[kMax];
        double all_trk_py[kMax];
        double all_trk_pz[kMax];
        double all_trk_dist_reco_vtx[kMax];
        double all_trk_dist_true_vtx[kMax];

        double all_trk_true_Efrac[kMax];
        int all_trk_true_pdg[kMax];
        int all_trk_true_pdg_mom[kMax];
        double all_trk_true_eng[kMax];
        double all_trk_true_px[kMax];
        double all_trk_true_py[kMax];
        double all_trk_true_pz[kMax];
        double all_trk_true_mom_px[kMax];
        double all_trk_true_mom_py[kMax];
        double all_trk_true_mom_pz[kMax];

        double all_trk2_true_Efrac[kMax];
        int all_trk2_true_pdg[kMax];
        int all_trk2_true_pdg_mom[kMax];
        double all_trk2_true_eng[kMax];
        double all_trk2_true_px[kMax];
        double all_trk2_true_py[kMax];
        double all_trk2_true_pz[kMax];
        double all_trk2_true_mom_px[kMax];
        double all_trk2_true_mom_py[kMax];
        double all_trk2_true_mom_pz[kMax];

        int leadingShowerID;
        std::vector<int> *trueDaughterPDGs;
        std::vector<float> *trueDaughterE;
        std::vector<float> *trueDaughterDepoE;
        std::vector<float> *trueDaughterPx;
        std::vector<float> *trueDaughterPy;
        std::vector<float> *trueDaughterPz;


        ch->SetBranchAddress("ievent", &ievt);
        ch->SetBranchAddress("NHits", &nhits);
        ch->SetBranchAddress("NTPCs_evt", &n_tpcs_evt);

        ch->SetBranchAddress("ChargeU", &ChargeU);
        ch->SetBranchAddress("ChargeV", &ChargeV);
        ch->SetBranchAddress("ChargeZ", &ChargeZ);
        ch->SetBranchAddress("CorrectedChargeU", &correctedChargeU);
        ch->SetBranchAddress("CorrectedChargeV", &correctedChargeV);
        ch->SetBranchAddress("CorrectedChargeZ", &correctedChargeZ);
        ch->SetBranchAddress("EnergyU", &EnergyU);
        ch->SetBranchAddress("EnergyV", &EnergyV);
        ch->SetBranchAddress("EnergyZ", &EnergyZ);
        ch->SetBranchAddress("CorrectedEnergyU", &correctedEnergyU);
        ch->SetBranchAddress("CorrectedEnergyV", &correctedEnergyV);
        ch->SetBranchAddress("CorrectedEnergyZ", &correctedEnergyZ);

        ch->SetBranchAddress("Shw_ChargeU", &shw_ChargeU);
        ch->SetBranchAddress("Shw_ChargeV", &shw_ChargeV);
        ch->SetBranchAddress("Shw_ChargeZ", &shw_ChargeZ);
        ch->SetBranchAddress("Shw_CorrectedChargeU", &shw_correctedChargeU);
        ch->SetBranchAddress("Shw_CorrectedChargeV", &shw_correctedChargeV);
        ch->SetBranchAddress("Shw_CorrectedChargeZ", &shw_correctedChargeZ);
        ch->SetBranchAddress("Shw_EnergyU", &shw_EnergyU);
        ch->SetBranchAddress("Shw_EnergyV", &shw_EnergyV);
        ch->SetBranchAddress("Shw_EnergyZ", &shw_EnergyZ);
        ch->SetBranchAddress("Shw_CorrectedEnergyU", &shw_correctedEnergyU);
        ch->SetBranchAddress("Shw_CorrectedEnergyV", &shw_correctedEnergyV);
        ch->SetBranchAddress("Shw_CorrectedEnergyZ", &shw_correctedEnergyZ);

        ch->SetBranchAddress("NuTruthN",    &nu_truth_N);
        ch->SetBranchAddress("NuEngTruth",  nueng_truth);
	ch->SetBranchAddress("NuModeTruth", numode_truth);
	ch->SetBranchAddress("NuCCNCTruth", nuccnc_truth);
	ch->SetBranchAddress("NuPDGTruth",  nupdg_truth);
	ch->SetBranchAddress("LepPDGTruth", leppdg_truth);
	ch->SetBranchAddress("LepP0Truth",  lepp0_truth);
	ch->SetBranchAddress("LepP1Truth",  lepp1_truth);
	ch->SetBranchAddress("LepP2Truth",  lepp2_truth);
	ch->SetBranchAddress("LepEngTruth", lepeng_truth);

        ch->SetBranchAddress("TrueEnergy", &trueEnergy);
        ch->SetBranchAddress("TrueVertexX", &trueVertex_X);
        ch->SetBranchAddress("TrueVertexY", &trueVertex_Y);
        ch->SetBranchAddress("TrueVertexZ", &trueVertex_Z);
        ch->SetBranchAddress("TrueEndX", &trueEnd_X);
        ch->SetBranchAddress("TrueEndY", &trueEnd_Y);
        ch->SetBranchAddress("TrueEndZ", &trueEnd_Z);
        ch->SetBranchAddress("TruePx",   &truePx);
        ch->SetBranchAddress("TruePy",   &truePy);
        ch->SetBranchAddress("TruePz",   &truePz);

        ch->SetBranchAddress("NuTruthFid",   &nutrue_fid);

        ch->SetBranchAddress("TrueVtxin",    &vtx_in_tpc);
        ch->SetBranchAddress("TrueVtxWire",  trueVtxWire);
        ch->SetBranchAddress("TrueVtxTPC",   trueVtxTPC);
        ch->SetBranchAddress("TrueVtxTime",  trueVtxTime);
        ch->SetBranchAddress("VertexDetectorDist",  &vtxdetdist);
	ch->SetBranchAddress("MuTrackCont", &mutrack_cont);

        ch->SetBranchAddress("Hit_Prong_Tag", hit_prong_tag);
        ch->SetBranchAddress("Hit_StartT", hit_startT);
        ch->SetBranchAddress("Hit_EndT", hit_endT);
        ch->SetBranchAddress("Hit_TPC", hit_tpc);
        ch->SetBranchAddress("Hit_Plane", hit_plane);
        ch->SetBranchAddress("Hit_Wire", hit_wire);
        ch->SetBranchAddress("Hit_Channel", hit_channel);
        ch->SetBranchAddress("Hit_PeakT", hit_peakT);
        ch->SetBranchAddress("Hit_Charge", hit_charge);
        ch->SetBranchAddress("Hit_CorrCharge", hit_corrcharge);
        ch->SetBranchAddress("Hit_Energy", hit_energy);
        ch->SetBranchAddress("Hit_trueE", hit_trueE);
        ch->SetBranchAddress("Hit_Global_Wire",  hit_global_wire);
        ch->SetBranchAddress("Hit_Global_Tick",  hit_global_tick);
        ch->SetBranchAddress("Hit_Global_Plane", hit_global_plane);
        ch->SetBranchAddress("Hit_RMST", hit_rmsT);

        ch->SetBranchAddress("led_shw_idx",  &led_shw_idx);
        ch->SetBranchAddress("led_shw_eng",  &led_shw_eng);
        ch->SetBranchAddress("led_shw_dirx", &led_shw_dirx);
        ch->SetBranchAddress("led_shw_diry", &led_shw_diry);
        ch->SetBranchAddress("led_shw_dirz", &led_shw_dirz);

        ch->SetBranchAddress("n_showers",      &n_showers);
        ch->SetBranchAddress("all_shw_eng",    all_shw_eng);
        ch->SetBranchAddress("all_shw_length", all_shw_length);
        ch->SetBranchAddress("all_shw_startx", all_shw_startx);
        ch->SetBranchAddress("all_shw_starty", all_shw_starty);
        ch->SetBranchAddress("all_shw_startz", all_shw_startz);
        ch->SetBranchAddress("all_shw_px", all_shw_px);
        ch->SetBranchAddress("all_shw_py", all_shw_py);
        ch->SetBranchAddress("all_shw_pz", all_shw_pz);
        ch->SetBranchAddress("all_shw_dist_reco_vtx", all_shw_dist_reco_vtx);
        ch->SetBranchAddress("all_shw_dist_true_vtx", all_shw_dist_true_vtx);
        ch->SetBranchAddress("all_shw_true_Efrac", all_shw_true_Efrac);
        ch->SetBranchAddress("all_shw_true_pdg", all_shw_true_pdg);
        ch->SetBranchAddress("all_shw_true_pdg_mom", all_shw_true_pdg_mom);
        ch->SetBranchAddress("all_shw_true_eng", all_shw_true_eng);
        ch->SetBranchAddress("all_shw_true_px", all_shw_true_px);
        ch->SetBranchAddress("all_shw_true_py", all_shw_true_py);
        ch->SetBranchAddress("all_shw_true_pz", all_shw_true_pz);
        ch->SetBranchAddress("all_shw_true_mom_px", all_shw_true_mom_px);
        ch->SetBranchAddress("all_shw_true_mom_py", all_shw_true_mom_py);
        ch->SetBranchAddress("all_shw_true_mom_pz", all_shw_true_mom_pz);
        ch->SetBranchAddress("all_shw2_true_Efrac", all_shw2_true_Efrac);
        ch->SetBranchAddress("all_shw2_true_pdg", all_shw2_true_pdg);
        ch->SetBranchAddress("all_shw2_true_pdg_mom", all_shw2_true_pdg_mom);
        ch->SetBranchAddress("all_shw2_true_eng", all_shw2_true_eng);
        ch->SetBranchAddress("all_shw2_true_px", all_shw2_true_px);
        ch->SetBranchAddress("all_shw2_true_py", all_shw2_true_py);
        ch->SetBranchAddress("all_shw2_true_pz", all_shw2_true_pz);
        ch->SetBranchAddress("all_shw2_true_mom_px", all_shw2_true_mom_px);
        ch->SetBranchAddress("all_shw2_true_mom_py", all_shw2_true_mom_py);
        ch->SetBranchAddress("all_shw2_true_mom_pz", all_shw2_true_mom_pz);

        ch->SetBranchAddress("all_track_calmom",    all_trk_calmom);
        ch->SetBranchAddress("all_track_length", all_trk_length);
        ch->SetBranchAddress("all_track_startx", all_trk_startx);
        ch->SetBranchAddress("all_track_starty", all_trk_starty);
        ch->SetBranchAddress("all_track_startz", all_trk_startz);
        ch->SetBranchAddress("all_track_px", all_trk_px);
        ch->SetBranchAddress("all_track_py", all_trk_py);
        ch->SetBranchAddress("all_track_pz", all_trk_pz);
        ch->SetBranchAddress("all_track_dist_reco_vtx", all_trk_dist_reco_vtx);
        ch->SetBranchAddress("all_track_dist_true_vtx", all_trk_dist_true_vtx);
        ch->SetBranchAddress("all_track_true_Efrac", all_trk_true_Efrac);
        ch->SetBranchAddress("all_track_true_pdg", all_trk_true_pdg);
        ch->SetBranchAddress("all_track_true_pdg_mom", all_trk_true_pdg_mom);
        ch->SetBranchAddress("all_track_true_eng", all_trk_true_eng);
        ch->SetBranchAddress("all_track_true_px", all_trk_true_px);
        ch->SetBranchAddress("all_track_true_py", all_trk_true_py);
        ch->SetBranchAddress("all_track_true_pz", all_trk_true_pz);
        ch->SetBranchAddress("all_track_true_mom_px", all_trk_true_mom_px);
        ch->SetBranchAddress("all_track_true_mom_py", all_trk_true_mom_py);
        ch->SetBranchAddress("all_track_true_mom_pz", all_trk_true_mom_pz);
        ch->SetBranchAddress("all_track2_true_Efrac", all_trk2_true_Efrac);
        ch->SetBranchAddress("all_track2_true_pdg", all_trk2_true_pdg);
        ch->SetBranchAddress("all_track2_true_pdg_mom", all_trk2_true_pdg_mom);
        ch->SetBranchAddress("all_track2_true_eng", all_trk2_true_eng);
        ch->SetBranchAddress("all_track2_true_px", all_trk2_true_px);
        ch->SetBranchAddress("all_track2_true_py", all_trk2_true_py);
        ch->SetBranchAddress("all_track2_true_pz", all_trk2_true_pz);
        ch->SetBranchAddress("all_track2_true_mom_px", all_trk2_true_mom_px);
        ch->SetBranchAddress("all_track2_true_mom_py", all_trk2_true_mom_py);
        ch->SetBranchAddress("all_track2_true_mom_pz", all_trk2_true_mom_pz);


        ch->SetBranchAddress("ErecoNu",   &ErecoNu);
        ch->SetBranchAddress("RecoLepEnNu",   &RecoLepEnNu);
        ch->SetBranchAddress("RecoHadEnNu",   &RecoHadEnNu);
        ch->SetBranchAddress("m_pf_vtx_x", &recoVertex_X);
        ch->SetBranchAddress("m_pf_vtx_y", &recoVertex_Y);
        ch->SetBranchAddress("m_pf_vtx_z", &recoVertex_Z);

        wire_adc = 0;
	wire_tick = 0;
	wire_corradc = 0;
	wire_shwflag = 0;
	wire_ledshwflag = 0;
	wire_tag_is_shw = 0;
	wire_tag_ith = 0;

        ch->SetBranchAddress("n_wires", &n_wires);
        ch->SetBranchAddress("Wire_assn", wire_assn);
        ch->SetBranchAddress("Wire_adc", &wire_adc);
        ch->SetBranchAddress("Wire_tick", &wire_tick);
        ch->SetBranchAddress("Wire_corradc", &wire_corradc);
        ch->SetBranchAddress("Wire_shwflag", &wire_shwflag);
        ch->SetBranchAddress("Wire_ledshwflag",   &wire_ledshwflag);
        ch->SetBranchAddress("Wire_tag_is_shw",   &wire_tag_is_shw);
        ch->SetBranchAddress("Wire_tag_ith",      &wire_tag_ith);

        ch->SetBranchAddress("Hit_offset",        hit_offset);
        ch->SetBranchAddress("TrueVtxGlobalWire", trueVtxGlobalWire);
        ch->SetBranchAddress("TrueVtxGlobalTick", trueVtxGlobalTick);
        ch->SetBranchAddress("TrueVtxGlobalPlane", trueVtxGlobalPlane);
        ch->SetBranchAddress("PFVtxTPC",        pfVtxTPC);
        ch->SetBranchAddress("PFVtxGlobalWire", pfVtxGlobalWire);
        ch->SetBranchAddress("PFVtxGlobalTick", pfVtxGlobalTick);
        ch->SetBranchAddress("PFVtxGlobalPlane", pfVtxGlobalPlane);

        ch->SetBranchAddress("leadingShowerID", &leadingShowerID);

        ch->SetBranchAddress("trueDaughterPDGs", &trueDaughterPDGs);
        ch->SetBranchAddress("trueDaughterE", &trueDaughterE);
        ch->SetBranchAddress("trueDaughterDepoE", &trueDaughterDepoE);
        ch->SetBranchAddress("trueDaughterPx", &trueDaughterPx);
        ch->SetBranchAddress("trueDaughterPy", &trueDaughterPy);
        ch->SetBranchAddress("trueDaughterPz", &trueDaughterPz);


	// for nue
	int x_coarse = 1;
	int y_coarse = 6;
	int xbins0 = 400;
	int ybins0 = 280*6; // 1680
	if (type == "nue"){
	    // default setting is for nue
	} else if (type == "numu"){
	    x_coarse = 7;
	    y_coarse = 24;
	    xbins0 = 400*x_coarse;
	    ybins0 = 280*y_coarse;
	} else {
	    std::cout << "error unknown type. It should be nue or numu" << std::endl;
	    std::abort();
	}
        const int xnbins = xbins0/x_coarse;
        const int ynbins = ybins0/y_coarse;
        int xmax = xnbins/2;
        int ymax = ynbins/2;

	std::cout << "Actual Pixel Map Converage (wire,tick): " << xbins0 << " , " << ybins0 << std::endl;
	std::cout << "Pixel Map Size (wire,tick): " << xnbins << " , " << ynbins << std::endl;


	// charge
	TH2D *h2_evt1[3];
	// corrcharge
	TH2D *h2_evt2[3];
	TH2D *h2_shw1[3];
	TH2D *h2_tag[3];
	for (int ip = 0; ip < 3; ip++){
		h2_evt1[ip] = new TH2D(Form("h2_evt1_%d",ip),";Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
		h2_evt2[ip] = new TH2D(Form("h2_evt2_%d",ip),";Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
		h2_shw1[ip] = new TH2D(Form("h2_shw1_%d",ip),";Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
		h2_tag[ip] = new TH2D(Form("h2_tag_%d",ip),";Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	}

	cout << "Start Loop" << endl;
	int entries = ch->GetEntries();
	cout << "Entries: " << entries << endl;

    f_unique_id = 0;

	for (int ie = 0; ie < entries; ie++){
		ch->GetEntry(ie);
		cout << "-->" <<  ie << " , Ievent: " << ievt << endl;
		// skip NC and not NuE events
		if (!(nuccnc_truth[0] == 0) && cutNC) continue;
		//if (!(nupdg_truth[0] == 12)) continue;
		if (nhits < 100) continue;
        f_unique_id++;

		for (int ip = 0; ip < 3; ip++){
			h2_evt1[ip]->Reset();
			h2_evt2[ip]->Reset();
			h2_shw1[ip]->Reset();
			h2_tag[ip]->Reset();
		}

                double sum_wire[3] = {0,};  // edited
                double sum_time[3] = {0,};  // edited
                double nhits_count[3] = {0,};  // edited

                int min_wire_U = 100000;
                int min_wire_V = 100000;
		bool fiducial_cut = false;
                for (int j = 0; j < nhits; j++){
                        //int iplane = (int)hit_plane[j];
                        int iplane = (int)hit_global_plane[j];
			int tpc = hit_tpc[j];
			int local_wire = hit_wire[j];
			int local_plane = hit_plane[j];
			double wire_in = hit_global_wire[j];  // edited int -> double
			double tick_in = hit_global_tick[j];  // edited int -> double
                        sum_wire[iplane] += wire_in;
                        sum_time[iplane] += tick_in;
                        nhits_count[iplane] += 1.;
			if (iplane==2){
				if (tpc < 4 && local_wire < 5){ fiducial_cut = true; }
				if (tpc > 19 && local_wire > 474){ fiducial_cut = true; } // last wire:479
			}
                        if (local_plane == 0 && local_wire < min_wire_U) min_wire_U = local_wire;
                        if (local_plane == 1 && local_wire < min_wire_V) min_wire_V = local_wire;
			if (hit_peakT[j]>4482) fiducial_cut = true;
                }

                int sum_min_wires = min_wire_U + min_wire_V;
                //if (sum_min_wires < 390) fiducial_cut = true;

		if (type == "nue" && fiducial_cut) continue;
                // get mean values
                int mean_wire[3] = {0,};
                int mean_time[3] = {0,};
                for (int ii = 0; ii < 3; ii++){
                        //mean_wire[ii] = int( round(sum_wire[ii]/nhits_count[ii]) );  // edited rounded
                        //mean_time[ii] = int( round(sum_time[ii]/nhits_count[ii]) );  // edited rouned
			mean_wire[ii] = pfVtxGlobalWire[  (int)pfVtxGlobalPlane[ii]];
			mean_time[ii] = pfVtxGlobalTick[  (int)pfVtxGlobalPlane[ii]];
                }
		int shift = 56*x_coarse;
		mean_wire[0] += xbins0/2-shift;
		mean_wire[1] -= xbins0/2-shift;
		mean_wire[2] += xbins0/2-shift;


		int tpc_id = -9;
		f_mean_tpc = -9;
		// make new array for pixel map
		int min_rel_dist = 100000;

        if (!hit_based_pixelmap) {
    		for (int iwire = 0; iwire < n_wires; iwire++){
    			int ihit = wire_assn[iwire];
                            int tpc = hit_tpc[ihit];
                            //int iplane = (int)hit_plane[ihit];
                            int iplane = (int)hit_global_plane[ihit];
    			tpc_id = hit_tpc[ihit]; // since we are looking at only one TPC
    			//int wire_in = hit_wire[ihit];
    			int wire_in = (int)hit_global_wire[ihit];
    			int rel_wire = wire_in-mean_wire[iplane]+xbins0/2;  //edited float->int & xbins0/2

                            auto adcs     = wire_adc->at(iwire);
                            auto corradcs = wire_corradc->at(iwire);
                            auto ticks    = wire_tick->at(iwire);
    			auto shwflags = wire_ledshwflag->at(iwire);

    			for (int iraw = 0; iraw < adcs.size(); iraw++){
    				int tick_in = ticks[iraw];

    				int rel_time = tick_in-mean_time[iplane]+ybins0/2;  //edited ybins0/2
    				// remove unnecessary hits first
    				if (rel_wire<0||rel_wire>=xbins0) continue; // edited
    				if (rel_time<0||rel_time>=ybins0) continue; // edited
    				double dist = sqrt(pow((wire_in-mean_wire[iplane])*0.5,2)+pow((tick_in-mean_time[iplane])*0.08,2));
    				if (dist < min_rel_dist){
    					min_rel_dist = dist;
    					f_mean_tpc = tpc;
    				}

    				int rel_wire_coarse = (int)round( (float)rel_wire/(float)x_coarse ); // edited rounded
    				int rel_time_coarse = (int)round( (float)rel_time/(float)y_coarse ); // edited rounded

    				h2_evt1[iplane]->Fill(rel_wire_coarse, rel_time_coarse, adcs[iraw]);
    				h2_evt2[iplane]->Fill(rel_wire_coarse, rel_time_coarse, corradcs[iraw]);
    			}
    		}
            for (int ihit = 0; ihit < nhits; ihit++) {
                int wire_in = (int)hit_global_wire[ihit];

                int start = hit_global_tick[ihit] - 3 * hit_rmsT[ihit];
                int end = hit_global_tick[ihit] + 3 * hit_rmsT[ihit];

                int iplane = (int)hit_global_plane[ihit];

                int rel_wire = wire_in-mean_wire[iplane]+xbins0/2;

                int prong_tag = hit_prong_tag[ihit];

                if (prong_tag == -1) continue;

                for (int i = start; i <= end; i++) {
                    int rel_time = i-mean_time[iplane]+ybins0/2;
                    int rel_wire_coarse = (int)round( (float)rel_wire/(float)x_coarse ); // edited rounded
                    int rel_time_coarse = (int)round( (float)rel_time/(float)y_coarse ); // edited rounded
                    if(prong_tag < kMaxClst){
                        h2_shw1[iplane]->Fill(rel_wire_coarse, rel_time_coarse);
                    }
                    if (prong_tag >= kMaxClst) {
                        h2_tag[iplane]->SetBinContent(rel_wire_coarse+1,rel_time_coarse+1, prong_tag - kMaxClst + 1);
                    }
                    else {
                        h2_tag[iplane]->SetBinContent(rel_wire_coarse+1,rel_time_coarse+1, prong_tag + 1000 + 1);
                    }
                }
            }
        }
        else {
            for (int ihit = 0; ihit < nhits; ihit++) {
                int wire_in = (int)hit_global_wire[ihit];

                if (tpc_id == -9) tpc_id = hit_tpc[ihit];

                int iplane = (int)hit_global_plane[ihit];

                int rel_wire = wire_in-mean_wire[iplane]+xbins0/2;
                int rel_time = hit_global_tick[ihit]-mean_time[iplane]+ybins0/2;
                int rel_wire_coarse = (int)round( (float)rel_wire/(float)x_coarse ); // edited rounded
                int rel_time_coarse = (int)round( (float)rel_time/(float)y_coarse ); // edited rounded

                int prong_tag = hit_prong_tag[ihit];

                h2_evt1[iplane]->Fill(rel_wire_coarse, rel_time_coarse, hit_charge[ihit]);
                h2_evt2[iplane]->Fill(rel_wire_coarse, rel_time_coarse, hit_corrcharge[ihit]);

                if (prong_tag == -1) continue;

                if(prong_tag < kMaxClst){
                    h2_shw1[iplane]->Fill(rel_wire_coarse, rel_time_coarse);
                }
                if (prong_tag >= kMaxClst) {
                    h2_tag[iplane]->SetBinContent(rel_wire_coarse+1,rel_time_coarse+1, prong_tag - kMaxClst + 1);
                }
                else {
                    h2_tag[iplane]->SetBinContent(rel_wire_coarse+1,rel_time_coarse+1, prong_tag + 1000 + 1);
                }
            }
        }


		// loop to print out text file
		int shw_id = 0; // for extra space

    if (trueDaughterPDGs->size() > maxNumberOfDaughters) numberOfDaugters = maxNumberOfDaughters;
    else numberOfDaugters = trueDaughterPDGs->size();

    for (int i = 0; i < numberOfDaugters; i++) {
      std::vector<float>::iterator it = std::max_element(trueDaughterDepoE->begin(), trueDaughterDepoE->end());
      int idx = std::distance(trueDaughterDepoE->begin(), it);
      daughterPDG[i] = trueDaughterPDGs->at(idx);
      daughterE[i] = trueDaughterE->at(idx);
      daughterDepoE[i] = trueDaughterDepoE->at(idx);
      daughterPx[i] = trueDaughterPx->at(idx);
      daughterPy[i] = trueDaughterPy->at(idx);
      daughterPz[i] = trueDaughterPz->at(idx);

      trueDaughterPDGs->erase(trueDaughterPDGs->begin() + idx);
      trueDaughterE->erase(trueDaughterE->begin() + idx);
      trueDaughterDepoE->erase(trueDaughterDepoE->begin() + idx);
      trueDaughterPx->erase(trueDaughterPx->begin() + idx);
      trueDaughterPy->erase(trueDaughterPy->begin() + idx);
      trueDaughterPz->erase(trueDaughterPz->begin() + idx);
    }

		for (int ip = 0; ip < 3; ip++){
		bool at_least_one = false;
		for (int ix = 0; ix < xnbins; ix++){
		for (int iy = 0; iy < ynbins; iy++){
			//int wire_pos = ix-xmax;
			//int time_pos = iy-ymax;
			int wire_pos = ix;
			int time_pos = iy;
			float phit_charge     = h2_evt1[ip]->GetBinContent(ix+1, iy+1);
			float phit_corrcharge = h2_evt2[ip]->GetBinContent(ix+1, iy+1);
			float phit_energy = 0;
			double shw_tot_energy = 0;
			double hit_tot_energy = 0;
			int tmp_shwflag = h2_shw1[ip]->GetBinContent(ix+1, iy+1);
			int itag = h2_tag[ip]->GetBinContent(ix+1, iy+1)-1;
			if (ip == 0){
				shw_tot_energy  = shw_correctedEnergyU;
				hit_tot_energy  = correctedEnergyU;
			}
			if (ip == 1){
				shw_tot_energy  = shw_correctedEnergyV;
				hit_tot_energy  = correctedEnergyV;
			}
			if (ip == 2){
				shw_tot_energy  = shw_correctedEnergyZ;
				hit_tot_energy  = correctedEnergyZ;
			}
			//if (!(phit_corrcharge>0)) continue; // to skip empty space
			if (at_least_one && phit_corrcharge == 0) continue;
			if (phit_corrcharge < -600) continue;
			//if (!(phit_corrcharge>-600)) continue; // to skip empty space

			f_ievt			= ievt;
			f_tpc_id 		= tpc_id;
			f_iplane 		= ip;
			f_shw_id 		= shw_id;
			f_shw_tot_energy 	= (float) shw_tot_energy;
			f_nwires                = xnbins;
			f_nticks                = ynbins;
			f_wcoarse               = x_coarse;
			f_tcoarse               = y_coarse;

			f_rel_wire 		= wire_pos; // note
			f_rel_tick 		= time_pos; // note
			f_mean_wire 		= mean_wire[ip];
			f_mean_tick 		= mean_time[ip];
			f_wire_charge 		= phit_charge;
			f_wire_corrcharge 	= phit_corrcharge;
			f_hit_tot_energy 	= (float)hit_tot_energy;
			//f_trueEnergy 		= (float)trueEnergy;
			f_trueEnergy 		= (float)nueng_truth[0]; // for neutrino
			f_truemode 		= (float)numode_truth[0]; // for neutrino
			f_trueccnc 		= (float)nuccnc_truth[0]; // for neutrino
			f_truenupdg 		= (float)nupdg_truth[0]; // for neutrino

			f_trueVtxWire 		= (int)trueVtxWire[ip];
			f_trueVtxTick 		= (int)trueVtxTime[ip];
			f_trueVertex_X 		= (float)trueVertex_X;
			f_trueVertex_Y 		= (float)trueVertex_Y;
			f_trueVertex_Z 		= (float)trueVertex_Z;
			f_truePx 		= (float)truePx;
			f_truePy 		= (float)truePy;
			f_truePz 		= (float)truePz;

			f_trueVtxGlobalWire 	= (int)trueVtxGlobalWire[(int)trueVtxGlobalPlane[ip]];
			f_trueVtxGlobalTick 	= (int)trueVtxGlobalTick[(int)trueVtxGlobalPlane[ip]];
			f_pfVtxGlobalWire 	= (int)pfVtxGlobalWire[  (int)pfVtxGlobalPlane[ip]];  // double
			f_pfVtxGlobalTick 	= (int)pfVtxGlobalTick[  (int)pfVtxGlobalPlane[ip]];

			f_trueleppdg            = (int)leppdg_truth[0];
			f_truelepeng            = (float)lepeng_truth[0];
			f_trueleppx             = (float)lepp0_truth[0];
			f_trueleppy             = (float)lepp1_truth[0];
			f_trueleppz             = (float)lepp2_truth[0];
			f_nshowers              = (int)n_showers;
			f_shwflag               = (int)( (tmp_shwflag>0) ? 1 : 0 );

			double moment = TMath::Sqrt(all_shw_eng[led_shw_idx]*all_shw_eng[led_shw_idx] - 0.0005109989461*0.0005109989461);
			f_shweng                = (float)all_shw_eng[led_shw_idx];
			f_shwpx                 = (float) (moment*led_shw_dirx);
			f_shwpy                 = (float) (moment*led_shw_diry);
			f_shwpz                 = (float) (moment*led_shw_dirz);
			f_offsetU               = (int)hit_offset[0];
			f_offsetV               = (int)hit_offset[1];
			f_offsetZ               = (int)hit_offset[2];

			f_recoNu_E 		= (float)ErecoNu;
            f_recoLep_E 		= (float)RecoLepEnNu;
            f_recoHad_E 		= (float)RecoHadEnNu;
			f_recoVertex_X 		= (float)recoVertex_X;
			f_recoVertex_Y 		= (float)recoVertex_Y;
			f_recoVertex_Z 		= (float)recoVertex_Z;

			f_trueTPCin		= (int)vtx_in_tpc;
			f_trueDETin		= (int)nutrue_fid;
			f_trackCont             = (int)mutrack_cont;

			if (itag<0){
                f_is_leading_shower = -1;
				f_prong_tag           = -1;
				f_prong_eng           = -1;
				f_prong_length        = -1;
				f_prong_startx        = -1;
				f_prong_starty        = -1;
				f_prong_startz        = -1;
				f_prong_px            = -1;
				f_prong_py            = -1;
				f_prong_pz            = -1;
				f_prong_true_Efrac    = -1;
				f_prong_true_pdg      = -1;
				f_prong_true_pdg_mom  = -1;
				f_prong_true_eng      = -1;
				f_prong_true_px       = -1;
				f_prong_true_py       = -1;
				f_prong_true_pz       = -1;
				f_prong_true_mom_px   = -1;
				f_prong_true_mom_py   = -1;
				f_prong_true_mom_pz   = -1;
				f_prong2_true_Efrac   = -1;
				f_prong2_true_pdg     = -1;
				f_prong2_true_pdg_mom = -1;
				f_prong2_true_eng     = -1;
				f_prong2_true_px      = -1;
				f_prong2_true_py      = -1;
				f_prong2_true_pz      = -1;
				f_prong2_true_mom_px  = -1;
				f_prong2_true_mom_py  = -1;
				f_prong2_true_mom_pz  = -1;
			} else if (itag<1000){
                f_is_leading_shower = -1;
				f_prong_tag           = (int)itag;
				f_prong_eng           = (float)all_trk_calmom[itag];
				f_prong_length        = (float)all_trk_length[itag];
				f_prong_startx        = (float)all_trk_startx[itag];
				f_prong_starty        = (float)all_trk_starty[itag];
				f_prong_startz        = (float)all_trk_startz[itag];
				f_prong_px            = (float)all_trk_px[itag];
				f_prong_py            = (float)all_trk_py[itag];
				f_prong_pz            = (float)all_trk_pz[itag];
				f_prong_true_Efrac    = (float)all_trk_true_Efrac[itag];
				f_prong_true_pdg      = (int)all_trk_true_pdg[itag];
				f_prong_true_pdg_mom  = (int)all_trk_true_pdg_mom[itag];
				f_prong_true_eng      = (float)all_trk_true_eng[itag];
				f_prong_true_px       = (float)all_trk_true_px[itag];
				f_prong_true_py       = (float)all_trk_true_py[itag];
				f_prong_true_pz       = (float)all_trk_true_pz[itag];
				f_prong_true_mom_px   = (float)all_trk_true_mom_px[itag];
				f_prong_true_mom_py   = (float)all_trk_true_mom_py[itag];
				f_prong_true_mom_pz   = (float)all_trk_true_mom_pz[itag];
				f_prong2_true_Efrac   = (float)all_trk2_true_Efrac[itag];
				f_prong2_true_pdg     = (int)all_trk2_true_pdg[itag];
				f_prong2_true_pdg_mom = (int)all_trk2_true_pdg_mom[itag];
				f_prong2_true_eng     = (float)all_trk2_true_eng[itag];
				f_prong2_true_px      = (float)all_trk2_true_px[itag];
				f_prong2_true_py      = (float)all_trk2_true_py[itag];
				f_prong2_true_pz      = (float)all_trk2_true_pz[itag];
				f_prong2_true_mom_px  = (float)all_trk2_true_mom_px[itag];
				f_prong2_true_mom_py  = (float)all_trk2_true_mom_py[itag];
				f_prong2_true_mom_pz  = (float)all_trk2_true_mom_pz[itag];
			} else{
                if (itag == leadingShowerID + 1000) {
                    f_is_leading_shower = 1;
                }
                else {
                    f_is_leading_shower = 0;
                }
				f_prong_tag           = (int)itag;
				f_prong_eng           = (float)all_shw_eng[itag-1000];
				f_prong_length        = -1;
				f_prong_startx        = (float)all_shw_startx[itag-1000];
				f_prong_starty        = (float)all_shw_starty[itag-1000];
				f_prong_startz        = (float)all_shw_startz[itag-1000];
				f_prong_px            = (float)all_shw_px[itag-1000];
				f_prong_py            = (float)all_shw_py[itag-1000];
				f_prong_pz            = (float)all_shw_pz[itag-1000];
				f_prong_true_Efrac    = (float)all_shw_true_Efrac[itag-1000];
				f_prong_true_pdg      = (int)all_shw_true_pdg[itag-1000];
				f_prong_true_pdg_mom  = (int)all_shw_true_pdg_mom[itag-1000];
				f_prong_true_eng      = (float)all_shw_true_eng[itag-1000];
				f_prong_true_px      = (float)all_shw_true_px[itag-1000];
				f_prong_true_py      = (float)all_shw_true_py[itag-1000];
				f_prong_true_pz      = (float)all_shw_true_pz[itag-1000];
				f_prong_true_mom_px  = (float)all_shw_true_mom_px[itag-1000];
				f_prong_true_mom_py  = (float)all_shw_true_mom_py[itag-1000];
				f_prong_true_mom_pz  = (float)all_shw_true_mom_pz[itag-1000];
				f_prong2_true_Efrac    = (float)all_shw2_true_Efrac[itag-1000];
				f_prong2_true_pdg      = (int)all_shw2_true_pdg[itag-1000];
				f_prong2_true_pdg_mom  = (int)all_shw2_true_pdg_mom[itag-1000];
				f_prong2_true_eng      = (float)all_shw2_true_eng[itag-1000];
				f_prong2_true_px      = (float)all_shw2_true_px[itag-1000];
				f_prong2_true_py      = (float)all_shw2_true_py[itag-1000];
				f_prong2_true_pz      = (float)all_shw2_true_pz[itag-1000];
				f_prong2_true_mom_px  = (float)all_shw2_true_mom_px[itag-1000];
				f_prong2_true_mom_py  = (float)all_shw2_true_mom_py[itag-1000];
				f_prong2_true_mom_pz  = (float)all_shw2_true_mom_pz[itag-1000];
			}

			newtree->Fill();
			at_least_one = true;

		} // iy tick#
		} // ix wire#
		} // ip plane#

	}
	newtree->Write();
	output->Close();
	gtimer.Stop();
	cout << "Running time " << gtimer.CpuTime() << endl;


}
