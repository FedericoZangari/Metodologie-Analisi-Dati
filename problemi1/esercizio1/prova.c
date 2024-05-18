void prova(){
    gRandom->SetSeed(13);
    TH1F *h = new TH1F("h", "h", 5, -6,6);
    TH1F *h2 = new TH1F("h2", "h2", 10, -6,6);
    for(int i = 0; i < 100000; i++){
    h->Fill(gRandom->Gaus(0,1));
    h2->Fill(gRandom->Gaus(0,0.5));
    }  
    gStyle->SetOptFit(11111);
    h->Fit("gaus");
    h->Draw();
    
}