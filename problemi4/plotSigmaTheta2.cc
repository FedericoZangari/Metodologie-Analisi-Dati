{

  gROOT->SetStyle("Plain");

  gROOT->ForceStyle();
  gStyle->SetPadLeftMargin(0.15);
TCanvas* canvas = new TCanvas("c", "Canvas 1", 10, 10, 800, 600);
    canvas->SetFillColor(0);
  

  // Open the file and list its contents, get the Ttrees
  ifstream f1;
  f1.open("sigma_theta2.txt",ios::in);

  TGraphErrors *f= new TGraphErrors();
  int i = 0;
  int n;
  double t;
  double s;

  for (;;){
        f1 >> n >> t >> s;
        f->SetPoint(i, n, t);
        f->SetPointError(i, 0., s);
        i++;
        if(f1.eof()){break;
		}		
	}
  // Create canvas and use implicitly associated 
  // histogram to set min/max values and set axis labels.
  
  
  f->GetXaxis()->SetTitle("Number of free parameters");
  f->GetYaxis()->SetTitle("#hat{#theta}");
  f->SetTitle("Estimators of #hat{#theta}");
  f->SetLineStyle(1);
  f->SetLineWidth(3);
  TAxis* xa = f->GetXaxis();
  TAxis* ya = f->GetYaxis();
  xa->SetTitleOffset(1.2);    //  factor multiplies default offset
    ya->SetTitleOffset(0.9);
  ya->SetTitleSize(0.065);
  xa->SetNdivisions(4);
  //f->SetMinimum(0.07);
    f->SetMarkerStyle(20);
    
    
  f->SetMarkerSize(1);
 
  f->SetMarkerColor(kBlue);
  f->Draw("ap");

  

  canvas->SaveAs("sigma_theta2.png");
}