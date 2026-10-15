#include "RecoEnergyClass.h"
#include <iostream>
#include <TStopwatch.h>
#include <TMath.h>
#include <TFile.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>


void Make_Example(int evt_in, int plane_in, int wire_in){
	gROOT->Reset();
	//gStyle -> SetOptStat(0);
	//gStyle -> SetOptStat(111111);
        gStyle->SetOptTitle(0);
        gStyle->SetOptStat(0);
        gStyle->SetHistLineWidth(2);
        gStyle->SetHistMinimumZero();
        gStyle->SetFrameLineWidth(2);
        //gStyle->SetPadGridX(1);
        //gStyle->SetPadGridY(1);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        //gStyle->SetPadLeftMargin(0.11);
        //gStyle->SetPadRightMargin(0.05);
        gStyle->SetPadLeftMargin(0.13);
        gStyle->SetPadRightMargin(0.09);
        gStyle->SetPadBottomMargin(0.13);
        gStyle->SetPadTopMargin(0.09);

	gStyle->SetCanvasPreferGL(1);

	TStopwatch gtimer;
	gtimer.Start();
	
	TString filename ("../work/RecoEnergyS.root");
	//TString filename ("../work/RecoEnergyS_20180918.root");
	//TString filename ("../work/RecoEnergyS.7GeV.root");
	TChain *ch = new TChain("recoEnergy/WC");

	ch->Add(filename);

	RecoEnergyClass *tt = new RecoEnergyClass(ch);

	int entries = tt->fChain->GetEntries();
	// ticks 0 - 4492, each tick 0.5 us/tick
	// drift velocity 1.6mm/us -> 0.8mm/tick -> 6 ticks -> 4.8 mm
	// wire ptich 4.67mm and 4.79 mm

	// number of wires Z: 480, X and Y: 400.
 	// X and Y plane has 3 indexes per wire
	

	TCanvas *ccc = new TCanvas("ccc", "Pixel Maps", 10, 10, 1200, 400);
	ccc->Divide(3, 1);
	TCanvas *cc2 = new TCanvas("cc2", "Pixel Maps", 10, 410, 1200, 400);
        cc2->SetLeftMargin(0.05);
        cc2->SetRightMargin(0.02);
        cc2->SetBottomMargin(0.11);
        cc2->SetTopMargin(0.02);

	//int xnbins2 = 2880; // maximum number of wire ID +?
	//int ynbins2 = 4493; // maximum number of tick + 1

	int xnbins = 400;
	int ynbins = 1680;
	int xmax = xnbins/2;
	int ymax = ynbins/2;

	TH2F *h2_sample_U;
	TH2F *h2_sample_V;
	TH2F *h2_sample_Z;

	TH1F *h1_ADC;

	TMarker *m1 = new TMarker(0, 0, 21);
	TMarker *m2 = new TMarker(0, 0, 21);
	TMarker *m3 = new TMarker(0, 0, 21);

	// this is an example for gaus fit fail
	//int target_evt = 7;
	//int which_wire = 240;
	//int which_plane = 2;
	int target_evt = evt_in;
	int which_plane = plane_in;
	int which_wire = wire_in;

	vector<TF1*> gaus_hit;
	char ctest;
	for (Long64_t ie = 0; ie < entries; ie++){
		if (ie%100 == 0) std::cout << ie << std::endl;
		tt->fChain->GetEntry(ie);
		
		cout << tt->ievent << endl;
		if (tt->ievent > target_evt) break;
		if (tt->ievent != target_evt) continue;
		//if (tt->TrueVtxin != 1) continue;

		int sum_wire[3]    = {0,};
		int sum_time[3]    = {0,};
		int nhits_count[3] = {0,};
		int sum_corrchg[3] = {0,};
		int count_gaus = 0;
		int multi_gaus = 0;
		for (int j = 0; j < tt->NHits; j++){
			int iplane = (int)tt->Hit_Plane[j];
			//int hit_px_wire = tt->Hit_Global_Wire[j]; 
			//int peak_time = round(tt->Hit_Global_Tick[j]);
			int hit_px_wire = tt->Hit_Wire[j]; 
			int peak_time = (int)round(tt->Hit_PeakT[j]);
			if (hit_px_wire == which_wire && iplane == which_plane){
				float amp = tt->Hit_Amp[j];
				float mean = tt->Hit_PeakT[j];
				float rms = tt->Hit_RMST[j];
				float minusT = tt->Hit_MinusT[j]-rms*1.5;
				float plusT = tt->Hit_PlusT[j]+rms*1.5;
				int multi = tt->Hit_Multi[j];
				cout << "Multi :" << multi << endl;
				if (multi == 1){
					gaus_hit.push_back( new TF1(Form("f1_%d",count_gaus), "gaus", minusT, plusT) );
					gaus_hit[count_gaus]->SetParameters(amp, mean, rms);
					gaus_hit[count_gaus]->SetNpx(1000);
					count_gaus ++;
}
				else {
					if (multi_gaus == 0){
						TString form;
						for (int im = 0; im < multi-1; ++im){
							form += Form("gaus(%d)+",im*3);
						} form += Form("gaus(%d)",(multi-1)*3);
						plusT = tt->Hit_PlusT[j+multi-1]+rms*1.5;
						gaus_hit.push_back( new TF1(Form("f1_%d",count_gaus), form.Data(), minusT, plusT) );			    }
					gaus_hit[count_gaus]->SetNpx(1000);
					gaus_hit[count_gaus]->SetParameter(0+multi_gaus*3, amp);
					gaus_hit[count_gaus]->SetParameter(1+multi_gaus*3, mean);
					gaus_hit[count_gaus]->SetParameter(2+multi_gaus*3, rms);
					multi_gaus ++;
					if (multi_gaus == multi) {
						multi_gaus = 0;
						count_gaus ++;
					}
				}
				//cout << "==>" << amp << " " << mean << " " << rms << endl;
			}

			sum_wire[iplane] += hit_px_wire; 
			sum_time[iplane] += peak_time; 
			nhits_count[iplane] += 1;
		}
		// get mean values
		int mean_wire[3] = {0,};
		int mean_time[3] = {0,};
		for (int ii = 0; ii < 3; ii++){
			mean_wire[ii] = int( (float)sum_wire[ii]/(float)nhits_count[ii] + 0.5 );
			mean_time[ii] = int( (float)sum_time[ii]/(float)nhits_count[ii] + 0.5 );
		}
		h2_sample_U = new TH2F("h2_sample_U", "U plane;Wire ID; Tick", 
		   xnbins, mean_wire[0]-xmax, mean_wire[0]+xmax, ynbins, mean_time[0]-ymax, mean_time[0]+ymax);
		h2_sample_V = new TH2F("h2_sample_V", "V plane;Wire ID; Tick", 
		   xnbins, mean_wire[1]-xmax, mean_wire[1]+xmax, ynbins, mean_time[1]-ymax, mean_time[1]+ymax);
		h2_sample_Z = new TH2F("h2_sample_Z", "Z plane;Wire ID; Tick",
		   xnbins, mean_wire[2]-xmax, mean_wire[2]+xmax, ynbins, mean_time[2]-ymax, mean_time[2]+ymax);

		cout << "Mean :" << mean_time[0] << " " << mean_time[1] << " " << mean_time[2]  << endl;		
		cout << "\nStart Fill the Hits on Histograms" << endl;
		int tick_min = 1e+7;
		int tick_max = 0;
		int which_count = -1;
		for (int icount = 0; icount < tt->n_wires; ++icount){
			int ihit = tt->Wire_assn[icount];
			int iplane = (int)tt->Hit_Plane[ihit];
			int itpc = (int)tt->Hit_TPC[ihit];
			//int hit_px_wire = tt->Hit_Global_Wire[ihit];
			int hit_px_wire = tt->Hit_Wire[ihit];
			int wire_id_in = hit_px_wire; 

			auto adcs = tt->Wire_adc->at(icount);
			auto corradcs = tt->Wire_corradc->at(icount);
			auto ticks = tt->Wire_tick->at(icount);
			for (int iraw = 0; iraw < adcs.size(); iraw++){
				float chg_raw     = (float)adcs[iraw];
				float chg_corrraw = (float)corradcs[iraw];
				int tick_raw      = (int) ticks[iraw];
				//if (itpc%2 == 0) tick_raw = -tick_raw;
				if (wire_id_in == which_wire && iplane == which_plane){
					if (tick_raw < tick_min) tick_min = tick_raw;
					if (tick_raw > tick_max) tick_max = tick_raw;
					which_count = icount;
				}

				if (iplane==0){
					h2_sample_U->Fill(wire_id_in, tick_raw, chg_raw);
				}
				if (iplane==1){
					h2_sample_V->Fill(wire_id_in, tick_raw, chg_raw);
				}
				if (iplane==2){
					h2_sample_Z->Fill(wire_id_in, tick_raw, chg_raw);
				}
			} // end of iraw
		} // end of icount
		if (which_count>0){
			cout << "==>" << which_count << " " << which_wire << " " << tick_min << " " << tick_max << endl;
			h1_ADC = new TH1F("h1_ADC", "; Tick; ADC", tick_max-tick_min+40, tick_min-20, tick_max+20);
			int ihit = tt->Wire_assn[which_count];
			auto adcs = tt->Wire_adc->at(which_count);
			auto ticks = tt->Wire_tick->at(which_count);
			for (int iraw = 0; iraw < adcs.size(); iraw++){
				float chg_raw     = (float)adcs[iraw];
				int tick_raw      = (int) ticks[iraw];
				h1_ADC->Fill(tick_raw, chg_raw);
			}
		}

		cout << "End of Filling the histograms" << endl;
		double trueVtxU_px_wire = tt->TrueVtxWire[0];
		double trueVtxV_px_wire = tt->TrueVtxWire[1];
		double trueVtxZ_px_wire = tt->TrueVtxWire[2];
		double trueVtxU_px_tick = tt->TrueVtxTime[0];
		double trueVtxV_px_tick = tt->TrueVtxTime[1];
		double trueVtxZ_px_tick = tt->TrueVtxTime[2];
	
		m1->SetX(trueVtxU_px_wire); m1->SetY(trueVtxU_px_tick);
		m2->SetX(trueVtxV_px_wire); m2->SetY(trueVtxV_px_tick);
		m3->SetX(trueVtxZ_px_wire); m3->SetY(trueVtxZ_px_tick);

		ccc->cd(1);
		h2_sample_U->GetYaxis()->SetTitleOffset(1.7);
		h2_sample_U->GetYaxis()->SetTitleSize(0.04);
		h2_sample_U->GetXaxis()->SetTitleSize(0.04);
		h2_sample_U->GetYaxis()->SetLabelSize(0.04);
		h2_sample_U->GetXaxis()->SetLabelSize(0.04);
		h2_sample_U->Draw("colz");
		m1->Draw("same");
		gPad->Update();
		ccc->cd(2);
		h2_sample_V->GetYaxis()->SetTitleOffset(1.7);
		h2_sample_V->GetYaxis()->SetTitleSize(0.04);
		h2_sample_V->GetXaxis()->SetTitleSize(0.04);
		h2_sample_V->GetYaxis()->SetLabelSize(0.04);
		h2_sample_V->GetXaxis()->SetLabelSize(0.04);
		h2_sample_V->Draw("colz");
		m2->Draw("same");
		gPad->Update();
		ccc->cd(3);
		h2_sample_Z->GetYaxis()->SetTitleOffset(1.7);
		h2_sample_Z->GetYaxis()->SetTitleSize(0.04);
		h2_sample_Z->GetXaxis()->SetTitleSize(0.04);
		h2_sample_Z->GetYaxis()->SetLabelSize(0.04);
		h2_sample_Z->GetXaxis()->SetLabelSize(0.04);
		h2_sample_Z->Draw("colz");
		m3->Draw("same");
		gPad->Update();
		ccc->Print(Form("Evt_Display_%d.pdf", evt_in), "pdf");

		cc2->cd();
		if(h1_ADC){
			h1_ADC->Draw("hist");
			for (int ig = 0; ig < gaus_hit.size(); ig++){
				cout << gaus_hit[ig]->GetTitle() << endl;
				//gaus_hit[ig]->SetLineColor(kOrange+7);
				gaus_hit[ig]->SetLineColorAlpha(kOrange+7, 0.9);
				gaus_hit[ig]->SetLineWidth(3);
				gaus_hit[ig]->Draw("same");
			}
			h1_ADC->GetYaxis()->SetTitleOffset(0.5);
			h1_ADC->GetYaxis()->SetTitleSize(0.05);
			h1_ADC->GetXaxis()->SetTitleSize(0.05);
			h1_ADC->GetYaxis()->SetLabelSize(0.05);
			h1_ADC->GetXaxis()->SetLabelSize(0.05);
			cc2->Print(Form("Charge_Distribution_%d_%d_%d.pdf",evt_in, plane_in, wire_in), "pdf");


		}

		gPad->Update();
		

		//if (ctest != 'r') {
		//	cout << "Press Enter to go next event" << endl;
		//	ctest = getchar();
		//	if (ctest == 'q') break;
		//}
		//h2_sample_U->Delete();
		//h2_sample_V->Delete();
		//h2_sample_Z->Delete();
		//if (h1_ADC) h1_ADC->Delete();
		//cout << "End of loop" << endl;
	}



	gtimer.Stop();
	std::cout<<"running time "<<gtimer.CpuTime()<<std::endl;
}
      
