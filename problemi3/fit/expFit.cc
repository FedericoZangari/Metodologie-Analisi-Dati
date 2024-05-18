// A simple C++ program to illustrate the use of ROOT class TMinuit 
// for function minimization.  The example shows a Maximum Likelihood
// fit for the mean of an exponential pdf in which TMinuit 
// minimizes - 2*log(L).   The user must specify what to minimize in the 
// function fcn, shown in this file below.

// fcn passes back f = -2*ln L by reference; this is the function to minimize.
// The factor of -2 allows MINUIT to get the errors using the same
// recipe as for least squares, i.e., go up from the minimum by 1.

// TMinuit does not allow fcn to be a member function, and the function
// arguments are fixed, so the one of the only ways to bring the data  
// into fcn is to declare a pointer to the data (xVecPtr) as global.

// For more info on TMinuit see root.cern.ch/root/html/TMinuit.html .

// Glen Cowan
// RHUL Physics
// 4 December 2006

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>

#include <TMinuit.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TF1.h>
#include <TH1F.h>
#include <TString.h>
#include <TAxis.h>
#include <TLine.h>
#include <TLegend.h>
#include <TMath.h>

using namespace std;

// Declare pointer to data as global (not elegant but TMinuit needs this).

vector<double>* xVecPtr = new vector<double>();

// The pdf to be fitted, here an exponential. 
// First argument needs to be a pointer in order to plot with the TF1 class.

double expPdf(double* xPtr, double par[]){        
  double x = *xPtr;
  double xi = par[0];     // mean of x
  double f = 0;
  if ( x >= 0 && xi > 0. ) { 
    f = (1.0/xi) * exp(-x/xi); 
  }
  return f;
}

double exp2Pdf(double* xPtr, double par[]){        
  double x = *xPtr;
  double alfa = par[0];     
  double xi1 = par[1]; 
  double xi2 = par[2]; 
  double f = 0;
  if ( x >= 0 && xi1 > 0. && xi2 > 0. && alfa > 0.) { 
    f = (alfa/xi1) * exp(-x/xi1) + (1-alfa)/xi2 * exp(-x/xi2); 
  }
  return f;
}

//-------------------------------------------------------------------------

//  function to read in the data from a file

void getData(vector<double>* xVecPtr){

  //string infile;
  //cout << "Enter name of input data file: ";
  //cin >> infile;

  ifstream f;
  //f.open(infile.c_str());
  //f.open("../generate/expData.txt");
  f.open("../generate/expData2.txt");
  if ( f.fail() ){
    cout << "Sorry, couldn't open file" << endl;
    exit(1);
  }

  double x ;
  bool acceptInput = true;
  while ( acceptInput ) {
    f >> x;
    acceptInput = !f.eof();
    if ( acceptInput ) {
      xVecPtr->push_back(x);
    }
  }
  f.close();

}

//-------------------------------------------------------------------------

// fcn passes back f = - 2*ln(L), the function to be minimized.

void fcn(int& npar, double* deriv, double& f, double par[], int flag){

  vector<double> xVec = *xVecPtr;           // xVecPtr is global
  int n = xVec.size();

  double lnL = 0.0;
  for (int i=0; i<n; i++){
    double x = xVec[i];
    //double pdf = expPdf(&x, par);
    double pdf = exp2Pdf(&x, par);
    if ( pdf > 0.0 ) {
      lnL += log(pdf);    // need positive f
    }
    else {
      cout << "WARNING -- pdf is negative!!!" << endl;
    }
  }
  f = -2.0 * lnL;         // factor of -2 so minuit gets the errors right
 
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
  
  gStyle->SetTitleX(0.3);

// Read in the data.  xVecPtr is global.

  getData(xVecPtr);   

// Initialize minuit, set initial values etc. of parameters.

  const int npar = 3;              // the number of parameters
  //const int npar = 1; 
  TMinuit minuit(npar);
  minuit.SetFCN(fcn);

  double par[npar];               // the start values
  double stepSize[npar];          // step sizes 
  double minVal[npar];            // minimum bound on parameter 
  double maxVal[npar];            // maximum bound on parameter
  string parName[npar];
/*
  par[0] = 0.3;            // a guess
  stepSize[0] = 0.1;       // take e.g. 0.1 of start value
  minVal[0] = 0.00001;   // if min and max values = 0, parameter is unbounded.
  maxVal[0] = 100000;
  parName[0] = "xi";
*/

  par[0] = 0.3;            // a guess
  stepSize[0] = 0.1;       // take e.g. 0.1 of start value
  minVal[0] = 0.0;   // if min and max values = 0, parameter is unbounded.
  maxVal[0] = 0;
  parName[0] = "alfa";

  par[1] = 1.5;            // a guess
  stepSize[1] = 0.1;       // take e.g. 0.1 of start value
  minVal[1] = 0.0;   // if min and max values = 0, parameter is unbounded.
  maxVal[1] = 0.0;
  parName[1] = "xi_1";

  par[2] = 3;            // a guess
  stepSize[2] = 0.1;       // take e.g. 0.1 of start value
  minVal[2] = 0.0;   // if min and max values = 0, parameter is unbounded.
  maxVal[2] = 0.0;
  parName[2] = "xi_2";

  for (int i=0; i<npar; i++){
    minuit.DefineParameter(i, parName[i].c_str(), 
      par[i], stepSize[i], minVal[i], maxVal[i]);
  }

// Do the minimization!

  minuit.Migrad();       // Minuit's best minimization algorithm
  double outpar[npar], err[npar];
  TString name;
  double val, er, lolim, uplim;
  int in;
  for (int i=0; i<npar; i++){
    minuit.GetParameter(i,outpar[i],err[i]);
    cout << parName[i] << ": " << outpar[i] << " +/- " << err[i] << endl; 
    minuit.mnpout(i, name, val,er,lolim,uplim,in);
    //cout << name << ":\t" << val << " +/- " << er << "\t" << lolim << "\t" << uplim << "\t" << in <<endl; 
  }

  double covmat [npar] [npar];
  minuit.mnemat (& covmat [0] [0], npar);
  cout << "covariance matrix" << endl;
  for (int i=0; i<npar; i++){
    for (int j=0; j<npar; j++){
      cout << covmat[i][j] << "\t";
    }
    cout << endl;
  }
  cout << "correlation coefficients matrix" << endl;
  for (int i=0; i<npar; i++){
    for (int j=0; j<npar; j++){
      cout << covmat[i][j]/err[i]/err[j] << "\t";
    }
    cout << endl;
  }
// Plot the result.  For this example plot x values as tick marks.

  double xmin = 0.0;
  double xmax = 15.0;
  TF1* func = new TF1("funcplot", exp2Pdf, xmin, xmax, npar);
  func->SetParameters(outpar);
  func->Draw();

  func->SetLineStyle(1);             //  1 = solid, 2 = dashed, 3 = dotted
  func->SetLineColor(kRed);             //  black (default)
  func->SetLineWidth(1);
  func->SetTitle("Maximum likelihood fit");
  func->GetXaxis()->SetTitle("x");
  func->GetYaxis()->SetTitleOffset(1.1);
  func->GetYaxis()->SetTitle("f(x; #alpha, #xi_{1}, #xi_{2})");

  vector<double> xVec = *xVecPtr;
  const double tickHeight = 0.05;
  TLine* tick = new TLine();
  for (int i=0; i<xVec.size(); i++){
    tick->DrawLine(xVec[i], 0, xVec[i], tickHeight);
  }

  TLegend* leg = new TLegend(0.2, 0.72, 0.6, 0.88); // x1, y1, x2, y2
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(func, "  fit function", "l");
  leg->AddEntry(tick, "  data", "l");
  leg->Draw();

// chi square test

  int n_bin = 8;
  vector<double> v = *xVecPtr;
  TH1F * h = new TH1F("h", "h", n_bin, 0, 15);
  TH1F * h2 = new TH1F("h2", "h2", n_bin, 0, 15);
  for(int  i = 0; i < v.size(); i++){
    h->Fill(v[i]);
  }
  cout << "chi square: " << h->Chisquare(func) << endl;
  double chi_square = 0;
  for(int i = 1; i <= n_bin; i ++){
    double nu = v.size() * func->Integral(h->GetBinLowEdge(i), h->GetBinLowEdge(i) + h->GetBinWidth(i));
    h2->SetBinContent(i, nu);
    //double nu = v.size() * func->Eval(h->GetBinCenter(i));
    cout << h->GetBinLowEdge(i) << "\t" << h->GetBinLowEdge(i) + h->GetBinWidth(i) << "\t" << h->GetBinContent(i)  << "\t" <<  nu << "\t" << pow(h->GetBinContent(i) - nu,2) << "\t" << pow(h->GetBinError(i),2)<< endl;
    chi_square += pow(h->GetBinContent(i) - nu,2) /pow(h->GetBinError(i),2);
  }
  cout << "chi square: " << chi_square << endl;
  cout << "number of degrees of freedom: " << n_bin - npar << endl;
  cout << "p-value:" << TMath::Prob(chi_square, n_bin - npar) << endl;
  TCanvas* canvas2 = new TCanvas();
  h->SetTitle("Binned data");
  h->GetXaxis()->SetTitle("x");
  h->GetYaxis()->SetTitle("entries");
  h->SetFillColor(kRed);
  h->SetMarkerColor(kRed);
  h->SetLineColor(kRed+1);
  h->SetLineWidth(2);
  h->SetFillStyle(3435);
  h->Draw();
  TH1F * hh = (TH1F*)h->Clone();
  hh->Draw("same pe");
  h2->SetMarkerStyle(20);
  h2->SetMarkerColor(kBlue);
  h2->SetLineColor(kBlue+2);
  h2->SetMarkerSize(1);
  h2->Draw("SAME pe");
  TLegend* leg2 = new TLegend(0.6, 0.72, 0.9, 0.88); // x1, y1, x2, y2
  leg2->SetTextSize(0.04);
  leg2->SetTextFont(42);
  leg2->SetBorderSize(0);
  leg2->SetFillColor(0);
  leg2->AddEntry(h, "  data", "lef");
  leg2->AddEntry(h2, "  model prediction", "pe");
  leg2->Draw();

  cout << "To exit, quit ROOT from the File menu of the plot" << endl;
  theApp.Run(true);
  //canvas->SaveAs("expFit2.png");
  
  canvas2->SaveAs("binned.png");
  canvas->Close();
  canvas2->Close();

  delete canvas;
  delete tick;
  //delete xVecPtr;

  return 0;

}
