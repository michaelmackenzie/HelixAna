// post-process job results

#include "TEfficiency.h"

TString _histDir = "/exp/mu2e/data/users/mmackenz/trigger/trigger_rates/histograms/";
TString _figDir = "figures/trigger_rates/";
TFile* _inputFile;

//----------------------------------------------------------------------------------------------------------
void load_input_file(TString filename) {
  _inputFile = TFile::Open(filename.Data(), "READ");
}

//----------------------------------------------------------------------------------------------------------
void efficiency_vs_pot(const char* name = "eff_vs_pot", const bool normalized = false) {

   // create canvas
  TCanvas* c0 = new TCanvas("c0","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1* instLumi       = (TH1*)_inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/evt_%i/inst_lumi"        , (normalized) ? 1 : 0));
  TH1* instLumiApr    = (TH1*)_inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/evt_%i/inst_lumi_apr"    , (normalized) ? 1 : 0));
  TH1* instLumiCpr    = (TH1*)_inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/evt_%i/inst_lumi_cpr"    , (normalized) ? 1 : 0));
  TH1* instLumiAprCpr = (TH1*)_inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/evt_%i/inst_lumi_apr_cpr", (normalized) ? 1 : 0));
  int group_factor = 15;
  instLumi      ->Rebin(group_factor);
  instLumiApr   ->Rebin(group_factor);
  instLumiCpr   ->Rebin(group_factor);
  instLumiAprCpr->Rebin(group_factor);
  std::cout << "number of bins after rebin = " << instLumi->GetNbinsX() << std::endl;
  // print out total efficiencies
  const double totalEvents = instLumi->Integral();
  const double totalEventsPassed = instLumiAprCpr->Integral();
  const double totalEfficiency = totalEventsPassed/totalEvents;
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
  aprcpr_effVsPOT->Draw();
  aprcpr_effVsPOT->SetTitle("efficiency vs. N(POT); N(POT); #epsilon");
  cpr_effVsPOT->Draw("same");
  apr_effVsPOT->Draw("same");

  c0->Modified(); c0->Update();

  aprcpr_effVsPOT->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.65, 1.05);

  TLegend *leg1 = new TLegend(0.65, 0.76, 0.90, 0.90);
  leg1->SetFillStyle(1001);
  leg1->SetFillColor(kWhite);
  leg1->SetBorderSize(1);
  leg1->SetShadowColor(0);
  leg1->AddEntry(apr_effVsPOT, "APR", "lpe");
  leg1->AddEntry(cpr_effVsPOT, "CPR", "lpe");
  leg1->AddEntry(aprcpr_effVsPOT, "APR+CPR", "lpe");
  leg1->Draw();

  c0->SaveAs(Form("%s%s.png", _figDir.Data(), name));
}

//----------------------------------------------------------------------------------------------------------
void efficiency_vs_track_params(const char* base_name = "hist") {

  // first vs max approach
  // create canvas
  TCanvas* c2 = new TCanvas("c2","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1* numerator_0   = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/trk_4/rMax");
  TH1* denominator_0 = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/trk_3/rMax");
  int group_factor_0 = 10;
  numerator_0  ->Rebin(group_factor_0);
  denominator_0->Rebin(group_factor_0);
  // make efficiency plot
  TEfficiency* eff_vs_max_approach = new TEfficiency(*numerator_0, *denominator_0);
  eff_vs_max_approach->SetLineColor(kBlue);
  eff_vs_max_approach->SetLineWidth(2);
  // draw plots
  c2->cd();
  eff_vs_max_approach->Draw();
  eff_vs_max_approach->SetTitle("efficiency vs. R(max); R(max) [mm]; #epsilon");
  c2->SaveAs(Form("%s%s_eff_rmax.png", _figDir.Data(), base_name));

  // let's do vs pt
  // create canvas
  TCanvas* c3 = new TCanvas("c3","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1* numerator_1   = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/trk_4/pt");
  TH1* denominator_1 = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/trk_3/pt");
  int group_factor_1 = 1;
  numerator_1->Rebin(group_factor_1);
  denominator_1->Rebin(group_factor_1);
  // make efficiency plot
  TEfficiency* eff_vs_pt = new TEfficiency(*numerator_1, *denominator_1);
  eff_vs_pt->SetLineColor(kBlue);
  eff_vs_pt->SetLineWidth(2);
  // draw plots
  c3->cd();
  eff_vs_pt->Draw();
  eff_vs_pt->SetTitle("efficiency vs. p_{T}; p_{T} [MeV/c]; #epsilon");
  c3->Modified(); c3->Update();

  // save one focused on the mu- --> e- window and one on the mu- --> e+ window
  eff_vs_pt->GetPaintedGraph()->GetXaxis()->SetRangeUser(-95., -75.);
  c3->SaveAs(Form("%s%s_eff_pt_neg.png", _figDir.Data(), base_name));

  eff_vs_pt->GetPaintedGraph()->GetXaxis()->SetRangeUser(60., 80.);
  c3->SaveAs(Form("%s%s_eff_pt_pos.png", _figDir.Data(), base_name));

  // let's do vs p
  // create canvas
  TCanvas* c4 = new TCanvas("c4","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1* numerator_2   = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/trk_4/p");
  TH1* denominator_2 = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/trk_3/p");
  int group_factor_2 = 1;
  numerator_2  ->Rebin(group_factor_2);
  denominator_2->Rebin(group_factor_2);
  // make efficiency plot
  TEfficiency* eff_vs_p = new TEfficiency(*numerator_2, *denominator_2);
  eff_vs_p->SetLineColor(kBlue);
  eff_vs_p->SetLineWidth(2);
  // draw plots
  c4->cd();
  eff_vs_p->Draw();
  eff_vs_p->SetTitle("efficiency vs. q*p; q*p [MeV/c]; #epsilon");
  c4->Modified(); c4->Update();

  // save one focused on the mu- --> e- window and one on the mu- --> e+ window
  eff_vs_p->GetPaintedGraph()->GetXaxis()->SetRangeUser(-110., -100.);
  c4->SaveAs(Form("%s%s_eff_p_neg.png", _figDir.Data(), base_name));

  eff_vs_p->GetPaintedGraph()->GetXaxis()->SetRangeUser(85., 95.);
  c4->SaveAs(Form("%s%s_eff_p_pos.png", _figDir.Data(), base_name));
}

//----------------------------------------------------------------------------------------------------------
/** Inputs:
      Dataset: Flag for dataset to process
              -1 : all
               0 : conversion electron
               1 : conversion positron
**/
int trigEfficiencyPlotting(int Dataset = -1) {
  gStyle->SetTitleY(0.964497);

  gSystem->Exec(Form("[ ! -d %s ] && mkdir -p %s", _figDir.Data(), _figDir.Data()));

  // mu- --> e- plots
  if(Dataset < 0 || Dataset == 0) {
    load_input_file(_histDir + "HelixAna.TrigAna.cele0b1s5r0000.hist");
    if(!_inputFile) return 1;

    efficiency_vs_pot         ("cele_eff_vs_pot");
    efficiency_vs_pot         ("cele_norm_eff_vs_pot", 1);
    efficiency_vs_track_params("cele");

    _inputFile->Close();
  }

  // mu- --> e+ plots
  if(Dataset < 0 || Dataset == 1) {
    load_input_file(_histDir + "HelixAna.TrigAna.cpos0b1s5r0000.hist");
    if(!_inputFile) return 1;

    efficiency_vs_pot         ("cpos_eff_vs_pot");
    efficiency_vs_pot         ("cpos_norm_eff_vs_pot", 1);
    efficiency_vs_track_params("cpos");

    _inputFile->Close();
  }

  return 0;
}
