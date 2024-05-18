// A simple program to generate exponential random numbers and
// store them in a histogram; also optionally writes the individual
// values to a file.

// Glen Cowan
// RHUL Physics
// 2 December 2006

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cmath>

#include <TFile.h>
#include <TH1D.h>
#include <TRandom3.h>

using namespace std;

int main(int argc, char **argv) {

// Set up output file

  ofstream dataFile;
  dataFile.open("expData2.txt");
  if ( dataFile.fail() ) {
    cout << "Could not open output file -- exit program" << endl;
    exit(1);
  }

// Create a TRandom object to generate random numbers uniform in ]0,1]
// Use the "Marsenne Twister" algorithm of TRandom3

  int seed = 12345;
  TRandom* ran = new TRandom3(seed);
 
//  Fill with exponential random numbers.

  const double xi = 1.0;              // mean value of the exponential
  int numVal = 0;
  cout << "Enter number of values to generate: ";
  cin >> numVal;
/*
  for (int i = 0; i<numVal; ++i){
    double r = ran->Rndm();
    double x = - xi * log(r);
    dataFile << x << endl;
  }
*/
  double alfa = 0.2;
  double xi1 = 1.0, xi2 = 5.0;
  for (int i = 0; i<numVal; ++i){
    double r = ran->Rndm();
    if (r < alfa){
      r = ran->Rndm();
      double x = - xi1 * log(r);
      dataFile << x << endl;
    }
    else{
      r = ran->Rndm();
      double x = - xi2 * log(r);
      dataFile << x << endl;
    }
    
  }
// Save all histograms and close up.

  dataFile.close();

  return 0;

}
