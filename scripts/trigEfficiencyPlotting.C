// post-process job results

#include "TEfficiency.h"

TFile* _inputFile;

void trigEfficiencyPlotting() {
  gStyle->SetTitleY(0.964497);
}

void load_input_file(const char* filename) {
  _inputFile = TFile::Open(filename);
}

void efficiency_vs_pot() {

   // create canvas
  TCanvas* c0 = new TCanvas("c0","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1F* instLumi = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/evt_0/inst_lumi");
  TH1F* instLumiApr = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/evt_0/inst_lumi_apr");
  TH1F* instLumiCpr = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/evt_0/inst_lumi_cpr");
  TH1F* instLumiAprCpr = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/evt_0/inst_lumi_apr_cpr");
  // print out total efficiencies
  double totalEvents = instLumi->Integral();
  double totalEventsPassed = instLumiAprCpr->Integral();
  double totalEfficiency = totalEventsPassed/totalEvents;
  std::cout << "reco efficiency = " << totalEfficiency << std::endl;
  // make efficiency plots
  TEfficiency* apr_effVsPOT = new TEfficiency(*instLumiApr, *instLumi);
  apr_effVsPOT->SetLineColor(kRed);
  apr_effVsPOT->SetLineWidth(2);
  TEfficiency* cpr_effVsPOT = new TEfficiency(*instLumiCpr, *instLumi);
  cpr_effVsPOT->SetLineColor(kCyan);
  cpr_effVsPOT->SetLineWidth(2);
  TEfficiency* aprcpr_effVsPOT = new TEfficiency(*instLumiAprCpr, *instLumi);
  aprcpr_effVsPOT->SetLineColor(kViolet);
  aprcpr_effVsPOT->SetLineWidth(2);
  // draw plots
  c0->cd();
  apr_effVsPOT->Draw();
  apr_effVsPOT->SetTitle("efficiency vs. nPOT; nPOT; #epsilon");
  cpr_effVsPOT->Draw("same");
  aprcpr_effVsPOT->Draw("same");
  TLegend *leg1 = new TLegend(0.65, 0.76, 0.88, 0.92);
  leg1->SetFillStyle(1001);
  leg1->SetFillColor(kWhite);
  leg1->SetBorderSize(1);
  leg1->SetShadowColor(0);
  leg1->AddEntry(apr_effVsPOT, "APR", "lpe");
  leg1->AddEntry(cpr_effVsPOT, "CPR", "lpe");
  leg1->AddEntry(aprcpr_effVsPOT, "APR+CPR", "lpe");
  leg1->Draw();
 
}
