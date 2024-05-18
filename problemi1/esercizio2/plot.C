
{

  // Set some defaults
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetCanvasBorderMode(0); // turn off canvas borders

  TCanvas *c1 = new TCanvas("c1", "Canvas 1", 400, 10, 600, 700);
  c1->Divide(1,2);
  



  // Open the file and list its contents
  TFile* f = new TFile("file.root");
  f->ls();

  // Get a couple of histograms
  TH1F* h1 = (TH1F*)f->Get("h");
  TH1F* h2 = (TH1F*)f->Get("h_AR");

  // Create a canvas, divide into two windows, 
  


  c1->cd(1);
  h1->SetMinimum(0.0);
  h1->SetTitle("PDF trasformazione inversa");
  h1->GetXaxis()->SetTitleSize(0.07);
  h1->GetYaxis()->SetTitleSize(0.07);   
  h1->GetXaxis()->SetTitleOffset(0.6);   
  h1->GetYaxis()->SetTitleOffset(0.6);
  h1->GetXaxis()->SetLabelSize(0.05);
  h1->GetYaxis()->SetLabelSize(0.05);
  h1->SetXTitle("x");
  h1->SetYTitle("f(x)");
  
  h1->SetLineColor(kBlue);
  h1->Draw();


  c1->cd(2);
  h2->SetMinimum(0.0);
  h2->SetTitle("PDF accept-reject"); 
  h2->SetXTitle("x");
  h2->SetYTitle("f(x)");
  h2->SetLineColor(kRed);
  h2->GetXaxis()->SetTitleSize(0.07);
  h2->GetYaxis()->SetTitleSize(0.07);   
  h2->GetXaxis()->SetTitleOffset(0.6);   
  h2->GetYaxis()->SetTitleOffset(0.6);
  h2->GetXaxis()->SetLabelSize(0.05);
  h2->GetYaxis()->SetLabelSize(0.05);
  h2->Draw();


  
  

  c1->SaveAs("2plot.png");
  //c1->SaveAs("fit.png");
  
}
