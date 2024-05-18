//  tmvaTrain.cc P. Manek and G. Cowan, 2019
//  simple program to train TMVA classifier (here, Fisher)

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <TFile.h>
#include <TString.h>

#include <TMVA/DataLoader.h>
#include <TMVA/Factory.h>

using namespace std;

int main(){
  std::vector<int> N = {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 10000, 50000};

  for(int i = 0; i < N.size(); i++){
    cout << N[i] << endl;
    string name1 = "BDT" + to_string(N[i]);
    string name2 = "NTrees="+ to_string(N[i]) + ":BoostType=AdaBoost";
  // Create ouput file, factory object and open the input file

    TFile* outputFile = TFile::Open("TMVA.root", "RECREATE");
    TMVA::Factory* factory = new TMVA::Factory("tmvaTest", outputFile, "");
    TMVA::DataLoader* dataLoader = new TMVA::DataLoader("dataset");
    TFile* trainingFile = new TFile("../generate/trainingData.root");
    TFile* testFile = new TFile("../generate/testData.root");

  // get the TTree objects from the input files

    TTree* sigTrain = (TTree*)trainingFile->Get("sig");
    TTree* bkgTrain = (TTree*)trainingFile->Get("bkg");
    int nSigTrain = sigTrain->GetEntries();
    int nBkgTrain = bkgTrain->GetEntries();

    TTree* sigTest = (TTree*)testFile->Get("sig");
    TTree* bkgTest = (TTree*)testFile->Get("bkg");
    int nSigTest = sigTest->GetEntries();
    int nBkgTest = bkgTest->GetEntries();

  // global event weights (see below for setting event-wise weights)

    double sigWeight = 1.0;
    double bkgWeight = 1.0;     //si possono ripesare gli eventi, lo si fa p.es per far si che il monte carlo segua meglio la distribuzione dei dati
    dataLoader->AddSignalTree(sigTrain, sigWeight, TMVA::Types::kTraining);
    dataLoader->AddBackgroundTree(bkgTrain, bkgWeight, TMVA::Types::kTraining);
    dataLoader->AddSignalTree(sigTest, sigWeight, TMVA::Types::kTesting);
    dataLoader->AddBackgroundTree(bkgTest, bkgWeight, TMVA::Types::kTesting);

  // Define the input variables that shall be used for the MVA training
  // (the variables used in the expression must exist in the original TTree).

    dataLoader->AddVariable("x", 'F');    //sono di tipo float
    dataLoader->AddVariable("y", 'F');
    dataLoader->AddVariable("z", 'F');

  // Book MVA methods (see TMVA manual).

    //factory->BookMethod(dataLoader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher");

    //factory->BookMethod(dataLoader, TMVA::Types::kMLP, "MLP", "H:!V:HiddenLayers=3");

    
      
      factory-> BookMethod(dataLoader, TMVA::Types::kBDT, name1.c_str(), name2.c_str());
    // Train, test and evaluate all methods

      factory->TrainAllMethods();
      factory->TestAllMethods();
      factory->EvaluateAllMethods();
    

  // Save the output and finish up

    outputFile->Close();
    std::cout << "==> wrote root file TMVA.root" << std::endl;
    std::cout << "==> TMVAnalysis is done!" << std::endl;

    delete factory;
    delete dataLoader;
  }
  return 0;

}
