
TCanvas* make_can(const char* cname, const char* ctitle, int xpos, int ypos, int xwth, int ywth)
{
	TCanvas *cc = new TCanvas(cname, ctitle, xpos, ypos, xwth, ywth);
        cc->SetLeftMargin(0.15);
        cc->SetBottomMargin(0.11);
        cc->SetRightMargin(0.04);
        cc->SetTopMargin(0.08);
	return cc;
}
TH1F* make_hist1d(const char* hname, const char* htitle, int nbins, double xmin, double xmax)
{
	TH1F *hh = new TH1F(hname, htitle, nbins, xmin, xmax);
	hh->GetXaxis()->SetTitleSize(0.05);
	hh->GetYaxis()->SetTitleSize(0.05);
	hh->GetXaxis()->SetLabelSize(0.05);
	hh->GetYaxis()->SetLabelSize(0.05);
	hh->SetLineColorAlpha(kTeal+2, 0.9);
	return hh;
}
void Plot_Hit_GoodnessofFit()
{
	TStopwatch gtimer;
	gtimer.Start();

        gStyle->SetOptTitle(0);
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
	gStyle->SetCanvasPreferGL(1);

	TCanvas *cc1 = make_can("cc1", "Pixel Maps", 40, 10, 500, 500);
	TCanvas *cc2 = make_can("cc2", "Pixel Maps", 40, 40, 500, 500);


	TChain *ch = new TChain("recoEnergy/WC");
	ch->Add("../work/RecoEnergyS_20180918.root");

	cc1->cd();
	TH1F *h1_good      = make_hist1d("h1_good", ";Chi2 / NDF; Entries (a.u)", 1000, -1.5, 7000);
	TH1F *h1_good_zoom = make_hist1d("h1_good_zoom", ";Chi2 / NDF; Entries (a.u)", 1000, -1.5, 15);
	ch->Draw("Hit_Good>>h1_good", "NuCCNCTruth[0]==0&&NuPDGTruth[0]==12");
	ch->Draw("Hit_Good>>h1_good_zoom", "NuCCNCTruth[0]==0&&NuPDGTruth[0]==12");
	cc1->cd();
	cc1->SetLogy();
	h1_good->Draw("hist");
	gPad->Update();
	cc2->cd();
	h1_good_zoom->Draw("hist");
	gPad->Update();

	TCanvas *cc3 = make_can("cc3", "blank", 40, 40, 500, 500);
	auto evtn0 = ch->GetEntries("NuCCNCTruth[0]==0&&NuPDGTruth[0]==12");
	auto n0 = ch->Draw("Hit_Good", "NuCCNCTruth[0]==0&&NuPDGTruth[0]==12");
	auto n1 = ch->Draw("Hit_Good", "NuCCNCTruth[0]==0&&NuPDGTruth[0]==12&&Hit_Good>0&&Hit_Good<100");
	auto fail_rate = float(n0-n1)/float(n1) * 100.;
	printf( "N0: %lld, N1: %lld, (N0-N1)/N0: %.2f\n", n0, n1, fail_rate);



	int ccnc[10], pdg[10], nhits;
	float good[50000];
	ch->SetBranchAddress("NuCCNCTruth",    ccnc);
	ch->SetBranchAddress("NuPDGTruth",     pdg);
	ch->SetBranchAddress("NHits",          &nhits);
	ch->SetBranchAddress("Hit_Good",       good);

	Long64_t nevents = 0;
	float sum_avg = 0.;
	for (Long64_t ie = 0; ie < ch->GetEntries(); ie++){
		ch->GetEntry(ie);
		if (ccnc[0] != 0) continue;
		if (pdg[0] != 12) continue;
		if (nhits == 0) continue;
		int bad_nhits = 0;
		for (int ii = 0; ii < nhits; ii++){
			//if (good[ii] <= 0 || good[ii] > 100){
			if (good[ii] <= 0 || good[ii] > 10){
				bad_nhits += 1;
			}
		}
		sum_avg += (float)bad_nhits / (float)nhits;
		nevents += 1;
	}
	printf("==> %lld, %.2f, %.2f\n", nevents, sum_avg, (sum_avg/(float)nevents));

	gtimer.Stop();
	std::cout<<"running time "<<gtimer.CpuTime()/60.<< " min" << std::endl;

}
