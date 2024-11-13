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
  int group_factor = 15;
  instLumi = (TH1F*) instLumi->Rebin(group_factor);
  instLumiApr = (TH1F*) instLumiApr->Rebin(group_factor);
  instLumiCpr = (TH1F*) instLumiCpr->Rebin(group_factor);
  instLumiAprCpr = (TH1F*) instLumiAprCpr->Rebin(group_factor);
  std::cout << "number of bins after rebin = " << instLumi->GetNbinsX() << std::endl;
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

void normalized_efficiency_vs_pot() {

   // create canvas
  TCanvas* c1 = new TCanvas("c1","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1F* instLumi_1 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/evt_1/inst_lumi");
  TH1F* instLumiApr_1 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/evt_1/inst_lumi_apr");
  TH1F* instLumiCpr_1 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/evt_1/inst_lumi_cpr");
  TH1F* instLumiAprCpr_1 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/evt_1/inst_lumi_apr_cpr");
  int group_factor = 15;
  instLumi_1 = (TH1F*) instLumi_1->Rebin(group_factor);
  instLumiApr_1 = (TH1F*) instLumiApr_1->Rebin(group_factor);
  instLumiCpr_1 = (TH1F*) instLumiCpr_1->Rebin(group_factor);
  instLumiAprCpr_1 = (TH1F*) instLumiAprCpr_1->Rebin(group_factor);
  std::cout << "number of bins after rebin = " << instLumi_1->GetNbinsX() << std::endl;
  // print out total efficiencies
  double totalEvents = instLumi_1->Integral();
  double totalEventsPassed = instLumiAprCpr_1->Integral();
  double totalEfficiency = totalEventsPassed/totalEvents;
  std::cout << "reco efficiency = " << totalEfficiency << std::endl;
  // make efficiency plots
  TEfficiency* apr_effVsPOT = new TEfficiency(*instLumiApr_1, *instLumi_1);
  apr_effVsPOT->SetLineColor(kRed);
  apr_effVsPOT->SetLineWidth(2);
  TEfficiency* cpr_effVsPOT = new TEfficiency(*instLumiCpr_1, *instLumi_1);
  cpr_effVsPOT->SetLineColor(kCyan);
  cpr_effVsPOT->SetLineWidth(2);
  TEfficiency* aprcpr_effVsPOT = new TEfficiency(*instLumiAprCpr_1, *instLumi_1);
  aprcpr_effVsPOT->SetLineColor(kViolet);
  aprcpr_effVsPOT->SetLineWidth(2);
  // draw plots
  c1->cd();
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

void efficiency_vs_track_params() {

  // first vs max approach
  // create canvas
  TCanvas* c2 = new TCanvas("c2","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1F* numerator_0 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/trk_4/maxApproach");
  TH1F* denominator_0 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/trk_3/maxApproach");
  int group_factor_0 = 10;
  numerator_0 = (TH1F*) numerator_0->Rebin(group_factor_0);
  denominator_0 = (TH1F*) denominator_0->Rebin(group_factor_0);
  // make efficiency plot
  TEfficiency* eff_vs_max_approach = new TEfficiency(*numerator_0, *denominator_0);
  eff_vs_max_approach->SetLineColor(kBlue);
  eff_vs_max_approach->SetLineWidth(2);
  // draw plots
  c2->cd();
  eff_vs_max_approach->Draw();
  eff_vs_max_approach->SetTitle("efficiency vs. max approach; max approach; #epsilon");

  // let's do vs pt
  // create canvas
  TCanvas* c3 = new TCanvas("c3","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1F* numerator_1 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/trk_4/pt");
  TH1F* denominator_1 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/trk_3/pt");
  int group_factor_1 = 1;
  numerator_1 = (TH1F*) numerator_1->Rebin(group_factor_1);
  denominator_1 = (TH1F*) denominator_1->Rebin(group_factor_1);
  // make efficiency plot
  TEfficiency* eff_vs_pt = new TEfficiency(*numerator_1, *denominator_1);
  eff_vs_pt->SetLineColor(kBlue);
  eff_vs_pt->SetLineWidth(2);
  // draw plots
  c3->cd();
  eff_vs_pt->Draw();
  eff_vs_pt->SetTitle("efficiency vs. pt; pt; #epsilon");

  // let's do vs p
  // create canvas
  TCanvas* c4 = new TCanvas("c4","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1F* numerator_2 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/trk_4/p");
  TH1F* denominator_2 = (TH1F*)_inputFile->Get("Ana/ePlus2024_TrigAna/Hist/trk_3/p");
  int group_factor_2 = 1;
  numerator_2 = (TH1F*) numerator_2->Rebin(group_factor_2);
  denominator_2 = (TH1F*) denominator_2->Rebin(group_factor_2);
  // make efficiency plot
  TEfficiency* eff_vs_p = new TEfficiency(*numerator_2, *denominator_2);
  eff_vs_p->SetLineColor(kBlue);
  eff_vs_p->SetLineWidth(2);
  // draw plots
  c4->cd();
  eff_vs_p->Draw();
  eff_vs_p->SetTitle("efficiency vs. p; p; #epsilon");
  
}
