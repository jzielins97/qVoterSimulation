void drawGraph(const char* iFile ="data.dat"){
  std::ifstream in(iFile);

  Float_t ip, im;
  Int_t n=0;
  Float_t p[200];
  Float_t m[200];

  if(in.is_open()){
    while(in>>ip){
      p[n] = ip;
      in>>im;
      m[n] = im;
      std::cout<<ip<<" "<<im<<std::endl;
      n++;
    }
  }else{
    std::cout<<"ERROR: no such file"<<std::endl;
    return;
  }

  TCanvas* c = new TCanvas("c","Testing for q=3",10,10,800,700);
  TGraph* gr = new TGraph(n,p,m);
  gr->GetXaxis()->SetTitle("p");
  gr->GetYaxis()->SetTitle("|<m>|");
  int q,N,R;
  std::cout<<"q=";
  std::cin>>q;
  std::cout<<"N=";
  std::cin>>N;
  std::cout<<"R=";
  std::cin>>R;
  gr->SetTitle(Form("q=%d, N=%d, R=%d",q,N,R));
  gr->SetLineColor(kBlue);
  gr->SetMarkerStyle(33);
  gr->SetMarkerColor(kBlue);
  gr->SetMarkerSize(2);
  gr->Draw("AP");

  c->Update();
  c->Draw();

  std::cout<<"Save graph?"<<std::endl;
  char ans;
  std::cin>>ans;
  if(ans == 'y'){
    c->SaveAs("graph.png");
  }
}

void setGraph(TGraph* gr, int q, int color, int style){
  gr->SetName(Form("gr_q%d",q));
  gr->GetXaxis()->SetTitle("p");
  gr->GetYaxis()->SetTitle("<m>");
  gr->SetTitle(Form("q=%d",q));
  gr->SetLineColor(color);
  gr->SetMarkerStyle(style);
  gr->SetMarkerColor(color);
  gr->SetMarkerSize(2);
}

void drawGraphs(){
  std::ifstream in;

  int N, R;

  Float_t ip, im;
  Int_t n[4] = {0,0,0,0};
  Float_t p[4][200];
  Float_t m[4][200];

  const char* iFile[4] = {"./output/60-40/data_q3.dat",
                          "./output/60-40/data_q4.dat",
                          "./output/60-40/data_q5.dat",
                          "./output/60-40/data_q6.dat"};

  int grColor[4] = {1,2,3,4}; //black, red, green, blue
  int grMarkerStyle[4] = {20, 21, 29, 33}; //circle, square, star, diamond

  TMultiGraph *mg = new TMultiGraph();
  TGraph* gr[4];

  auto legend = new TLegend(0.8,0.9,0.95,0.75);

  for(int i=0; i<4; i++){
    in.open(iFile[i]);
    if(in.is_open()){
      while(in>>ip){
        p[i][n[i]] = ip;
        in>>im;
        m[i][n[i]] = im;
        std::cout<<ip<<" "<<im<<std::endl;
        n[i]++;
      }
    }else{
      std::cout<<"ERROR: no such file"<<std::endl;
      return;
    }
    in.close();

    gr[i] = new TGraph(n[i],p[i],m[i]);
    setGraph(gr[i], i+3, grColor[i], grMarkerStyle[i]);
    legend->AddEntry(gr[i],Form("q=%d",i+3),"lp");
    mg->Add(gr[i]);
  }

  TCanvas* c = new TCanvas("c","Drawing multigraph",10,10,900,850);
  c->SetRightMargin(0.05);
  std::cout<<"N=";
  std::cin>>N;
  std::cout<<"R=";
  std::cin>>R;
  mg->SetTitle(Form("<m> vs p (N=%d, R=%d);p;<m>", N, R));
  mg->GetXaxis()->SetRangeUser(0.0,0.7);
  mg->Draw("ALP");
  legend->Draw();

  c->SaveAs("multigraph.png");

}
