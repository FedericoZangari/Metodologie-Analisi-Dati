// A simple C++ program to illustrate the use of ROOT class TMinuit 
// for function minimization.  The example shows a Maximum Likelihood
// fit for a mixture of truncated exponential and Gaussian.
// TMinuit minimizes -2*log(L) as calculated in the function fcn

// Glen Cowan
// RHUL Physics
// November 2020

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>

#include <TMinuit.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TF1.h>
#include <TH1.h>
#include <TAxis.h>
#include <TLine.h>
#include <TRandom3.h>
#include <TLegend.h>
#include <Math/ProbFuncMathCore.h>
#include <Math/PdfFuncMathCore.h>
#include <Math/QuantFuncMathCore.h>
#include <Math/Random.h>
#include <TLatex.h>

using namespace std;

// Declare pointer to data as global (not elegant but TMinuit needs this).

vector<double>* xVecPtr = new vector<double>();

// The pdf to be fitted.

double pdf(double* xPtr, double par[]){
  double x = *xPtr;
  double theta = par[0];
  double xi = par[1];
  double mu = par[2];
  double sigma = par[3];
  double xMax = par[4];
  double normConst = ROOT::Math::gaussian_cdf(xMax, sigma, mu) -
    ROOT::Math::gaussian_cdf(0., sigma, mu);
  double f = 0;
  if (x >= 0 && x <= xMax && xi > 0) {
    double lambda = 1/xi;
    f = theta * ROOT::Math::gaussian_pdf(x, sigma, mu) / normConst  +
      (1. - theta) * ROOT::Math::exponential_pdf(x, lambda) /
      (1. - exp(-lambda*xMax));
  }
  return f;
}

//-------------------------------------------------------------------------

void generateData(vector<double>* xVecPtr, double parGen[], int numVal){

  int seed = 12345;
  TRandom3 ran(seed);

  double theta = parGen[0];
  double xi = parGen[1];
  double mu = parGen[2];
  double sigma = parGen[3];
  double xMax = parGen[4];

  for (int i = 0; i<numVal; ++i){
    double r1 = ran.Rndm();
    double x =  0.;
    if (r1 < theta){
      bool gotValue = false;
      while(!gotValue){
	x = ran.Gaus(mu, sigma);
	gotValue = x >= 0. && x <= xMax;
      }
    }
    else {
      double r2 = ran.Rndm();
      x = - xi * log(1. - r2*(1. - exp(-xMax/xi)));
    }
    xVecPtr->push_back(x);
  }

}

//-------------------------------------------------------------------------

// fcn passes back fObj = -2*ln(L) by reference, the function to be minimized.

void fcn(int& npar, double* deriv, double& fObj, double par[], int flag){

  vector<double> xVec = *xVecPtr;           // xVecPtr is global
  int n = xVec.size();

  double lnL = 0.0;
  for (int i=0; i<n; i++){
    double x = xVec[i];
    double f = pdf(&x, par);
    if (f > 0.0) {
      lnL += log(f);    // need positive f
    }
    else {
      cout << "WARNING -- pdf is negative!!!" << endl;
    }
  }
  fObj = - 2.*lnL;         // factor of -2 so minuit gets the errors right

}                         // end of fcn

//-------------------------------------------------------------------------

int main(int argc, char **argv) {

  TApplication theApp("App", &argc, argv);
  TCanvas* canvas = new TCanvas();

// Set a bunch of parameters to make the plot look nice

  canvas->SetFillColor(0);
  canvas->UseCurrentStyle();
  canvas->SetBorderMode(0);       // still leaves red frame bottom and right
  canvas->SetFrameBorderMode(0);   // need this to turn off red hist frame!
  gROOT->SetStyle("Plain");
  canvas->UseCurrentStyle();
  gROOT->ForceStyle();

  gStyle->SetOptStat(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleSize(0.04);
  gStyle->SetTitleFont(42, "hxy");      // for histogram and axis titles
  gStyle->SetLabelFont(42, "xyz");      // for axis labels (values)
  gROOT->ForceStyle();

  gPad->SetLeftMargin(0.12);
  gPad->SetRightMargin(0.06);
  gPad->SetTopMargin(0.06);
  gPad->SetBottomMargin(0.12);

// Generate data.  xVecPtr is global.

  double theta = 0.2;           // signal fraction
  double xi = 5.;               // mean of exponential
  double mu = 10.;              // mean of Gaussian
  double sigma = 2.;            // std. dev. of Gaussian
  double xMax = 20.;

  const int npar = 5;  
  double parGen[npar];
  parGen[0] = theta;
  parGen[1] = xi;
  parGen[2] = mu;
  parGen[3] = sigma;
  parGen[4] = xMax;

  int numVal = 200;
  generateData(xVecPtr, parGen, numVal);
  vector<double> xVec = *xVecPtr;

// Initialize minuit, set initial values etc. of parameters.

  TMinuit minuit(npar);
  minuit.SetFCN(fcn);

  double par[npar];               // the start values
  double stepSize[npar];          // step sizes 
  double minVal[npar];            // minimum bound on parameter 
  double maxVal[npar];            // maximum bound on parameter
  string parName[npar];

  par[0] = 0.4;             // a guess
  stepSize[0] = 0.04;       // take e.g. 0.1 of start value
  minVal[0] = 0.;           // both min/max values zero means no limit
  maxVal[0] = 1.;
  parName[0] = "theta";

  par[1] = 4.;             
  stepSize[1] = 0.4; 
  minVal[1] = 1.e-3;
  maxVal[1] = 1.e3;
  parName[1] = "xi";

  par[2] = mu;
  stepSize[2] = 0.;     // fixed
  minVal[2] = 0.;
  maxVal[2] = 0.;
  //minVal[2] = 1.e-3;
  //maxVal[2] = 1.e3;
  parName[2] = "mu";

  par[3] = sigma;
  stepSize[3] = 0.;     // fixed
  minVal[3] = 0.;
  maxVal[3] = 0.;
  //minVal[3] = 1.e-3;
  //maxVal[3] = 1.e3;
  parName[3] = "sigma";

  par[4] = xMax;
  stepSize[4] = 0.;     // fixed
  minVal[4] = 0.;
  maxVal[4] = 0.;
  parName[4] = "xMax";

  for (int i=0; i<npar; i++){
    minuit.DefineParameter(i, parName[i].c_str(), 
      par[i], stepSize[i], minVal[i], maxVal[i]);
  }

  //minuit.FixParameter(1);
  //minuit.FixParameter(2);
  //minuit.FixParameter(3);
  //minuit.FixParameter(4);

//for(int k = 0; k < 4; k++){
// Do the minimization
  minuit.Migrad();
  double MLE[npar], sigmaMLE[npar];
  for (int i=0; i<npar; i++){
    minuit.GetParameter(i, MLE[i], sigmaMLE[i]);
  }
  
  cout << "par. index, MLE and std. dev.:" << endl;
  for (int i=0; i<2; i++){
    cout << i << "  " << MLE[i] << "  " << sigmaMLE[i] << endl;
  }
  cout << endl;
  ofstream out;
  out.open("sigma_theta2.txt", ios::app);
  //out << numVal << "\t" << sigmaMLE[0] << endl;
  //out << k+1 << "\t" << MLE[0] << "\t" << sigmaMLE[0] << endl;

  int nparFree = minuit.GetNumFreePars();
  double cov[nparFree][nparFree];
  double rho[nparFree][nparFree];
  minuit.mnemat(&cov[0][0], nparFree);
  cout << "par. indices, cov, correlation coef.:" << endl;
  for (int i=0; i<nparFree; i++){
    for (int j=0; j<nparFree; j++){
      rho[i][j] = cov[i][j] / sqrt(cov[i][i]*cov[j][j]);
      cout << i << "  " << j << "  " << cov[i][j] << "  " << rho[i][j] << endl;
    }
  }
  //minuit.Release(k+1);
//}
// Plot the result.

  TF1* func = new TF1("", pdf, 0., xMax, npar);
  func->SetParameters(MLE);

  TAxis* xa = func->GetHistogram()->GetXaxis();
  TAxis* ya = func->GetHistogram()->GetYaxis();
  xa->SetTitleOffset(1.2);    //  factor multiplies default offset
  ya->SetTitleOffset(1.5);
  xa->SetLabelOffset(0.005);
  ya->SetLabelOffset(0.02);

  func->SetLineStyle(1);             //  1 = solid, 2 = dashed, 3 = dotted
  func->SetLineColor(kRed);             //  black (default)
  func->SetLineWidth(3);
  func->GetXaxis()->SetTitle("x");
  func->GetYaxis()->SetTitle("f(x; #theta, #xi)");
  func->SetTitle("Maximum likelihood fit (exponential + Gaussian)");
  func->Draw();

  double zero = 0.;
  double yMax = pdf(&zero, MLE)*1.1;
  func->SetMinimum(0.);
  func->SetMaximum(yMax);

  const double tickHeight = yMax*0.05;
  TLine* tick = new TLine();
  for (int i=0; i<xVec.size(); i++){
    if ( xVec[i] <= xMax ){
      tick->DrawLine(xVec[i], 0., xVec[i], tickHeight);
    }
  }

  TLegend* leg = new TLegend(0.2, 0.72, 0.6, 0.88); // x1, y1, x2, y2
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(func, "  fit function", "l");
  leg->AddEntry(tick, "  data", "l");
  leg->Draw();
  canvas->Update();
  canvas->Show();

// Make -2*logL scan for theta
  int ierr;
  double arglist[4];
  arglist[0] = 1;      // par no.  (starts at 1, not 0)
  arglist[1] = 100;     // num points
  arglist[2] = MLE[0] - 0.1;    // from
  arglist[3] = MLE[0] + 0.1;     // to
  minuit.mnexcm("SCAN", arglist, 4, ierr);
  TCanvas* c1 = new TCanvas();
  c1->UseCurrentStyle();
  TGraph* tg1 = (TGraph*)minuit.GetPlot();
  tg1->GetXaxis()->SetTitle("#theta");
  tg1->GetYaxis()->SetTitle("-2 log L");
  tg1->SetTitle("-2 log L scan for #theta");
  TAxis* xa1 = tg1->GetHistogram()->GetXaxis();
  TAxis* ya1 = tg1->GetHistogram()->GetYaxis();
  xa1->SetTitleOffset(1.1);    //  factor multiplies default offset
  ya1->SetTitleOffset(1.3);
  xa1->SetLabelOffset(0.005);
  ya1->SetLabelOffset(0.01);
  double logL;
  double * der;
  int n = npar;
  fcn(n, der, logL, MLE, 0);
  cout << "-2logL at MLE: " << logL << endl;
  double theta_min = 1, theta_max=0;
  for(int i = 0; i < tg1->GetN(); i++){
    double x = tg1->GetPointX(i);
    double y = tg1->GetPointY(i);
    if( y <= logL+1 && x < theta_min) theta_min = x;
    if( y <= logL+1 && x > theta_max) theta_max = x;
  }
  cout << "theta min: " << theta_min << "\t" << "theta max: " << theta_max << endl;
  TLine* line = new TLine(xa1->GetXmin()*1.2, logL+1, xa1->GetXmax()*0.95, logL+1);
  TLine* line2 = new TLine(theta_min, ya1->GetXmin(), theta_min, logL+2);
  TLine* line3 = new TLine(theta_max, ya1->GetXmin(), theta_max, logL+2);
  //TLine* line2 = new TLine(0.1586, ya1->GetXmin(), 0.1586, logL+2);
  //TLine* line3 = new TLine(0.2492, ya1->GetXmin(), 0.2492, logL+2);
  tg1->Draw();
  line->SetLineStyle(2);
  line2->SetLineStyle(2);
  line3->SetLineStyle(2);
  line->Draw("same");
  line2->Draw("same");
  line3->Draw("same");
  TLatex* tl = new TLatex();
  tl->SetTextAlign(11);
  tl->SetTextSize(0.04);
  tl->SetTextFont(42);
  tl->SetNDC();
  tl->DrawLatex(.70, 0.25, " -2 log L_{max} +1");
  TLatex* t = new TLatex();
  t->SetTextAlign(11);
  t->SetTextSize(0.05);
  t->SetTextFont(42);
  t->SetNDC();
  t->DrawLatex(.33, 0.15, "#theta_{-} ");
  TLatex* t2 = new TLatex();
  t2->SetTextAlign(11);
  t2->SetTextSize(0.05);
  t2->SetTextFont(42);
  t2->SetNDC();
  t2->DrawLatex(.65, 0.15, " #theta_{+}");
  c1->Update();
  c1->Show();

// Make contour plot
  TCanvas* c2 = new TCanvas();
  c2->UseCurrentStyle();
  TGraph* tg2 = (TGraph*)minuit.Contour(100);
  tg2->GetXaxis()->SetTitle("#theta");
  tg2->GetYaxis()->SetTitle("#xi");
  tg2->SetTitle("Contour plot for #theta , #xi");
  TAxis* xa2 = tg2->GetHistogram()->GetXaxis();
  TAxis* ya2 = tg2->GetHistogram()->GetYaxis();
  double x_min = 1000, y_min = 1000;
  double x_max = 0, y_max = 0;
  for(int i = 0; i < 101; i++){
    double x = tg2->GetPointX(i);
    double y = tg2->GetPointY(i);
    if( x < x_min) x_min = x;
    if( x > x_max) x_max = x;
    if( y < y_min) y_min = y;
    if( y > y_max) y_max = y;
  }
  cout << "theta_min " << x_min << " theta_max " << x_max << endl;
  cout << "xi_min " << y_min << " xi_max " << y_max << endl; 
  TLine* l1 = new TLine(xa2->GetXmin(), y_max, xa2->GetXmax()*0.8, y_max);
  TLine* l2 = new TLine(xa2->GetXmin(), y_min, xa2->GetXmax()*0.97, y_min);
  l1->SetLineStyle(2);
  l2->SetLineStyle(2);
  TLine* l3 = new TLine( x_max, ya2->GetXmin(), x_max, ya2->GetXmax()*0.88);
  TLine* l4 = new TLine( x_min, ya2->GetXmin(), x_min, ya2->GetXmax()*0.96);
  l3->SetLineStyle(2);
  l4->SetLineStyle(2);
  xa2->SetTitleOffset(1.2);    //  factor multiplies default offset
  ya2->SetTitleOffset(1.2);
  xa2->SetLabelOffset(0.005);
  ya2->SetLabelOffset(0.02);
  tg2->Draw();
  l1->Draw("same");
  l2->Draw("same");
  l3->Draw("same");
  l4->Draw("same");
  c2->Update();
  c2->Show();
  
  //canvas->SaveAs("mlFit_exp_gaus.png");
  c1->SaveAs("logL_scan.png");
  //c2->SaveAs("contour_logL.png");

  cout << "To exit, quit ROOT from the File menu of the plot" << endl;
  theApp.Run(true);
  canvas->Close();

  delete canvas;
  delete tick;
  delete xVecPtr;
  delete c1;
  delete c2;
  
  return 0;

}
