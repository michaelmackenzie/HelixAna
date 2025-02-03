// post-process job results

#include "TEfficiency.h"

TString _histDir = "/exp/mu2e/data/users/mmackenz/trigger/trigger_rates/histograms/";
TString _figDir = "figures/trigger_rates/";
TFile* _inputFile;

//----------------------------------------------------------------------------------------------------------
struct PlotData_t {
  TString _title;
  float   _xmin;
  float   _xmax;
  float   _ymin;
  float   _ymax;
  int     _rebin;
  PlotData_t(TString title = "",
             double xmin = 1., double xmax = -1.,
             double ymin = 1., double ymax = -1.,
             int rebin = 1) : _title(title), _xmin(xmin), _xmax(xmax),
                              _ymin(ymin), _ymax(ymax), _rebin(rebin) {}
};

//----------------------------------------------------------------------------------------------------------
void load_input_file(TString filename) {
  _inputFile = TFile::Open(filename.Data(), "READ");
}

//----------------------------------------------------------------------------------------------------------
void set_style(TH1* h, int color) {
  if(!h) return;
  h->SetLineWidth(2);
  h->SetLineStyle(kSolid);
  h->SetLineColor(color);
  h->SetMarkerColor(color);
  h->SetMarkerStyle(20);
  h->SetMarkerSize(0.6);
  h->SetFillStyle(0);
  h->SetFillColor(0);
}

//----------------------------------------------------------------------------------------------------------
int plot_param_and_ratio(const char* name, const char* param, PlotData_t plot_data = PlotData_t()) {
  gStyle->SetOptStat(0);

  // Retrieve the input distributions
  TH1* apr     = (TH1*) _inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/trk_10/%s", param)); //Online APR, good Offline track found
  TH1* cpr     = (TH1*) _inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/trk_11/%s", param)); //Online CPR, good Offline track found
  TH1* num_apr = (TH1*) _inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/trk_6/%s" , param)); //Offline + passes APR
  TH1* num_cpr = (TH1*) _inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/trk_7/%s" , param)); //Offline + passes CPR
  TH1* num_tot = (TH1*) _inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/trk_4/%s" , param)); //Offline + passes APR || CPR
  TH1* den_tot = (TH1*) _inputFile->Get(Form("Ana/HelixAna_TrigAna/Hist/trk_3/%s" , param)); //Offline
  if(!apr || !cpr || !num_apr || !num_cpr || !num_tot || !den_tot) {
    printf("%s: Histograms for %s not found!\n", __func__, param);
    return 1;
  }

  // Clone the histograms to not change the data
  apr     = (TH1*) apr    ->Clone("apr_tmp");
  cpr     = (TH1*) cpr    ->Clone("cpr_tmp");
  num_apr = (TH1*) num_apr->Clone("num_apr_tmp");
  num_cpr = (TH1*) num_cpr->Clone("num_cpr_tmp");
  num_tot = (TH1*) num_tot->Clone("num_tot_tmp");
  den_tot = (TH1*) den_tot->Clone("den_tot_tmp");

  // Create the canvas
  TCanvas* c = new TCanvas(param, param, 1200, 600);
  c->Divide(2,1);

  // Draw the distributions
  auto pad = c->cd(1);
  pad->SetRightMargin(0.03);
  set_style(num_tot, kViolet);
  set_style(den_tot, kViolet);
  set_style(apr    , kRed   );
  set_style(cpr    , kCyan  );

  den_tot->Draw("E1");
  apr->Draw("E1 SAME");
  cpr->Draw("E1 SAME");
  c->Update();
  if(plot_data._xmin < plot_data._xmax) den_tot->GetXaxis()->SetRangeUser(plot_data._xmin, plot_data._xmax);
  if(plot_data._ymin < plot_data._ymax) den_tot->GetYaxis()->SetRangeUser(plot_data._ymin, plot_data._ymax);
  else                                  den_tot->GetYaxis()->SetRangeUser(0.1, 1.2*max(max(den_tot->GetMaximum(), apr->GetMaximum()), cpr->GetMaximum()));
  if(plot_data._title != "") den_tot->SetTitle(Form("Track %s distributions;%s;Entries", plot_data._title.Data(), plot_data._title.Data()));

  // Add a legend
  TLegend *leg = new TLegend(0.10, 0.80, 0.97, 0.90);
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(1);
  leg->SetShadowColor(0);
  leg->SetNColumns(3);
  leg->SetTextSize(0.04);
  leg->AddEntry(apr    , "APR"    , "lpe");
  leg->AddEntry(cpr    , "CPR"    , "lpe");
  leg->AddEntry(den_tot, "Offline", "lpe");
  leg->Draw();
  gPad->RedrawAxis();

  // Draw the ratios
  pad = c->cd(2);
  pad->SetRightMargin(0.03);
  TEfficiency* apr_eff = new TEfficiency(*num_apr, *den_tot);
  apr_eff->SetLineColor(apr->GetLineColor());
  apr_eff->SetLineWidth(apr->GetLineWidth());
  TEfficiency* cpr_eff = new TEfficiency(*num_cpr, *den_tot);
  cpr_eff->SetLineColor(cpr->GetLineColor());
  cpr_eff->SetLineWidth(cpr->GetLineWidth());
  TEfficiency* tot_eff = new TEfficiency(*num_tot, *den_tot);
  tot_eff->SetLineColor(num_tot->GetLineColor());
  tot_eff->SetLineWidth(num_tot->GetLineWidth());

  // TGraph axes don't cooperate as well
  auto haxis = (TH1*) num_tot->Clone("tmp_axis");
  for(int ibin = 0; ibin <= haxis->GetNbinsX(); ++ibin) haxis->SetBinContent(ibin, 0.);
  haxis->SetLineColor(0);
  haxis->SetMarkerColor(0);
  haxis->SetFillColor(0);
  haxis->SetFillStyle(0);
  haxis->Draw("hist");
  tot_eff->Draw("E1 SAME");
  apr_eff->Draw("E1 SAME");
  cpr_eff->Draw("E1 SAME");

  const double xmin((plot_data._xmin < plot_data._xmax) ? plot_data._xmin : haxis->GetXaxis()->GetXmin());
  const double xmax((plot_data._xmin < plot_data._xmax) ? plot_data._xmax : haxis->GetXaxis()->GetXmax());
  const double ymin((plot_data._ymin < plot_data._ymax) ? plot_data._ymin : 0.65);
  const double ymax((plot_data._ymin < plot_data._ymax) ? plot_data._ymax : 1.10);

  pad->Update();
  haxis->GetXaxis()->SetRangeUser(xmin, xmax);
  haxis->GetYaxis()->SetRangeUser(ymin, ymax);
  if(plot_data._title != "") haxis->SetTitle(Form("Online track reconstruction efficiency;%s;#epsilon;", plot_data._title.Data()));

  // add a reference line
  TLine line(xmin, 1., xmax, 1.);
  line.SetLineWidth(2);
  line.SetLineStyle(kDashed);
  line.SetLineColor(kBlack);
  line.Draw("SAME");

  leg = new TLegend(0.10, 0.80, 0.97, 0.90);
  leg->SetFillStyle(1001);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(1);
  leg->SetShadowColor(0);
  leg->SetNColumns(3);
  leg->SetTextSize(0.04);
  leg->AddEntry(apr    , "APR"    , "lpe");
  leg->AddEntry(cpr    , "CPR"    , "lpe");
  leg->AddEntry(num_tot, "APR+CPR", "lpe");
  leg->Draw();

  c->SaveAs(Form("%s%s_%s_and_eff.png", _figDir.Data(), name, param));

  // Cleanup the memory
  delete c;
  delete tot_eff;
  delete apr_eff;
  delete cpr_eff;
  delete apr    ;
  delete cpr    ;
  delete num_apr;
  delete num_cpr;
  delete num_tot;
  delete den_tot;

  return 0;
}

//----------------------------------------------------------------------------------------------------------
void rejection_plot(const char* name = "mnbs0b1") {

   // create canvas
  TCanvas* c0 = new TCanvas("c0","",900,700);
  gStyle->SetOptStat(0);
  // read in signal dataset plots
  TH1* instLumi       = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/evt_0/inst_lumi"        )->Clone("lumi_tot");
  TH1* instLumiApr    = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/evt_0/inst_lumi_apr"    )->Clone("lumi_apr");
  TH1* instLumiCpr    = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/evt_0/inst_lumi_cpr"    )->Clone("lumi_cpr");
  TH1* instLumiAprCpr = (TH1*)_inputFile->Get("Ana/HelixAna_TrigAna/Hist/evt_0/inst_lumi_apr_cpr")->Clone("lumi_both");
  TH1* lumi_nom = (TH1*) instLumi->Clone("lumi_nom"); //version with high binning

  const int group_factor = 15;
  instLumi      ->Rebin(group_factor);
  instLumiApr   ->Rebin(group_factor);
  instLumiCpr   ->Rebin(group_factor);
  instLumiAprCpr->Rebin(group_factor);

  // normalization info
  const int nsim  = instLumi      ->GetEntries(); //N(simulated events)
  const int napr  = instLumiApr   ->GetEntries(); //N(APR events);
  const int ncpr  = instLumiCpr   ->GetEntries(); //N(CPR events);
  const int nboth = instLumiAprCpr->GetEntries(); //N(APR || CPR events);
  const double event_time = 1.695e-6; //micro-bunch period
  const double nevents_per_second = 0.4/1.4/event_time;  //events per second
  const double norm = nevents_per_second/nsim; //normalization factor for N(sim) --> N(per second)
  const double napr_per_second = norm*napr;
  const double ncpr_per_second = norm*ncpr;
  const double nboth_per_second = norm*nboth;
  const double lumi_avg = lumi_nom->GetMean(); //average beam intensity

  printf("N(events / second) = %6.0f +- %5.1f (%7i sim events)\n", nevents_per_second, norm*sqrt(nsim ), nsim ); //error doesn't make sense here
  printf("N(APR    / second) = %6.0f +- %5.1f (%7i sim events)\n", napr_per_second   , norm*sqrt(napr ), napr );
  printf("N(CPR    / second) = %6.0f +- %5.1f (%7i sim events)\n", ncpr_per_second   , norm*sqrt(ncpr ), ncpr );
  printf("N(Either / second) = %6.0f +- %5.1f (%7i sim events)\n", nboth_per_second  , norm*sqrt(nboth), nboth);

  // Scale the luminosity plots to rates per second
  lumi_nom      ->Scale(norm);
  instLumi      ->Scale(norm);
  instLumiApr   ->Scale(norm);
  instLumiCpr   ->Scale(norm);
  instLumiAprCpr->Scale(norm);

  // Create the background rejection factor plots
  TH1* hRejApr  = (TH1*) instLumi->Clone("RejAPR" ); hRejApr ->Divide(instLumiApr);
  TH1* hRejCpr  = (TH1*) instLumi->Clone("RejCPR" ); hRejCpr ->Divide(instLumiCpr);
  TH1* hRejBoth = (TH1*) instLumi->Clone("RejBoth"); hRejBoth->Divide(instLumiAprCpr);

  // Create the instantaneous trigger rate normalized by POT bin
  TH1* hRateApr  = (TH1*) instLumiApr   ->Clone("RateAPR" ); hRateApr ->Divide(instLumi); hRateApr ->Scale(nevents_per_second); // 1./event_time);
  TH1* hRateCpr  = (TH1*) instLumiCpr   ->Clone("RateCPR" ); hRateCpr ->Divide(instLumi); hRateCpr ->Scale(nevents_per_second); // 1./event_time);
  TH1* hRateBoth = (TH1*) instLumiAprCpr->Clone("RateBoth"); hRateBoth->Divide(instLumi); hRateBoth->Scale(nevents_per_second); // 1./event_time);

  printf("Average beam intensity = %.2e, Inst. trigger rates: APR = %.1f, CPR = %.1f, Both = %.1f\n", lumi_avg,
         hRateApr ->GetBinContent(hRateApr ->FindBin(lumi_avg)),
         hRateCpr ->GetBinContent(hRateCpr ->FindBin(lumi_avg)),
         hRateBoth->GetBinContent(hRateBoth->FindBin(lumi_avg)));

  // Set the figure styles
  set_style(instLumiAprCpr, kViolet);
  set_style(instLumiApr   , kRed   );
  set_style(instLumiCpr   , kCyan  );
  set_style(hRejBoth      , kViolet);
  set_style(hRejApr       , kRed   );
  set_style(hRejCpr       , kCyan  );
  set_style(hRateBoth     , kViolet);
  set_style(hRateApr      , kRed   );
  set_style(hRateCpr      , kCyan  );
  set_style(lumi_nom      , kBlue  );

  // draw the input lumi distribution
  lumi_nom->Draw("hist");
  lumi_nom->SetTitle("Luminosity profile;N(POT);Rate [Hz]");
  lumi_nom->GetXaxis()->SetRangeUser(0., 99.e6);
  c0->SaveAs(Form("%s%s_lumi.png", _figDir.Data(), name));

  // draw trigger rate plots
  instLumiAprCpr->Draw("E1");
  instLumiApr   ->Draw("E1 same");
  instLumiCpr   ->Draw("E1 same");
  instLumiAprCpr->SetTitle("Background trigger rate;N(POT);Rate [Hz]");
  instLumiAprCpr->GetXaxis()->SetRangeUser(0., 99.e6);

  c0->Modified(); c0->Update();

  TLegend *leg1 = new TLegend(0.65, 0.76, 0.90, 0.90);
  leg1->SetFillStyle(1001);
  leg1->SetFillColor(kWhite);
  leg1->SetBorderSize(1);
  leg1->SetShadowColor(0);

  leg1->AddEntry(instLumiApr   , "APR", "lpe");
  leg1->AddEntry(instLumiCpr   , "CPR", "lpe");
  leg1->AddEntry(instLumiAprCpr, "APR+CPR", "lpe");
  leg1->Draw();

  c0->SaveAs(Form("%s%s_trigger_rate.png", _figDir.Data(), name));

  // draw the rejection factors
  hRejBoth->Draw("E1");
  hRejApr ->Draw("E1 same");
  hRejCpr ->Draw("E1 same");
  hRejBoth->SetTitle("Background rejection factor;N(POT);rejection factor");
  hRejBoth->GetYaxis()->SetRangeUser(500., 5.*max(hRejApr->GetMaximum(), hRejCpr->GetMaximum()));
  hRejBoth->GetXaxis()->SetRangeUser(0., 99.e6);
  c0->SetLogy();

  leg1 = new TLegend(0.65, 0.76, 0.90, 0.90);
  leg1->SetFillStyle(1001);
  leg1->SetFillColor(kWhite);
  leg1->SetBorderSize(1);
  leg1->SetShadowColor(0);

  leg1->AddEntry(hRejApr , "APR", "lpe");
  leg1->AddEntry(hRejCpr , "CPR", "lpe");
  leg1->AddEntry(hRejBoth, "APR+CPR", "lpe");
  leg1->Draw();

  c0->SaveAs(Form("%s%s_rejection.png", _figDir.Data(), name));

  // draw the trigger rate per bin factors
  hRateBoth->Draw("E1");
  hRateApr ->Draw("E1 same");
  hRateCpr ->Draw("E1 same");
  hRateBoth->SetTitle("Instantaneous trigger rate;N(POT);Rate [Hz]");
  hRateBoth->GetXaxis()->SetRangeUser(0., 99.e6);
  c0->SetLogy(false);

  leg1 = new TLegend(0.65, 0.76, 0.90, 0.90);
  leg1->SetFillStyle(1001);
  leg1->SetFillColor(kWhite);
  leg1->SetBorderSize(1);
  leg1->SetShadowColor(0);

  leg1->AddEntry(hRateApr , "APR", "lpe");
  leg1->AddEntry(hRateCpr , "CPR", "lpe");
  leg1->AddEntry(hRateBoth, "APR+CPR", "lpe");
  leg1->Draw();

  c0->SaveAs(Form("%s%s_inst_trig_rate.png", _figDir.Data(), name));
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
  const double APREventsPassed = instLumiApr->Integral();
  const double CPREventsPassed = instLumiCpr->Integral();
  const double totalEventsPassed = instLumiAprCpr->Integral();
  const double totalEfficiency = totalEventsPassed/totalEvents;
  printf("APR eff = %.3f, CPR eff = %.3f, Trigger eff = %.3f\n",
         APREventsPassed/totalEvents, CPREventsPassed/totalEvents, totalEfficiency);
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
  aprcpr_effVsPOT->GetPaintedGraph()->GetXaxis()->SetRangeUser(0., 100.e6);

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
    load_input_file(_histDir + "HelixAna.trg_ana.cele0b1s5r0000.hist");
    if(!_inputFile) return 1;

    plot_param_and_ratio      ("cele", "d0"      , PlotData_t("d_{0}"          , -100.,  150.));
    plot_param_and_ratio      ("cele", "p_2"     , PlotData_t("p"              ,  100.,  105.));
    plot_param_and_ratio      ("cele", "pCenter" , PlotData_t("p(Center)"      , -110., -100.));
    plot_param_and_ratio      ("cele", "pt"      , PlotData_t("p_{T}"          ,  -95.,  -75.));
    plot_param_and_ratio      ("cele", "dP"      , PlotData_t("p - p(MC)"      ,   -5.,    5.));
    plot_param_and_ratio      ("cele", "clusterE", PlotData_t("Cluster energy" ,    0.,  110.));
    plot_param_and_ratio      ("cele", "ep"      , PlotData_t("E / P"          ,    0.,   1.2));
    plot_param_and_ratio      ("cele", "nActive" , PlotData_t("N(active)"      ,   10.,   80.));
    plot_param_and_ratio      ("cele", "rMax"    , PlotData_t("R(max)"         ,  430.,  800.));
    plot_param_and_ratio      ("cele", "radius"  , PlotData_t("Radius"         ,  200.,  300.));
    plot_param_and_ratio      ("cele", "tanDip"  , PlotData_t("tan(dip)"       ,   0.5,   1.5));
    plot_param_and_ratio      ("cele", "chi2NDof", PlotData_t("#chi^{2}/N(DOF)",    0.,    5.));
    efficiency_vs_pot         ("cele_eff_vs_pot");
    efficiency_vs_pot         ("cele_norm_eff_vs_pot", 1);
    efficiency_vs_track_params("cele");

    _inputFile->Close();
  }

  // mu- --> e+ plots
  if(Dataset < 0 || Dataset == 1) {
    load_input_file(_histDir + "HelixAna.trg_ana.cpos0b1s5r0000.hist");
    if(!_inputFile) return 1;

    plot_param_and_ratio      ("cpos", "d0"      , PlotData_t("d_{0}"          , -150.,  100.));
    plot_param_and_ratio      ("cpos", "p_2"     , PlotData_t("p"              ,   85.,   93.));
    plot_param_and_ratio      ("cpos", "pCenter" , PlotData_t("p(Center)"      ,   85.,   95.));
    plot_param_and_ratio      ("cpos", "pt"      , PlotData_t("p_{T}"          ,   60.,   80.));
    plot_param_and_ratio      ("cpos", "dP"      , PlotData_t("p - p(MC)"      ,   -5.,    5.));
    plot_param_and_ratio      ("cpos", "clusterE", PlotData_t("Cluster energy" ,    0.,  100.));
    plot_param_and_ratio      ("cpos", "ep"      , PlotData_t("E / P"          ,    0.,   1.2));
    plot_param_and_ratio      ("cpos", "nActive" , PlotData_t("N(active)"      ,   10.,   80.));
    plot_param_and_ratio      ("cpos", "rMax"    , PlotData_t("R(max)"         ,  430.,  600.));
    plot_param_and_ratio      ("cpos", "radius"  , PlotData_t("Radius"         ,  180.,  270.));
    plot_param_and_ratio      ("cpos", "tanDip"  , PlotData_t("tan(dip)"       ,   0.5,   1.5));
    plot_param_and_ratio      ("cpos", "chi2NDof", PlotData_t("#chi^{2}/N(DOF)",    0.,    5.));
    efficiency_vs_pot         ("cpos_eff_vs_pot");
    efficiency_vs_pot         ("cpos_norm_eff_vs_pot", 1);
    efficiency_vs_track_params("cpos");

    _inputFile->Close();
  }

  // No primary plots (1BB)
  if(Dataset < 0 || Dataset == 2) {
    load_input_file(_histDir + "HelixAna.trg_ana.mnbs0b1s5r0000.hist");
    if(!_inputFile) return 1;

    rejection_plot("mnbs0b1");
  }

  // No primary plots (2BB)
  if(Dataset < 0 || Dataset == 3) {
    load_input_file(_histDir + "HelixAna.trg_ana.mnbs0b2s5r0000.hist");
    if(!_inputFile) return 1;

    rejection_plot("mnbs0b2");
  }

  return 0;
}
