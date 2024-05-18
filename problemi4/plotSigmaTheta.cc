{

  gROOT->SetStyle("Plain");

  gROOT->ForceStyle();
TCanvas* canvas = new TCanvas("c", "Canvas 1", 10, 10, 800, 600);
    canvas->SetFillColor(0);
  

  // Open the file and list its contents, get the Ttrees
  ifstream f1;
  f1.open("sigma_theta.txt",ios::in);

  TGraph *f= new TGraph();
  int i = 0;
  int n;
  double s;

  for (;;){
        f1 >> n >> s;
        f->SetPoint(i, n, s);
        
        i++;
        if(f1.eof()){break;
		}		
	}
  // Create canvas and use implicitly associated 
  // histogram to set min/max values and set axis labels.
  
  
  f->GetXaxis()->SetTitle("Sample size");
  f->GetYaxis()->SetTitle("#sigma_{#hat{#theta}}");
  f->SetTitle("Standard deviation of #hat{#theta}");
  f->SetLineStyle(1);
  f->SetLineWidth(3);
  f->SetLineColor(kBlue);
  TAxis* xa = f->GetXaxis();
  TAxis* ya = f->GetYaxis();
  xa->SetTitleOffset(1.2);    //  factor multiplies default offset
    ya->SetTitleOffset(0.9);
  ya->SetTitleSize(0.065);
  //f->SetMinimum(0.07);
    f->SetMarkerStyle(20);
    
    
  f->SetMarkerSize(1);
 
  f->SetMarkerColor(kBlue);
  f->Draw();

  

  canvas->SaveAs("sigma_theta.png");
}