// Make helix comparison plots using the THelixAnaModule output

//-----------------------------------------------------------------------------
// Make a comparison plot
int print_comparison(const char* name, const int hist_set, const char* outname, TFile* f, const char* figdir) {
  TH1* hapr = (TH1*) f->Get(Form("Ana/HelixAna_HelixAna/Hist/cmp_%i/Apr%s", hist_set, name));
  TH1* hcpr = (TH1*) f->Get(Form("Ana/HelixAna_HelixAna/Hist/cmp_%i/Cpr%s", hist_set, name));
  if(!hapr || !hcpr) {
    cout << "Histograms " << name << " with set " << hist_set << " not found!\n";
    return 1;
  }

  gStyle->SetOptStat(0);

  TCanvas* c = new TCanvas(Form("c_%s_%i", name, hist_set), Form("c_%s_%i", name, hist_set), 800, 800);
  TPad* pad1 = new TPad("pad1", "pad1", 0., 0.3, 1., 1.0); pad1->Draw();
  TPad* pad2 = new TPad("pad2", "pad2", 0., 0.0, 1., 0.3); pad2->Draw();
  pad1->SetBottomMargin(0.05);
  pad2->SetTopMargin(0.03);
  pad1->SetRightMargin(0.05);
  pad2->SetRightMargin(0.05);

  pad1->cd();
  hapr->SetLineWidth(2);
  hapr->SetLineColor(kBlue);
  hapr->SetFillColor(kBlue);
  hapr->SetFillStyle(3003);
  hcpr->SetLineWidth(2);
  hcpr->SetLineColor(kRed);
  hcpr->SetMarkerColor(kRed);
  hcpr->SetMarkerStyle(6);
  hcpr->SetMarkerSize(1.2);
  hapr->SetTitle("");

  hapr->Draw("hist");
  hcpr->Draw("E1 same");
  const double max_val = max(hapr->GetMaximum(), hcpr->GetMaximum());
  hapr->GetYaxis()->SetRangeUser(0., 1.2*max_val);

  TLegend* leg = new TLegend(0.65, 0.8, 0.95, 0.9);
  leg->AddEntry(hapr, "APR");
  leg->AddEntry(hcpr, "CPR");
  leg->Draw("same");

  pad2->cd();
  TH1* ratio = (TH1*) hcpr->Clone(Form("ratio_%s_%i", outname, hist_set));
  ratio->Divide(hapr);
  ratio->Draw("hist");
  ratio->GetYaxis()->SetRangeUser(0., 2.);
  // ratio->GetYaxis()->SetNdivisions(8);
  ratio->SetTitle("");
  ratio->SetYTitle("CPR / APR");
  ratio->GetXaxis()->SetLabelSize(0.08);
  ratio->GetYaxis()->SetLabelSize(0.08);
  ratio->GetYaxis()->SetTitleSize(0.08);
  ratio->GetYaxis()->SetTitleOffset(0.5);

  TLine* line = new TLine(ratio->GetXaxis()->GetXmin(), 1., ratio->GetXaxis()->GetXmax(), 1.);
  line->SetLineWidth(2);
  line->SetLineStyle(kDashed);
  line->SetLineColor(kBlack);
  line->Draw("same");

  c->SaveAs(Form("%s/%s.png", figdir, outname));
  hapr->GetYaxis()->SetRangeUser(max_val/1.e4, 5.*max_val);
  c->SaveAs(Form("%s/%s_log.png", figdir, outname));

  return 0;
}

//-----------------------------------------------------------------------------
// Main function
int plot_helix_comparison(const char* filename = "cele0b2s8r0000.hist", const char* figdir = "figures") {

  // create the output directory
  gSystem->Exec(Form("[ ! -d %s ] && mkdir -p %s", figdir, figdir));

  // read in the histogram file
  TFile* f = TFile::Open(filename, "READ");
  if(!f) return 1;

  int status(0);

  status += print_comparison("HelixDeltaP", 1, "helix_dp", f, figdir);
  status += print_comparison("TrackDeltaP", 1, "track_dp", f, figdir);

  return status;
}
