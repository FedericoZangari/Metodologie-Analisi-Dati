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
  f1.open("BDT_error_test.dat",ios::in);
  f2.open("BDT_error_train.dat",ios::in);

  TGraph *f= new TGraph();
  TGraph *g= new TGraph();
  int i = 0;
  int N;
  double err;

  for (;;){
        f1 >> N >> err;
        f->SetPoint(i, N, err);
        f2 >> N >> err;
        g->SetPoint(i, N, err);
        
        i++;
        if(f1.eof()){break;
		}		
	}
  // Create canvas and use implicitly associated 
  // histogram to set min/max values and set axis labels.
  TCanvas* c = new TCanvas("c", "Canvas 1", 10, 10, 800, 600);
  
  f->GetXaxis()->SetTitle("N_Boost");
  f->GetYaxis()->SetTitle("Error rate");
  f->SetTitle("BDT error rate");
  f->SetLineStyle(1);
  f->SetLineWidth(3);
  f->SetLineColor(kBlue);
  g->SetLineStyle(2);
  g->SetLineWidth(3);
  g->SetLineColor(kRed);
  TAxis* xa = f->GetXaxis();
  TAxis* ya = f->GetYaxis();
  xa->SetTitleOffset(1.2);    //  factor multiplies default offset
  ya->SetTitleOffset(1.6);
  double maxSig = f->GetMinimum();
  double maxBkg = g->GetMinimum();
  double maxVal = std::min(maxSig, maxBkg) *0.1;
  f->SetMinimum(0.07);

  gPad->SetLogx(1);
  f->Draw();
  g->Draw("same");

  TLegend* leg = new TLegend(0.5, 0.72, 0.9, 0.88); // x1, y1, x2, y2
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(f, "  test sample", "l");
  leg->AddEntry(g, "  training sample", "l");
  leg->Draw();

  c->SaveAs("BDT_error.png");
}
