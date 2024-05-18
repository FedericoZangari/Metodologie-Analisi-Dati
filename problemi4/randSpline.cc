void randSpline()
{
    // making vectors
    double xVector[5] = {0, 1, 2, 3, 4};
    double yVector[5] = {27, 25, 15, 18, 22};

    // filling vectors with random numbers
    
    // making graph
	TGraph *gr = new TGraph(5, xVector, yVector);

    //Plot
    TCanvas *c1 = new TCanvas("c1","My Awesome Test Graph!!",200,10,700,500);
    gr->SetMarkerSize(2);
    gr->SetMarkerColor(kBlue);
    gr->SetMarkerStyle(1);
    gr->Draw("ap*c");

// Draw the Cubic Spline to the same canvas
   TSpline3 *sp = new TSpline3("Cubic Spline", gr,"b2e2", 0, 0);
   
   sp->SetLineColor(kRed);
   sp->Draw("lsame");

    c1->Update();
}