#include "RecoEnergyClass_Sep2018_v3.h"
#include <iostream>
#include <TStopwatch.h>
#include <TMath.h>
#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>


class numu_pm{
public:
  numu_pm();
  virtual ~numu_pm();
  void begin_run();

private:
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


  TChain *fch;
  RecoEnergyClass_Sep2018_v2 *ftt;

  void Init(TChain *fchain);
  void Find_Best_Dir_npm(int best_pm_dir[3]);
  void Find_Best_Dir(int ipm, int best_pm_dir[3], int xwidth, int ywidth, int cut[3]);
  void Check_Boundary(int ipm, int local_dirs[3]);
  void Find_Track_PM(int npm, int best_pm_dir[3], int max_npm[3]);
  void Reset_Boundary(int npm);

};

numu_pm::numu_pm() : fch(0), 
		     frebinx(1), frebiny(6), fxnbins(400/frebinx), fynbins(1680/frebiny), 
		     fxnbins_m(100), fynbins_m(120)
{
	//TString filename ("../work/RecoEnergyS.root");
	TString filename ("../work/RecoEnergyS_408.root");
	//TString filename ("../work/RecoEnergyS_402.root");
	fch = new TChain("recoEnergy/WC");
	fch->Add(filename);
	//fch->Add("../work/RecoEnergyS_402.root");
	//fch->Add("../work/RecoEnergyS_405.root");
	//fch->Add("../work/RecoEnergyS_176.root");

	Init(fch);
}
numu_pm::~numu_pm()
{
  if (!fch) return;
  delete fch->GetCurrentFile();
}

void numu_pm::Init(TChain *fchain)
{
	if (!fchain) return;
	ftt = new RecoEnergyClass_Sep2018_v2(fchain);
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

void numu_pm::Reset_Boundary(int npm)
{
	for (int ipm = 1; ipm < npm; ipm ++){
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
	int npm = 30;
	int max_max_npm[3] = {0, 0, 0};
	int init_dir[3] = {0, 0, 0};
	for (int ii = 0; ii < 4; ii++){
		int dir = pow(2,ii);
		int max_npm[3] = {0, 0, 0};
		int test_dir[3] = {dir, dir, dir};
		Find_Track_PM(npm, test_dir, max_npm);
		Reset_Boundary(npm);
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
	for (int j = 0; j < ftt->NHits; j++){
		int iplane      = (int) ftt->Hit_Global_Plane[j];
		int hit_px_wire = (int) ftt->Hit_Global_Wire[j]; 
		int peak_time   = (int) ftt->Hit_Global_Tick[j];
		float charge    = ftt->Hit_CorrCharge[j];

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

void numu_pm::Find_Track_PM(int npm, int best_pm_dir[3], int max_npm[3])
{
	int local_best_pm_dir[3] = {best_pm_dir[0], best_pm_dir[1], best_pm_dir[2]};

	for (int ipm = 0; ipm < npm-1; ipm ++){
		for (int ii = 0; ii < 3; ii++){
			std::vector<int> tmp_avg_wire;
			std::vector<int> tmp_avg_tick;

			int best_pm = local_best_pm_dir[ii];
			float sum_chg_box = 0;
			for (int j = 0; j < ftt->NHits; j++){
				int iplane = (int) ftt->Hit_Global_Plane[j];
				int iwire  = (int) ftt->Hit_Global_Wire[j]; 
				int peak_time   = (int) ftt->Hit_Global_Tick[j];
				if (iplane != ii) continue;

				float charge    = ftt->Hit_CorrCharge[j];
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


	int entries = ftt->fChain->GetEntries();
	// ticks 0 - 4492, each tick 0.5 us/tick
	// drift velocity 1.6mm/us -> 0.8mm/tick -> 6 ticks -> 4.8 mm
	// wire ptich 4.67mm and 4.79 mm

	// number of wires Z: 480, X and Y: 400.
 	// X and Y plane has 3 indexes per wire
	
	//const int frebinx = 1;
	//const int frebiny = 6;
	//const int fxnbins = 400/frebinx;
	//const int fynbins = 1680/frebiny;
	//const int fxnbins_m = 100;
	//const int fynbins_m = 120;
	
	fxnbins_l = fxnbins;
	fynbins_l = fynbins;
	frebinx_l = 7;
	//frebiny_l = 42;
	frebiny_l = 24;
        fwire_off = 21;
	const int xmax = fxnbins_l;
	const int ymax = fynbins_l;
	//TCanvas *ccc1 = new TCanvas("ccc1", "evt original vs corsed with 6 ticks", 0, 600, 1800, 600);
	TCanvas *ccc1 = new TCanvas("ccc1", "evt original vs corsed with 6 ticks", 0, 0, 1200, 400);
	ccc1 -> Divide(3, 1);

	TH2F *h2_evt1_U = new TH2F("h2_evt1_U", "U plane;Rel Wire; Rel Time", fxnbins_l, 0, xmax, fynbins_l, 0, ymax);
	TH2F *h2_evt1_V = new TH2F("h2_evt1_V", "V plane;Rel Wire; Rel Time", fxnbins_l, 0, xmax, fynbins_l, 0, ymax);
	TH2F *h2_evt1_Z = new TH2F("h2_evt1_Z", "Z plane;Rel Wire; Rel Time", fxnbins_l, 0, xmax, fynbins_l, 0, ymax);

	TH1F *h1_U_dup_hits = new TH1F("h1_U_dup_hits", "# of Duplicated Hits", 200, 0, 0.5);
	TH1F *h1_V_dup_hits = new TH1F("h1_V_dup_hits", "# of Duplicated Hits", 200, 0, 0.5);
	TH1F *h1_Z_dup_hits = new TH1F("h1_Z_dup_hits", "# of Duplicated Hits", 200, 0, 0.5);

	TH1F *h1_track_pm[3];
	TH1F *h1_LPM_nhits[3];	
	TString ss;
	for (int ip = 0; ip < 3; ip++){
		ss.Form("h1_track_pm%d", ip);
		h1_track_pm[ip] = new TH1F(ss, ";Fraction ", 100, 0.8, 1.01);
		ss.Form("h1_LPM_nhits%d", ip);
		h1_LPM_nhits[ip] = new TH1F(ss, ";Fraction ", 100, 0.8, 1.01);
	}

	//TCanvas *ccc = new TCanvas("ccc", "Pixel Maps", 10, 10, 1800, 600);
	TCanvas *ccc = new TCanvas("ccc", "Pixel Maps", 10, 100, 1350, 450);
	ccc->Divide(3, 1);
	TCanvas *ccc2 = new TCanvas("ccc2", "Event with Gaus Hit", 10, 300, 1350, 450);
	ccc2->Divide(3, 1);

	//int fxnbins2 = 1160; // maximum number of wire ID +?
	int fxnbins2 = 2880; // maximum number of wire ID +?
	int fynbins2 = 4493; // maximum number of tick + 1

	TH2F *h2_sample_U;
	TH2F *h2_sample_V;
	TH2F *h2_sample_Z;

	TH2F *h2_sample_gaus[3];

	TGraph *gr_muon[3];

	TMarker *m0[3];
	//m0[0] = new TMarker(0, 0, 21);
	//m0[1] = new TMarker(0, 0, 21);
	//m0[2] = new TMarker(0, 0, 21);
	m0[0] = new TMarker(0, 0, 30);
	m0[1] = new TMarker(0, 0, 30);
	m0[2] = new TMarker(0, 0, 30);
	m0[0]->SetMarkerColor(kOrange+7);
	m0[1]->SetMarkerColor(kOrange+7);
	m0[2]->SetMarkerColor(kOrange+7);
	m0[0]->SetMarkerSize(2);
	m0[1]->SetMarkerSize(2);
	m0[2]->SetMarkerSize(2);

	TMarker *mm1 = new TMarker(0, 0, 30);
	TMarker *mm2 = new TMarker(0, 0, 30);
	TMarker *mm3 = new TMarker(0, 0, 30);
	mm1->SetMarkerColor(kOrange+7);
	mm2->SetMarkerColor(kOrange+7);
	mm3->SetMarkerColor(kOrange+7);
	mm1->SetMarkerSize(2);
	mm2->SetMarkerSize(2);
	mm3->SetMarkerSize(2);


	TMarker *mr1 = new TMarker(0, 0, 24);
	TMarker *mr2 = new TMarker(0, 0, 24);
	TMarker *mr3 = new TMarker(0, 0, 24);
	mr1->SetMarkerColor(2);
	mr2->SetMarkerColor(2);
	mr3->SetMarkerColor(2);

	float wire_sumchg[3][4000] = {{0}};
	int count_ntrial = 0;
	int count_vtx_in[3] = {0, 0, 0};
	int count_vtx_in_LPM[3] = {0, 0, 0};
	int count_nhits_in_LPM[3] = {0, 0, 0};
	int count_box[3] = {0, 0, 0};
	int count_box_mu[3] = {0, 0, 0};
	
	float sum_charge_in = 0;
	char ctest;
	char buf[100];
	vector<int> evt_list;

	fchg_th[0] = 1e-4;
	fchg_th[1] = 1e-4;
	fchg_th[2] = 1e-4;

	for (Long64_t ie = 0; ie < entries; ie++){
		if (ie%100 == 0) std::cout << ie << std::endl;
		cout << "Start Loop" << endl;
		//cout << ftt.LoadTree(ie) << endl;
		ftt->fChain->GetEntry(ie);
		if (ftt->NuCCNCTruth[0] != 0) continue;
		if (abs(ftt->NuPDGTruth[0]) != 14) continue;
		if (ftt->NHits < 100) continue;
		//if (n_tpcs_evt != 1) continue;
		//if (evthaspripf != 1) continue;
		//if (ftt->TrueVtxin != 1) continue;
		//if (ftt->NuTruthFid != 1) continue;
		cout << ftt->ievent << endl;
		int ievt = ftt->ievent;
//		if (
//ievt != 165881 &&  ievt != 165881 &&  ievt != 165910 &&  ievt != 165910 &&  ievt != 165910 &&  ievt != 760291 &&  ievt != 760291 &&  ievt != 760291 &&  ievt != 760484 &&  ievt != 760484 &&  ievt != 760484 &&  ievt != 760494 &&  ievt !=
//760494 &&  ievt != 760494 &&  ievt != 760546 &&  ievt != 760546 &&  ievt != 760546 &&  ievt != 760566 &&  ievt != 760566 &&  ievt != 760566 &&  ievt != 760746 &&  ievt != 760872 &&  ievt != 760906 &&  ievt != 760906 &&  ievt != 760906) continue;	
//

		memset(wire_sumchg, 0, sizeof(wire_sumchg));
		memset(fsum_corrchg,0, sizeof(fsum_corrchg));

		//if (ftt->ievent != 5) continue;
		//if (ievt !=2 && ievt != 8) continue;
		int min_wire[3]    = {100000, 100000, 100000};
		int max_wire[3]    = {0,};
		int min_time[3]    = {10000, 10000, 10000};
		int max_time[3]    = {-10000,-10000,-10000};

		bool fiducial_cut = false;

		int min_wire_U = 100000;
		int min_wire_V = 100000;
		for (int j = 0; j < ftt->NHits; j++){
			//int iplane = (int)ftt->Hit_Plane[j];
			int Hit_TPC = ftt->Hit_TPC[j];
			int local_wire  = ftt->Hit_Wire[j];
			int local_plane = ftt->Hit_Plane[j];
			int tpc         = ftt->Hit_TPC[j];
			int iplane      = (int)ftt->Hit_Global_Plane[j];
			int hit_px_wire = (int)ftt->Hit_Global_Wire[j]; 
			float charge    = ftt->Hit_CorrCharge[j];
			int peak_time   = (int)ftt->Hit_Global_Tick[j];

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
                                //if (tpc < 4 && local_wire < 5){ fiducial_cut = true; }
                                //if (tpc > 19 && local_wire > 474){ fiducial_cut = true; } // last wire:479
                                if (tpc < 4 && local_wire < 3){ fiducial_cut = true; }
                                if (tpc > 19 && local_wire > 476){ fiducial_cut = true; } // last wire:479
                        }
                        //if (ftt->Hit_PeakT[j]>4482) fiducial_cut = true;
                        if (ftt->Hit_PeakT[j]>4490) fiducial_cut = true;
		}
		int sum_min_wires = min_wire_U + min_wire_V;
		if (fiducial_cut) continue;

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
				//if (chg_sum/fsum_corrchg[ii]*100. > .3){
					wire_bound[ii] = idx0;
					break;
				}

			}
		}
		wire_bound[0] -= 1;
		wire_bound[1] += 1;
		wire_bound[2] -= 1;
		//int full_wire_window[3] = {max_wire[0]-wire_bound[0], wire_bound[1]-min_wire[1], max_wire[2]-wire_bound[2]};
		//int full_tick_window[3] = {max_tick[0]-min_tick[0], min_tick[1]-min_tick[1], max_tick[2]-min_tick[2]};
		//int npm = 1+((*max_element(full_wire_window, full_wire_window+3))/(fxnbins*frebinx));
		//int npm_tick = 1+((*max_element(full_tick_window, full_tick_window+3))/(fynbins*frebiny));
		//cout << full_wire_window[0] << " " << full_wire_window[1] << " " << full_wire_window[2] << endl;
		//cout << 1+(*max_element(full_wire_window, full_wire_window+3))/(fxnbins*frebinx) << endl;

		int npm = 35;
		cout << "npm " << npm << endl;


		// get mean wire and tick from boundary
		double sum_wire[3] = {0};
		double sum_time[3] = {0};
		double nhits_count[3] = {0};

		double sum_time_lm[3] = {0};
		double nhits_count_lm[3] = {0};
		double mean_tick_lm[3] = {0};

		// get mean values
		fmean_wire.resize(npm, std::vector<int>(3,0));
		fmean_tick.resize(npm, std::vector<int>(3,0));
		fbd_min_wire.resize(npm, std::vector<int>(3,0));
		fbd_min_tick.resize(npm, std::vector<int>(3,0));
		fbd_max_wire.resize(npm, std::vector<int>(3,0));
		fbd_max_tick.resize(npm, std::vector<int>(3,0));

		int ipm = 0;
		// charge threshold to discard small energy hits
		// find the first pixel map
		for (int j = 0; j < ftt->NHits; j++){
			int    iplane      = (int) ftt->Hit_Global_Plane[j];
			double hit_px_wire = ftt->Hit_Global_Wire[j]; 
			double peak_time   = ftt->Hit_Global_Tick[j];
			int    rel_wire    = (int)hit_px_wire - wire_bound[iplane];
			if (iplane == 1) rel_wire = -rel_wire;


			if (rel_wire < 0) continue;
			sum_time_lm[iplane] += peak_time;
			nhits_count_lm[iplane] += 1.0;

			if (rel_wire > fxnbins*frebinx) continue;

			// not sure why this makes worse result
			float charge    = ftt->Hit_CorrCharge[j];
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
			mean_tick_lm[ii]    = int( round(sum_time_lm[ii]/nhits_count_lm[ii]) );

			fbd_min_wire[ipm][ii] = wire_bound[ii];
			fbd_min_tick[ipm][ii] = fmean_tick[ipm][ii]-fynbins*frebiny/2;
			fbd_max_wire[ipm][ii] = wire_bound[ii]+fxnbins*frebinx;
			fbd_max_tick[ipm][ii] = fmean_tick[ipm][ii]+fynbins*frebiny/2;
			if (ii == 1){
				fbd_max_wire[ipm][ii] = wire_bound[ii];
				fbd_min_wire[ipm][ii] = wire_bound[ii]-fxnbins*frebinx;
			}
			cout << "W bound: " << fbd_min_wire[ipm][ii] << " " << fbd_max_wire[ipm][ii] << endl;
			cout << "T bound: " << fbd_min_tick[ipm][ii] << " " << fbd_max_tick[ipm][ii] << endl;

		}
		cout << "Finding PM direction" << endl;
		int best_pm_dir[3] = {0, 0, 0};
		int init_dir[3] = {1, 2, 1};
		//Find_Best_Dir(ipm, best_pm_dir, fxnbins*frebinx, fynbins*frebiny, best_pm_dir);
		//Find_Best_Dir(ipm, best_pm_dir, fxnbins*frebinx, fynbins*frebiny, init_dir);
		Find_Best_Dir_npm(best_pm_dir);
		// npm-1 is to avoid overflow
		// find pm box for second 
		// skip if there is no extra charge outside of main pixel map	
		cout << "Best Direction: " << best_pm_dir[0] << " " << best_pm_dir[1] << " " << best_pm_dir[2] << endl;
		//if (best_pm_dir[0] == 0 || best_pm_dir[1] == 0 || best_pm_dir[2] == 0) npm = 1; // why do I need this?
		// find pixel map for muon tracks
		int max_npm[3] = {0, 0, 0};
		Find_Track_PM(npm, best_pm_dir, max_npm);

		cout << "mean wire : " << fmean_wire[0][0] << " " << fmean_wire[0][1] << " " << fmean_wire[0][2] << endl;
		cout << "mean tick : " << fmean_tick[0][0] << " " << fmean_tick[0][1] << " " << fmean_tick[0][2] << endl;
		cout << "wire bound: " << wire_bound[0] << " " << wire_bound[1] << " " << wire_bound[2] << endl;

		gr_muon[0] = new TGraph();
		gr_muon[1] = new TGraph();
		gr_muon[2] = new TGraph();
		int wire_sp = 10;
		int tick_sp = 10;
		h2_sample_U = new TH2F("h2_sample_U", "U plane;Wire ID; Tick", 
			   max_wire[0]-min_wire[0]+2*wire_sp, min_wire[0]-wire_sp, max_wire[0]+wire_sp, 
			   max_time[0]-min_time[0]+2*tick_sp, min_time[0]-tick_sp, max_time[0]+tick_sp);
		h2_sample_V = new TH2F("h2_sample_V", "V plane;Wire ID; Tick", 
			   max_wire[1]-min_wire[1]+2*wire_sp, min_wire[1]-wire_sp, max_wire[1]+wire_sp, 
			   max_time[1]-min_time[1]+2*tick_sp, min_time[1]-tick_sp, max_time[1]+tick_sp);
		h2_sample_Z = new TH2F("h2_sample_Z", "Z plane;Wire ID; Tick",
			   max_wire[2]-min_wire[2]+2*wire_sp, min_wire[2]-wire_sp, max_wire[2]+wire_sp, 
			   max_time[2]-min_time[2]+2*tick_sp, min_time[2]-tick_sp, max_time[2]+tick_sp);

		h2_sample_gaus[0] = new TH2F("h2_sample_U_gaus", "U plane ;Wire ID; Tick", 
			   max_wire[0]-min_wire[0]+2*wire_sp, min_wire[0]-wire_sp, max_wire[0]+wire_sp, 
			   max_time[0]-min_time[0]+2*tick_sp, min_time[0]-tick_sp, max_time[0]+tick_sp);
		h2_sample_gaus[1] = new TH2F("h2_sample_V_gaus", "V plane ;Wire ID; Tick", 
			   max_wire[1]-min_wire[1]+2*wire_sp, min_wire[1]-wire_sp, max_wire[1]+wire_sp, 
			   max_time[1]-min_time[1]+2*tick_sp, min_time[1]-tick_sp, max_time[1]+tick_sp);
		h2_sample_gaus[2] = new TH2F("h2_sample_Z_gaus", "Z plane ;Wire ID; Tick",
			   max_wire[2]-min_wire[2]+2*wire_sp, min_wire[2]-wire_sp, max_wire[2]+wire_sp, 
			   max_time[2]-min_time[2]+2*tick_sp, min_time[2]-tick_sp, max_time[2]+tick_sp);
	

		// get true vertex
		int trueVtxU_px_wire = ftt->TrueVtxGlobalWire[0];
		int trueVtxV_px_wire = ftt->TrueVtxGlobalWire[1];
		int trueVtxZ_px_wire = ftt->TrueVtxGlobalWire[2];
		int trueVtxU_px_tick = ftt->TrueVtxGlobalTick[0];
		int trueVtxV_px_tick = ftt->TrueVtxGlobalTick[1];
		int trueVtxZ_px_tick = ftt->TrueVtxGlobalTick[2];
	
		if (ftt->TrueVtxGlobalPlane[0] != 0 && ftt->TrueVtxGlobalPlane[1] != 1){
			std::swap(trueVtxU_px_wire, trueVtxV_px_wire);
			std::swap(trueVtxU_px_tick, trueVtxV_px_tick);
		}
		//int trueVtxU_px_wire = trueVtxWire[0];
		//int trueVtxV_px_wire = trueVtxWire[1];
		//int trueVtxZ_px_wire = trueVtxWire[2];

		int truevtx_wire[3] = {trueVtxU_px_wire, trueVtxV_px_wire, trueVtxZ_px_wire};
		cout << "\nStart Fill the Hits on Histograms" << endl;
		int min_dN = 0;
		int nmu[3] = {0, 0, 0};
		int temp_all_nhits_LPM[3] = {0, 0, 0};
		int temp_nhits_LPM[3] = {0, 0, 0};
		for (int j = 0; j < ftt->NHits; j++){
			int iplane       = (int)ftt->Hit_Global_Plane[j];
			int hit_px_wire  = (int)ftt->Hit_Global_Wire[j]; 
			double peak_time = ftt->Hit_Global_Tick[j];
			float charge     = ftt->Hit_CorrCharge[j];
			h2_sample_gaus[iplane]->Fill(hit_px_wire, peak_time, charge);
			int dN_wire = hit_px_wire - truevtx_wire[iplane];
			if (iplane == 1) dN_wire = -dN_wire;

			if (dN_wire < min_dN) min_dN = dN_wire;
			if (ftt->Hit_mu_flag[j] == 1){
				gr_muon[iplane]->SetPoint(nmu[iplane], hit_px_wire, peak_time);
				nmu[iplane] ++;
			}
			int rel_wire_lm = (hit_px_wire-wire_bound[iplane]+fwire_off)/frebinx_l;
			int rel_tick_lm = (round(peak_time) - mean_tick_lm[iplane])/frebiny_l + fynbins_l/2;
			if (iplane == 1){
				rel_wire_lm = (hit_px_wire- wire_bound[iplane] - fwire_off) / frebinx_l;
				rel_wire_lm = rel_wire_lm + fxnbins_l;
			} 
			temp_all_nhits_LPM[iplane] += 1;
			if ( 0 < rel_wire_lm && rel_wire_lm < xmax && 0 < rel_tick_lm && rel_tick_lm < ymax){
				temp_nhits_LPM[iplane] += 1;
			}
		}
		cout << "Minimum dN Wire " << min_dN << endl;
		cout << "Min and Max wire \n";
		for (int ip = 0; ip < 3; ip++){
			h1_LPM_nhits[ip]->Fill( (float)temp_nhits_LPM[ip]/(float)temp_all_nhits_LPM[ip] );
			cout << " " << min_wire[ip] << " " << max_wire[ip] << endl;
		}
		cout << "True Vtx: " << trueVtxU_px_wire << " " << trueVtxV_px_wire << " " << trueVtxZ_px_wire << endl;

		int temp_wire_id = -10000;
		int temp_pre_wire = 0;
		int count_dup_hits[3] = {0};
		for (int icount = 0; icount < ftt->n_wires; ++icount){
			int ihit = ftt->Wire_assn[icount];
			int iplane = (int)ftt->Hit_Global_Plane[ihit];
			int itpc = (int)ftt->Hit_TPC[ihit];
			int hit_px_wire = (int)ftt->Hit_Global_Wire[ihit];
			int wire_id_in = hit_px_wire; 
			auto adcs = ftt->Wire_adc->at(icount);
			auto corradcs = ftt->Wire_corradc->at(icount);
			auto ticks = ftt->Wire_tick->at(icount);

			int pm_idx = 0;
			for (int iraw = 0; iraw < adcs.size(); iraw++){
				float chg_raw     = (float)adcs[iraw];
				float chg_corrraw = (float)corradcs[iraw];
				int tick_raw      = (int) ticks[iraw];

				//int rel_wire = (wire_id_in - wire_bound[iplane]) / frebinx;
				//if (iplane == 1) rel_wire = rel_wire + fxnbins - frebinx;
				//int rel_tick = (tick_raw - fmean_tick[pm_idx][iplane])/frebiny + fynbins/2;
				int rel_wire = (wire_id_in - wire_bound[iplane] + fwire_off) / frebinx_l;
				if (iplane == 1) {
				         rel_wire = (wire_id_in - wire_bound[iplane] - fwire_off) / frebinx_l;
					 //rel_wire = rel_wire + fxnbins_l - frebinx_l;
					 rel_wire = rel_wire + fxnbins_l;
				}
				int rel_tick = (tick_raw - mean_tick_lm[iplane])/frebiny_l + fynbins_l/2;


				int x_idx = rel_wire;
				int y_idx = rel_tick; 
				if (iplane==0){
					h2_sample_U->Fill(wire_id_in, tick_raw);
					if (x_idx < 0 || x_idx > fxnbins_l) continue;
					if (y_idx < 0 || y_idx > fynbins_l) continue;
					if (h2_evt1_U->GetBinContent(x_idx, y_idx)!=0){
						count_dup_hits[iplane] += 1;
					}
					h2_evt1_U -> Fill(rel_wire, rel_tick, chg_raw);
				}
				if (iplane==1){
					h2_sample_V->Fill(wire_id_in, tick_raw);
					if (x_idx < 0 || x_idx > fxnbins_l) continue;
					if (y_idx < 0 || y_idx > fynbins_l) continue;
					if (h2_evt1_V->GetBinContent(x_idx, y_idx)!=0){
						count_dup_hits[iplane] += 1;
					}
					h2_evt1_V -> Fill(rel_wire, rel_tick, chg_raw);
				}
				if (iplane==2){
					h2_sample_Z->Fill(wire_id_in, tick_raw);
					if (x_idx < 0 || x_idx > fxnbins_l) continue;
					if (y_idx < 0 || y_idx > fynbins_l) continue;
	
					if (h2_evt1_Z->GetBinContent(x_idx, y_idx)!=0){
						count_dup_hits[iplane] += 1;
					}
					h2_evt1_Z -> Fill(rel_wire, rel_tick, chg_raw);
				}
			} // end of iraw
		} // end of icount

		cout << "End of Filling the histograms" << endl;
		h1_U_dup_hits->Fill((float)count_dup_hits[0]/(float)h2_evt1_U->GetEntries());
		h1_V_dup_hits->Fill((float)count_dup_hits[1]/(float)h2_evt1_V->GetEntries());
		h1_Z_dup_hits->Fill((float)count_dup_hits[2]/(float)h2_evt1_Z->GetEntries());

		count_ntrial ++;

		m0[0]->SetX(trueVtxU_px_wire); m0[0]->SetY(trueVtxU_px_tick);
		m0[1]->SetX(trueVtxV_px_wire); m0[1]->SetY(trueVtxV_px_tick);
		m0[2]->SetX(trueVtxZ_px_wire); m0[2]->SetY(trueVtxZ_px_tick);


		// get true vertex
		int recoVtxU_px_wire = ftt->PFVtxGlobalWire[0];
		int recoVtxV_px_wire = ftt->PFVtxGlobalWire[1];
		int recoVtxZ_px_wire = ftt->PFVtxGlobalWire[2];
		int recoVtxU_px_tick = ftt->PFVtxGlobalTick[0];
		int recoVtxV_px_tick = ftt->PFVtxGlobalTick[1];
		int recoVtxZ_px_tick = ftt->PFVtxGlobalTick[2];
	
		if (ftt->PFVtxGlobalPlane[0] != 0 && ftt->PFVtxGlobalPlane[1] != 1){
			std::swap(recoVtxU_px_wire, recoVtxV_px_wire);
			std::swap(recoVtxU_px_tick, recoVtxV_px_tick);
		}

		mm1->SetX((trueVtxU_px_wire-wire_bound[0]+fwire_off)/frebinx_l);        
		mm1->SetY((trueVtxU_px_tick-mean_tick_lm[0])/frebiny_l+fynbins_l/2);
		mm2->SetX((trueVtxV_px_wire-wire_bound[1]-fwire_off)/frebinx_l+fxnbins_l); 
		mm2->SetY((trueVtxV_px_tick-mean_tick_lm[1])/frebiny_l+fynbins_l/2);
		mm3->SetX((trueVtxZ_px_wire-wire_bound[2]+fwire_off)/frebinx_l);        
		mm3->SetY((trueVtxZ_px_tick-mean_tick_lm[2])/frebiny_l+fynbins_l/2);

		mr1->SetX((recoVtxU_px_wire-wire_bound[0]+fwire_off)/frebinx_l);
		mr1->SetY((recoVtxU_px_tick-mean_tick_lm[0])/frebiny_l+fynbins_l/2);
		mr2->SetX((recoVtxV_px_wire-wire_bound[1]-fwire_off)/frebinx_l+fxnbins_l); 
		mr2->SetY((recoVtxV_px_tick-mean_tick_lm[1])/frebiny_l+fynbins_l/2);
		mr3->SetX((recoVtxZ_px_wire-wire_bound[2]+fwire_off)/frebinx_l);
		mr3->SetY((recoVtxZ_px_tick-mean_tick_lm[2])/frebiny_l+fynbins_l/2);

		cout << "TPC 0 => " << ftt->TrueVtxTPC[0] << endl;

		ccc1->cd(1);
		h2_evt1_U->Draw("colz");
		mm1->Draw("same");
		mr1->Draw("same");
		gPad->Update();
		ccc1->cd(2);
		h2_evt1_V->Draw("colz");
		mm2->Draw("same");
		mr2->Draw("same");
		gPad->Update();
		ccc1->cd(3);
		h2_evt1_Z->Draw("colz");
		mm3->Draw("same");
		mr3->Draw("same");
		gPad->Update();
		if (0 < mm1->GetX() && mm1->GetX() < xmax && 0 < mm1->GetY() && mm1->GetY() < ymax)
			count_vtx_in_LPM[0] += 1;
		if (0 < mm2->GetX() && mm2->GetX() < xmax && 0 < mm2->GetY() && mm2->GetY() < ymax)
			count_vtx_in_LPM[1] += 1;
		if (0 < mm3->GetX() && mm3->GetX() < xmax && 0 < mm3->GetY() && mm3->GetY() < ymax)
			count_vtx_in_LPM[2] += 1;


		ccc->cd(1);
		h2_sample_U->Draw("colz");
		m0[0]->Draw("same");
		gPad->Update();
		ccc->cd(2);
		h2_sample_V->Draw("colz");
		m0[1]->Draw("same");
		gPad->Update();
		ccc->cd(3);
		h2_sample_Z->Draw("colz");
		m0[2]->Draw("same");
		gPad->Update();
		cout << "Min Wire Sum: " << sum_min_wires << endl;

		// make box for pm windows
		std::vector<std::vector<TBox>> pm_box (npm, std::vector<TBox>(3, TBox()));

		for (int ip = 0; ip < 3; ip++){
			ccc2->cd(ip+1);
			h2_sample_gaus[ip]->Draw("colz");
			m0[ip]->Draw("same");
			gr_muon[ip]->SetMarkerColor(2);
			gr_muon[ip]->Draw("p same");
			for (int ii = 0; ii < npm; ii++){
				double x1 = (double)(fbd_min_wire[ii][ip]);
				double x2 = (double)(fbd_max_wire[ii][ip]);
				double y1 = (double)(fbd_min_tick[ii][ip]);
				double y2 = (double)(fbd_max_tick[ii][ip]);
				if (x1 == 0) continue;

				pm_box[ii][ip].SetX1(x1);
				pm_box[ii][ip].SetX2(x2);
				pm_box[ii][ip].SetY1(y1);
				pm_box[ii][ip].SetY2(y2);
				if (ii == 0){
					pm_box[ii][ip].SetLineColor(2);
					if (x1 < m0[ip]->GetX() && m0[ip]->GetX() < x2
					    && y1 < m0[ip]->GetY() && m0[ip]->GetY() < y2)
					{
						count_vtx_in[ip] += 1;
					}else {
					    cout << ip << ": Missed the True Vertex" << endl;
					    evt_list.push_back(ievt);
					}
				}
				else {
					pm_box[ii][ip].SetLineColor(4);
				}
				pm_box[ii][ip].SetFillStyle(0);
				pm_box[ii][ip].Draw("l");
			} // end of draw boxes
			int temp_count_all_mu = 0;
			int temp_count_box_mu = 0;
			for (int ig = 0; ig < gr_muon[ip]->GetN(); ig++){
				double xxx = gr_muon[ip]->GetX()[ig];
				double yyy = gr_muon[ip]->GetY()[ig];
				count_box[ip] += 1;
				temp_count_all_mu += 1;
				for (int ii = 0; ii < npm; ii++){
					double x1 = (double)(fbd_min_wire[ii][ip]);
					double x2 = (double)(fbd_max_wire[ii][ip]);
					double y1 = (double)(fbd_min_tick[ii][ip]);
					double y2 = (double)(fbd_max_tick[ii][ip]);
					if (x1 == 0) continue;
					if (x1 <= xxx && xxx < x2 && y1 <= yyy && yyy < y2){
						count_box_mu[ip] += 1;
						temp_count_box_mu += 1;
						break;
					}
				}
			} // end of gr_muon
			h1_track_pm[ip]->Fill((float)temp_count_box_mu/(float)temp_count_all_mu);
			gPad->Update();
		}

		//gSystem->ProcessEvents();
		if (ctest != 'r') {
		//if (min_dN < -1900) {
			cout << "Press Enter to go next event" << endl;
			ctest = getchar();
			if (ctest == 'q') break;
		}
		//if (ctest == 'r') gSystem->Sleep(1000);// sleep 1 sec
		
		//h2_sample_U->Reset();
		//h2_sample_V->Reset();
		//h2_sample_Z->Reset();
		h2_sample_U->Delete();
		h2_sample_V->Delete();
		h2_sample_Z->Delete();

		h2_sample_gaus[0]->Delete();
		h2_sample_gaus[1]->Delete();
		h2_sample_gaus[2]->Delete();
		pm_box.clear();

		h2_evt1_U->Reset();
		h2_evt1_V->Reset();
		h2_evt1_Z->Reset();

		fmean_wire.clear();
		fmean_tick.clear();
		fbd_min_wire.clear();
		fbd_min_tick.clear();
		fbd_max_wire.clear();
		fbd_max_tick.clear();

		gr_muon[0]->Delete();
		gr_muon[1]->Delete();
		gr_muon[2]->Delete();
		cout << "End of loop" << endl;
	}

	//cout << "Energy Fraction in Map: " << sum_charge_in/(float)count_ntrial << endl;

	TCanvas *can1 = new TCanvas("can1", "can1", 0, 0, 600, 200);
	can1->Divide(3, 1);
	can1->cd(1);
	h1_U_dup_hits->Draw("hist");
	can1->cd(2);
	h1_V_dup_hits->Draw("hist");
	can1->cd(3);
	h1_Z_dup_hits->Draw("hist");
	//TCanvas *can2 = new TCanvas("can2", "can2", 0, 0, 600, 200);
	//can2->Divide(3, 1);
	//TCanvas *can3 = new TCanvas("can3", "can3", 0, 0, 600, 200);
	//can3->Divide(3, 1);
	TString s;
	for (int ip = 0; ip < 3; ip++){
		float frac = (float)count_vtx_in[ip]/(float)count_ntrial;
		s.Form( "Total number of events %d , N with Vtx: %d => Fraction: %.2f", count_ntrial, count_vtx_in[ip], frac*100.);
		cout << s.Data() << endl;
		frac = (float)count_box_mu[ip]/(float)count_box[ip];
		s.Form( "Total number of muon hits %d, muon hist in boxes %d  => Franction: %.2f", count_box[ip], count_box_mu[ip], (frac)*100.);
		cout << s.Data() << endl;
	}
	for (int ip = 0; ip < 3; ip++){
		float frac = (float)count_vtx_in_LPM[ip]/(float)count_ntrial;
		s.Form( "LPM: Total number of events %d , N with Vtx: %d => Fraction: %.2f", count_ntrial, count_vtx_in_LPM[ip], frac*100.);
		cout << s.Data() << endl;
	}
	cout << "missing evt list " << endl;
	for (auto ii = 0; ii < evt_list.size(); ii++){
		cout << " ievt != " << evt_list[ii] << " && ";
	}
	cout << endl;

	//TFile *output = new TFile("PM_Check_for_numu.root", "recreate");
	//for (int ip = 0; ip < 3; ip++){
	//	h1_track_pm[ip]->Write();
	//	h1_LPM_nhits[ip]->Write();
	//}
	//output->Close();
}
void Make_Hit_Maps_for_numu_v1() {
	gROOT->Reset();
	gStyle -> SetOptStat(0);
	//gStyle -> SetOptStat(111111);
	TStopwatch gtimer;
	gtimer.Start();
	std::cout << "Start Run" << std::endl;
	numu_pm mypm;
	mypm.begin_run();
	gtimer.Stop();
	std::cout<<"running time "<<gtimer.CpuTime()<<std::endl;

}

