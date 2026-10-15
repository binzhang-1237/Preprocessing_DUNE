from ROOT import *

def make_can(cname, ctitle, xpos, ypos, xwth, ywth):
	cc = TCanvas(cname, ctitle, xpos, ypos, xwth, ywth)
        cc.SetLeftMargin(0.15)
        cc.SetBottomMargin(0.11)
        cc.SetRightMargin(0.04)
        cc.SetTopMargin(0.08)
	return cc

def make_hist1d(hname, htitle, nbins, xmin, xmax):
	hh = TH1F(hname, htitle, nbins, xmin, xmax)
	hh.GetXaxis().SetTitleSize(0.05)
	hh.GetYaxis().SetTitleSize(0.05)
	hh.GetXaxis().SetLabelSize(0.05)
	hh.GetYaxis().SetLabelSize(0.05)
	hh.SetLineColorAlpha(kTeal+2, 0.9);
	return hh

def main():
        gStyle.SetOptTitle(0);
        gStyle.SetOptStat(0);
        gStyle.SetHistLineWidth(2);
        gStyle.SetHistMinimumZero();
        gStyle.SetFrameLineWidth(2);
        gStyle.SetPadTickX(1);
        gStyle.SetPadTickY(1);
        gStyle.SetPadLeftMargin(0.13);
        gStyle.SetPadRightMargin(0.09);
        gStyle.SetPadBottomMargin(0.13);
        gStyle.SetPadTopMargin(0.09);
	gStyle.SetCanvasPreferGL(1);

	cc1 = make_can("cc1", "Pixel Maps", 40, 10, 500, 500);
	cc2 = make_can("cc2", "Pixel Maps", 40, 40, 500, 500);


	ch = TChain("recoEnergy/WC")
	#ch.Add("../work/RecoEnergyS_20180918.root")
	ch.Add("../work/RecoEnergyS.root")

	cc1.cd()
	h1_good      = make_hist1d("h1_good", ";Chi2 / NDF; Entries (a.u)", 1000, -1.5, 7000)
	h1_good_zoom = make_hist1d("h1_good_zoom", ";Chi2 / NDF; Entries (a.u)", 1000, -1.5, 15)
	ch.Draw("Hit_Good>>h1_good", "NuCCNCTruth[0]==0&&NuPDGTruth[0]==12")
	ch.Draw("Hit_Good>>h1_good_zoom", "NuCCNCTruth[0]==0&&NuPDGTruth[0]==12")
	cc1.cd()
	cc1.SetLogy()
	h1_good.Draw("hist")
	gPad.Update()
	cc2.cd()
	h1_good_zoom.Draw("hist")
	gPad.Update()

	cc3 = make_can("cc3", "blank", 40, 40, 500, 500);
	evtn0 = ch.GetEntries("NuCCNCTruth[0]==0&&NuPDGTruth[0]==12")
	n0 = ch.Draw("Hit_Good", "NuCCNCTruth[0]==0&&NuPDGTruth[0]==12")
	n1 = ch.Draw("Hit_Good", "NuCCNCTruth[0]==0&&NuPDGTruth[0]==12&&Hit_Good>0&&Hit_Good<100")
	fail_rate = float(n0-n1)/float(n1) * 100.
	print "N0: %d, N1: %d, (N0-N1)/N0: %.2f" % (n0, n1, fail_rate)


	nevents = 0
	sum_avg = 0.
	for evt in ch:
		if not evt.NuCCNCTruth[0] == 0: continue
		if not evt.NuPDGTruth[0] == 12: continue
		Nhits = evt.NHits
		if Nhits == 0: continue
		bad_nhits = 0
		for ii in range(0, Nhits):
			if (evt.Hit_Good[ii] <= 0 or evt.Hit_Good[ii]>100):
				bad_nhits += 1

		sum_avg += float(bad_nhits)/float(Nhits)
		nevents += 1
	print "==>", nevents, sum_avg, "%2.f" % (sum_avg/float(nevents))
	raw_input("press")


main()
