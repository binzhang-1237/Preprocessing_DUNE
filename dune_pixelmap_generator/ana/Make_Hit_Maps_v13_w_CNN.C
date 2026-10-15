// 
// This script to check the Pixel map and data generated RecoEnergyS_module.cc
//
#include "RecoEnergyClass_Sep2018.h"
#include <iostream>
#include <TStopwatch.h>
#include <TMath.h>
#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>


void Make_Hit_Maps_v13_w_CNN() {
        map<int,int> pdg_cmap;
        pdg_cmap.insert(pair<int,int>(0,3));
        pdg_cmap.insert(pair<int,int>(11,2));
        pdg_cmap.insert(pair<int,int>(2212,9));
        pdg_cmap.insert(pair<int,int>(22,7));
        pdg_cmap.insert(pair<int,int>(211,8));
        pdg_cmap.insert(pair<int,int>(2112,4));
        pdg_cmap.insert(pair<int,int>(-1,28));

        map<int,int> pdg_idx;
        pdg_idx.insert(pair<int,int>(0,   1));
        pdg_idx.insert(pair<int,int>(11,  2));
        pdg_idx.insert(pair<int,int>(2212,3));
        pdg_idx.insert(pair<int,int>(22,  4));
        pdg_idx.insert(pair<int,int>(211, 5));
        pdg_idx.insert(pair<int,int>(2112,6));
        pdg_idx.insert(pair<int,int>(-1,  7));


	gROOT->Reset();
	gStyle -> SetOptStat(0);
	TStopwatch gtimer;
	gtimer.Start();
	
	TString filename ("../work/reco_test_test_pand.root");
	TChain *ch = new TChain("recoEnergy/WC");
	ch->Add(filename);


	// make a class to read all branched in the root file
	RecoEnergyClass_Sep2018 *tt = new RecoEnergyClass_Sep2018(ch);

	int entries = tt->fChain->GetEntries();
	// ticks 0 - 4492, each tick 0.5 us/tick
	// drift velocity 1.6mm/us -> 0.8mm/tick -> 6 ticks -> 4.8 mm
	// wire ptich 4.67mm and 4.79 mm

	// number of wires Z: 480, X and Y: 400.
 	// X and Y plane has 3 indexes per wire
	
	int xnbins = 400;
	int ynbins = 1680;

	int xmax = xnbins/2;
	int ymax = ynbins/2;
	TCanvas *ccc1 = new TCanvas("ccc1", "evt original vs corsed with 6 ticks", 0, 0, 1200, 400);
	ccc1 -> Divide(3, 1);
	TCanvas *cccZ = new TCanvas("cccZ", "Z Plane", 0, 0, 400, 400);

	const int n_map = (int)pdg_cmap.size()+1;
	TCanvas *ccc2 = new TCanvas("ccc2", "prong pid", 0, 0, 800, 1000);
	TCanvas *ccc2_2 = new TCanvas("ccc2_2", "EMShower", 0, 0, 800, 1000);
	TCanvas *ccc2_3 = new TCanvas("ccc2_3", "Missed", 0, 0, 1200, 400);
	ccc2 -> Divide(3, n_map);
	ccc2_2 -> Divide(3, n_map);
	ccc2_3 -> Divide(3, 1);

	const int n_clust = 10;
	TH2F *h2_evt1[3];
	TH2F *h2_hit_pdg[n_map][3];
	TH2F *h2_evt_pdg[n_map][3];
	TH2F *h2_tag_pdg[n_clust][3];
	// define histograms for pixel maps 
	string hname[3] = {"U", "V", "Z"};
	for (int i = 0; i < 3; i++){
		h2_evt1[i] = new TH2F(Form("h2_evt1_%s",hname[i].c_str()), 
				Form("%s plane;Rel Wire; Rel Time",hname[i].c_str()), 
				xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
		for (int j = 0; j < n_map; j++){
			h2_hit_pdg[j][i] = new TH2F(Form("h2_hit_pdg_%s_%d",hname[i].c_str(), j), 
					Form("%s plane;Rel Wire; Rel Time",hname[i].c_str()), 
					xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
			h2_evt_pdg[j][i] = new TH2F(Form("h2_evt_pdg_%s_%d",hname[i].c_str(), j), 
					Form("%s plane;Rel Wire; Rel Time",hname[i].c_str()), 
					xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
		}
		for (int j = 0; j < n_clust; j++){
			h2_tag_pdg[j][i] = new TH2F(Form("h2_tag_pdg_%s_%d",hname[i].c_str(), j), 
					Form("%s plane;Rel Wire; Rel Time",hname[i].c_str()), 
					xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
		}
	}
	TCanvas *ccc3[n_clust];
	//for (int i = 0; i < n_clust; i++){
	for (int i = 0; i < 2; i++){
		ccc3[i] = new TCanvas(Form("ccc3_%d",i), "prong pids for each clustering", 0, 0, 800, 1000);
		ccc3[i]->Divide(3,n_clust/2);
	}


	TCanvas *ccc = new TCanvas("ccc", "Pixel Maps", 10, 10, 1350, 450);
	ccc->Divide(3, 1);

	TH2F *h2_sample[3];

	TMarker *m1 = new TMarker(0, 0, 30);
	TMarker *m2 = new TMarker(0, 0, 30);
	TMarker *m3 = new TMarker(0, 0, 30);
	TMarker *ma1 = new TMarker(0, 0, 24);
	TMarker *ma2 = new TMarker(0, 0, 24);
	TMarker *ma3 = new TMarker(0, 0, 24);
	TMarker *mb1 = new TMarker(0, 0, 25);
	TMarker *mb2 = new TMarker(0, 0, 25);
	TMarker *mb3 = new TMarker(0, 0, 25);

	int mm_n = 28;
	int mr_n = 20;
	int mc_n = 21;
	TMarker *mm1 = new TMarker(0, 0, 28);
	TMarker *mm2 = new TMarker(0, 0, 28);
	TMarker *mm3 = new TMarker(0, 0, 28);
	TMarker *mr1 = new TMarker(0, 0, 20);
	TMarker *mr2 = new TMarker(0, 0, 20);
	TMarker *mr3 = new TMarker(0, 0, 20);
	TMarker *mc1 = new TMarker(0, 0, 47);
	TMarker *mc2 = new TMarker(0, 0, 47);
	TMarker *mc3 = new TMarker(0, 0, 47);
	TMarker *mcvn1 = new TMarker(0, 0, 47);
	TMarker *mcvn2= new TMarker(0, 0, 47);
	TMarker *mcvn3 = new TMarker(0, 0, 47);

	mm1->SetMarkerColor(0);
	mm2->SetMarkerColor(0);
	mm3->SetMarkerColor(0);

	//mm1->SetMarkerSize(2);
	//mm2->SetMarkerSize(2);
	//mm3->SetMarkerSize(2);

	// vertex on pixel map
	mr1->SetMarkerColor(2);
	mr2->SetMarkerColor(2);
	mr3->SetMarkerColor(2);

	//mc1->SetMarkerSize(1.5);
	//mc2->SetMarkerSize(1.5);
	//mc3->SetMarkerSize(1.5);

	mc1->SetMarkerColor(kMagenta-4);
	mc2->SetMarkerColor(kMagenta-4);
	mc3->SetMarkerColor(kMagenta-4);

	//mcvn1->SetMarkerSize(2.0);
	//mcvn2->SetMarkerSize(2.0);
	//mcvn3->SetMarkerSize(2.0);

	mcvn1->SetMarkerColor(kSpring-8);
	mcvn2->SetMarkerColor(kSpring-8);
	mcvn3->SetMarkerColor(kSpring-8);


	ma1->SetMarkerColor(2);
	ma2->SetMarkerColor(2);
	ma3->SetMarkerColor(2);
	mb1->SetMarkerColor(3);
	mb2->SetMarkerColor(3);
	mb3->SetMarkerColor(3);




	int count_ntrial = 0;
	float sum_charge_in = 0;
	char ctest;
	char buf[100];
	cout << "Start Loop" << endl;
	for (Long64_t ie = 0; ie < entries; ie++){
		if (ie%100 == 0) std::cout << ie << std::endl;
		//cout << tt.LoadTree(ie) << endl;
		tt->fChain->GetEntry(ie);
		if (tt->NHits < 100) continue; // number of hits (from Reco::Hit)
		if (tt->NuTruthFid == 0) continue; // True Vertex Cut 
		//if (tt->TrueVtxin != 1) continue; // check true Vertex can be converted into wire/TDC 
		
		if (tt->NuCCNCTruth[0] != 0) continue; // CC only


		int ievt = tt->ievent;
		cout << "ievent:" << ievt << endl;

		int min_wire[3]    = {100000, 100000, 100000};
		int max_wire[3]    = {0,};
		int min_time[3]    = {10000, 10000, 10000};
		int max_time[3]    = {-10000,-10000,-10000};

		int sum_wire[3]    = {0,};
		int sum_time[3]    = {0,};
		int nhits_count[3] = {0,};
		int sum_corrchg[3] = {0,};
		bool fiducial_cut = false;

		int min_wire_U = 100000;
		int min_wire_V = 100000;
		// loop to evalute the averages of wires and peakT
		for (int j = 0; j < tt->NHits; j++){
			int Hit_TPC = tt->Hit_TPC[j];
			int local_wire  = tt->Hit_Wire[j];
			int local_plane = tt->Hit_Plane[j];
			int iplane      = (int)tt->Hit_Global_Plane[j]; // global plane
			int hit_px_wire = tt->Hit_Global_Wire[j]; 
			int peak_time = tt->Hit_Global_Tick[j];
			sum_wire[iplane] += hit_px_wire; 
			sum_time[iplane] += peak_time; 
			nhits_count[iplane] += 1;
			if ( min_wire[iplane] > hit_px_wire) min_wire[iplane] = hit_px_wire;
			if ( max_wire[iplane] < hit_px_wire) max_wire[iplane] = hit_px_wire;
			if ( min_time[iplane] > peak_time) min_time[iplane] = peak_time;
			if ( max_time[iplane] < peak_time) max_time[iplane] = peak_time;

			if (local_plane == 0 && local_wire < min_wire_U) min_wire_U = local_wire;
			if (local_plane == 1 && local_wire < min_wire_V) min_wire_V = local_wire;
                        if (iplane==2){
                                if (Hit_TPC < 4 && local_wire < 5){ fiducial_cut = true; }
                                if (Hit_TPC > 19 && local_wire > 474){ fiducial_cut = true; } // last wire:479
                        }
                        if (tt->Hit_PeakT[j]>4482) fiducial_cut = true;
		}
		int sum_min_wires = min_wire_U + min_wire_V;
		// get mean values
		int mean_wire[3] = {0,};
		int mean_time[3] = {0,};
		for (int ii = 0; ii < 3; ii++){
			mean_wire[ii] = int( (float)sum_wire[ii]/(float)nhits_count[ii] + 0.5 );
			mean_time[ii] = int( (float)sum_time[ii]/(float)nhits_count[ii] + 0.5 );
		}
		//if (fiducial_cut) continue;
		for (int i = 0; i < 3; i++){
			h2_sample[i] = new TH2F(Form("h2_sample_%s",hname[i].c_str()), Form("%s plane;Wire ID; Tick", hname[i].c_str()), 
			   max_wire[i]-min_wire[i]+10, min_wire[i]-5, max_wire[i]+5, max_time[i]-min_time[i]+10, min_time[i]-5, max_time[i]+5);

		}

		// get true vertices
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
		cout << "##### " << trueVtxU_px_wire << " " << trueVtxV_px_wire << " " << trueVtxZ_px_wire << endl;

		cout << Form("Mean Wire : %d %d %d", mean_wire[0], mean_wire[1], mean_wire[2]) << endl;
		cout << Form("Mean Tick : %d %d %d", mean_time[0], mean_time[1], mean_time[2]) << endl;

		
		int shift = 50;// shift center position
		mean_wire[0] = tt->PFVtxGlobalWire[(int)tt->PFVtxGlobalPlane[0]]+xnbins/2-shift; mean_wire[1] = tt->PFVtxGlobalWire[(int)tt->PFVtxGlobalPlane[1]]-xnbins/2+shift; mean_wire[2] = tt->PFVtxGlobalWire[2]+xnbins/2-shift;
		mean_time[0] = tt->PFVtxGlobalTick[(int)tt->PFVtxGlobalPlane[0]]; mean_time[1] = tt->PFVtxGlobalTick[(int)tt->PFVtxGlobalPlane[1]]; mean_time[2] = tt->PFVtxGlobalTick[2];

		cout << "Using True Vertices " << endl;
		cout << Form("Mean Wire : %d %d %d", mean_wire[0], mean_wire[1], mean_wire[2]) << endl;
		cout << Form("Mean Tick : %d %d %d", mean_time[0], mean_time[1], mean_time[2]) << endl;

		// test with emshower hits
		for (int icount = 0; icount < tt->NHits_Shw; icount++){
			int shwplane =   (int)tt->Hit_Shw_Global_Plane[icount];
			int shwwire =    (int)tt->Hit_Shw_Global_Wire[icount];
			float shwpeakT = (float)tt->Hit_Shw_Global_Tick[icount];

			int pdg = abs(tt->Hit_Shw_PDG[icount]);
			double shwhiteng = tt->Hit_Shw_CorrCharge[icount];

			int rel_wire = shwwire-mean_wire[shwplane];
			float rel_tdc = shwpeakT-(float)mean_time[shwplane];
			rel_wire += xnbins/2+1;
			rel_tdc += (float)ynbins/2+1;
			h2_hit_pdg[0][shwplane]->Fill(rel_wire, rel_tdc, shwhiteng);
			if (pdg != 0 && pdg != 11 && pdg != 22 && pdg != 2112 && pdg != 211 && pdg != 2212){
				pdg = -1;
			}
			h2_hit_pdg[pdg_idx[pdg]][shwplane] -> Fill(rel_wire, rel_tdc, shwhiteng);
			h2_hit_pdg[pdg_idx[pdg]][shwplane] -> SetMarkerColor(pdg_cmap[pdg]);
			h2_hit_pdg[pdg_idx[pdg]][shwplane] -> SetTitle(Form("%s Plane: %d (%d)", hname[shwplane].c_str(), pdg, tt->Hit_Shw_PDG_Mom[icount]));
		}


		cout << "\nStart Fill the Hits on Histograms" << endl;
		int temp_wire_id = -10000;
		int temp_pre_wire = 0;
		int non_categ[3] = {0};
		float non_categ_eng[3] = {0};
		float total_wire_eng[3] = {0};
		for (int icount = 0; icount < tt->n_wires; ++icount){
			int ihit = tt->Wire_assn[icount];
			int iplane = (int)tt->Hit_Global_Plane[ihit];
			int itpc = (int)tt->Hit_TPC[ihit];
			int hit_px_wire = tt->Hit_Global_Wire[ihit];
			int wire_id_in = hit_px_wire; 
			auto adcs = tt->Wire_adc->at(icount);
			auto corradcs = tt->Wire_corradc->at(icount);
			auto ticks = tt->Wire_tick->at(icount);
			auto tags = tt->Wire_tag_ith->at(icount);  
			auto pdgs = tt->Wire_tag_pdg->at(icount);
			auto isshws = tt->Wire_tag_is_shw->at(icount);
			auto mom_pdgs = tt->Wire_tag_pdg_mom->at(icount);

			for (int iraw = 0; iraw < adcs.size(); iraw++){
				float chg_raw     = (float)adcs[iraw];
				float chg_corrraw = (float)corradcs[iraw];
				int tick_raw      = (int) ticks[iraw];
				int pdg           = (int) abs(pdgs[iraw]);
				int mom_pdg       = (int) mom_pdgs[iraw];
				int tag           = (int) tags[iraw];
				int isshw         = (int) isshws[iraw];

				int rel_wire = wire_id_in - mean_wire[iplane];
				int rel_tick = tick_raw - mean_time[iplane];

				int x_idx = rel_wire+xnbins/2+1;
				int y_idx = rel_tick+ynbins/2+1; 
				h2_sample[iplane]->Fill(wire_id_in, tick_raw, chg_corrraw);

				// skip charge outside of pixel map boundary
				if (x_idx <= 0 || x_idx > xnbins) continue;
				if (y_idx <= 0 || y_idx > ynbins) continue;

				h2_evt1[iplane] -> Fill(x_idx, y_idx, chg_corrraw);
				h2_evt_pdg[0][iplane] -> Fill(x_idx, y_idx, chg_corrraw);
				total_wire_eng[iplane] += chg_corrraw;
				if (tag == -10){
					non_categ[iplane] += 1;
					non_categ_eng[iplane] += chg_corrraw;
					pdg = -1;
				}
				h2_evt_pdg[pdg_idx[pdg]][iplane] -> Fill(x_idx, y_idx, chg_corrraw);
				h2_evt_pdg[pdg_idx[pdg]][iplane] -> SetMarkerColor(pdg_cmap[pdg]);
				h2_evt_pdg[pdg_idx[pdg]][iplane] -> SetTitle(Form("%s Plane: %d (%d)", hname[iplane].c_str(), pdg, mom_pdg));

				if (tag != -10){
					if (tag >= n_clust/2) continue;
					h2_tag_pdg[isshw*n_clust/2+tag][iplane] -> Fill(x_idx, y_idx, chg_corrraw);
					h2_tag_pdg[isshw*n_clust/2+tag][iplane] -> SetTitle(Form("%s Plane: %d (%d)", hname[iplane].c_str(), pdg, mom_pdg));
				}


			} // end of iraw
		} // end of icount

		cout << "End of Filling the histograms" << endl;

		count_ntrial ++;

		m1->SetX(trueVtxU_px_wire); m1->SetY(trueVtxU_px_tick);
		m2->SetX(trueVtxV_px_wire); m2->SetY(trueVtxV_px_tick);
		m3->SetX(trueVtxZ_px_wire); m3->SetY(trueVtxZ_px_tick);
		mm1->SetX(trueVtxU_px_wire-mean_wire[0]+xnbins/2+1); mm1->SetY(trueVtxU_px_tick-mean_time[0]+ynbins/2+1);
		mm2->SetX(trueVtxV_px_wire-mean_wire[1]+xnbins/2+1); mm2->SetY(trueVtxV_px_tick-mean_time[1]+ynbins/2+1);
		mm3->SetX(trueVtxZ_px_wire-mean_wire[2]+xnbins/2+1); mm3->SetY(trueVtxZ_px_tick-mean_time[2]+ynbins/2+1);

		if (tt->PFVtxGlobalPlane[0] != 0 && tt->PFVtxGlobalPlane[1] != 1){
			mr1->SetX(tt->PFVtxGlobalWire[1]-mean_wire[0]+xnbins/2+1); mr1->SetY(tt->PFVtxGlobalTick[1]-mean_time[0]+ynbins/2+1);
			mr2->SetX(tt->PFVtxGlobalWire[0]-mean_wire[1]+xnbins/2+1); mr2->SetY(tt->PFVtxGlobalTick[0]-mean_time[1]+ynbins/2+1);
			ma1->SetX(tt->PFVtxGlobalWire[1]); ma1->SetY(tt->PFVtxGlobalTick[1]);
			ma2->SetX(tt->PFVtxGlobalWire[0]); ma2->SetY(tt->PFVtxGlobalTick[0]);
		} else {
			mr1->SetX(tt->PFVtxGlobalWire[0]-mean_wire[0] +xnbins/2+1); mr1->SetY(tt->PFVtxGlobalTick[0]-mean_time[0] +ynbins/2+1);
			mr2->SetX(tt->PFVtxGlobalWire[1]-mean_wire[1] +xnbins/2+1); mr2->SetY(tt->PFVtxGlobalTick[1]-mean_time[1] +ynbins/2+1);
			ma1->SetX(tt->PFVtxGlobalWire[0]); ma1->SetY(tt->PFVtxGlobalTick[0]);
			ma2->SetX(tt->PFVtxGlobalWire[1]); ma2->SetY(tt->PFVtxGlobalTick[1]);
		}
		if (tt->CVNVtxGlobalPlane[0] != 0 && tt->CVNVtxGlobalPlane[1] != 1){
			mc1->SetX(tt->CVNVtxGlobalWire[1]-mean_wire[0]+xnbins/2+1); mc1->SetY(tt->CVNVtxGlobalTick[1]-mean_time[0]+ynbins/2+1);
			mc2->SetX(tt->CVNVtxGlobalWire[0]-mean_wire[1]+xnbins/2+1); mc2->SetY(tt->CVNVtxGlobalTick[0]-mean_time[1]+ynbins/2+1);
			mb1->SetX(tt->CVNVtxGlobalWire[1]); mb1->SetY(tt->CVNVtxGlobalTick[1]);
			mb2->SetX(tt->CVNVtxGlobalWire[0]); mb2->SetY(tt->CVNVtxGlobalTick[0]);
		} else {
			mc1->SetX(tt->CVNVtxGlobalWire[0]-mean_wire[0]+xnbins/2+1); mc1->SetY(tt->CVNVtxGlobalTick[0]-mean_time[0]+ynbins/2+1);
			mc2->SetX(tt->CVNVtxGlobalWire[1]-mean_wire[1]+xnbins/2+1); mc2->SetY(tt->CVNVtxGlobalTick[1]-mean_time[1]+ynbins/2+1);
			mb1->SetX(tt->CVNVtxGlobalWire[0]); mb1->SetY(tt->CVNVtxGlobalTick[0]);
			mb2->SetX(tt->CVNVtxGlobalWire[1]); mb2->SetY(tt->CVNVtxGlobalTick[1]);
		}
		if (tt->CVNVtx2GlobalPlane[0] != 0 && tt->CVNVtx2GlobalPlane[1] != 1){
			mcvn1->SetX(tt->CVNVtx2GlobalWire[1]-mean_wire[0]+xnbins/2+1); mcvn1->SetY(tt->CVNVtx2GlobalTick[1]-mean_time[0]+ynbins/2+1);
			mcvn2->SetX(tt->CVNVtx2GlobalWire[0]-mean_wire[1]+xnbins/2+1); mcvn2->SetY(tt->CVNVtx2GlobalTick[0]-mean_time[1]+ynbins/2+1);
		} else {
			mcvn1->SetX(tt->CVNVtx2GlobalWire[0]-mean_wire[0]+xnbins/2+1); mcvn1->SetY(tt->CVNVtx2GlobalTick[0]-mean_time[0]+ynbins/2+1);
			mcvn2->SetX(tt->CVNVtx2GlobalWire[1]-mean_wire[1]+xnbins/2+1); mcvn2->SetY(tt->CVNVtx2GlobalTick[1]-mean_time[1]+ynbins/2+1);
		}

		mr3->SetX(tt->PFVtxGlobalWire[2]-mean_wire[2] +xnbins/2+1); mr3->SetY(tt->PFVtxGlobalTick[2]-mean_time[2] +ynbins/2+1);
		mc3->SetX(tt->CVNVtxGlobalWire[2]-mean_wire[2]+xnbins/2+1); mc3->SetY(tt->CVNVtxGlobalTick[2]-mean_time[2]+ynbins/2+1);
		mcvn3->SetX(tt->CVNVtx2GlobalWire[2]-mean_wire[2]+xnbins/2+1); mcvn3->SetY(tt->CVNVtx2GlobalTick[2]-mean_time[2]+ynbins/2+1);

		ma3->SetX(tt->PFVtxGlobalWire[2]); ma3->SetY(tt->PFVtxGlobalTick[2]);
		mb3->SetX(tt->CVNVtxGlobalWire[2]); mb3->SetY(tt->CVNVtxGlobalTick[2]);
		cout << "TPC 0 => " << tt->TrueVtxTPC[0] << endl;

		ccc1->cd(1);
		h2_evt1[0]->Draw("colz");
		mm1->Draw("same");
		mr1->Draw("same");
		mc1->Draw("same");
		mcvn1->Draw("same");
		gPad->Update();
		ccc1->cd(2);
		h2_evt1[1]->Draw("colz");
		mm2->Draw("same");
		mr2->Draw("same");
		mc2->Draw("same");
		mcvn2->Draw("same");
		gPad->Update();
		ccc1->cd(3);
		h2_evt1[2]->Draw("colz");
		mm3->Draw("same");
		mr3->Draw("same");
		mc3->Draw("same");
		mcvn3->Draw("same");
		gPad->Update();

		cccZ->cd();
		h2_evt1[2]->Draw("colz");
		mm3->Draw("same");
		mr3->Draw("same");
		mcvn3->Draw("same");
		gPad->Update();


		for (int i = 0; i <3; i++){
			for (int j = 0; j<n_map; j++){
				ccc2->cd(i+1+j*3);
				h2_evt_pdg[j][i]->Draw("colz");
				gPad->Update();
				ccc2_2->cd(i+1+j*3);
				h2_hit_pdg[j][i]->Draw("colz");
				gPad->Update();
			}
			ccc2_3->cd(i+1);
			h2_evt_pdg[n_map-1][i]->Draw("colz");
			gPad->Update();
			cout << "number of hits not categorized: " << non_categ[i] << " EngFrac: " << non_categ_eng[i]/total_wire_eng[i]*100. << endl;
			for (int j = 0; j<n_clust; j++){
				//ccc3[j]->cd(i+1);
				if (j < n_clust/2){
					ccc3[0]->cd(i+1+j*3);
				} else {
					ccc3[1]->cd(i+1+(j%(n_clust/2))*3);
				}
				h2_tag_pdg[j][i]->Draw("colz");
				gPad->Update();
			}
		}
		gPad->Update();


		ccc->cd(1);
		h2_sample[0]->Draw("colz");
		m1->Draw("same");
		ma1->Draw("same");
		mb1->Draw("same");
		gPad->Update();
		ccc->cd(2);
		h2_sample[1]->Draw("colz");
		m2->Draw("same");
		ma2->Draw("same");
		mb2->Draw("same");

		gPad->Update();
		ccc->cd(3);
		h2_sample[2]->Draw("colz");
		m3->Draw("same");
		ma3->Draw("same");
		mb3->Draw("same");

		gPad->Update();
		cout << "Min Wire Sum: " << sum_min_wires << endl;
		cout << "Vertices1 : " << tt->TrueVertexY << " " << tt->CVNVertex[1] << endl;
		cout << "Vertices2 : " << tt->TrueVertexY << " " << tt->CVNVertex2[1] << endl;
		cout << "CNN1 : " << tt->CVNVtxGlobalPlane[0] << " " << tt->CVNVtxGlobalWire[0] <<  " " << tt->CVNVtxGlobalTick[0] << endl;
		cout << "       " << tt->CVNVtxGlobalPlane[1] << " " << tt->CVNVtxGlobalWire[1] <<  " " << tt->CVNVtxGlobalTick[1] << endl;
		cout << "CNN2 : " << tt->CVNVtx2GlobalPlane[0] << " " << tt->CVNVtx2GlobalWire[0] <<  " " << tt->CVNVtx2GlobalTick[0] << endl;
		cout << "       " << tt->CVNVtx2GlobalPlane[1] << " " << tt->CVNVtx2GlobalWire[1] <<  " " << tt->CVNVtx2GlobalTick[1] << endl;

		cout << "TPC #: " << tt->TrueVtxTPC[0] << " " << tt->TrueVtxTPC[1] << endl;
		cout << "     : " << tt->CVNVtxTPC[0] << " " << tt->CVNVtxTPC[1] << endl;
		cout << "     : " << tt->CVNVtx2TPC[0] << " " << tt->CVNVtx2TPC[1] << endl;

		//gSystem->ProcessEvents();
		if (ctest != 'r') {
			cout << "Press Enter to go next event" << endl;
			ctest = getchar();
			if (ctest == 'q') break;
		}
		//if (ctest == 'r') gSystem->Sleep(1000);// sleep 1 sec
		
		for (int i = 0; i < 3; i++){
			h2_evt1[i]->Reset();
			h2_sample[i]->Delete();
			for (int j = 0; j < n_map; j++){
				h2_evt_pdg[j][i]->Reset();
				h2_hit_pdg[j][i]->Reset();
			}
			for (int j = 0; j < n_clust; j++){
				h2_tag_pdg[j][i]->Reset();
			}
		}
		cout << "End of loop" << endl;
	}

	//cout << "Energy Fraction in Map: " << sum_charge_in/(float)count_ntrial << endl;

	TCanvas *can1 = new TCanvas("can1", "can1", 0, 0, 600, 200);

	gtimer.Stop();
	std::cout<<"running time "<<gtimer.CpuTime()<<std::endl;
}
      
