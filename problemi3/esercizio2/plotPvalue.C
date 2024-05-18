{

  // Set some defaults
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetCanvasBorderMode(0); // turn off canvas borders
  gStyle->SetOptStat(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFont(42, "h");        // for histogram title
  gStyle->SetTitleFont(42, "xyz");      // for axis labels
  gStyle->SetLabelFont(42, "xyz");      // for axis values
  gStyle->SetTitleSize(0.04, "h");
  gStyle->SetTitleSize(0.04, "xyz");
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.08);
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetTitleAlign(33);
  gStyle->SetTitleX(0.55);

  gROOT->ForceStyle();

  // Open the file and list its contents, get the Ttrees
  ifstream f1, f2;
  f1.open("pValue.txt",ios::in);

  TGraph *f= new TGraph();
  int i = 0;
  int npar;
  double p;

  for (;;){
        f1 >> npar >> p;
        f->SetPoint(i, npar-1, p);
        
        i++;
        if(f1.eof()){break;
		}		
	}
  // Create canvas and use implicitly associated 
  // histogram to set min/max values and set axis labels.
  TCanvas* c = new TCanvas("c", "Canvas 1", 10, 10, 800, 600);
  
  f->GetXaxis()->SetTitle("Order of polynomial");
  f->GetYaxis()->SetTitle("p-value");
  f->SetTitle("p-Value");
  f->SetLineStyle(1);
  f->SetLineWidth(3);
  f->SetLineColor(kBlue);
  TAxis* xa = f->GetXaxis();
  TAxis* ya = f->GetYaxis();
  xa->SetTitleOffset(1.2);    //  factor multiplies default offset
  ya->SetTitleOffset(1.6);
  //f->SetMinimum(0.07);

  
  f->Draw();

  

  c->SaveAs("pValue.png");
}
