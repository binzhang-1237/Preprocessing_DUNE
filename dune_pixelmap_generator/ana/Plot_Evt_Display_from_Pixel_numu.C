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

void Plot_Evt_Display_from_Pixel_numu(int inevt = -1, char ctest=0){ 
        gStyle->SetOptStat(0);
        gStyle->SetHistLineWidth(2);
        gStyle->SetHistMinimumZero();
        gStyle->SetFrameLineWidth(2);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        gStyle->SetPadLeftMargin(0.13);
        gStyle->SetPadRightMargin(0.09);
        gStyle->SetPadBottomMargin(0.13);
        gStyle->SetPadTopMargin(0.09);

	TStopwatch gtimer;
	gtimer.Start();
	
	TString filename ("../make_text/test_numu_test.root");
	TChain ch("pixelmap");

	ch.Add(filename);

	int b_ievt, b_tpc_id, b_iplane;
	int b_rel_wire, b_rel_tick, b_mean_wire, b_mean_tick;
	int b_trueVtxWire, b_trueVtxTick;
	float b_shw_tot_energy, b_wire_charge, b_wire_corrcharge, b_hit_tot_energy;
	float b_trueEnergy, b_trueVertex_X, b_trueVertex_Y, b_trueVertex_Z;
	float b_truePx, b_truePy, b_truePz;
	int b_trueVtxGlobalWire, b_trueVtxGlobalTick;
	int b_pfVtxGlobalWire, b_pfVtxGlobalTick;
	int b_trueTPCin;
	int b_pmidx;
	int b_rebin_wire, b_rebin_tick;
	int b_nbins_wire, b_nbins_tick;

	ch.SetBranchAddress("ievent",            &b_ievt);
	ch.SetBranchAddress("pmidx",             &b_pmidx);

	ch.SetBranchAddress("tpc_id",            &b_tpc_id);
	ch.SetBranchAddress("iplane",            &b_iplane);
        ch.SetBranchAddress("rel_wire",          &b_rel_wire);
        ch.SetBranchAddress("rel_tick",          &b_rel_tick);
        ch.SetBranchAddress("mean_wire",         &b_mean_wire);
        ch.SetBranchAddress("mean_tick",         &b_mean_tick);
        ch.SetBranchAddress("rebin_wire",        &b_rebin_wire);
        ch.SetBranchAddress("rebin_tick",        &b_rebin_tick);
        ch.SetBranchAddress("nbins_wire",        &b_nbins_wire);
        ch.SetBranchAddress("nbins_tick",        &b_nbins_tick);

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

        ch.SetBranchAddress("trueTPCin",            &b_trueTPCin);

	int entries = ch.GetEntries();
	// ticks 0 - 4492, each tick 0.5 us/tick
	// drift velocity 1.6mm/us -> 0.8mm/tick -> 6 ticks -> 4.8 mm
	// wire ptich 4.67mm and 4.79 mm

	// number of wires Z: 480, X and Y: 400.
 	// X and Y plane has 3 indexes per wire


	int coarse = 6;	
	int xnbins = 400;
	int ynbins = 1680/6;
	//int ynbins = 1680;
	//int xnbins = 480;
	//int ynbins = 4494;

	int xmax = xnbins/2;
	int ymax = ynbins/2;

	TH2D *h2_evt_dis[3];
	h2_evt_dis[0] = new TH2D("h2_evt_dis_U", "U Planel; Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	h2_evt_dis[1] = new TH2D("h2_evt_dis_V", "V Planel; Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	h2_evt_dis[2] = new TH2D("h2_evt_dis_Z", "Z Planel; Rel Wire; Rel Tick", xnbins, 0, 2*xmax, ynbins, 0, 2*ymax);
	TString ss;
	const int npm = 40;
	TH2D *h2_hit_map[3][npm];
	TCanvas *c_pm[3];
	for (int ip = 0; ip < 3; ip++){
		ss.Form("cc_pm%d", ip);
		c_pm[ip] = new TCanvas(ss, "Pixel Maps SPM", 10, 10, 1000, 800);
		c_pm[ip]->Divide(7, 6);
		ss.Form("h2_hit_map_%d_pm%d", ip, 0);
		h2_hit_map[ip][0] = new TH2D(ss, "; Rel Wire; Rel Tick", 400, 0, 400, 280, 0, 280);

		for (int ipm = 1; ipm < npm; ipm++){
			ss.Form("h2_hit_map_%d_pm%d", ip, ipm);
			h2_hit_map[ip][ipm] = new TH2D(ss, "; Rel Wire; Rel Tick", 100, 0, 100, 120, 0, 120);
	  	}
	}

	TMarker *mm[3];
	TMarker *mr[3];
        mm[0] = new TMarker(0, 0, 24); mm[0]->SetMarkerColor(2);
        mm[1] = new TMarker(0, 0, 24); mm[1]->SetMarkerColor(2);
        mm[2] = new TMarker(0, 0, 24); mm[2]->SetMarkerColor(2);
        mr[0] = new TMarker(0, 0, 30); mr[0]->SetMarkerColor(3);
        mr[1] = new TMarker(0, 0, 30); mr[1]->SetMarkerColor(3);
        mr[2] = new TMarker(0, 0, 30); mr[2]->SetMarkerColor(3);
	mr[0]->SetMarkerSize(2);
	mr[1]->SetMarkerSize(2);
	mr[2]->SetMarkerSize(2);


	TH1F *h1_energy_ref = new TH1F("h1_eng_ref", "Reco E with Z;(RecoE-TrueE)/TrueE", 100, -1.1, 1.1);
	TH1F *h1_energy_map = new TH1F("h1_eng_map", "Using Same Pixel Map Reco E with Z;RecoE/TrueE", 100, -1.1, 1.1);
	TH1F *h1_energy[3];
	TH1F *h1_energy_pos[3];
	h1_energy[0] = new TH1F("h1_eng_U", ";(Sum E-TrueE)/TrueE", 100, -1.1, 1.1);
	h1_energy[1] = new TH1F("h1_eng_V", ";(Sum E-TrueE)/TrueE", 100, -1.1, 1.1);
	h1_energy[2] = new TH1F("h1_eng_Z", ";(Sum E-TrueE)/TrueE", 100, -1.1, 1.1);
	h1_energy_pos[0] = new TH1F("h1_eng_U_pos", "Add pos charge only;(Sum E-TrueE)/TrueE", 100, -1.1, 1.1);
	h1_energy_pos[1] = new TH1F("h1_eng_V_pos", "Add pos charge only;(Sum E-TrueE)/TrueE", 100, -1.1, 1.1);
	h1_energy_pos[2] = new TH1F("h1_eng_Z_pos", "Add pos charge only;(Sum E-TrueE)/TrueE", 100, -1.1, 1.1);

	TH2F *h2_energy[3];
	h2_energy[0] = new TH2F("h2_eng_U", ";True E (GeV) ;(Sum E-TrueE)/TrueE", 100, 0, 7, 100, -1.1, 1.1);
	h2_energy[1] = new TH2F("h2_eng_V", ";True E (GeV) ;(Sum E-TrueE)/TrueE", 100, 0, 7, 100, -1.1, 1.1);
	h2_energy[2] = new TH2F("h2_eng_Z", ";True E (GeV) ;(Sum E-TrueE)/TrueE", 100, 0, 7, 100, -1.1, 1.1);


	TCanvas *ccc = new TCanvas("ccc", "Pixel Maps", 10, 10, 1200, 400);
	ccc->Divide(3, 1);


	//int xnbins2 = 1160; // maximum number of wire ID +?
	int xnbins2 = 2880; // maximum number of wire ID +?
	int ynbins2 = 4493; // maximum number of tick + 1

	// collect first hit info
	ch.GetEntry(0);
	int temp_ievent = b_ievt;
	bool first_hit = true;
	float temp_true_energy = b_trueEnergy;
	
	int temp_trueVtxGlobalWire[3];
	int temp_trueVtxGlobalTick[3];
	int temp_pfVtxGlobalWire[3];
	int temp_pfVtxGlobalTick[3];
	int temp_meanWire[3];
	int temp_meanTick[3];
	int temp_nbins_wire_lm;
	int temp_nbins_tick_lm;
	int temp_rebin_wire_lm;
	int temp_rebin_tick_lm;

	float sum_charge[3] = {0, 0, 0};
	float sum_charge_pos[3] = {0, 0, 0};
	std::cout << "==> " << entries << ", " << temp_ievent << std::endl;
	for (int ie = 0; ie < entries; ie++){
		if (ie%1000000 == 0) std::cout << ie << " " << b_ievt << " : " << (float)ie/(float)entries*100. << " " << std::endl;
		ch.GetEntry(ie);

		if (TMath::IsNaN(b_wire_corrcharge))
			std::cout << b_ievt << " " << b_iplane << " " << b_wire_corrcharge << std::endl;

		if (temp_ievent != b_ievt){
			for (int ip = 0; ip < 3; ip++){
				float sum_eng = convert_to_energy(sum_charge[ip]);
				float sum_eng_pos = convert_to_energy(sum_charge_pos[ip]);
				h1_energy[ip] -> Fill((sum_eng-temp_true_energy)/temp_true_energy);
				h1_energy_pos[ip] -> Fill((sum_eng_pos-temp_true_energy)/temp_true_energy);

				h2_energy[ip] -> Fill(temp_true_energy, (sum_eng-temp_true_energy)/temp_true_energy);

				sum_charge[ip] = 0;
				sum_charge_pos[ip] = 0;
			}

              		if ((ctest != 'r' && inevt==-1)|| (temp_ievent == inevt)) {
				std::cout << "==> event#: " << temp_ievent << std::endl;
				for (int ip = 0; ip < 3; ip++){
					ccc->cd(ip+1);
					if (ip == 1){
						TString s;
						s.Form("evt: %d  TrueE: %.2f (GeV)", temp_ievent, temp_true_energy);
						h2_evt_dis[ip]->SetTitle(s.Data());
					}

					mr[ip]->SetX( (temp_trueVtxGlobalWire[ip]-temp_meanWire[ip])/temp_rebin_wire_lm+temp_nbins_wire_lm/2);
					mr[ip]->SetY( (temp_trueVtxGlobalTick[ip]-temp_meanTick[ip])/temp_rebin_tick_lm+temp_nbins_tick_lm/2);
					mm[ip]->SetX( (temp_pfVtxGlobalWire[ip]  -temp_meanWire[ip])/temp_rebin_wire_lm+temp_nbins_wire_lm/2);
					mm[ip]->SetY( (temp_pfVtxGlobalTick[ip]  -temp_meanTick[ip])/temp_rebin_tick_lm+temp_nbins_tick_lm/2);


					h2_evt_dis[ip]->Draw("colz");
					mr[ip]->Draw("same");
					mm[ip]->Draw("same");
					gPad->Update();
					for (int ipm = 0; ipm < npm; ipm++){
						c_pm[ip]->cd(ipm+1);
						h2_hit_map[ip][ipm]->Draw("colz");
					}
					gPad->Update();
				} // end of ip

                        	cout << "Press Enter to go next event" << endl;
                        	if (ctest != 'r') ctest = getchar();
                       		if (ctest == 'q') break;
				if (temp_ievent == inevt){
					TString s;
					gROOT->SetBatch();
					s.Form("numu_pm_examples/numu_pm_example_ievt_%d.png", inevt);
					ccc->Print(s.Data());
					int can_scale = 2;
					for (int ip = 0; ip < 3; ip++){
						s.Form("numu_pm_examples/numu_pm_example_ievt_%d_spm_%d.png", inevt, ip);
						//c_pm[ip]->SetBatch(1);
						c_pm[ip]->SetCanvasSize(can_scale*1000, can_scale*800);
						c_pm[ip]->Update();
						c_pm[ip]->Print(s.Data());
					}
					break;
				}
               		 }
			for (int ip = 0; ip < 3; ip++){
				h2_evt_dis[ip]->Reset();
				for (int ipm = 0; ipm < npm; ipm++){
					h2_hit_map[ip][ipm] -> Reset();
			  	}
			}


			first_hit = true;
			temp_ievent = b_ievt;
			temp_true_energy = b_trueEnergy;

		} // end of save data

		sum_charge[b_iplane] += b_wire_corrcharge;
		if (b_wire_corrcharge>0) sum_charge_pos[b_iplane] += b_wire_corrcharge;

		if (b_pmidx == 0){
			h2_evt_dis[b_iplane] -> Fill(b_rel_wire, b_rel_tick, b_wire_corrcharge);
			temp_trueVtxGlobalWire[b_iplane]  = b_trueVtxGlobalWire;
			temp_trueVtxGlobalTick[b_iplane]  = b_trueVtxGlobalTick;
			temp_pfVtxGlobalWire[b_iplane]    = b_pfVtxGlobalWire;
			temp_pfVtxGlobalTick[b_iplane]    = b_pfVtxGlobalTick;
			temp_meanWire[b_iplane]           = b_mean_wire;
			temp_meanTick[b_iplane]           = b_mean_tick;
			temp_nbins_wire_lm 		  = b_nbins_wire;
			temp_nbins_tick_lm 		  = b_nbins_tick;
			temp_rebin_wire_lm 		  = b_rebin_wire;
			temp_rebin_tick_lm 		  = b_rebin_tick;

		}
		else {
			h2_hit_map[b_iplane][b_pmidx-1] -> Fill(b_rel_wire, b_rel_tick, b_wire_corrcharge);
		}


		// to use event level info
		if (first_hit == true && b_iplane == 2){
			h1_energy_ref->Fill((b_hit_tot_energy-b_trueEnergy)/b_trueEnergy);
			first_hit = false;
		}
	} // end of ie
	// This loop miss last event.

	TCanvas *can1 = new TCanvas("can1", "RecoE / True E with Z plane (cal.)", 400, 0, 400, 400);
	TF1 *ff_can1 = new TF1("ff_can1", "gaus", -0.3, 0.3);
	ff_can1->SetParameters(100, 0, 1);
	h1_energy_ref->Fit("ff_can1", "R");
	h1_energy_ref->Draw("hist");
	ff_can1->Draw("same");
	ff_can1->SetLineColor(2);
	gPad->Update();

	TCanvas *can2 = new TCanvas("can2", "RecoE / True E with Z plane", 400, 0, 400, 400);
	h1_energy_map->Draw("hist");
	gPad->Update();

	TCanvas *ccc1 = new TCanvas("ccc1", "Sum Charge / True E", 0, 0, 1200, 400);
	TCanvas *ccc2 = new TCanvas("ccc2", "Sum Charge / True E (Sum Pos. Charge only)", 0, 0, 1200, 400);
	ccc1 -> Divide(3, 1);
	ccc2 -> Divide(3, 1);
	TF1 *f1[3];
	char buf[10];
	for (int ip = 0; ip < 3; ip++){
		sprintf(buf, "f1_%d", ip);
		f1[ip] = new TF1(buf, "gaus", -0.3, 0.3);
		f1[ip]->SetParameters(100, 0, 1);
		ccc1->cd(ip+1);
		h1_energy[ip] -> Fit(buf, "R");
		h1_energy[ip] -> Draw("hist");
		f1[ip]->SetLineColor(2);
		f1[ip]->Draw("same");
		gPad->Update();
		ccc2->cd(ip+1);
		h1_energy_pos[ip] -> Draw("hist");
		gPad->Update();
	}


	TCanvas *ccc3 = new TCanvas("ccc3", "Sum Charge / True E", 0, 0, 1200, 400);
	ccc3->Divide(3, 1);
	ccc3->cd(1);
	h2_energy[0]->Draw("colz");
	ccc3->cd(2);
	h2_energy[1]->Draw("colz");
	ccc3->cd(3);
	h2_energy[2]->Draw("colz");
	gPad->Update();


	gtimer.Stop();
	std::cout<<"running time "<<gtimer.CpuTime()<<std::endl;
}
      
