#include<iostream>
#include<TH1F.h>
#include<math.h>
#include<TFile.h>
#include<TRandom3.h>


using namespace std;

double f(double x, double xi){
    return exp(-x/xi)/xi;
}

int main(){

    TFile *file = new TFile("file.root", "RECREATE");

    TH1F *unif = new TH1F("h_unif", "h_unif", 100, 0, 1);
    TH1F *exp = new TH1F("h_exp", "h_exp", 500, 0, 5);
    TH1F *exp_AR = new TH1F("h_exp_AR", "h_exp_AR", 500, 0, 5);

    int seed = 23;
    double xi = 0.5;
    TRandom3 *ran = new TRandom3(seed);
    int n = 0;
    int n_gen = 10000;

    for(int i = 0; i < n_gen; i++){
        unif->Fill(ran->Rndm());
        double r = ran->Rndm();
        exp->Fill(-xi*log(r));
        double x;
        double y;
        do{
            x = ran->Rndm() *5;
            y = ran->Rndm()/xi;
            n++;
        }while(y > f(x,xi));
        exp_AR->Fill(x);
   }
    unif->Scale(1/(unif->Integral()*unif->GetBinWidth(0)));
    exp->Scale(1/(exp->Integral()*exp->GetBinWidth(0)));
    exp_AR->Scale(1/(exp_AR->Integral()*exp_AR->GetBinWidth(0)));
    cout << f(1,xi)<<endl;
    cout << "Accept/reject efficiency: " << n_gen/ static_cast<double>(n) << endl;
    file->Write();
    file->Close();

    return 0;
}