#include<iostream>
#include<TH1F.h>
#include<math.h>
#include<TFile.h>
#include<TRandom3.h>


using namespace std;

double f(double x, double x_max){
    return 2.*x/x_max/x_max;
}

int main(){

    TFile *file = new TFile("file.root", "RECREATE");

    double x_max = 1;

    TH1F *h = new TH1F("h", "h", 100, 0, x_max);
    TH1F *h_AR = new TH1F("h_AR", "Accept-Reject Method", 100, 0, x_max);

    int seed = 23;
    
    TRandom3 *ran = new TRandom3(seed);
    int n = 0;
    int n_gen = 100000;

    for(int i = 0; i < n_gen; i++){
        
        double r = ran->Rndm();
        h->Fill(sqrt(r)*x_max);
        double x;
        double y;
        do{
            x = ran->Rndm() *x_max;
            y = ran->Rndm()*2./x_max;
            n++;
        }while(y > f(x,x_max));
        h_AR->Fill(x);
   }
    h->Scale(1/(h->Integral()*h->GetBinWidth(0)));
    h_AR->Scale(1/(h_AR->Integral()*h_AR->GetBinWidth(0)));
    cout << "Accept/reject efficiency: " << n_gen/ static_cast<double>(n) << endl;
    file->Write();
    file->Close();

    return 0;
}