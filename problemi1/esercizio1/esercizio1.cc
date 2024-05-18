#include<iostream>
#include<TH1F.h>
#include<math.h>
#include<TFile.h>
#include<TRandom3.h>


using namespace std;


int main(){

    TFile *file = new TFile("file.root", "RECREATE");

    

    TH1F *a = new TH1F("a", "a", 100, -1, 1);
    TH1F *b = new TH1F("b", "b", 200, -2, 2);
    TH1F *c = new TH1F("c", "c", 600, -6, 6);

    int seed = 23;
    
    TRandom3 *ran = new TRandom3(seed);

    int n_gen = 1000000;

    for(int i = 0; i < n_gen; i++){
        double r;
        double x_a=0, x_b=0, x_c=0;
        for(int j = 0; j <12; j++){
            r = ran->Rndm();
            if(j<2) x_a += r;
            if(j<4) x_b += r;
            x_c += r;
        }

        a->Fill(x_a-1);
        b->Fill(x_b-2);
        c->Fill(x_c-6);
   }
    a->Scale(1/(a->Integral()*a->GetBinWidth(0)));
    b->Scale(1/(b->Integral()*b->GetBinWidth(0)));
    c->Scale(1/(c->Integral()*c->GetBinWidth(0)));

    file->Write();
    file->Close();

    return 0;
}