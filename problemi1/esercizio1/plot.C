
{

  // Set some defaults
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetCanvasBorderMode(0); // turn off canvas borders

  TCanvas *c1 = new TCanvas("c1", "Canvas 1", 400, 10, 600, 700);
  c1->Divide(1,3);
  



  // Open the file and list its contents
  TFile* f = new TFile("file.root");
  f->ls();

  // Get a couple of histograms
  TH1F* h1 = (TH1F*)f->Get("a");
  TH1F* h2 = (TH1F*)f->Get("b");
  TH1F* h3 = (TH1F*)f->Get("c");

  // Create a canvas, divide into two windows, 
  


  c1->cd(1);
  h1->SetMinimum(0.0);
  h1->SetTitle("PDF a");
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
  h2->SetTitle("PDF b"); 
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


  
  c1->cd(3);
  gStyle->SetOptFit(11111);
  h3->SetMinimum(0.0);
  h3->SetTitle("PDF c"); 
  h3->SetXTitle("x");
  h3->SetYTitle("f(x)");
  h3->SetLineColor(kGreen);
  h3->GetXaxis()->SetTitleSize(0.07);
  h3->GetYaxis()->SetTitleSize(0.07);   
  h3->GetXaxis()->SetTitleOffset(0.6);   
  h3->GetYaxis()->SetTitleOffset(0.6);
  h3->GetXaxis()->SetLabelSize(0.05);
  h3->GetYaxis()->SetLabelSize(0.05);
  h3->Fit("gaus","L");
  h3->Draw();

  c1->SaveAs("3plot.png");
  //c1->SaveAs("fit.png");
  
}
