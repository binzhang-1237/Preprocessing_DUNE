TH1F* make_hist1d(char *hname, char *htitle, int nbins, float xmin, float xmax, int icolor = 1){
	TH1F *h1 = new TH1F(hname, htitle, nbins, xmin, xmax);
	h1->SetLineWidth(1);
	h1->SetLineColor(icolor);
	h1->SetMarkerColor(icolor);
	h1->SetLabelSize(0.05, "XY");
	h1->SetTitleOffset(1.1, "X");
	h1->SetTitleOffset(1.2, "Y");
	h1->GetXaxis()->CenterTitle(1);
	h1->GetYaxis()->SetTitle("Entries (a.u.)");
	return h1;
}
void SetPadBorder(TVirtualPad *pad){
	pad->SetLeftMargin(0.13);
	pad->SetBottomMargin(0.12);
	pad->SetRightMargin(0.02);
	//pad->SetTopMargin(0.02);
	pad->SetTopMargin(0.1);
}

void Get_Mean_RMS(vector<float> v, float &mean, float &rms){
	float sum = std::accumulate(std::begin(v), std::end(v), 0.0);
	float m =  sum / v.size();
	
	float accum = 0.0;
	std::for_each (std::begin(v), std::end(v), [&](const float d) {
	    accum += (d - m) * (d - m);
	});
	
	//float stdev = sqrt(accum / (v.size()-1));
	float stdev = sqrt(accum / (v.size()));
	mean = m;
	rms = stdev;
}
void Quick_Plot_Vertex_Resolution(){


	TChain ch("regcnnvtxana/anatree");
	ch.Add("merged_regcnn_ana_vtx_with_tpc_v2.root");
	//ch.Add("merge_regcnnvtx.root");


	int ievent;
	int NuTruthFid;
	double TrueVertexX, TrueVertexY, TrueVertexZ;
	double PandVertexX, PandVertexY, PandVertexZ;
	float CVNVertex[3];
	float CVNVertex1ststep[3];
	float CVNVertex2ndstep[3];
	float CVNVertex2[3];

	const unsigned int ntrue = 10;
	int NuPDGTruth[ntrue];
	int NuCCNCTruth[ntrue]; 
	double NuEngTruth[ntrue];

	ch.SetBranchAddress("ievent", &ievent);
	ch.SetBranchAddress("FidCut", &NuTruthFid);
	ch.SetBranchAddress("NuCCNCTruth", NuCCNCTruth);
	ch.SetBranchAddress("NuPDGTruth", NuPDGTruth);
	ch.SetBranchAddress("NuEngTruth", NuEngTruth);

	ch.SetBranchAddress("NuVtxXTruth", &TrueVertexX);
	ch.SetBranchAddress("NuVtxYTruth", &TrueVertexY);
	ch.SetBranchAddress("NuVtxZTruth", &TrueVertexZ);

	ch.SetBranchAddress("CNNVertex", CVNVertex);
	ch.SetBranchAddress("CNNVertex1ststep", CVNVertex1ststep);
	ch.SetBranchAddress("CNNVertex2ndstep", CVNVertex2ndstep);
	ch.SetBranchAddress("CNNVertex2", CVNVertex2);

	ch.SetBranchAddress("PandNuVertexX", &PandVertexX);
	ch.SetBranchAddress("PandNuVertexY", &PandVertexY);
	ch.SetBranchAddress("PandNuVertexZ", &PandVertexZ);



	string surfix[4] = {"X", "Y", "Z", "R"};
	TH1F *h1_pand[4], *h1_cvn1[4], *h1_cvn2[4];
	//int nbins = 100;
	//float xrange = 20;
	int nbins = 10;
	float xrange = 2;
	for (int ii = 0; ii < 3; ii++){
		h1_pand[ii] = make_hist1d(Form("h1_pand_%s",surfix[ii].c_str()), 
				          Form(";#Delta %s (cm)", surfix[ii].c_str()),
					  nbins, -xrange, xrange, 2);
		h1_cvn1[ii] = make_hist1d(Form("h1_cvn1_%s",surfix[ii].c_str()), 
			      		  Form(";#Delta %s (cm)", surfix[ii].c_str()),
			      		  nbins, -xrange, xrange, 8);
		h1_cvn2[ii] = make_hist1d(Form("h1_cvn2_%s",surfix[ii].c_str()), 
				          Form(";#Delta %s (cm)", surfix[ii].c_str()),
					  nbins, -xrange, xrange, 4);
	}
	int nbins2 = 100000;
	float xrange2 = 1500;
	//int nbins2 = 100;
	//float xrange2 = 100;
	h1_pand[3] = make_hist1d(Form("h1_pand_%s",surfix[3].c_str()), 
			          Form(";#Delta %s (cm)", surfix[3].c_str()),
				  nbins2, 0, xrange2, 8);
	h1_cvn1[3] = make_hist1d(Form("h1_cvn1_%s",surfix[3].c_str()), 
		      		  Form(";#Delta %s (cm)", surfix[3].c_str()),
		      		  nbins2, 0, xrange2, 2);
	h1_cvn2[3] = make_hist1d(Form("h1_cvn2_%s",surfix[3].c_str()), 
			          Form(";#Delta %s (cm)", surfix[3].c_str()),
				  nbins2, 0, xrange2, 4);


	int entries = ch.GetEntries();
	cout << "Entries: " << entries << endl;
	vector<vector<float>> dv_pand(3);
	vector<vector<float>> dv_cvn1(3);
	vector<vector<float>> dv_cvn2(3);

	int count_none_pand = 0;
	int count_none_cvn1 = 0;
	int count_none_cvn2 = 0;

	int evt_count = 0;
	for (int ie = 0; ie < entries; ie++){
		ch.GetEntry(ie);
		if (ie%100000==0) cout << "=> " << ie << endl;
		if (NuCCNCTruth[0] != 0) continue;
		if (abs(NuPDGTruth[0]) != 12) continue;
		if (NuTruthFid != 1) continue;
		//if (NuEngTruth[0] > 10) continue;

		evt_count ++;
		float true_vtx[3] = {(float)TrueVertexX, (float)TrueVertexY, (float)TrueVertexZ};
		float pand_vtx[3] = {(float)PandVertexX, (float)PandVertexY, (float)PandVertexZ};

		float dr[3] = {0, 0, 0};
		for (int ii = 0; ii < 3; ii++){
			float cvn1_vtx = CVNVertex[ii];
			if (cvn1_vtx < -999){
				cvn1_vtx = CVNVertex2ndstep[ii];
				if (cvn1_vtx < -999){
					cvn1_vtx = CVNVertex1ststep[ii];
				}
			}

			float d_cvn1 = cvn1_vtx - true_vtx[ii];
			float d_cvn2 = CVNVertex2[ii] - true_vtx[ii];
			float d_pand = pand_vtx[ii] - true_vtx[ii];
			//if ((true_vtx[ii]>20&&CVNVertex2[ii]<-20)||(true_vtx[ii]<-20&&CVNVertex2[ii]>20)){
			//	d_cvn2 = true_vtx[ii]+CVNVertex2[ii];
			//}
			if (pand_vtx[ii] > -999) {
				h1_pand[ii]->Fill(d_pand);
				dv_pand[ii].push_back(d_pand);
				dr[0] += pow(d_pand,2);
			}
			if (cvn1_vtx > -999) {
				h1_cvn1[ii]->Fill(d_cvn1);
				dv_cvn1[ii].push_back(d_cvn1);
				dr[1] += pow(d_cvn1,2);
			}
			if (CVNVertex2[ii] > -999) {
				h1_cvn2[ii]->Fill(d_cvn2);
				dv_cvn2[ii].push_back(d_cvn2);
				dr[2] += pow(d_cvn2,2);
			}

			if (ii == 1){
				if (pand_vtx[ii] < -999) count_none_pand++;
				if (cvn1_vtx < -999) count_none_cvn1++;
				if (CVNVertex2[ii] < -999) count_none_cvn2++;
			}
		}
		h1_pand[3]->Fill(TMath::Sqrt(dr[0]));
		h1_cvn1[3]->Fill(TMath::Sqrt(dr[1]));
		h1_cvn2[3]->Fill(TMath::Sqrt(dr[2]));
	}

	//TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 1600, 400);
	TCanvas *c1 = new TCanvas("c1", "c1", 0, 0, 1200, 300);
	c1->Divide(3, 1);

	//TLegend *leg = new TLegend(0.65, 0.55, 0.95, 0.80);
	TLegend *leg = new TLegend(0.65, 0.60, 0.95, 0.85);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->AddEntry(h1_pand[0], "Standard", "l");
	//leg->AddEntry(h1_cvn1[0], "CNN: 3 steps", "l");
	leg->AddEntry(h1_cvn2[0], "RegCNN: 2-stage", "l");
	for (int ii = 0; ii < 3; ii++){
		c1->cd(ii+1);
		SetPadBorder(c1->cd(ii+1));
		h1_cvn2[ii]->Draw("hist");
		//h1_cvn1[ii]->Draw("hist sames");
		h1_pand[ii]->Draw("hist sames");

		//if (ii == 0) leg->Draw();
		leg->Draw();
		TString s1 (Form("CVN2 %s : Mean = %.4f , RMS = %.4f", surfix[ii].c_str(), h1_cvn2[ii]->GetMean(), h1_cvn2[ii]->GetRMS()));
		TString s2 (Form("CVN1 %s : Mean = %.4f , RMS = %.4f", surfix[ii].c_str(), h1_cvn1[ii]->GetMean(), h1_cvn1[ii]->GetRMS()));
		TString s3 (Form("Pand %s : Mean = %.4f , RMS = %.4f", surfix[ii].c_str(), h1_pand[ii]->GetMean(), h1_pand[ii]->GetRMS()));
		cout << s1 << endl;
		cout << s2 << endl;
		cout << s3 << endl;
	}
	c1->cd(3);
	TLatex *prel = new TLatex (0.7, 0.915, "#color[14]{DUNE Preliminary}");
	prel->SetNDC();
	prel->Draw("same");
	gPad->Update();

	TCanvas *c2 = new TCanvas("c2", "c2", 0, 0, 400, 400);
	SetPadBorder(c2->cd());
	h1_cvn2[3]->Draw("hist");
	h1_pand[3]->Draw("hist sames");
	//h1_cvn1[3]->Draw("hist sames");
	gPad->SetLogy();
	gPad->Update();
	const int nq = 1000;
	const int t = 680;
	double xq[nq], yq[nq];
	for (int i = 0; i < nq; i++) xq[i] = float(i+1)/nq;
	h1_pand[3]->GetQuantiles(nq, yq, xq);
	double quan = yq[t];
	cout << "Quantile: " << yq[t] << endl;
	int ibin = int(round(quan/h1_pand[3]->GetBinWidth(1)));
	cout << h1_pand[3]->GetBinWidth(1) << " " << ibin << endl;
	cout << "check: " << h1_pand[3]->Integral(0, ibin)/h1_pand[3]->Integral(0,nbins2)*100. << endl;
	h1_cvn2[3]->GetQuantiles(nq, yq, xq);
	quan = yq[t];
	cout << "Quantile: " << yq[t] << endl;
	ibin = int(round(quan/h1_cvn2[3]->GetBinWidth(1)));
	cout << h1_cvn2[3]->GetBinWidth(1) << " " << ibin << endl;
	cout << "check: " << h1_cvn2[3]->Integral(0, ibin)/h1_cvn2[3]->Integral(0,nbins2)*100. << endl;



	cout << " Entire Ranges " << endl;
	float mean_pand, rms_pand;
	float mean_cvn1, rms_cvn1;
	float mean_cvn2, rms_cvn2;
	cout << "-------------------------------------------" << endl;
	cout << "\\begin{tabular}{c | c | c | c | c || c | c | c } " << endl;
	cout << " \\hline" << endl;
	cout << Form("& &  \\multicolumn{3}{c||}{Entire Range} & \\multicolumn{3}{c}{in Range(-20,20)}  \\\\") << endl;
	cout << "\\hline \\hline" << endl;
	cout << Form(" & (cm)  &  Pand  & CVN1  &  CVN2  &  Pand  &  CVN1  &  CVN2 \\\\") << endl;
	cout << " \\hline" << endl;
	for (int ii = 0; ii < 3; ii++){
		Get_Mean_RMS(dv_cvn2[ii], mean_cvn2, rms_cvn2);
		Get_Mean_RMS(dv_cvn1[ii], mean_cvn1, rms_cvn1);
		Get_Mean_RMS(dv_pand[ii], mean_pand, rms_pand);
		cout << Form("\\multirow{2}{*}{$\\Delta$%s}& Mean & %.4f & %.4f & %.4f & %.4f & %.4f & %.4f \\\\", surfix[ii].c_str(), mean_pand, mean_cvn1, mean_cvn2, h1_pand[ii]->GetMean(), h1_cvn1[ii]->GetMean(), h1_cvn2[ii]->GetMean()) << endl;
		cout << Form("& RMS  & %.4f & %.4f & %.4f & %.4f & %.4f & %.4f \\\\", rms_pand, rms_cvn1, rms_cvn2, h1_pand[ii]->GetRMS(), h1_cvn1[ii]->GetRMS(), h1_cvn2[ii]->GetRMS()) << endl;
		cout << " \\hline" << endl;
	}
	cout << " \\hline" << endl;
	cout << "\\end{tabular}" << endl;

	cout << "----------------Two Comp---------------------------" << endl;
	cout << "\\begin{tabular}{c | c | c | c || c | c } " << endl;
	cout << " \\hline" << endl;
	cout << Form("& &  \\multicolumn{2}{c||}{Entire Range} & \\multicolumn{2}{c}{in Range(-20,20)}  \\\\") << endl;
	cout << "\\hline \\hline" << endl;
	cout << Form(" & (cm)  &  Pand  & CVN  &  Pand  &  CVN \\\\") << endl;
	cout << " \\hline" << endl;
	for (int ii = 0; ii < 3; ii++){
		Get_Mean_RMS(dv_cvn2[ii], mean_cvn2, rms_cvn2);
		Get_Mean_RMS(dv_pand[ii], mean_pand, rms_pand);
		cout << Form("\\multirow{2}{*}{$\\Delta$%s}& Mean & %.4f & %.4f & %.4f & %.4f \\\\", surfix[ii].c_str(), mean_pand, mean_cvn2, h1_pand[ii]->GetMean(), h1_cvn2[ii]->GetMean()) << endl;
		cout << Form("& RMS  & %.4f & %.4f & %.4f & %.4f \\\\", rms_pand, rms_cvn2, h1_pand[ii]->GetRMS(), h1_cvn2[ii]->GetRMS()) << endl;
		cout << " \\hline" << endl;
	}
	cout << " \\hline" << endl;
	cout << "\\end{tabular}" << endl;


	cout << "--------------------short--------------------" << endl;
	cout << "\\begin{tabular}{c | c | c | c } " << endl;
	cout << " \\hline" << endl;
	cout << Form("& &  \\multicolumn{2}{c}{in Range(-20,20)}  \\\\") << endl;
	cout << "\\hline \\hline" << endl;
	cout << Form(" & (cm)  &  Pandora &  RegCNN \\\\") << endl;
	cout << " \\hline" << endl;
	for (int ii = 0; ii < 3; ii++){
		Get_Mean_RMS(dv_cvn2[ii], mean_cvn2, rms_cvn2);
		Get_Mean_RMS(dv_pand[ii], mean_pand, rms_pand);
		cout << Form("\\multirow{2}{*}{$\\Delta$%s}& Mean & %.2f & %.2f \\\\", surfix[ii].c_str(), h1_pand[ii]->GetMean(), h1_cvn2[ii]->GetMean()) << endl;
		cout << Form("& RMS  & %.2f & %.2f \\\\", h1_pand[ii]->GetRMS(), h1_cvn2[ii]->GetRMS()) << endl;
		cout << " \\hline" << endl;
	}
	cout << " \\hline" << endl;
	cout << "\\end{tabular}" << endl;


	cout << "No Vertex events:" << endl;
	cout << "CVN2: " << count_none_cvn2 << " , CVN1: " << count_none_cvn1 << " , Pandora: " << count_none_pand << endl;
	cout << "CVN2: " << count_none_cvn2/float(evt_count)*100. << 
		" , CVN1: " << count_none_cvn1/float(evt_count)*100. << 
		" , Pandora: " << count_none_pand/float(evt_count)*100. << endl;

	cout << "Total Number of events processed: " << evt_count << endl;
}
