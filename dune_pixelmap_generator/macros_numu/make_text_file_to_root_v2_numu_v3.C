
const int kMaxHits = 50000;

class numu_pm{
public:
  numu_pm();
  virtual ~numu_pm();
  void begin_run();
  TString finput;
  TString fouttext;
  int fin_coarse;

private:
  int   fnpm;
  float fsum_corrchg[3];
  float fchg_th[3];

  const int frebinx;
  const int frebiny;
  const int fxnbins;
  const int fynbins;
  const int fxnbins_m;
  const int fynbins_m;
   
  int fxnbins_l;
  int fynbins_l;
  int fwire_off;
  int frebinx_l;
  int frebiny_l;

 
  std::vector<std::vector<int>> fmean_wire;
  std::vector<std::vector<int>> fmean_tick;
  std::vector<std::vector<int>> fbd_min_wire;
  std::vector<std::vector<int>> fbd_min_tick;
  std::vector<std::vector<int>> fbd_max_wire;
  std::vector<std::vector<int>> fbd_max_tick;

  //-- variables of recoE
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
  int mutrack_cont;
  
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
  double trueVtxWire[3];
  double trueVtxTPC[3];
  double trueVtxTime[3];
  double vtxdetdist;
  
  
  double ErecoNu;
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
  
  int    n_showers;
  int    all_shw_idx[kMaxHits];
  double all_shw_eng[kMaxHits];
  double all_shw_nhits[kMaxHits];
  double all_shw_length[kMaxHits];
  
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
  // end of recoE variables


  void Init(TChain *fchain);
  void Find_Best_Dir_npm(int best_pm_dir[3]);
  void Find_Best_Dir(int ipm, int best_pm_dir[3], int xwidth, int ywidth, int cut[3]);
  void Check_Boundary(int ipm, int local_dirs[3]);
  void Find_Track_PM(int best_pm_dir[3], int max_npm[3]);
  void Reset_Boundary();

};

numu_pm::numu_pm() : fnpm(35),
		     frebinx(1), frebiny(6), fxnbins(400/frebinx), fynbins(1680/frebiny), 
		     fxnbins_m(100), fynbins_m(120)
{
}
numu_pm::~numu_pm()
{
}


void numu_pm::Check_Boundary(int ipm, int local_dirs[3])
{
	// check all boundray to avoid any overlaps
	for (int ip = 0; ip < 3; ip++){
		int dir = local_dirs[ip];
		bool inside = false;
		for (int ii = 0; ii <= ipm+1; ii ++)
		{
			int xmin0 = fbd_min_wire[ii][ip];
			int xmax0 = fbd_max_wire[ii][ip];
			int ymin0 = fbd_min_tick[ii][ip];
			int ymax0 = fbd_max_tick[ii][ip];
			if (xmin0 == 0 || xmax0 == 0 || ymin0 == 0 || ymax0 == 0) continue;
			for (int jj = ii+1; jj <= ipm+1; jj ++)
			{
				int xmin1 = fbd_min_wire[jj][ip];
				int xmax1 = fbd_max_wire[jj][ip];
				int ymin1 = fbd_min_tick[jj][ip];
				int ymax1 = fbd_max_tick[jj][ip];
				if (xmin1 == 0 || xmax1 == 0 || ymin1 == 0 || ymax1 == 0) continue;

				if (xmin0 < xmin1 && xmin1 < xmax0 && ymin0 < ymin1 && ymin1 < ymax0){
					inside = true;
				}
				if (xmin0 < xmin1 && xmin1 < xmax0 && ymin0 < ymax1 && ymax1 < ymax0){
					inside = true;
				}
				if (xmin0 < xmax1 && xmax1 < xmax0 && ymin0 < ymin1 && ymin1 < ymax0){
					inside = true;
				}
				if (xmin0 < xmax1 && xmax1 < xmax0 && ymin0 < ymax1 && ymax1 < ymax0){
					inside = true;
				}
			}
		} // end of ipm
		if (inside){
			if (dir == 4 || dir == 8){
				fbd_min_wire[ipm+1][ip] = fbd_min_wire[ipm][ip];
				fbd_max_wire[ipm+1][ip] = fbd_max_wire[ipm][ip];
				fmean_wire[ipm+1][ip]   = fbd_min_wire[ipm+1][ip] + fxnbins_m/2;
			}
			if (dir == 1 || dir == 2){
				fbd_min_tick[ipm+1][ip] = fbd_min_tick[ipm][ip];
				fbd_max_tick[ipm+1][ip] = fbd_max_tick[ipm][ip];
				fmean_tick[ipm+1][ip]   = fbd_min_tick[ipm+1][ip] + fynbins_m/2;
			}
		}


	}// end of ip

}

void numu_pm::Reset_Boundary()
{
	for (int ipm = 1; ipm < fnpm; ipm ++){
	for (int ii = 0; ii < 3; ii++){
		fbd_min_wire[ipm][ii] = 0; 
		fbd_max_wire[ipm][ii] = 0;
		fmean_wire[ipm][ii]   = 0;
		fmean_tick[ipm][ii]   = 0;
		fbd_min_tick[ipm][ii] = 0;
		fbd_max_tick[ipm][ii] = 0;
		}
	}

}

void numu_pm::Find_Best_Dir_npm(int best_pm_dir[3])
{
	int max_max_npm[3] = {0, 0, 0};
	int init_dir[3] = {0, 0, 0};
	for (int ii = 0; ii < 4; ii++){
		int dir = pow(2,ii);
		int max_npm[3] = {0, 0, 0};
		int test_dir[3] = {dir, dir, dir};
		Find_Track_PM(test_dir, max_npm);
		Reset_Boundary();
		for (int ip = 0; ip < 3; ip++){
			if (max_max_npm[ip] < max_npm[ip]){
				max_max_npm[ip] = max_npm[ip];
				init_dir[ip] = dir;
			}
		}
	}

	for (int ip = 0; ip < 3; ip++){
		//cout << init_dir[ip] << " " << max_max_npm[ip] << endl;
		if (max_max_npm[ip] < 1) best_pm_dir[ip] = 0;
		else best_pm_dir[ip] = init_dir[ip];
	}

}

void numu_pm::Find_Best_Dir(int ipm, int best_pm_dir[3], int xwidth, int ywidth, int main_dir[3])
{
	float res_chg[3][4] = {{0}};
	float sum_rem_chg[3] = {0, 0, 0};
	int nhits_rem[3] = {0, 0, 0};
	int nhits_in_box[3] = {0, 0, 0};
	for (int j = 0; j < nhits; j++){
		int iplane      = (int) hit_global_plane[j];
		int hit_px_wire = (int) hit_global_wire[j]; 
		int peak_time   = (int) hit_global_tick[j];
		float charge    = hit_corrcharge[j];

		if (fbd_min_wire[ipm][iplane] == 0 && fbd_min_tick[ipm][iplane] == 0) continue;
		if (charge/fsum_corrchg[iplane] < fchg_th[iplane]) continue;

		int rel_wire = hit_px_wire - fbd_min_wire[ipm][iplane];
		int rel_tick = peak_time - fbd_min_tick[ipm][iplane];
		if (0 < rel_wire && rel_wire < xwidth && 0 < rel_tick && rel_tick < ywidth){
			nhits_in_box[iplane] += 1;
		}


		if ( ((main_dir[iplane]>>0)&0x1) == 1 && rel_wire < 0) continue;
		if ( ((main_dir[iplane]>>1)&0x1) == 1 && rel_wire > xwidth) continue;
		if ( ((main_dir[iplane]>>2)&0x1) == 1 && rel_tick < 0) continue;
		if ( ((main_dir[iplane]>>3)&0x1) == 1 && rel_tick > ywidth) continue;

		sum_rem_chg[iplane] += charge;
		nhits_rem[iplane] += 1;
		if (rel_wire > xwidth){
			res_chg[iplane][0] += charge;
		}
		if (rel_wire < 0 ){
			res_chg[iplane][1] += charge;
		}
		if (rel_tick > ywidth){
			res_chg[iplane][2] += charge;
		}
		if (rel_tick < 0){
			res_chg[iplane][3] += charge;
		}
	}
	for (int ii = 0; ii < 3; ii++){
		float max_chg = 0;
		best_pm_dir[ii] = 0;
		//if (nhits_rem[ii] < 10) continue;
		//if (nhits_in_box[ii]>0) cout << "==> " << ii << " " << nhits_in_box[ii] << endl;
		if (nhits_in_box[ii] < 5) {
			fmean_wire[ipm][ii]   = 0;
			fmean_tick[ipm][ii]   = 0;
			fbd_min_wire[ipm][ii] = 0; 
			fbd_max_wire[ipm][ii] = 0;
			fbd_min_tick[ipm][ii] = 0;
			fbd_max_tick[ipm][ii] = 0;
			continue;
		}
		for (int jj = 0; jj < 4; jj++){
			//float chg_frac = res_chg[ii][jj] / sum_rem_chg[ii];
			float chg_frac = res_chg[ii][jj] / fsum_corrchg[ii];
			if (chg_frac < 1e-6) continue; // to skip empty
			if (max_chg < chg_frac){
				max_chg = chg_frac;
				best_pm_dir[ii] = pow(2,jj);
			}
			//cout << ii << " ==> " << jj << " : " << chg_frac*100. << "% " << res_chg[ii][jj] << " " << fsum_corrchg[ii] << endl;
		}
	}
	// store previous informaion
	for (int ii = 0; ii < 3; ii++){
		if (main_dir[ii] <= 0) continue;
		main_dir[ii] = main_dir[ii]|best_pm_dir[ii];
	}
}

void numu_pm::Find_Track_PM(int best_pm_dir[3], int max_npm[3])
{
	int local_best_pm_dir[3] = {best_pm_dir[0], best_pm_dir[1], best_pm_dir[2]};

	for (int ipm = 0; ipm < fnpm-1; ipm ++){
		for (int ii = 0; ii < 3; ii++){
			std::vector<int> tmp_avg_wire;
			std::vector<int> tmp_avg_tick;

			int best_pm = local_best_pm_dir[ii];
			float sum_chg_box = 0;
			for (int j = 0; j < nhits; j++){
				int iplane = (int) hit_global_plane[j];
				int iwire  = (int) hit_global_wire[j]; 
				int peak_time   = (int) hit_global_tick[j];
				if (iplane != ii) continue;

				float charge    = hit_corrcharge[j];
				if (charge/fsum_corrchg[ii] < fchg_th[ii]) continue;

				if (best_pm == 1){
					// discard hits based on the earlier boundary
					if (iwire < fbd_max_wire[ipm][ii]) continue;
					if (iwire >= fbd_max_wire[ipm][ii]+fxnbins_m) continue;
					if (peak_time > fbd_max_tick[ipm][ii]+fynbins_m) continue;
					if (peak_time < fbd_min_tick[ipm][ii]-fynbins_m) continue;
					tmp_avg_tick.push_back(peak_time);
				}
				else if (best_pm == 2){
					if (iwire >= fbd_min_wire[ipm][ii]) continue;
					if (iwire < fbd_min_wire[ipm][ii]-fxnbins_m) continue;
					if (peak_time > fbd_max_tick[ipm][ii]+fynbins_m) continue;
					if (peak_time < fbd_min_tick[ipm][ii]-fynbins_m) continue;
					tmp_avg_tick.push_back(peak_time);
				}
				else if (best_pm == 4){
					if (peak_time < fbd_max_tick[ipm][ii]) continue;
					if (peak_time >= fbd_max_tick[ipm][ii]+fynbins_m) continue;
					if (iwire > fbd_max_wire[ipm][ii]+fxnbins_m) continue;
					if (iwire < fbd_min_wire[ipm][ii]-fxnbins_m) continue;
					tmp_avg_wire.push_back(iwire);
				}
				else if (best_pm == 8){
					if (peak_time >= fbd_min_tick[ipm][ii]) continue;
					if (peak_time < fbd_min_tick[ipm][ii]-fynbins_m) continue;
					if (iwire > fbd_max_wire[ipm][ii]+fxnbins_m) continue;
					if (iwire < fbd_min_wire[ipm][ii]-fxnbins_m) continue;
					tmp_avg_wire.push_back(iwire);
				}
				sum_chg_box += charge;

			} // end of j
			// if the next pm is empty
			if (tmp_avg_wire.size() == 0 && tmp_avg_tick.size() == 0) continue;
			//cout << "===> " << ii << " " << tmp_avg_wire.size() << " " <<  tmp_avg_tick.size() << endl;
			//if (tmp_avg_wire.size() < 5 && tmp_avg_tick.size() < 5) continue;
			//cout << "passed" << endl;
			//if (sum_chg_box / fsum_corrchg[ii] < 0.01) continue;
			// if the previous pm is empty
			if (fbd_min_wire[ipm][ii] == 0 || fbd_min_tick[ipm][ii] == 0 || 
			    fbd_max_wire[ipm][ii] == 0 || fbd_min_tick[ipm][ii] == 0) continue;
			max_npm[ii] += 1;
			int tmp_mean_tick = (tmp_avg_tick.size() == 0) ? 0 : 
					    (int) (std::accumulate(tmp_avg_tick.begin(), tmp_avg_tick.end(),0)/tmp_avg_tick.size());
			int tmp_mean_wire = (tmp_avg_wire.size() == 0) ? 0 :
					    (int) (std::accumulate(tmp_avg_wire.begin(), tmp_avg_wire.end(),0)/tmp_avg_wire.size());
			if (best_pm == 1){
				fbd_min_wire[ipm+1][ii] = fbd_max_wire[ipm][ii];
				fbd_max_wire[ipm+1][ii] = fbd_min_wire[ipm+1][ii] + fxnbins_m;
				fmean_wire[ipm+1][ii]   = fbd_min_wire[ipm+1][ii] + fxnbins_m/2;
				fmean_tick[ipm+1][ii]   = tmp_mean_tick;
				fbd_min_tick[ipm+1][ii] = fmean_tick[ipm+1][ii]-fynbins_m/2;
				fbd_max_tick[ipm+1][ii] = fmean_tick[ipm+1][ii]+fynbins_m/2;
			}
			else if (best_pm == 2){
				fbd_max_wire[ipm+1][ii] = fbd_min_wire[ipm][ii];
				fbd_min_wire[ipm+1][ii] = fbd_max_wire[ipm+1][ii] - fxnbins_m;
				fmean_wire[ipm+1][ii] = fbd_min_wire[ipm+1][ii] + fxnbins_m/2;
				fmean_tick[ipm+1][ii] = tmp_mean_tick;
				fbd_min_tick[ipm+1][ii] = fmean_tick[ipm+1][ii]-fynbins_m/2;
				fbd_max_tick[ipm+1][ii] = fmean_tick[ipm+1][ii]+fynbins_m/2;

			}
			else if (best_pm == 4){
				fbd_min_tick[ipm+1][ii] = fbd_max_tick[ipm][ii];
				fbd_max_tick[ipm+1][ii] = fbd_min_tick[ipm+1][ii] + fynbins_m;
				fmean_tick[ipm+1][ii] = fbd_min_tick[ipm+1][ii]+fynbins_m/2;
				fmean_wire[ipm+1][ii] = tmp_mean_wire;
				fbd_min_wire[ipm+1][ii] = fmean_wire[ipm+1][ii]-fxnbins_m/2;
				fbd_max_wire[ipm+1][ii] = fmean_wire[ipm+1][ii]+fxnbins_m/2;

			}
			else if (best_pm == 8){
				fbd_max_tick[ipm+1][ii] = fbd_min_tick[ipm][ii];
				fbd_min_tick[ipm+1][ii] = fbd_max_tick[ipm+1][ii] - fynbins_m;
				fmean_tick[ipm+1][ii] = fbd_min_tick[ipm+1][ii]+fynbins_m/2;
				fmean_wire[ipm+1][ii] = tmp_mean_wire;
				fbd_min_wire[ipm+1][ii] = fmean_wire[ipm+1][ii]-fxnbins_m/2;
				fbd_max_wire[ipm+1][ii] = fmean_wire[ipm+1][ii]+fxnbins_m/2;

			}
			else {
				cout << "what...................." << endl;
				abort();
			}
			//if (ii == 0){
  			//cout << "1 ==> " << ii << " " << best_pm << " " << tmp_avg_wire.size() << " " << tmp_avg_tick.size() << " " << fmean_tick[ipm][ii] << " " << fmean_wire[ipm][ii] << endl;
			//cout << " 2 ==> " << ii << " " << best_pm << " " << tmp_avg_wire.size() << " " << tmp_avg_tick.size() << " " << fmean_tick[ipm+1][ii] << " " << fmean_wire[ipm+1][ii] << endl;
			//}

			tmp_avg_wire.clear();
			tmp_avg_tick.clear();
		}
		Check_Boundary(ipm, local_best_pm_dir);
		Find_Best_Dir(ipm+1, local_best_pm_dir, fxnbins_m, fynbins_m, best_pm_dir);
	} // end of npm
	cout << "number of pixel maps: " << max_npm[0] << " " << max_npm[1] << " " << max_npm[2] << endl;


}



void numu_pm::begin_run() {
        cout << "Start Making Text File" << endl;
	TStopwatch gtimer;
	gtimer.Start();


	TString name_tree;
	if (finput.Contains(".skim.")) name_tree = "WC";
	else name_tree = "recoEnergy/WC";

	TChain *ch = new TChain("recoEnergy/WC");
	ch->Add(finput);
        cout << ">>>" << finput.Data() << endl;
	ch->SetMakeClass(1);

	TFile *output = new TFile(fouttext.Data(), "recreate");
	TTree *newtree = new TTree("pixelmap", "pixelmap");
	//output->SetCompressionAlgorithm(2);
	output->SetCompressionLevel(9);
	cout << output->GetCompressionAlgorithm() << endl;
	
	int f_ievt, f_tpc_id, f_iplane, f_shw_id;
	float f_shw_tot_energy;
	int f_rel_wire, f_rel_tick, f_mean_wire, f_mean_tick;
	int f_rebin_wire, f_rebin_tick;
	int f_nbins_wire, f_nbins_tick;
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
	float f_recoNu_E, f_recoVertex_X, f_recoVertex_Y, f_recoVertex_Z;
	int f_trueTPCin, f_trueDETin;
        int f_trackCont;

	int f_pmidx;

	newtree->Branch("pmidx",  &f_pmidx, "pmidx/I");
	newtree->Branch("ievent", &f_ievt, "ievent/I");
	newtree->Branch("tpc_id", &f_tpc_id, "tpc_id/I");
	newtree->Branch("iplane", &f_iplane, "iplane/I");
	newtree->Branch("shw_id", &f_shw_id, "shw_id/I");
	newtree->Branch("rel_wire", &f_rel_wire, "rel_wire/I");
	newtree->Branch("rel_tick", &f_rel_tick, "rel_tick/I");
	newtree->Branch("mean_wire", &f_mean_wire, "mean_wire/I");
	newtree->Branch("mean_tick", &f_mean_tick, "mean_tick/I");
	newtree->Branch("rebin_wire", &f_rebin_wire, "rebin_wire/I");
	newtree->Branch("rebin_tick", &f_rebin_tick, "rebin_tick/I");
	newtree->Branch("nbins_wire", &f_nbins_wire, "nbins_wire/I");
	newtree->Branch("nbins_tick", &f_nbins_tick, "nbins_tick/I");

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

	ch->SetBranchAddress("MuTrackCont", &mutrack_cont);
       

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

        ch->SetBranchAddress("led_shw_idx",  &led_shw_idx);
        ch->SetBranchAddress("led_shw_eng",  &led_shw_eng);
        ch->SetBranchAddress("led_shw_dirx", &led_shw_dirx);
        ch->SetBranchAddress("led_shw_diry", &led_shw_diry);
        ch->SetBranchAddress("led_shw_dirz", &led_shw_dirz);

        ch->SetBranchAddress("n_showers",      &n_showers);
        ch->SetBranchAddress("all_shw_idx",    all_shw_idx);
        ch->SetBranchAddress("all_shw_eng",    all_shw_eng);
        ch->SetBranchAddress("shw_nhits",      all_shw_nhits);
        ch->SetBranchAddress("all_shw_length", all_shw_length);
	
        ch->SetBranchAddress("ErecoNu",   &ErecoNu);
        ch->SetBranchAddress("m_pf_vtx_x", &recoVertex_X);
        ch->SetBranchAddress("m_pf_vtx_y", &recoVertex_Y);
        ch->SetBranchAddress("m_pf_vtx_z", &recoVertex_Z);

        wire_adc = 0;
	wire_tick = 0;
	wire_corradc = 0;
	wire_shwflag = 0;
	wire_ledshwflag = 0;
        ch->SetBranchAddress("n_wires", &n_wires);
        ch->SetBranchAddress("Wire_assn", wire_assn);
        ch->SetBranchAddress("Wire_adc", &wire_adc);
        ch->SetBranchAddress("Wire_tick", &wire_tick);
        ch->SetBranchAddress("Wire_corradc", &wire_corradc);
        ch->SetBranchAddress("Wire_shwflag", &wire_shwflag);
        ch->SetBranchAddress("Wire_ledshwflag", &wire_ledshwflag);

        ch->SetBranchAddress("Hit_offset",        hit_offset);
        ch->SetBranchAddress("TrueVtxGlobalWire", trueVtxGlobalWire); 
        ch->SetBranchAddress("TrueVtxGlobalTick", trueVtxGlobalTick); 
        ch->SetBranchAddress("TrueVtxGlobalPlane", trueVtxGlobalPlane); 
        ch->SetBranchAddress("PFVtxTPC",        pfVtxTPC);
        ch->SetBranchAddress("PFVtxGlobalWire", pfVtxGlobalWire);
        ch->SetBranchAddress("PFVtxGlobalTick", pfVtxGlobalTick);
        ch->SetBranchAddress("PFVtxGlobalPlane", pfVtxGlobalPlane);
 

	int x_coarse = fin_coarse;
	int y_coarse = fin_coarse*6;
	if (fin_coarse == 0){
		x_coarse = 1;
		y_coarse = 1;
        }
	const int xbins0 = 400;
	const int ybins0 = 1680;

	// for largest pixel map
	fxnbins_l = fxnbins;
	fynbins_l = fynbins;
	frebinx_l = 7;
	frebiny_l = 24;
        fwire_off = 21;
	const int xmax = fxnbins_l/2;
	const int ymax = fynbins_l/2;


	// largest pixel map 
	TH2D *h2_evt1[3], *h2_evt2[3];

	TString ss1, ss2;
	for (int ip = 0; ip < 3; ip++){
		ss1.Form("h2_evt1_%d", ip);
		ss2.Form("Plane %d; Rel Wire; Rel Time", ip);
        	h2_evt1[ip] = new TH2D(ss1, ss2, fxnbins_l, 0, fxnbins_l, fynbins_l, 0, fynbins_l);
		ss1.Form("h2_evt2_%d", ip);
        	h2_evt2[ip] = new TH2D(ss1, ss2, fxnbins_l, 0, fxnbins_l, fynbins_l, 0, fynbins_l);

	}
	// shower flag
        TH2D *h2_shw_U = new TH2D("h2_shw_U", "U plane;Rel Wire; Rel Time", fxnbins, 0, 2*xmax, fynbins, 0, 2*ymax);
        TH2D *h2_shw_V = new TH2D("h2_shw_V", "V plane;Rel Wire; Rel Time", fxnbins, 0, 2*xmax, fynbins, 0, 2*ymax);
        TH2D *h2_shw_Z = new TH2D("h2_shw_Z", "Z plane;Rel Wire; Rel Time", fxnbins, 0, 2*xmax, fynbins, 0, 2*ymax);


	TH2F *h2_sample_gaus[3], *h2_sample[3];
	int count_box[3] = {0, 0, 0};
	int count_box_mu[3] = {0, 0, 0};


	float wire_sumchg[3][4000] = {{0}};

	cout << "Start Loop" << endl;
	int entries = ch->GetEntries();
	cout << "Entries: " << entries << endl;
	for (int ie = 0; ie < entries; ie++){
		ch->GetEntry(ie);
		cout << "-->" <<  ie << " , Ievent: " << ievt << endl;
		// skip NC and not NuE events
		if (!(nuccnc_truth[0] == 0)) continue;
		if (!(fabs(nupdg_truth[0]) == 14)) continue;
		if (nhits < 100) continue;
		//if (ievt > 76010) break;
		for (int ip = 0; ip < 3; ip++){
			h2_evt1[ip]->Reset();
			h2_evt2[ip]->Reset();
		}
		h2_shw_U->Reset();
		h2_shw_V->Reset();
		h2_shw_Z->Reset();

		memset(wire_sumchg, 0, sizeof(wire_sumchg));
		memset(fsum_corrchg,0, sizeof(fsum_corrchg));

		int min_wire[3]    = {100000, 100000, 100000};
		int max_wire[3]    = {0,};
		int min_time[3]    = {10000, 10000, 10000};
		int max_time[3]    = {-10000,-10000,-10000};

		bool fiducial_cut = false;

		int min_wire_U = 100000;
		int min_wire_V = 100000;
		for (int j = 0; j < nhits; j++){
			//int iplane = (int)hit_plane[j];
			int Hit_TPC = hit_tpc[j];
			int local_wire  = hit_wire[j];
			int local_plane = hit_plane[j];
			int tpc         = hit_tpc[j];
			int iplane      = (int)hit_global_plane[j];
			int hit_px_wire = (int)hit_global_wire[j]; 
			float charge    = hit_corrcharge[j];
			int peak_time   = (int)hit_global_tick[j];

			fsum_corrchg[iplane] += charge;
			wire_sumchg[iplane][hit_px_wire] += charge;
			//wire_sumchg[iplane][hit_px_wire] += 1;
			
			if ( min_wire[iplane] > hit_px_wire) min_wire[iplane] = hit_px_wire;
			if ( max_wire[iplane] < hit_px_wire) max_wire[iplane] = hit_px_wire;
			if ( min_time[iplane] > peak_time) min_time[iplane] = peak_time;
			if ( max_time[iplane] < peak_time) max_time[iplane] = peak_time;

			if (local_plane == 0 && local_wire < min_wire_U) min_wire_U = local_wire;
			if (local_plane == 1 && local_wire < min_wire_V) min_wire_V = local_wire;
                        if (iplane==2){
                                if (tpc < 4 && local_wire < 5){ fiducial_cut = true; }
                                if (tpc > 19 && local_wire > 474){ fiducial_cut = true; } // last wire:479
                        }
                        //if (hit_peakT[j]>4482) fiducial_cut = true;
                        if (hit_peakT[j]>4490) fiducial_cut = true;
		}
		int sum_min_wires = min_wire_U + min_wire_V;
		//if (fiducial_cut) continue;
	
		// get wire boundary
		int wire_bound[3] = {0};
		int n_sum_wire = 50;
		for (int ii = 0; ii < 3; ii++){
			for (int jj = 0; jj <= 4000-n_sum_wire; jj++){
				int idx0 = jj;
				if (ii == 1) idx0 = 4000-jj-1;
				float chg_sum = wire_sumchg[ii][idx0];
				for (int kk = 1; kk < n_sum_wire; kk++){
					int idx1 = idx0+kk;
					if (ii == 1) idx1 = idx0-kk;
					chg_sum += wire_sumchg[ii][idx1];
				}
				if (chg_sum/fsum_corrchg[ii]*100. > 2){
					wire_bound[ii] = idx0;
					break;
				}

			}
		}
		wire_bound[0] -= 1;
		wire_bound[1] += 1;
		wire_bound[2] -= 1;

		cout << "npm " << fnpm << endl;

		// get mean wire and tick from boundary
		double sum_wire[3] = {0};
		double sum_time[3] = {0};
		double nhits_count[3] = {0};

		double sum_time_lm[3] = {0};
		double nhits_count_lm[3] = {0};
		double mean_wire_lm[3] = {0};
		double mean_tick_lm[3] = {0};

		// get mean values
		fmean_wire.resize(fnpm, std::vector<int>(3,0));
		fmean_tick.resize(fnpm, std::vector<int>(3,0));
		fbd_min_wire.resize(fnpm, std::vector<int>(3,0));
		fbd_min_tick.resize(fnpm, std::vector<int>(3,0));
		fbd_max_wire.resize(fnpm, std::vector<int>(3,0));
		fbd_max_tick.resize(fnpm, std::vector<int>(3,0));

		int ipm = 0;
		// charge threshold to discard small energy hits
		// find the first pixel map
		for (int j = 0; j < nhits; j++){
			int    iplane      = (int) hit_global_plane[j];
			double hit_px_wire = hit_global_wire[j]; 
			double peak_time   = hit_global_tick[j];
			int    rel_wire    = (int)hit_px_wire - wire_bound[iplane];
			if (iplane == 1) rel_wire = -rel_wire;
			if (rel_wire < 0) continue;
			sum_time_lm[iplane] += peak_time;
			nhits_count_lm[iplane] += 1.0;

			if (rel_wire > fxnbins*frebinx) continue;

			// not sure why this makes worse result
			float charge    = hit_corrcharge[j];
			if (charge/fsum_corrchg[iplane] < fchg_th[iplane]) continue;

			sum_wire[iplane] += hit_px_wire; 
			sum_time[iplane] += peak_time; 
			nhits_count[iplane] += 1.;
		}
		fmean_tick[ipm][2] = int( round(sum_time[2]/nhits_count[2]) );
		for (int ii = 0; ii < 3; ii++){
			fmean_wire[ipm][ii] = int( round(sum_wire[ii]/nhits_count[ii]) );
			//fmean_tick[ipm][ii] = int( round(sum_time[ii]/nhits_count[ii]) );
			fmean_tick[ipm][ii] = fmean_tick[ipm][2];

			mean_wire_lm[ii]    = int( wire_bound[ii] - fwire_off + fxnbins_l*frebinx_l/2);
			mean_tick_lm[ii]    = int( round(sum_time_lm[ii]/nhits_count_lm[ii]) );

			fbd_min_wire[ipm][ii] = wire_bound[ii];
			fbd_min_tick[ipm][ii] = fmean_tick[ipm][ii]-fynbins*frebiny/2;
			fbd_max_wire[ipm][ii] = wire_bound[ii]+fxnbins*frebinx;
			fbd_max_tick[ipm][ii] = fmean_tick[ipm][ii]+fynbins*frebiny/2;
			if (ii == 1){
				mean_wire_lm[ii]    = int( wire_bound[ii] + fwire_off - fxnbins_l*frebinx_l/2);
				fbd_max_wire[ipm][ii] = wire_bound[ii];
				fbd_min_wire[ipm][ii] = wire_bound[ii]-fxnbins*frebinx;
			}
			//cout << "W bound: " << fbd_min_wire[ipm][ii] << " " << fbd_max_wire[ipm][ii] << endl;
			//cout << "T bound: " << fbd_min_tick[ipm][ii] << " " << fbd_max_tick[ipm][ii] << endl;

		}

		cout << "Finding PM direction" << endl;
		int best_pm_dir[3] = {0, 0, 0};
		int init_dir[3] = {1, 2, 1};
		Find_Best_Dir_npm(best_pm_dir);
		// npm-1 is to avoid overflow
		// find pm box for second 
		// skip if there is no extra charge outside of main pixel map	
		//cout << "Best Direction: " << best_pm_dir[0] << " " << best_pm_dir[1] << " " << best_pm_dir[2] << endl;
		// find pixel map for muon tracks
		int max_npm[3] = {0, 0, 0};
		Find_Track_PM(best_pm_dir, max_npm);

		//cout << "mean wire : " << fmean_wire[0][0] << " " << fmean_wire[0][1] << " " << fmean_wire[0][2] << endl;
		//cout << "mean tick : " << fmean_tick[0][0] << " " << fmean_tick[0][1] << " " << fmean_tick[0][2] << endl;
		//cout << "wire bound: " << wire_bound[0] << " " << wire_bound[1] << " " << wire_bound[2] << endl;

		int wire_sp = 10;
		int tick_sp = 10;
		h2_sample[0] = new TH2F("h2_sample_U", "U plane;Wire ID; Tick", 
			   max_wire[0]-min_wire[0]+2*wire_sp, min_wire[0]-wire_sp, max_wire[0]+wire_sp, 
			   max_time[0]-min_time[0]+2*tick_sp, min_time[0]-tick_sp, max_time[0]+tick_sp);
		h2_sample[1] = new TH2F("h2_sample_V", "V plane;Wire ID; Tick", 
			   max_wire[1]-min_wire[1]+2*wire_sp, min_wire[1]-wire_sp, max_wire[1]+wire_sp, 
			   max_time[1]-min_time[1]+2*tick_sp, min_time[1]-tick_sp, max_time[1]+tick_sp);
		h2_sample[2] = new TH2F("h2_sample_Z", "Z plane;Wire ID; Tick",
			   max_wire[2]-min_wire[2]+2*wire_sp, min_wire[2]-wire_sp, max_wire[2]+wire_sp, 
			   max_time[2]-min_time[2]+2*tick_sp, min_time[2]-tick_sp, max_time[2]+tick_sp);

		h2_sample_gaus[0] = new TH2F("h2_sample_U_gaus", "U plane (gaus hit);Wire ID; Tick", 
			   max_wire[0]-min_wire[0]+2*wire_sp, min_wire[0]-wire_sp, max_wire[0]+wire_sp, 
			   max_time[0]-min_time[0]+2*tick_sp, min_time[0]-tick_sp, max_time[0]+tick_sp);
		h2_sample_gaus[1] = new TH2F("h2_sample_V_gaus", "V plane (gaus hit);Wire ID; Tick", 
			   max_wire[1]-min_wire[1]+2*wire_sp, min_wire[1]-wire_sp, max_wire[1]+wire_sp, 
			   max_time[1]-min_time[1]+2*tick_sp, min_time[1]-tick_sp, max_time[1]+tick_sp);
		h2_sample_gaus[2] = new TH2F("h2_sample_Z_gaus", "Z plane (gaus hit);Wire ID; Tick",
			   max_wire[2]-min_wire[2]+2*wire_sp, min_wire[2]-wire_sp, max_wire[2]+wire_sp, 
			   max_time[2]-min_time[2]+2*tick_sp, min_time[2]-tick_sp, max_time[2]+tick_sp);
	

		//std::cout << "Get Wire Info" << std::endl;
		int tpc_id = -9;
		for (int ii = 0; ii < fnpm; ii++){
			f_pmidx = -999;
			tpc_id = -9;

			std::vector<float> temp_chg;
			std::vector<float> temp_corrchg;
			for (int icount = 0; icount < n_wires; ++icount){
				int ihit = wire_assn[icount];
				int iplane = (int)hit_global_plane[ihit];
				int itpc = (int)hit_tpc[ihit];
				tpc_id = itpc;
				int hit_px_wire = (int)hit_global_wire[ihit];
				int wire_id_in = hit_px_wire; 
				auto adcs = wire_adc->at(icount);
				auto corradcs = wire_corradc->at(icount);
				auto ticks = wire_tick->at(icount);

				temp_chg.resize(300, 0);
				temp_corrchg.resize(300, 0);

				double x1 = (double)(fbd_min_wire[ii][iplane]);
				double x2 = (double)(fbd_max_wire[ii][iplane]);
				double y1 = (double)(fbd_min_tick[ii][iplane]);
				double y2 = (double)(fbd_max_tick[ii][iplane]);
				if (x1 == 0) continue;

				int rel_wire = (wire_id_in - x1);
				if (ii == 0) rel_wire /= frebinx;
				for (int iraw = 0; iraw < adcs.size(); iraw++){
					float chg_raw     = (float)adcs[iraw];
					float chg_corrraw = (float)corradcs[iraw];
					int tick_raw      = (int) ticks[iraw];

					int rel_tick = (tick_raw - y1);

					if (ii == 0){
						rel_tick /= frebiny;

						int x_idx = (wire_id_in - mean_wire_lm[iplane]) / frebinx_l + fxnbins_l/2;
						int y_idx = (tick_raw - mean_tick_lm[iplane])/frebiny_l + fynbins_l/2;
						if ( 0 <= x_idx && x_idx < fxnbins_l && 0 <= y_idx && y_idx < fynbins_l){
							h2_evt1[iplane] -> Fill(x_idx, y_idx, chg_raw);
							h2_evt2[iplane] -> Fill(x_idx, y_idx, chg_corrraw);
						}
						h2_sample[iplane]->Fill(wire_id_in, tick_raw);
					}
					if (x1 <= hit_px_wire && hit_px_wire < x2 && y1 <= tick_raw && tick_raw < y2){
						temp_chg[rel_tick] += chg_raw;
						temp_corrchg[rel_tick] += chg_corrraw;
					}

				} // end of iraw
				for (int itick = 0; itick < temp_chg.size(); itick++){
					float in_chg = temp_chg[itick];
					float in_corrchg = temp_corrchg[itick];
					if (in_chg == 0 && in_corrchg == 0) continue;
					if (in_corrchg < -600) continue;

					f_pmidx                 = ii+1;
					f_ievt			= ievt;
					f_tpc_id 		= tpc_id;
					f_iplane 		= iplane;
					f_shw_id 		= -1;
					f_shw_tot_energy 	= -1;
					f_rel_wire 		= rel_wire;
					f_rel_tick 		= itick;
					if (ii == 0){
						f_rebin_wire 		= frebinx;
						f_rebin_tick 		= frebiny;
						f_nbins_wire 		= fxnbins;
						f_nbins_tick 		= fynbins;
					} else {
						f_rebin_wire 		= 1;
						f_rebin_tick 		= 1;
						f_nbins_wire 		= fxnbins_m;
						f_nbins_tick 		= fynbins_m;
					}

					f_mean_wire 		= fmean_wire[ii][iplane];
					f_mean_tick 		= fmean_tick[ii][iplane];
					f_wire_charge 		= in_chg;
					f_wire_corrcharge 	= in_corrchg;
					f_hit_tot_energy 	= -1;
					//f_trueEnergy 		= (float)trueEnergy;
					f_trueEnergy 		= (float)nueng_truth[0]; // for neutrino
					f_truemode 		= (float)numode_truth[0]; // for neutrino
					f_trueccnc 		= (float)nuccnc_truth[0]; // for neutrino
					f_truenupdg 		= (float)nupdg_truth[0]; // for neutrino
			
					f_trueVtxWire 		= (int)trueVtxWire[iplane];
					f_trueVtxTick 		= (int)trueVtxTime[iplane];
					f_trueVertex_X 		= (float)trueVertex_X;
					f_trueVertex_Y 		= (float)trueVertex_Y;
					f_trueVertex_Z 		= (float)trueVertex_Z;
					f_truePx 		= (float)truePx;
					f_truePy 		= (float)truePy;
					f_truePz 		= (float)truePz;
			
					f_trueVtxGlobalWire 	= (int)trueVtxGlobalWire[(int)trueVtxGlobalPlane[iplane]];
					f_trueVtxGlobalTick 	= (int)trueVtxGlobalTick[(int)trueVtxGlobalPlane[iplane]];
					f_pfVtxGlobalWire 	= (int)pfVtxGlobalWire[  (int)pfVtxGlobalPlane[iplane]];  // double
					f_pfVtxGlobalTick 	= (int)pfVtxGlobalTick[  (int)pfVtxGlobalPlane[iplane]];
			
					f_trueleppdg            = (int)leppdg_truth[0];
					f_truelepeng            = (float)lepeng_truth[0];
					f_trueleppx             = (float)lepp0_truth[0];
					f_trueleppy             = (float)lepp1_truth[0];
					f_trueleppx             = (float)lepp2_truth[0];
					f_nshowers              = -1;
					f_shwflag               = -1;
					f_shweng                = -1; 
					f_shwpx                 = -1; 
					f_shwpy                 = -1; 
					f_shwpz                 = -1; 
					f_offsetU               = -1; 
					f_offsetV               = -1; 
					f_offsetZ               = -1; 

					f_recoNu_E 		= (float)ErecoNu;
					f_recoVertex_X 		= (float)recoVertex_X;
					f_recoVertex_Y 		= (float)recoVertex_Y;
					f_recoVertex_Z 		= (float)recoVertex_Z;
				
					f_trueTPCin		= (int)vtx_in_tpc;
					f_trueDETin		= (int)nutrue_fid;
					f_trackCont             = (int)mutrack_cont;
				
					newtree->Fill();
				}
				temp_chg.clear();
				temp_corrchg.clear();
				} // end of icount
		} // end of npm

		// loop to print out text file
		int shw_id = 0; // for extra space
		for (int ip = 0; ip < 3; ip++){
		for (int ix = 0; ix < fxnbins_l; ix++){
		for (int iy = 0; iy < fynbins_l; iy++){
			//int wire_pos = ix-xmax;
			//int time_pos = iy-ymax;
			int wire_pos = ix;
			int time_pos = iy;

			float phit_energy = 0;
			double shw_tot_energy = 0;
			double hit_tot_energy = 0;
			int tmp_shwflag = 0;
			float phit_charge     = h2_evt1[ip]->GetBinContent(ix+1, iy+1);
			float phit_corrcharge = h2_evt2[ip]->GetBinContent(ix+1, iy+1);

			if (ip == 0){
				shw_tot_energy  = shw_correctedEnergyU;
				hit_tot_energy  = correctedEnergyU;
				tmp_shwflag     = h2_shw_U->GetBinContent(ix+1, iy+1);
			}
			if (ip == 1){
				shw_tot_energy  = shw_correctedEnergyV;
				hit_tot_energy  = correctedEnergyV;
				tmp_shwflag     = h2_shw_V->GetBinContent(ix+1, iy+1);
			}
			if (ip == 2){
				shw_tot_energy  = shw_correctedEnergyZ;
				hit_tot_energy  = correctedEnergyZ;
				tmp_shwflag     = h2_shw_Z->GetBinContent(ix+1, iy+1);
			}
			//if (!(phit_corrcharge>0)) continue; // to skip empty space
			if (phit_corrcharge == 0) continue;
			if (phit_corrcharge < -600) continue;
			//if (!(phit_corrcharge>-600)) continue; // to skip empty space
				
			f_pmidx                 = 0;
			f_ievt			= ievt;
			f_tpc_id 		= tpc_id;
			f_iplane 		= ip;
			f_shw_id 		= shw_id;
			f_shw_tot_energy 	= (float) shw_tot_energy;
			f_rel_wire 		= wire_pos; // note
			f_rel_tick 		= time_pos; // note
			f_rebin_wire 		= frebinx_l;
			f_rebin_tick 		= frebiny_l;
			f_nbins_wire 		= fxnbins_l;
			f_nbins_tick 		= fynbins_l;

			f_mean_wire 		= mean_wire_lm[ip];
			f_mean_tick 		= mean_tick_lm[ip];
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
			f_trueleppx             = (float)lepp2_truth[0];
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
			f_recoVertex_X 		= (float)recoVertex_X;
			f_recoVertex_Y 		= (float)recoVertex_Y;
			f_recoVertex_Z 		= (float)recoVertex_Z;

			f_trueTPCin		= (int)vtx_in_tpc;
			f_trueDETin		= (int)nutrue_fid;
			f_trackCont             = (int)mutrack_cont;

			newtree->Fill();
			
		} // iy tick#
		} // ix wire#
		} // ip plane#

		for (int ip = 0; ip < 3; ip++){
			h2_sample[ip]->Delete();
			h2_sample_gaus[ip]->Delete();
		}

		fmean_wire.clear();
		fmean_tick.clear();
		fbd_min_wire.clear();
		fbd_min_tick.clear();
		fbd_max_wire.clear();
		fbd_max_tick.clear();

	}
	newtree->Write();
	output->Close();
	gtimer.Stop();
	cout << "Running time" << gtimer.CpuTime() << endl;

  
}

void make_text_file_to_root_v2_numu_v3(TString input, TString outtext, int in_coarse=1){
	gROOT->Reset();
	gStyle -> SetOptStat(0);
	//gStyle -> SetOptStat(111111);
	TStopwatch gtimer;
	gtimer.Start();
	std::cout << "Start Run" << std::endl;
	numu_pm mypm;
	mypm.finput = input;
	mypm.fouttext = outtext;
	mypm.fin_coarse = in_coarse;
	mypm.begin_run();
	gtimer.Stop();
	std::cout<<"running time "<<gtimer.CpuTime()<<std::endl;

}

      
