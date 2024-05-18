#include <iostream>
#include <fstream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TPad.h>
#include <TAxis.h>
#include <TH2F.h>
#include <TF2.h>
#include <TLine.h>
#include "Event.h"
#include "OutFunc.h"
#include <TMVA/Reader.h>

using namespace std;

OutFunc* testStat;      // need global for contour later

int main() {

  std::vector<int> N = {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 10000, 50000};
  //TFile* histFile = new TFile("analysis.root", "RECREATE");
  TFile* histFile = new TFile("analysis_BDT.root", "RECREATE");  
  ofstream out;
  out.open("BDT_error_test.dat",ios::app);
  //out.open("BDT_error_train.dat",ios::app);
// Set up an output file and book some histograms
   for(int z = 0; z < N.size(); z++){
    //TFile* histFile = new TFile("analysis.root", "UPDATE");  
    //TH1D* hSig = new TH1D("hSig", "Fisher, signal", 100, -5.0, 5.0);
    //TH1D* hBkg = new TH1D("hBkg", "Fisher, background", 100, -5.0, 5.0);
    //TH1D* hSig = new TH1D("hSig", "MLP, signal", 100, -5.0, 5.0);
    //TH1D* hBkg = new TH1D("hBkg", "MLP, background", 100, -5.0, 5.0);
    string title = "hSig" + to_string(N[z]) + "_train";
    string title2 = "hBkg" + to_string(N[z]) + "_train";
    TH1D* hSig = new TH1D(title.c_str(), "BDT, signal", 100, -5.0, 5.0);
    TH1D* hBkg = new TH1D(title2.c_str(), "BDT, background", 100, -5.0, 5.0);
    //TH1D* hSig = new TH1D("hSig", "BDT200 test, signal", 100, -5.0, 5.0);
    //TH1D* hBkg = new TH1D("hBkg", "BDT200 test, background", 100, -5.0, 5.0);
    //TH1D* hSig = new TH1D("hSig", "BDT200 training, signal", 100, -5.0, 5.0);
    //TH1D* hBkg = new TH1D("hBkg", "BDT200 training, background", 100, -5.0, 5.0);

    TList* hList = new TList();      // list of histograms to store
    hList->Add(hSig);
    hList->Add(hBkg);

  // Set up the OutFunc object.  First argument must be one of the classifiers.
  // 4th argument is offset for contour.
  // 5th argument is bool array indicating which variables were used in training

    std::string dir    = "../train/dataset/weights/";
    std::string prefix = "tmvaTest";
    const double tCut = 0.;
    std::vector<bool> useVar(3);
    useVar[0] = true;      // x
    useVar[1] = true;      // y
    useVar[2] = true;      // z
    //testStat = new OutFunc("Fisher", dir, prefix, tCut, useVar);
    //testStat = new OutFunc("MLP", dir, prefix, tCut, useVar);
    //testStat = new OutFunc("BDT200", dir, prefix, tCut, useVar);
    string name = "BDT" + to_string(N[z]);
    testStat = new OutFunc(name.c_str(), dir, prefix, tCut, useVar);

  // Open input file, get the TTrees, put into a vector

    TFile* inputFile = new TFile("../generate/testData.root");
    //TFile* inputFile = new TFile("../generate/trainingData.root");
    inputFile->ls();
    TTree* sig = dynamic_cast<TTree*>(inputFile->Get("sig"));
    TTree* bkg = dynamic_cast<TTree*>(inputFile->Get("bkg"));
    std::vector<TTree*> treeVec;
    treeVec.push_back(sig);
    treeVec.push_back(bkg);

  // Loop over TTrees, i=0 is signal, i=1 is background

    int nSig, nBkg;
    int nSigAcc = 0;
    int nBkgAcc = 0;
    int nAcc = 0;
    int error = 0;
    //double cut = 0.5;
    double cut = 0.;
    for (int i=0; i<treeVec.size(); i++){

      treeVec[i]->Print();
      Event evt;
      treeVec[i]->SetBranchAddress("evt", &evt);
      int numEntries = treeVec[i]->GetEntries();
      if ( i == 0 ) { nSig = numEntries; }
      if ( i == 1 ) { nBkg = numEntries; }
      std::cout << "number of entries = " << numEntries << std::endl;

  // Loop over events.

      for (int j=0; j<numEntries; j++){

        treeVec[i]->GetEntry(j);                   // sets evt
        double t = testStat->val(&evt);            // evaluate test statistic
        // cout << t << " " << evt.x << "  " << evt.y << endl;
        
        if ( i == 0 ){                       // signal
          hSig->Fill(t);
          if ( t >= cut ) { nSigAcc++; }
          if ( t < cut ) { error++; }
        }
        else if ( i == 1 ){                  // background
          hBkg->Fill(t);
          if ( t >= cut ) { nBkgAcc++; error++;}
        }

        // Add your code here to cound number of times that t is greater
        // or less than a given threshold for each hypothesis; from these
        // compute below the fraction of events with t >= tCut for each class.

      }

    }

  //  Compute efficiencies (power, size) 

    double epsSig = static_cast<double>(nSigAcc)/static_cast<double>(nSig);
    cout << "nSigAcc, nSig = " << nSigAcc << " , " << nSig << endl;
    cout << "signal efficiency (power) = " << epsSig << endl << endl;

    double epsBkg = static_cast<double>(nBkgAcc)/static_cast<double>(nBkg);
    cout << "nBkgAcc, nBkg = " << nBkgAcc << " , " << nBkg << endl;
    cout << "background efficiency (size) = " << epsBkg << endl << endl;

    double purity = static_cast<double>(nSigAcc)/static_cast<double>(nSigAcc+nBkgAcc);
    cout << "nSigAcc, nAcc = " << nSigAcc << " , " << nSigAcc+nBkgAcc << endl;
    cout << "signal purity = " << purity << endl << endl;

    double error_rate = static_cast<double>(error)/static_cast<double>(nSig + nBkg);
    cout << "nSig + nBkg, error = " << nSig + nBkg << " , " << error << endl;
    cout << "error rate = " << error_rate << endl << endl;
    
    out << N[z] << "\t" << error_rate << endl;

  // Close up

    inputFile->Close();
    histFile->Write();
//  }
  histFile->Close();
  out.close();
  return 0;

}
