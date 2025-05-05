// $Id: $

/*!
 * \file QA_Draw_Utility.C
 * \brief
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision:   $
 * \date $Date: $
 */

#ifndef QA_Draw_Utility_C
#define QA_Draw_Utility_C

#include <TCanvas.h>
#include <TClass.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TSystem.h>

#include <TGraphErrors.h>
#include <TH1.h>
#include <TH1F.h>
#include <TList.h>
#include <TMath.h>
#include <TObject.h>
#include <TString.h>
#include <TStyle.h>
#include <TTree.h>

#include <TLegend.h>
#include <TLegendEntry.h>
#include <TLine.h>
#include <TPad.h>

#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

#include <iostream>
#include <stdexcept>

using namespace std;

// Custom assert handling for clang interpreter
#ifdef assert
#undef assert
#endif

#define assert(cond) if (!(cond)) {cerr <<"Fatal Error: Assertion of (" <<#cond <<") is false. Exiting. \nError condition raised at "<< __FILE__<<" line "<< __LINE__<<" Function "<<__PRETTY_FUNCTION__<<endl; throw runtime_error("Assertion Failure for expression: "+string(#cond));}


// KSTest summary utilities
class KSTestSummary
{
public:
  static KSTestSummary *getInstance()
  {
    if (instance == nullptr)
    {
      instance = new KSTestSummary();
    }
    return instance;
  }

  KSTestSummary()
  {
    h_pValue = new TH1F("h_pValue", "p-Value Summary;p-Value;Count of plots", 50, 0-1e-10, 1+1e-10);
    h_Log_pValue = new TH1F("h_Log_pValue", "Log p-Value Summary;Log[p-Value];Count of plots", 50, -20, 1e-10);
    
    h_pValue->SetFillColor(kBlue-2);
    h_Log_pValue->SetFillColor(kBlue-2);
  }

  void PushKSTest(double pValue)
  {
    if (pValue<=0)
    {
      cout <<"KSTestSummary::PushKSTest - Warning - received pValue = "<<pValue<<". Reset to an arbitary small non-zero value (e^(-15))"<<endl; 
      pValue = exp(-15);
    }
    
    m_vecpValues.push_back(pValue);

    h_pValue->Fill(pValue);
    h_Log_pValue->Fill(log(pValue));
  }

  const vector<double> &get_vecpValues() const { return m_vecpValues; }
  const TH1 *get_h_pValue() const { return h_pValue; }
  const TH1 *get_h_Log_pValue() const { return h_Log_pValue; }

  //! combined p-Value under Fisher's combined probability test
  double get_Combined_pValue() const
  {
    return TMath::Prob(get_Combined_Chi2(), get_Combined_NDF());
  }

  //! combined Chi2 under Fisher's combined probability test
  double get_Combined_Chi2() const
  {
    double Chi2(0);
    for (auto &p_value : m_vecpValues)
    {
      Chi2 += log(p_value);
    }
    Chi2 *= -2;
    return Chi2;
  }

  //! combined degree of freedom under Fisher's combined probability test
  int get_Combined_NDF() const
  {
    return 2 * m_vecpValues.size();
  }
  
  int get_nTest() const
  {
    return m_vecpValues.size();
  }
  
  const string make_summary_txt(const string & result_file);
  TCanvas *  make_summary_TCanvas();
  
 private:
  vector<double> m_vecpValues;
  TH1 *h_pValue = nullptr;
  TH1 *h_Log_pValue = nullptr;

  static KSTestSummary *instance;
};

KSTestSummary *KSTestSummary::instance = nullptr;


const string KSTestSummary::make_summary_txt(const string & result_file)
{
    // const string result_file("QA-tracking.txt");
    
    fstream fs_result_file(result_file, ios_base::out);
    
    ostringstream soutput;
    
    cout <<"This notebook contains "<<KSTestSummary::getInstance()->get_nTest()<<" KSTets: ";
    
    soutput <<"combined Chi2/nDoF = "<< KSTestSummary::getInstance()->get_Combined_Chi2();
    soutput <<" / "<<KSTestSummary::getInstance()-> get_Combined_NDF();
    soutput <<", and combined __p-Value = "<<KSTestSummary::getInstance()->get_Combined_pValue()<<"__";
    
    cout << soutput.str()<<endl;
    fs_result_file<< soutput.str();
    
    return soutput.str();
}
  
TCanvas *  KSTestSummary::make_summary_TCanvas()
{
    
    TCanvas *c1 = new TCanvas(TString("Summary") ,
                            TString("Summary") ,
                            950, 600);
    c1->Divide(2, 1);
    int idx = 1;
    TPad *p;

    if (KSTestSummary::getInstance()->get_h_pValue())
    {
        p = (TPad *) c1->cd(idx++);
        c1->Update();
        //p->SetLogy();

        KSTestSummary::getInstance()->get_h_pValue()->DrawClone();
    }

    if (KSTestSummary::getInstance()->get_h_Log_pValue())
    {
        p = (TPad *) c1->cd(idx++);
        c1->Update();
        p->SetLogy();

        KSTestSummary::getInstance()->get_h_Log_pValue()->DrawClone();
    }
    
    return c1;    
}


//! Divide canvas in a given number of pads, with a number of pads in both directions that match the width/height ratio of the canvas
void DivideCanvas(TVirtualPad *p, int npads)
{
  if (!p) return;
  if (!npads) return;
  const double ratio = double(p->GetWw()) / p->GetWh();
  Int_t columns = std::max(1, int(std::sqrt(npads * ratio)));
  Int_t rows = std::max(1, int(npads / columns));
  while (rows * columns < npads)
  {
    columns++;
    if (rows * columns < npads) rows++;
  }
  p->Divide(rows, columns);
}

//! Draw a vertical line in a pad at given x coordinate
TLine *VerticalLine(TVirtualPad *p, Double_t x)
{
  p->Update();

  Double_t yMin = p->GetUymin();
  Double_t yMax = p->GetUymax();

  if (p->GetLogy())
  {
    yMin = std::pow(10, yMin);
    yMax = std::pow(10, yMax);
  }

  TLine *line = new TLine(x, yMin, x, yMax);
  line->SetLineStyle(2);
  line->SetLineWidth(1);
  line->SetLineColor(1);
  return line;
}

//! Draw a horizontal line in a pad at given x coordinate
TLine *HorizontalLine(TVirtualPad *p, Double_t y)
{
  p->Update();

  Double_t xMin = p->GetUxmin();
  Double_t xMax = p->GetUxmax();

  if (p->GetLogx())
  {
    xMin = std::pow(10, xMin);
    xMax = std::pow(10, xMax);
  }

  TLine *line = new TLine(xMin, y, xMax, y);
  line->SetLineStyle(2);
  line->SetLineWidth(1);
  line->SetLineColor(1);
  return line;
}

//! Service function to SaveCanvas()
void SavePad(TPad *p)
{
  if (!p)
    return;

  TList *l = p->GetListOfPrimitives();
  //  l->Print();

  TIter next(l);
  TObject *obj = NULL;
  while ((obj = next()))
  {
    if (obj->IsA()->GetBaseClassOffset(TClass::GetClass("TPad")) >= 0)
    {
      if ((TPad *) obj != p)
        SavePad((TPad *) obj);
    }
    else if (obj->IsA()->GetBaseClassOffset(TClass::GetClass("TH1")) >= 0)
    {
      //cout << "Save TH1 " << obj->GetName() << endl;
      obj->Clone()->Write(obj->GetName(), TObject::kOverwrite);
    }
    else if (obj->IsA()->GetBaseClassOffset(TClass::GetClass("TF1")) >= 0)
    {
      //cout << "Save TF1 " << obj->GetName() << endl;
      obj->Clone()->Write(obj->GetName(), TObject::kOverwrite);
    }
    else if (obj->IsA()->GetBaseClassOffset(TClass::GetClass("TGraph")) >= 0)
    {
      //cout << "Save TGraph " << obj->GetName() << endl;
      obj->Clone()->Write(obj->GetName(), TObject::kOverwrite);
    }
  }
}

//! Save canvas to multiple formats
/*!
 *  @param[in] c    pointer to the canvas
 *  @param[in] name Base of the file name. The default is the name of the cavas
 *  @param[in] bEPS true = save .eps and .pdf format too.
 */
void SaveCanvas(TCanvas *c, TString name = "", Bool_t bEPS = kTRUE)
{
  if (name.Length() == 0)
    name = c->GetName();

  c->Print(name + ".png");

  TDirectory *oldd = gDirectory;

  TString rootfilename;

  c->Print(rootfilename = name + ".root");

  TFile f(rootfilename, "update");

  SavePad(c);

  f.Close();

  oldd->cd();

  if (bEPS)
  {
    //      c->Print(name + ".pdf");

    float x = 20;
    float y = 20;
    gStyle->GetPaperSize(x, y);

    gStyle->SetPaperSize(c->GetWindowWidth() / 72 * 2.54,
                         c->GetWindowHeight() / 72 * 2.54);
    //      c->Print(name + ".eps");
    c->Print(name + ".svg");
    gSystem->Exec("rsvg-convert -f pdf -o " + name + ".pdf " + name + ".svg");
    gSystem->Exec("rm -fv " + name + ".svg");

    gStyle->SetPaperSize(x, y);
  }
  //      c->Print(name+".C");
}

//! Draw 1D histogram along with its reference as shade
//! @param[in] draw_href_error whether to draw error band for reference plot. Otherwise, it is a filled histogram (default)
double DrawReference(TH1 *hnew, TH1 *href, bool draw_href_error = false, bool do_kstest = true)
{
  hnew->SetLineColor(kBlue + 3);
  hnew->SetMarkerColor(kBlue + 3);
  //  hnew->SetLineWidth(2);
  hnew->SetMarkerStyle(kFullCircle);
  //  hnew->SetMarkerSize(1);

  if (href)
  {
    if (draw_href_error)
    {
      href->SetLineColor(kGreen + 1);
      href->SetFillColor(kGreen + 1);
      href->SetLineStyle(kSolid);
      href->SetMarkerColor(kGreen + 1);
      //      href->SetLineWidth(2);
      href->SetMarkerStyle(kDot);
      href->SetMarkerSize(0);
    }
    else
    {
      href->SetLineColor(kGreen + 1);
      href->SetFillColor(kGreen + 1);
      href->SetLineStyle(0);
      href->SetMarkerColor(kGreen + 1);
      href->SetLineWidth(0);
      href->SetMarkerStyle(kDot);
      href->SetMarkerSize(0);
    }
  }

  hnew->Draw();  // set scale

  double ks_test = numeric_limits<double>::signaling_NaN();

  if (href)
  {
    if (draw_href_error)
    {
      href->DrawClone("E2 same");
      href->SetFillStyle(0);
      href->SetLineWidth(8);
      href->DrawClone("HIST same ][");
    }
    else
      href->Draw("HIST same");
    hnew->Draw("same");  // over lay data points

    if (do_kstest)
      ks_test = hnew->KolmogorovTest(href, "");
  }

  // ---------------------------------
  // now, make summary header
  // ---------------------------------
  if (href)
  {
    gPad->SetTopMargin(.14);
    TLegend *legend = new TLegend(0, .93, 0, 1, hnew->GetTitle(), "NB NDC");
    legend->Draw();
    legend = new TLegend(0, .86, .3, .93, NULL, "NB NDC");
    legend->AddEntry(href, Form("Reference"), "f");
    legend->Draw();
    legend = new TLegend(0.3, .86, 1, .93, NULL, "NB NDC");

    if (do_kstest)
    {
      TLegendEntry *le = legend->AddEntry(hnew, Form("New: KS-Test P=%.3f", ks_test), "lpe");
      if (ks_test >= 1)
        le->SetTextColor(kBlue + 1);
      else if (ks_test >= .2)
        le->SetTextColor(kGreen + 2);
      else if (ks_test >= .05)
        le->SetTextColor(kYellow + 1);
      else
        le->SetTextColor(kRed + 1);
      legend->Draw();
    }
    else
    {
      TLegendEntry *le = legend->AddEntry(hnew, Form("New Result"), "lpe");
      legend->Draw();
    }
  }
  else
  {
    gPad->SetTopMargin(.07);
    TLegend *legend = new TLegend(0, .93, 0, 1, hnew->GetTitle(), "NB NDC");
    legend->Draw();
  }

  if (do_kstest)
  {
    KSTestSummary::getInstance()->PushKSTest(ks_test);
  }

  return ks_test;
}

//! Draw 1D TGraph along with its reference as shade
//! @param[in] draw_href_error whether to draw error band for reference plot. Otherwise, it is a filled histogram (default)
void DrawReference(TGraph *hnew, TGraph *href, bool draw_href_error = true)
{
  hnew->SetLineColor(kBlue + 3);
  hnew->SetMarkerColor(kBlue + 3);
  hnew->SetLineWidth(2);
  hnew->SetMarkerStyle(kFullCircle);
  hnew->SetMarkerSize(1);

  if (href)
  {
    if (draw_href_error)
    {
      href->SetLineColor(kGreen + 1);
      href->SetFillColor(kGreen + 1);
      href->SetFillStyle(0);
      href->SetLineStyle(kSolid);
      href->SetMarkerColor(kGreen + 1);
      href->SetLineWidth(4);
      href->SetMarkerStyle(kDot);
      href->SetMarkerSize(0);
    }
    else
    {
      href->SetLineColor(kGreen + 1);
      href->SetFillColor(kGreen + 1);
      href->SetLineStyle(0);
      href->SetMarkerColor(kGreen + 1);
      href->SetLineWidth(0);
      href->SetMarkerStyle(kDot);
      href->SetMarkerSize(0);
    }
  }

  if (href)
  {
    if (draw_href_error)
    {
      href->DrawClone("E2");
    }
    else
      href->Draw("HIST same");
    hnew->Draw("p e");  // over lay data points
  }
  
  // ---------------------------------
  // now, make summary header
  // ---------------------------------
  if (href)
  {
    gPad->SetTopMargin(.14);
    TLegend *legend = new TLegend(0, .93, 0, 1, hnew->GetTitle(), "NB NDC");
    legend->Draw();
    legend = new TLegend(0, .86, .3, .93, NULL, "NB NDC");
    legend->AddEntry(href, Form("Reference"), "f");
    legend->Draw();
    legend = new TLegend(0.3, .86, 1, .93, NULL, "NB NDC");
    TLegendEntry *le = legend->AddEntry(hnew, Form("New"), "lpe");
    legend->Draw();
  }
  else
  {
    gPad->SetTopMargin(.07);
    TLegend *legend = new TLegend(0, .93, 0, 1, hnew->GetTitle(), "NB NDC");
    legend->Draw();
  }
}

//! Fit for resolution of TH2F
TGraphErrors *
FitResolution(const TH2F *h2, const bool normalize_mean = true, const int param = 2)
{ 
  std::unique_ptr<TProfile> p2(h2->ProfileX());
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> ey;

  for (int i = 1; i <= h2->GetNbinsX(); i++)
  {
    std::unique_ptr<TH1D> h1(h2->ProjectionY(Form("htmp_%d", rand()), i, i));

    // skip if too few entries
    if (h1->GetSum() < 10) continue;

    TF1 fgaus("fgaus", "gaus", -p2->GetBinError(i) * 4, p2->GetBinError(i) * 4);
    fgaus.SetParameter(1, p2->GetBinContent(i));
    fgaus.SetParameter(2, p2->GetBinError(i));

    h1->Fit(&fgaus, "MQ0");

    x.push_back( p2->GetBinCenter(i) );

    const double norm = normalize_mean ? fgaus.GetParameter(1) : 1;

    y.push_back( fgaus.GetParameter(param) / norm );
    ey.push_back( fgaus.GetParError(param) / norm );
  }

  auto ge = new TGraphErrors(x.size(), &x[0], &y[0], nullptr, &ey[0]);
  ge->SetName(TString(h2->GetName()) + "_FitResolution");

  ge->SetLineColor(kBlue + 3);
  ge->SetMarkerColor(kBlue + 3);
  ge->SetLineWidth(2);
  ge->SetMarkerStyle(kFullCircle);
  ge->SetMarkerSize(1);
  return ge;
}

//! Fit for resolution of TH2F
TH1*
FitResolution_hist(const TH2F *h2, const bool normalize_mean = true, const int param = 2)
{ 
  std::unique_ptr<TProfile> p2(h2->ProfileX());
  TH1* hout = new TH1F( TString(h2->GetName())+"_FitResolution", "", h2->GetNbinsX(), h2->GetXaxis()->GetXmin(), h2->GetXaxis()->GetXmax() );
  
  for (int i = 1; i <= h2->GetNbinsX(); i++)
  {
    std::unique_ptr<TH1D> h1(h2->ProjectionY(Form("htmp_%d", rand()), i, i));

    // skip if too few entries
    if (h1->GetSum() < 10) continue;

    TF1 fgaus("fgaus", "gaus", -p2->GetBinError(i) * 4, p2->GetBinError(i) * 4);
    fgaus.SetParameter(1, p2->GetBinContent(i));
    fgaus.SetParameter(2, p2->GetBinError(i));

    h1->Fit(&fgaus, "MQ0");

    const double norm = normalize_mean ? fgaus.GetParameter(1) : 1;
    hout->SetBinContent( i, fgaus.GetParameter(param) / norm );
    hout->SetBinError( i, fgaus.GetParError(param) / norm );
  }
  return hout;
}

//! Fit for profile along the x direction of TH2F
TGraphErrors*
FitProfile(const TH2 *h2)
{
  std::unique_ptr<TProfile> p2( h2->ProfileX() );
  std::vector<double> x;
  std::vector<double> ex;
  std::vector<double> y;
  std::vector<double> ey;

  for (int i = 1; i <= h2->GetNbinsX(); i++)
  {
    std::unique_ptr<TH1D> h1( h2->ProjectionY(Form("htmp_%d", rand()), i, i) );
    if (h1->GetSum() < 10) continue;

    TF1 fgaus("fgaus", "gaus", -p2->GetBinError(i) * 4, p2->GetBinError(i) * 4);
    fgaus.SetParameter(1, p2->GetBinContent(i));
    fgaus.SetParameter(2, p2->GetBinError(i));
    h1->Fit(&fgaus, "MQ0");

    x.push_back(p2->GetBinCenter(i));
    ex.push_back(p2->GetBinWidth(i)/2);
    y.push_back(fgaus.GetParameter(1));
    ey.push_back(fgaus.GetParameter(2));
  }

  TGraphErrors *ge = new TGraphErrors(x.size(), &x[0], &y[0], &ex[0], &ey[0]);
  ge->SetName(TString(h2->GetName()) + "_FitProfile");
  ge->SetLineColor(kBlue + 3);
  ge->SetMarkerColor(kBlue + 3);
  ge->SetLineWidth(2);
  ge->SetMarkerStyle(kFullCircle);
  ge->SetMarkerSize(1);
  return ge;
}

//!ratio between two histograms with binominal error based on Wilson score interval. Assuming each histogram is count.
TH1 *GetBinominalRatio(TH1 *h_pass, TH1 *h_n_trial, bool process_zero_bins = false)
{
  assert(h_pass);
  assert(h_n_trial);

  assert(h_pass->GetNbinsX() == h_n_trial->GetNbinsX());
  assert(h_pass->GetNbinsY() == h_n_trial->GetNbinsY());
  assert(h_pass->GetNbinsZ() == h_n_trial->GetNbinsZ());

  TH1 *h_ratio = (TH1 *) h_pass->Clone(TString(h_pass->GetName()) + "_Ratio");
  assert(h_ratio);
  h_ratio->Divide(h_n_trial);  // a rough estimation first, also taking care of the overflow bins and zero bins

  for (int x = 1; x <= h_n_trial->GetNbinsX(); ++x)
    for (int y = 1; y <= h_n_trial->GetNbinsY(); ++y)
      for (int z = 1; z <= h_n_trial->GetNbinsZ(); ++z)
      {
        const double n_trial = h_n_trial->GetBinContent(x, y, z);

        if (n_trial > 0)
        {
          const double p = h_pass->GetBinContent(x, y, z) / n_trial;

          // Wilson score interval
          h_ratio->SetBinContent(x, y, z,  //
                                 (p + 1 / (2 * n_trial)) / (1 + 1 / n_trial));
          h_ratio->SetBinError(x, y,
                               z,  //
                               std::sqrt(
                                   1. / n_trial * p * (1 - p) + 1. / (4 * n_trial * n_trial)) /
                                   (1 + 1 / n_trial));
        }
        else if (process_zero_bins)
        {
          h_ratio->SetBinContent(x, y, z, 0.5);
          h_ratio->SetBinError(x, y, z, 0.5);
        }
      }

  return h_ratio;
}

//! Put input file name on the canvas
//! \param[in] c1 canvas pointer
//! \param[in] bottom height fraction height of the canvas to be used for this label
//! \param[in] new_file_name new file name being inspected
//! \param[in] ref_file_name reference file name
void PutInputFileName(TCanvas *c1, const double height, const char *new_file_name, const char *ref_file_name)
{
  c1->cd();
  TPad *pad = new TPad("PutInputFileName", "PutInputFileName", 0, 0, 1, height, 18);
  pad->Draw();
  pad->cd();

  if (new_file_name)
  {
    TText *t = new TText(0.05, 0.75, TString("New QA File:          ") + TString(new_file_name));
    t->SetTextAlign(12);
    t->SetTextColor(kBlue + 3);
    t->SetTextSize(.4);
    t->Draw();
  }
  if (ref_file_name)
  {
    TText *t = new TText(0.05, 0.25, TString("Reference QA File: ") + TString(ref_file_name));
    t->SetTextAlign(12);
    t->SetTextColor(kGreen + 1);
    t->SetTextSize(.4);
    t->Draw();
  }
}

//! GraphError to CSV
void SaveGraphError2CSV(TGraph* ge, const std::string & csv_name_base)
{
  if (not ge) return;

  std::stringstream sheader, sdata_err, sdata_center;

  for (int bin = 0; bin<ge->GetN(); ++bin)
  {
    sheader <<std::setprecision(3) << ge->GetX()[bin] - ge->GetEX()[bin]<<" - "<< ge->GetX()[bin] + ge->GetEX()[bin];

    sdata_center << ge->GetY()[bin] ;
    sdata_err << ge->GetEY()[bin] ;

    if (bin<ge->GetN()-1) 
    {
      sheader<<", ";
      sdata_center<<", ";
      sdata_err<<", ";
    }

  }
  
  std::ofstream outfile;
  outfile.open(csv_name_base + "_centeral_value.csv");
  outfile << sheader.str() << std::endl<<sdata_center.str() << std::endl;
  outfile.close();
  outfile.open(csv_name_base + "_errorbar.csv");
  outfile << sheader.str() << std::endl<<sdata_err.str() << std::endl;
  outfile.close();

}

#endif
