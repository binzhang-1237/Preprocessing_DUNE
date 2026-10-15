#include "RecoEnergyClass_Sep2018_v2.h"
#include <iostream>
#include <TStopwatch.h>
#include <TMath.h>
#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>


void Make_Hit_Maps_for_numu() {
	gROOT->Reset();
	gStyle -> SetOptStat(0);
	//gStyle -> SetOptStat(111111);
	TStopwatch gtimer;
	gtimer.Start();
	
	//TString filename ("../work/RecoEnergyS.root");
	TString filename ("../work/RecoEnergyS_408.root");
	//TString filename ("../work/RecoEnergyS_402.root");
	TChain *ch = new TChain("recoEnergy/WC");

	ch->Add(filename);

	RecoEnergyClass_Sep2018_v2 *tt = new RecoEnergyClass_Sep2018_v2(ch);

	int entries = tt->fChain->GetEntries();
	// ticks 0 - 4492, each tick 0.5 us/tick
	// drift velocity 1.6mm/us -> 0.8mm/tick -> 6 ticks -> 4.8 mm
	// wire ptich 4.67mm and 4.79 mm

	// number of wires Z: 480, X and Y: 400.
 	// X and Y plane has 3 indexes per wire
	
	const int rebinx = 1;
	const int rebiny = 6;
	//int xnbins = 400/rebinx;
	//int ynbins = 1680;
	const int xnbins = 400/rebinx;
	const int ynbins = 1680/rebiny;
	//int ynbins = 1680/rebiny;
	//int ynbins = 3000;
	//int ynbins = 1680*2;
	//int xnbins = 480;
	//int ynbins = 4490;
	//
	const int xnbins_m = 100;
	const int ynbins_m = 120;

	const int xmax = xnbins;
	const int ymax = ynbins;
	//TCanvas *ccc1 = new TCanvas("ccc1", "evt original vs corsed with 6 ticks", 0, 600, 1800, 600);
	TCanvas *ccc1 = new TCanvas("ccc1", "evt original vs corsed with 6 ticks", 0, 0, 1200, 400);
	ccc1 -> Divide(3, 1);

	TH2F *h2_evt1_U = new TH2F("h2_evt1_U", "U plane;Rel Wire; Rel Time", xnbins, 0, xmax, ynbins, 0, ymax);
	TH2F *h2_evt1_V = new TH2F("h2_evt1_V", "V plane;Rel Wire; Rel Time", xnbins, 0, xmax, ynbins, 0, ymax);
	TH2F *h2_evt1_Z = new TH2F("h2_evt1_Z", "Z plane;Rel Wire; Rel Time", xnbins, 0, xmax, ynbins, 0, ymax);

	TH1F *h1_U_dup_hits = new TH1F("h1_U_dup_hits", "# of Duplicated Hits", 200, 0, 0.5);
	TH1F *h1_V_dup_hits = new TH1F("h1_V_dup_hits", "# of Duplicated Hits", 200, 0, 0.5);
	TH1F *h1_Z_dup_hits = new TH1F("h1_Z_dup_hits", "# of Duplicated Hits", 200, 0, 0.5);
	

	TH1F *h1_U_dup_wire[5];
	TH1F *h1_V_dup_wire[5];
	TH1F *h1_Z_dup_wire[5];
	//TCanvas *ccc = new TCanvas("ccc", "Pixel Maps", 10, 10, 1800, 600);
	TCanvas *ccc = new TCanvas("ccc", "Pixel Maps", 10, 100, 1350, 450);
	ccc->Divide(3, 1);
	TCanvas *ccc2 = new TCanvas("ccc2", "Event with Gaus Hit", 10, 300, 1350, 450);
	ccc2->Divide(3, 1);

	//int xnbins2 = 1160; // maximum number of wire ID +?
	int xnbins2 = 2880; // maximum number of wire ID +?
	int ynbins2 = 4493; // maximum number of tick + 1

	TH2F *h2_sample_U;
	TH2F *h2_sample_V;
	TH2F *h2_sample_Z;

	TH2F *h2_sample_gaus[3];


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
	float sum_charge_in = 0;
	char ctest;
	char buf[100];

	for (Long64_t ie = 0; ie < entries; ie++){
		if (ie%100 == 0) std::cout << ie << std::endl;
		cout << "Start Loop" << endl;
		//cout << tt.LoadTree(ie) << endl;
		tt->fChain->GetEntry(ie);
		if (tt->NuCCNCTruth[0] != 0) continue;
		if (abs(tt->NuPDGTruth[0]) != 14) continue;
		if (tt->NHits < 100) continue;
		//if (n_tpcs_evt != 1) continue;
		//if (evthaspripf != 1) continue;
		if (tt->TrueVtxin != 1) continue;
		cout << tt->ievent << endl;
		int ievt = tt->ievent;
		memset(wire_sumchg, 0, sizeof(wire_sumchg));

		//if (tt->ievent != 5) continue;
		//if (ievt !=2 && ievt != 8) continue;
		int min_wire[3]    = {100000, 100000, 100000};
		int max_wire[3]    = {0,};
		int min_time[3]    = {10000, 10000, 10000};
		int max_time[3]    = {-10000,-10000,-10000};
		float sum_corrchg[3] = {0};

		bool fiducial_cut = false;

		int min_wire_U = 100000;
		int min_wire_V = 100000;
		for (int j = 0; j < tt->NHits; j++){
			//int iplane = (int)tt->Hit_Plane[j];
			int Hit_TPC = tt->Hit_TPC[j];
			int local_wire  = tt->Hit_Wire[j];
			int local_plane = tt->Hit_Plane[j];
			int tpc         = tt->Hit_TPC[j];
			int iplane      = (int)tt->Hit_Global_Plane[j];
			int hit_px_wire = (int)tt->Hit_Global_Wire[j]; 
			float charge    = tt->Hit_CorrCharge[j];
			int peak_time   = (int)tt->Hit_Global_Tick[j];

			sum_corrchg[iplane] += charge;
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
                        if (tt->Hit_PeakT[j]>4482) fiducial_cut = true;
		}
		int sum_min_wires = min_wire_U + min_wire_V;
		if (fiducial_cut) continue;

		// get wire boundary
		int wire_bound[3] = {0};
		int n_sum_wire = 5;
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
				if (chg_sum/sum_corrchg[ii]*100. > .5){
					wire_bound[ii] = idx0;
					break;
				}

			}
		}
		wire_bound[0] -= 1;
		wire_bound[1] += 1;
		wire_bound[2] -= 1;
		int full_wire_window[3] = {max_wire[0]-wire_bound[0], wire_bound[1]-min_wire[1], max_wire[2]-wire_bound[2]};
		//int full_tick_window[3] = {max_tick[0]-min_tick[0], min_tick[1]-min_tick[1], max_tick[2]-min_tick[2]};
		//int npm = 1+((*max_element(full_wire_window, full_wire_window+3))/(xnbins*rebinx));
		//int npm_tick = 1+((*max_element(full_tick_window, full_tick_window+3))/(ynbins*rebiny));
		int npm = 30;
		cout << "npm " << npm << endl;

		cout << full_wire_window[0] << " " << full_wire_window[1] << " " << full_wire_window[2] << endl;
		cout << 1+(*max_element(full_wire_window, full_wire_window+3))/(xnbins*rebinx) << endl;

		// get mean wire and tick from boundary
		double sum_wire[3] = {0};
		double sum_time[3] = {0};
		double nhits_count[3] = {0};
		// get mean values
		std::vector<std::vector<int>> mean_wire (npm, std::vector<int>(3,0));
		std::vector<std::vector<int>> mean_tick (npm, std::vector<int>(3,0));
		std::vector<std::vector<int>> bd_min_wire (npm, std::vector<int>(3,0));
		std::vector<std::vector<int>> bd_min_tick (npm, std::vector<int>(3,0));
		std::vector<std::vector<int>> bd_max_wire (npm, std::vector<int>(3,0));
		std::vector<std::vector<int>> bd_max_tick (npm, std::vector<int>(3,0));

		int ipm = 0;
		// charge threshold to discard small energy hits
		float chg_th[3] = {1e-4, 1e-4, 1e-4};
		// find the first pixel map
		for (int j = 0; j < tt->NHits; j++){
			int    iplane      = (int) tt->Hit_Global_Plane[j];
			double hit_px_wire = tt->Hit_Global_Wire[j]; 
			double peak_time   = tt->Hit_Global_Tick[j];
			int    rel_wire    = (int)hit_px_wire - wire_bound[iplane];
			if (iplane == 1) rel_wire = -rel_wire;
			if (rel_wire < 0) continue;
			if (rel_wire > xnbins*rebinx) continue;

			// not sure why this makes worse result
			float charge    = tt->Hit_CorrCharge[j];
			if (charge/sum_corrchg[iplane] < chg_th[iplane]) continue;

			sum_wire[iplane] += hit_px_wire; 
			sum_time[iplane] += peak_time; 
			nhits_count[iplane] += 1.;
		}
		for (int ii = 0; ii < 3; ii++){
			mean_wire[ipm][ii] = int( round(sum_wire[ii]/nhits_count[ii]) );
			mean_tick[ipm][ii] = int( round(sum_time[ii]/nhits_count[ii]) );

			bd_min_wire[ipm][ii] = wire_bound[ii];
			bd_min_tick[ipm][ii] = mean_tick[ipm][ii]-ynbins*rebiny/2;
			bd_max_wire[ipm][ii] = wire_bound[ii]+xnbins*rebinx;
			bd_max_tick[ipm][ii] = mean_tick[ipm][ii]+ynbins*rebiny/2;
			if (ii == 1){
				bd_max_wire[ipm][ii] = wire_bound[ii];
				bd_min_wire[ipm][ii] = wire_bound[ii]-xnbins*rebinx;
			}
			cout << "W bound: " << bd_min_wire[ipm][ii] << " " << bd_max_wire[ipm][ii] << endl;
			cout << "T bound: " << bd_min_tick[ipm][ii] << " " << bd_max_tick[ipm][ii] << endl;

		}
		cout << "Finding PM direction" << endl;
		float res_chg[3][4] = {{0}};
		TString s;
		for (int j = 0; j < tt->NHits; j++){
			int iplane      = (int) tt->Hit_Global_Plane[j];
			int hit_px_wire = (int) tt->Hit_Global_Wire[j]; 
			int peak_time   = (int) tt->Hit_Global_Tick[j];
			float charge    = tt->Hit_CorrCharge[j];

			int rel_wire = hit_px_wire - bd_min_wire[ipm][iplane];
			int rel_tick = peak_time - bd_min_tick[ipm][iplane];

			if (charge/sum_corrchg[iplane] < chg_th[iplane]) continue;
			int dir_idx = -1;

			if (rel_wire > xnbins*rebinx){
				 dir_idx = 0;
				res_chg[iplane][0] += charge/sum_corrchg[iplane];
			}
			if (rel_wire < 0){
				 dir_idx = 1;
				res_chg[iplane][1] += charge/sum_corrchg[iplane];
			}
			if (rel_tick > ynbins*rebiny){
				 dir_idx = 2;
				res_chg[iplane][2] += charge/sum_corrchg[iplane];
			}
			if (rel_tick < 0){
				 dir_idx = 3;
				res_chg[iplane][3] += charge/sum_corrchg[iplane];
			}
			if (dir_idx == -1) continue;
		}
		int best_pm_dir[3] = {-1, -1, -1};
		for (int ii = 0; ii < 3; ii++){
			float max_chg = 0;
			for (int jj = 0; jj < 4; jj++){
				if (max_chg < res_chg[ii][jj]){
					max_chg = res_chg[ii][jj];
					best_pm_dir[ii] = jj;
				}
				cout << "==> " << res_chg[ii][jj]*100. << "%" << endl;
			}
		}
		// npm-1 is to avoid overflow
		// find pm box for second 
		// skip if there is no extra charge outside of main pixel map	
		cout << "Best Direction: " << best_pm_dir[0] << " " << best_pm_dir[1] << " " << best_pm_dir[2] << endl;
		if (best_pm_dir[0] == -1 || best_pm_dir[1] == -1 || best_pm_dir[2] == -1) npm = 1;
		int max_npm[3] = {1, 1, 1};
		for (ipm = 0; ipm < npm-1; ipm ++){
			for (int ii = 0; ii < 3; ii++){
				std::vector<int> tmp_avg_wire;
				std::vector<int> tmp_avg_tick;
				int best_pm = best_pm_dir[ii];
				for (int j = 0; j < tt->NHits; j++){
					int iplane = (int) tt->Hit_Global_Plane[j];
					int iwire  = (int) tt->Hit_Global_Wire[j]; 
					int peak_time   = (int) tt->Hit_Global_Tick[j];
					if (iplane != ii) continue;

					float charge    = tt->Hit_CorrCharge[j];
					if (charge/sum_corrchg[ii] < chg_th[ii]) continue;

					if (best_pm == 0){
						// discard hits based on the earlier boundary
						if (iwire < bd_max_wire[ipm][ii]) continue;
						if (iwire >= bd_max_wire[ipm][ii]+xnbins_m) continue;
						//if (peak_time > bd_max_tick[ipm][ii]+ynbins_m) continue;
						//if (peak_time < bd_min_tick[ipm][ii]-ynbins_m) continue;
						tmp_avg_tick.push_back(peak_time);
					}
					else if (best_pm == 1){
						if (iwire >= bd_min_wire[ipm][ii]) continue;
						if (iwire < bd_min_wire[ipm][ii]-xnbins_m) continue;
						//if (peak_time > bd_max_tick[ipm][ii]+ynbins_m) continue;
						//if (peak_time < bd_min_tick[ipm][ii]-ynbins_m) continue;
						tmp_avg_tick.push_back(peak_time);
					}
					else if (best_pm == 2){
						if (peak_time < bd_max_tick[ipm][ii]) continue;
						if (peak_time >= bd_max_tick[ipm][ii]+ynbins_m) continue;
						//if (iwire > bd_max_wire[ipm][ii]+xnbins_m) continue;
						//if (iwire < bd_min_wire[ipm][ii]-xnbins_m) continue;
						tmp_avg_wire.push_back(iwire);
					}
					else if (best_pm == 3){
						if (peak_time >= bd_min_tick[ipm][ii]) continue;
						if (peak_time < bd_min_tick[ipm][ii]-ynbins_m) continue;
						//if (iwire > bd_max_wire[ipm][ii]+xnbins_m) continue;
						//if (iwire < bd_min_wire[ipm][ii]-xnbins_m) continue;

						tmp_avg_wire.push_back(iwire);
					}
	
				} // end of j
				// if the next pm is empty
				if (tmp_avg_wire.size() == 0 && tmp_avg_tick.size() == 0) continue;
				// if the previous pm is empty
				if (bd_min_wire[ipm][ii] == 0 || bd_min_tick[ipm][ii] == 0 || 
				    bd_max_wire[ipm][ii] == 0 || bd_min_tick[ipm][ii] == 0) continue;
				max_npm[ii] += 1;
				if (best_pm == 0){
					bd_min_wire[ipm+1][ii] = bd_max_wire[ipm][ii];
					bd_max_wire[ipm+1][ii] = bd_min_wire[ipm+1][ii] + xnbins_m;
					mean_wire[ipm+1][ii] = (bd_max_wire[ipm+1][ii]-bd_min_wire[ipm+1][ii])/2;
					mean_tick[ipm+1][ii] = (int)(std::accumulate(tmp_avg_tick.begin(), tmp_avg_tick.end(),0)/tmp_avg_tick.size());
					bd_min_tick[ipm+1][ii] = mean_tick[ipm+1][ii]-ynbins_m/2;
					bd_max_tick[ipm+1][ii] = mean_tick[ipm+1][ii]+ynbins_m/2;
				}
				else if (best_pm == 1){
					bd_max_wire[ipm+1][ii] = bd_min_wire[ipm][ii];
					bd_min_wire[ipm+1][ii] = bd_max_wire[ipm+1][ii] - xnbins_m;
					mean_wire[ipm+1][ii] = (bd_max_wire[ipm+1][ii]-bd_min_wire[ipm+1][ii])/2;
					mean_tick[ipm+1][ii] = (int)(std::accumulate(tmp_avg_tick.begin(), tmp_avg_tick.end(),0)/tmp_avg_tick.size());
					bd_min_tick[ipm+1][ii] = mean_tick[ipm+1][ii]-ynbins_m/2;
					bd_max_tick[ipm+1][ii] = mean_tick[ipm+1][ii]+ynbins_m/2;

				}
				else if (best_pm == 2){
					bd_min_tick[ipm+1][ii] = bd_max_tick[ipm][ii];
					bd_max_tick[ipm+1][ii] = bd_min_tick[ipm+1][ii] + ynbins_m;
					mean_tick[ipm+1][ii] = (bd_max_tick[ipm+1][ii]-bd_min_tick[ipm+1][ii])/2;
					mean_wire[ipm+1][ii] = (int)(std::accumulate(tmp_avg_wire.begin(), tmp_avg_wire.end(),0)/tmp_avg_wire.size());
					bd_min_wire[ipm+1][ii] = mean_wire[ipm+1][ii]-ynbins_m/2;
					bd_max_wire[ipm+1][ii] = mean_wire[ipm+1][ii]+ynbins_m/2;

				}
				else if (best_pm == 3){
					bd_max_tick[ipm+1][ii] = bd_min_tick[ipm][ii];
					bd_min_tick[ipm+1][ii] = bd_max_tick[ipm+1][ii] - ynbins_m;
					mean_tick[ipm+1][ii] = (bd_max_tick[ipm+1][ii]-bd_min_tick[ipm+1][ii])/2;
					mean_wire[ipm+1][ii] = (int)(std::accumulate(tmp_avg_wire.begin(), tmp_avg_wire.end(),0)/tmp_avg_wire.size());
					bd_min_wire[ipm+1][ii] = mean_wire[ipm+1][ii]-ynbins_m/2;
					bd_max_wire[ipm+1][ii] = mean_wire[ipm+1][ii]+ynbins_m/2;

				}
				else {
					cout << "what...................." << endl;
					abort();
				}

				tmp_avg_wire.clear();
				tmp_avg_tick.clear();
			}
		} // end of npm
		cout << "number of pixel maps: " << max_npm[0] << " " << max_npm[1] << " " << max_npm[2] << endl;
		cout << "mean wire : " << mean_wire[0][0] << " " << mean_wire[0][1] << " " << mean_wire[0][2] << endl;
		cout << "mean tick : " << mean_tick[0][0] << " " << mean_tick[0][1] << " " << mean_tick[0][2] << endl;
		cout << "wire bound: " << wire_bound[0] << " " << wire_bound[1] << " " << wire_bound[2] << endl;
		


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

		h2_sample_gaus[0] = new TH2F("h2_sample_U_gaus", "U plane (gaus hit);Wire ID; Tick", 
			   max_wire[0]-min_wire[0]+2*wire_sp, min_wire[0]-wire_sp, max_wire[0]+wire_sp, 
			   max_time[0]-min_time[0]+2*tick_sp, min_time[0]-tick_sp, max_time[0]+tick_sp);
		h2_sample_gaus[1] = new TH2F("h2_sample_V_gaus", "V plane (gaus hit);Wire ID; Tick", 
			   max_wire[1]-min_wire[1]+2*wire_sp, min_wire[1]-wire_sp, max_wire[1]+wire_sp, 
			   max_time[1]-min_time[1]+2*tick_sp, min_time[1]-tick_sp, max_time[1]+tick_sp);
		h2_sample_gaus[2] = new TH2F("h2_sample_Z_gaus", "Z plane (gaus hit);Wire ID; Tick",
			   max_wire[2]-min_wire[2]+2*wire_sp, min_wire[2]-wire_sp, max_wire[2]+wire_sp, 
			   max_time[2]-min_time[2]+2*tick_sp, min_time[2]-tick_sp, max_time[2]+tick_sp);
	

		// get true vertex
		int trueVtxU_px_wire = tt->TrueVtxGlobalWire[0];
		int trueVtxV_px_wire = tt->TrueVtxGlobalWire[1];
		int trueVtxZ_px_wire = tt->TrueVtxGlobalWire[2];
		int trueVtxU_px_tick = tt->TrueVtxGlobalTick[0];
		int trueVtxV_px_tick = tt->TrueVtxGlobalTick[1];
		int trueVtxZ_px_tick = tt->TrueVtxGlobalTick[2];
	
		if (tt->TrueVtxGlobalPlane[0] != 0 && tt->TrueVtxGlobalPlane[1] != 1){
			std::swap(trueVtxU_px_wire, trueVtxV_px_wire);
			std::swap(trueVtxU_px_tick, trueVtxV_px_tick);
		}
		//int trueVtxU_px_wire = trueVtxWire[0];
		//int trueVtxV_px_wire = trueVtxWire[1];
		//int trueVtxZ_px_wire = trueVtxWire[2];

		int truevtx_wire[3] = {trueVtxU_px_wire, trueVtxV_px_wire, trueVtxZ_px_wire};
		cout << "\nStart Fill the Hits on Histograms" << endl;
		int min_dN = 0;
		for (int j = 0; j < tt->NHits; j++){
			int iplane       = (int)tt->Hit_Global_Plane[j];
			int hit_px_wire  = (int)tt->Hit_Global_Wire[j]; 
			double peak_time = tt->Hit_Global_Tick[j];
			float charge     = tt->Hit_CorrCharge[j];
			h2_sample_gaus[iplane]->Fill(hit_px_wire, peak_time, charge);
			int dN_wire = hit_px_wire - truevtx_wire[iplane];
			if (iplane == 1) dN_wire = -dN_wire;

			if (dN_wire < min_dN) min_dN = dN_wire;
		}
		cout << "Minimum dN Wire " << min_dN << endl;
		cout << "Min and Max wire \n";
		for (int ii = 0; ii < 3; ii++){
			cout << " " << min_wire[ii] << " " << max_wire[ii] << endl;
		}
		cout << "True Vtx: " << trueVtxU_px_wire << " " << trueVtxV_px_wire << " " << trueVtxZ_px_wire << endl;

		int temp_wire_id = -10000;
		int temp_pre_wire = 0;
		int count_dup_hits[3] = {0};
		for (int icount = 0; icount < tt->n_wires; ++icount){
			int ihit = tt->Wire_assn[icount];
			//int iplane = (int)tt->Hit_Plane[ihit];
			int iplane = (int)tt->Hit_Global_Plane[ihit];
			int itpc = (int)tt->Hit_TPC[ihit];
			int hit_px_wire = (int)tt->Hit_Global_Wire[ihit];
			//int hit_px_wire = hit_wire[ihit];
			int wire_id_in = hit_px_wire; 
			auto adcs = tt->Wire_adc->at(icount);
			auto corradcs = tt->Wire_corradc->at(icount);
			auto ticks = tt->Wire_tick->at(icount);

			int pm_idx = 0;
			for (int iraw = 0; iraw < adcs.size(); iraw++){
				float chg_raw     = (float)adcs[iraw];
				float chg_corrraw = (float)corradcs[iraw];
				int tick_raw      = (int) ticks[iraw];

				//int rel_wire = wire_id_in - mean_wire[iplane] + xnbins/2;
				//int x_idx = rel_wire+xnbins/2+1;
				int rel_wire = (wire_id_in - wire_bound[iplane]) / rebinx;
				if (iplane == 1) rel_wire = rel_wire + xnbins - rebinx;
				int rel_tick = (tick_raw - mean_tick[pm_idx][iplane])/rebiny + ynbins/2;

				int x_idx = rel_wire;
				int y_idx = rel_tick; 
				if (iplane==0){
					h2_sample_U->Fill(wire_id_in, tick_raw);
					if (x_idx < 0 || x_idx > xnbins) continue;
					if (y_idx < 0 || y_idx > ynbins) continue;
					if (h2_evt1_U->GetBinContent(x_idx, y_idx)!=0){
						count_dup_hits[iplane] += 1;
					}
					h2_evt1_U -> Fill(rel_wire, rel_tick, chg_raw);
				}
				if (iplane==1){
					h2_sample_V->Fill(wire_id_in, tick_raw);
					if (x_idx < 0 || x_idx > xnbins) continue;
					if (y_idx < 0 || y_idx > ynbins) continue;
					if (h2_evt1_V->GetBinContent(x_idx, y_idx)!=0){
						count_dup_hits[iplane] += 1;
					}
					h2_evt1_V -> Fill(rel_wire, rel_tick, chg_raw);
				}
				if (iplane==2){
					h2_sample_Z->Fill(wire_id_in, tick_raw);
					if (x_idx < 0 || x_idx > xnbins) continue;
					if (y_idx < 0 || y_idx > ynbins) continue;
	
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
		mm1->SetX((trueVtxU_px_wire-wire_bound[0])/rebinx);        
		mm1->SetY((trueVtxU_px_tick-mean_tick[0][0])/rebiny+ynbins/2);
		mm2->SetX((trueVtxV_px_wire-wire_bound[1])/rebinx+xnbins); 
		mm2->SetY((trueVtxV_px_tick-mean_tick[0][1])/rebiny+ynbins/2);
		mm3->SetX((trueVtxZ_px_wire-wire_bound[2])/rebinx);        
		mm3->SetY((trueVtxZ_px_tick-mean_tick[0][2])/rebiny+ynbins/2);


		// get true vertex
		int recoVtxU_px_wire = tt->PFVtxGlobalWire[0];
		int recoVtxV_px_wire = tt->PFVtxGlobalWire[1];
		int recoVtxZ_px_wire = tt->PFVtxGlobalWire[2];
		int recoVtxU_px_tick = tt->PFVtxGlobalTick[0];
		int recoVtxV_px_tick = tt->PFVtxGlobalTick[1];
		int recoVtxZ_px_tick = tt->PFVtxGlobalTick[2];
	
		if (tt->PFVtxGlobalPlane[0] != 0 && tt->PFVtxGlobalPlane[1] != 1){
			std::swap(recoVtxU_px_wire, recoVtxV_px_wire);
			std::swap(recoVtxU_px_tick, recoVtxV_px_tick);
		}
		mr1->SetX((recoVtxU_px_wire-wire_bound[0])/rebinx);        mr1->SetY((recoVtxU_px_tick-mean_tick[0][0])/rebiny+ynbins/2);
		mr2->SetX((recoVtxV_px_wire-wire_bound[1])/rebinx+xnbins); mr2->SetY((recoVtxV_px_tick-mean_tick[0][1])/rebiny+ynbins/2);
		mr3->SetX((recoVtxZ_px_wire-wire_bound[2])/rebinx);        mr3->SetY((recoVtxZ_px_tick-mean_tick[0][2])/rebiny+ynbins/2);

		cout << "TPC 0 => " << tt->TrueVtxTPC[0] << endl;

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
			for (int ii = 0; ii < npm; ii++){
				double x1 = (double)(bd_min_wire[ii][ip]);
				double x2 = (double)(bd_max_wire[ii][ip]);
				double y1 = (double)(bd_min_tick[ii][ip]);
				double y2 = (double)(bd_max_tick[ii][ip]);
				if (x1 == 0) continue;

				pm_box[ii][ip].SetX1(x1);
				pm_box[ii][ip].SetX2(x2);
				pm_box[ii][ip].SetY1(y1);
				pm_box[ii][ip].SetY2(y2);
				if (ii == 0)
					pm_box[ii][ip].SetLineColor(2);
				else 	
					pm_box[ii][ip].SetLineColor(4);
				pm_box[ii][ip].SetFillStyle(0);
				pm_box[ii][ip].Draw("l");
			}
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

		mean_wire.clear();
		mean_tick.clear();
		bd_min_wire.clear();
		bd_min_tick.clear();
		bd_max_wire.clear();
		bd_max_tick.clear();

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

	gtimer.Stop();
	std::cout<<"running time "<<gtimer.CpuTime()<<std::endl;
}

