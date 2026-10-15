#include <iostream>
#include <TStopwatch.h>
#include <TMath.h>
#include <TFile.h>
#include <TChain.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TStyle.h>

float convert_to_energy(float charge){
	float fIntShwEnergy = -0.01;
	float fGradShwEnergy = 0.99;
	return ((charge * (1.0 / 0.63) * (23.6e-9 / 4.966e-3)) - fIntShwEnergy) / fGradShwEnergy ;

}

void Plot_Evt_Display_from_Pixel_prong_v2(int inevt = -1, char ctest=0){ 
	//gStyle -> SetOptStat(111111);
	//gStyle -> SetOptFit(1);
        //gStyle->SetOptTitle(0);
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

	TStopwatch gtimer;
	gtimer.Start();
	
	//TString filename ("../make_text/pm_test.root");
	TString filename ("../make_text/test.root");
	//TString filename ("../make_text/pm_test_numu.root");
	//TString filename ("../make_text/Pixel_Map_low_resolution_prong_numu_80.root");
	//TString filename ("../make_text/Pixel_Map_low_resolution_prong_nue_436.root");
	TChain ch("pixelmap");

	ch.Add(filename);

	int b_ievt, b_tpc_id, b_iplane;
	int b_nwires, b_nticks;
	int b_wcoarse, b_tcoarse;
	int b_rel_wire, b_rel_tick, b_mean_wire, b_mean_tick;
	int b_trueVtxWire, b_trueVtxTick;
	float b_shw_tot_energy, b_wire_charge, b_wire_corrcharge, b_hit_tot_energy;
	float b_trueEnergy, b_trueVertex_X, b_trueVertex_Y, b_trueVertex_Z;
	float b_truePx, b_truePy, b_truePz;
	int b_trueVtxGlobalWire, b_trueVtxGlobalTick;
	int b_CVNVtxGlobalWire, b_CVNVtxGlobalTick;
	int b_pfVtxGlobalWire, b_pfVtxGlobalTick;
	int b_trueTPCin, b_trueDETin;

	int b_prong_tag, b_prong_pdg, b_prong_pdg_mom;

	ch.SetBranchAddress("ievent",            &b_ievt);

	ch.SetBranchAddress("tpc_id",            &b_tpc_id);
	ch.SetBranchAddress("iplane",            &b_iplane);

	ch.SetBranchAddress("nwires",            &b_nwires);
	ch.SetBranchAddress("nticks",            &b_nticks);
	ch.SetBranchAddress("wcoarse",           &b_wcoarse);
	ch.SetBranchAddress("tcoarse",           &b_tcoarse);
        ch.SetBranchAddress("rel_wire",          &b_rel_wire);
        ch.SetBranchAddress("rel_tick",          &b_rel_tick);
        ch.SetBranchAddress("mean_wire",         &b_mean_wire);
        ch.SetBranchAddress("mean_tick",         &b_mean_tick);

        ch.SetBranchAddress("trueVtxWire",       &b_trueVtxWire);
        ch.SetBranchAddress("trueVtxTick",       &b_trueVtxTick);
        ch.SetBranchAddress("shw_tot_energy",    &b_shw_tot_energy);
        ch.SetBranchAddress("wire_charge",       &b_wire_charge);
        ch.SetBranchAddress("wire_corrcharge",   &b_wire_corrcharge);
        ch.SetBranchAddress("hit_tot_energy",    &b_hit_tot_energy);
        ch.SetBranchAddress("trueEnergy",        &b_trueEnergy);
        ch.SetBranchAddress("trueVertex_X",      &b_trueVertex_X);
        ch.SetBranchAddress("trueVertex_Y",      &b_trueVertex_Y);
        ch.SetBranchAddress("trueVertex_Z",      &b_trueVertex_Z);
        ch.SetBranchAddress("truePx",            &b_truePx);
        ch.SetBranchAddress("truePy",            &b_truePy);
        ch.SetBranchAddress("truePz",            &b_truePz);

        ch.SetBranchAddress("trueVtxGlobalWire",   &b_trueVtxGlobalWire);
        ch.SetBranchAddress("trueVtxGlobalTick",   &b_trueVtxGlobalTick);
        ch.SetBranchAddress("pfVtxGlobalWire",     &b_pfVtxGlobalWire);
        ch.SetBranchAddress("pfVtxGlobalTick",     &b_pfVtxGlobalTick);
        ch.SetBranchAddress("CVNVtxGlobalWire",    &b_CVNVtxGlobalWire);
        ch.SetBranchAddress("CVNVtxGlobalTick",    &b_CVNVtxGlobalTick);

        ch.SetBranchAddress("trueTPCin",            &b_trueTPCin);
        ch.SetBranchAddress("trueDETin",            &b_trueDETin);

        ch.SetBranchAddress("prong_tag", &b_prong_tag);
        ch.SetBranchAddress("prong_true_pdg", &b_prong_pdg);
        ch.SetBranchAddress("prong_true_pdg_mom", &b_prong_pdg_mom);

	int entries = ch.GetEntries();
	// ticks 0 - 4492, each tick 0.5 us/tick
	// drift velocity 1.6mm/us -> 0.8mm/tick -> 6 ticks -> 4.8 mm
	// wire ptich 4.67mm and 4.79 mm

	// number of wires Z: 480, X and Y: 400.
 	// X and Y plane has 3 indexes per wire

	ch.GetEntry(0);

	int x_coarse = b_wcoarse;
	int y_coarse = b_tcoarse;
	int xnbins = b_nwires;
	int ynbins = b_nticks;

	//int ynbins = 1680;
	//int xnbins = 480;
	//int ynbins = 4494;

	int xmax = xnbins/2;
	int ymax = ynbins/2;

	TH2D *h2_evt_dis[3];
	TH2D *h2_evt_led[3];
	h2_evt_dis[0] = new TH2D("h2_evt_dis_U", "U Plane; Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	h2_evt_dis[1] = new TH2D("h2_evt_dis_V", "V Plane; Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	h2_evt_dis[2] = new TH2D("h2_evt_dis_Z", "Collection Plane; Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	h2_evt_led[0] = new TH2D("h2_evt_led_U", "U Plane; Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	h2_evt_led[1] = new TH2D("h2_evt_led_V", "V Plane; Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	h2_evt_led[2] = new TH2D("h2_evt_led_Z", "Z Plane; Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	TH2D *h2_evt_prongs[10];
	for (int ii = 0; ii < 10; ii++){
		h2_evt_prongs[ii] = new TH2D(
				Form("h2_evt_prongs_%d",ii), "Collection Plane; Rel Wire; Rel Tick", 
				xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
		h2_evt_prongs[ii]->SetMarkerColor(ii+1);
	}

	TMarker *mm[3];
	TMarker *mr[3];
	TMarker *mv[3];
        mm[0] = new TMarker(0, 0, 24); mm[0]->SetMarkerColor(2);
        mm[1] = new TMarker(0, 0, 24); mm[1]->SetMarkerColor(2);
        mm[2] = new TMarker(0, 0, 24); mm[2]->SetMarkerColor(2);
        mr[0] = new TMarker(0, 0, 30); mr[0]->SetMarkerColor(3);
        mr[1] = new TMarker(0, 0, 30); mr[1]->SetMarkerColor(3);
        mr[2] = new TMarker(0, 0, 30); mr[2]->SetMarkerColor(3);
	mr[0]->SetMarkerSize(2);
	mr[1]->SetMarkerSize(2);
	mr[2]->SetMarkerSize(2);
        mv[0] = new TMarker(0, 0, 25); mv[0]->SetMarkerColor(6);
        mv[1] = new TMarker(0, 0, 25); mv[1]->SetMarkerColor(6);
        mv[2] = new TMarker(0, 0, 25); mv[2]->SetMarkerColor(6);


	TCanvas *ccc = new TCanvas("ccc", "Pixel Maps", 10, 10, 1200, 400);
	ccc->Divide(3, 1);
	TCanvas *ccp = new TCanvas("ccp", "Prong Pixel Maps", 10, 10, 1200, 400);
	ccp->Divide(3, 1);
	TCanvas *cccZ = new TCanvas("cccZ", "Pixel Maps Z Plane", 10, 10, 600, 600);
	TCanvas *cccZprong = new TCanvas("cccZprong", "Prong Pixel Maps Z Plane", 10, 10, 600, 600);
	//gPad->SetLeftMargin(0.15);
	//gPad->SetRightMargin(0.15);
	//gPad->SetBottomMargin(0.15);
	//gPad->SetTopMargin(0.15);


	//int xnbins2 = 1160; // maximum number of wire ID +?
	//int xnbins2 = 2880; // maximum number of wire ID +?
	//int ynbins2 = 4493; // maximum number of tick + 1

	// collect first hit info
	ch.GetEntry(0);
	int temp_ievent = b_ievt;
	bool first_hit = true;
	float temp_true_energy = b_trueEnergy;
	int temp_pdg = 0;
	int temp_pdg_mom = 0;
	int temp_trueVtxGlobalWire[3];
	int temp_trueVtxGlobalTick[3];
	int temp_CVNVtxGlobalWire[3];
	int temp_CVNVtxGlobalTick[3];
	int temp_pfVtxGlobalWire[3];
	int temp_pfVtxGlobalTick[3];
	int temp_meanWire[3];
	int temp_meanTick[3];
	int temp_nbins_wire_lm;
	int temp_nbins_tick_lm;
	int temp_rebin_wire_lm;
	int temp_rebin_tick_lm;

	std::cout << "==> " << entries << ", " << temp_ievent << std::endl;
	for (int ie = 0; ie < entries; ie++){
		//if (ie%1000000 == 0) std::cout << ie << " " << b_ievt << std::endl;
		if (ie%1000000 == 0) std::cout << ie << " " << b_ievt << " : " << (float)ie/(float)entries*100. << " " << std::endl;
		ch.GetEntry(ie);
		//if (b_trueDETin == 0) continue;
		//if (b_trueTPCin == 0) continue;
		//cout << ievt << endl;
        	//if (!( inevt>-1 && (temp_ievent == inevt || b_ievt == inevt))) continue;

		if (TMath::IsNaN(b_wire_corrcharge))
			std::cout << b_ievt << " " << b_iplane << " " << b_wire_corrcharge << std::endl;

		if (temp_ievent != b_ievt){

              		if ((ctest != 'r' && inevt==-1)|| (temp_ievent == inevt)) {
				std::cout << "==> event#: " << temp_ievent << std::endl;
				for (int ip = 0; ip < 3; ip++){
					ccc->cd(ip+1);
					if (ip == 1){
						TString s;
						s.Form("evt: %d  TrueE: %.2f (GeV)", temp_ievent, temp_true_energy);
						h2_evt_dis[ip]->SetTitle(s.Data());
						s.Form("evt: %d  TrueE: %.2f (GeV) PDG: %d MOM: %d", temp_ievent, temp_true_energy, temp_pdg, temp_pdg_mom);
						h2_evt_led[ip]->SetTitle(s.Data());
					}
					cout << "plane" << ip << " " << temp_meanWire[ip] << " " << temp_meanTick[ip] << endl;
					cout << "plane" << ip << " " << (temp_CVNVtxGlobalWire[ip]-temp_meanWire[ip]) +xmax << " " << (temp_CVNVtxGlobalTick[ip]-temp_meanTick[ip]) /6.0+ymax << endl;
					mr[ip]->SetX( (temp_trueVtxGlobalWire[ip]-temp_meanWire[ip])/(float)x_coarse+xmax);
					mr[ip]->SetY( (temp_trueVtxGlobalTick[ip]-temp_meanTick[ip])/(float)y_coarse+ymax);
					mm[ip]->SetX( (temp_pfVtxGlobalWire[ip]  -temp_meanWire[ip])/(float)x_coarse+xmax);
					mm[ip]->SetY( (temp_pfVtxGlobalTick[ip]  -temp_meanTick[ip])/(float)y_coarse+ymax);
					mv[ip]->SetX( (temp_CVNVtxGlobalWire[ip]-temp_meanWire[ip]) /(float)x_coarse+xmax);
					mv[ip]->SetY( (temp_CVNVtxGlobalTick[ip]-temp_meanTick[ip]) /(float)y_coarse+ymax);

					h2_evt_dis[ip]->Draw("colz");
					mr[ip]->Draw("same");
					mm[ip]->Draw("same");
					mv[ip]->Draw("same");
					gPad->Update();
					ccp->cd(ip+1);
					h2_evt_led[ip]->Draw("colz");
					mr[ip]->Draw("same");
					mm[ip]->Draw("same");
					mv[ip]->Draw("same");
					gPad->Update();
					if (ip == 2){
					    cccZ->cd();
					    h2_evt_dis[ip]->GetZaxis()->SetRangeUser(-100,1000);
					    h2_evt_dis[ip]->Draw("col");
					    h2_evt_dis[ip]->SetTitleSize(0.05, "XY");
					    h2_evt_dis[ip]->SetTitleOffset(1.1, "X");
					    h2_evt_dis[ip]->SetTitleOffset(1.3, "Y");
					    h2_evt_dis[ip]->GetXaxis()->CenterTitle();
					    h2_evt_dis[ip]->GetYaxis()->CenterTitle();
					    mr[ip]->Draw("same");
					    //mm[ip]->Draw("same");
					    //mv[ip]->Draw("same");
					    for (int ii = 0; ii < 10; ii++){
					    	h2_evt_prongs[ii]->Draw("same");
					    }
					    gPad->Update();

					    cccZprong->cd();
					    h2_evt_led[ip]->Draw("colz");
					    h2_evt_led[ip]->GetZaxis()->SetRangeUser(-100,1000);
					    h2_evt_led[ip]->Draw("col");
					    h2_evt_led[ip]->SetTitleSize(0.05, "XY");
					    h2_evt_led[ip]->SetTitleOffset(1.1, "X");
					    h2_evt_led[ip]->SetTitleOffset(1.3, "Y");
					    h2_evt_led[ip]->GetXaxis()->CenterTitle();
					    h2_evt_led[ip]->GetYaxis()->CenterTitle();
					    mr[ip]->Draw("same");
					    gPad->Update();
					}

				} // end of ip

                        	cout << "Press Enter to go next event" << endl;
                        	if (ctest != 'r') ctest = getchar();
                       		if (ctest == 'q') break;
               		 }

			for (int ip = 0; ip < 3; ip++){
				h2_evt_dis[ip]->Reset();
				h2_evt_led[ip]->Reset();
			}
			for (int ii = 0; ii < 10; ii++){
				h2_evt_prongs[ii]->Reset();
			}


			first_hit = true;
			temp_ievent = b_ievt;
			temp_true_energy = b_trueEnergy;

		} // end of save data

		h2_evt_dis[b_iplane] -> Fill(b_rel_wire, b_rel_tick, b_wire_corrcharge);
		if (b_prong_tag == 1000){
		    h2_evt_led[b_iplane] -> Fill(b_rel_wire, b_rel_tick, b_wire_corrcharge);
		    temp_pdg = b_prong_pdg;
		    temp_pdg_mom = b_prong_pdg_mom;
		}
		if (b_iplane == 2){
			if (1000<=b_prong_tag&&b_prong_tag<=1004){
				h2_evt_prongs[b_prong_tag%1000] -> Fill(b_rel_wire, b_rel_tick);
			} 
			if (0<= b_prong_tag&&b_prong_tag<=4){
				h2_evt_prongs[b_prong_tag+5] -> Fill(b_rel_wire, b_rel_tick);
			}
		}
		temp_trueVtxGlobalWire[b_iplane]  = b_trueVtxGlobalWire;
		temp_trueVtxGlobalTick[b_iplane]  = b_trueVtxGlobalTick;
		temp_CVNVtxGlobalWire[b_iplane]   = b_CVNVtxGlobalWire;
		temp_CVNVtxGlobalTick[b_iplane]   = b_CVNVtxGlobalTick;
		temp_pfVtxGlobalWire[b_iplane]    = b_pfVtxGlobalWire;
		temp_pfVtxGlobalTick[b_iplane]    = b_pfVtxGlobalTick;
		temp_meanWire[b_iplane]           = b_mean_wire;
		temp_meanTick[b_iplane]           = b_mean_tick;


		// to use event level info
		if (first_hit == true && b_iplane == 2){
			first_hit = false;
		}
	} // end of ie
	// This loop miss last event.

	//TFile *output = new TFile("output_plots.root", "recreate");
	//can1->Write();
	//can2->Write();
	//ccc1->Write();
	//ccc2->Write();
	//output->Close();

	gtimer.Stop();
	std::cout<<"running time "<<gtimer.CpuTime()<<std::endl;
}
      
