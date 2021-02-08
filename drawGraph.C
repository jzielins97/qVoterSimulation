void drawGraph(const char* iFile ="data.dat", int color = 4, int markerStyle = 33){
  std::ifstream in;

  Float_t ip, im;
  Int_t n=0;
  Float_t p[200];
  Float_t m[200];

  in.open(iFile);
  if(in.is_open()){
    while(in>>ip){
      p[n] = ip;
      in>>im;
      m[n] = im;
      //std::cout<<ip<<" "<<im<<std::endl;
      n++;
    }
  }else{
    std::cout<<"ERROR: no such file"<<std::endl;
    return;
  }

  TCanvas* c = new TCanvas("c","Testing for q=3",10,10,800,700);
  TGraph* gr = new TGraph(n,p,m);
  gr->GetXaxis()->SetTitle("p");
  gr->GetYaxis()->SetTitle("<|m|>");
  int q,N,R;
  std::cout<<"q=";
  std::cin>>q;
  std::cout<<"N=";
  std::cin>>N;
  std::cout<<"R=";
  std::cin>>R;
  gr->SetTitle(Form("q=%d, N=%d, R=%d",q,N,R));
  gr->SetLineColor(kBlue);
  gr->SetMarkerStyle(markerStyle);
  gr->SetMarkerColor(color);
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
  gr->GetYaxis()->SetTitle("<|m|>");
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

  const char* iFile[4] = {"./output/95-05/data_q3.dat",
                          "./output/95-05/data_q4.dat",
                          "./output/95-05/data_q5.dat",
                          "./output/95-05/data_q6.dat"};

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
        //std::cout<<ip<<" "<<im<<std::endl;
        n[i]++;
      }
    }else{
      std::cout<<"ERROR: no such file"<<std::endl;
      return;
    }
    in.close();

    gr[i] = new TGraph(n[i],p[i],m[i]);
    setGraph(gr[i], i+3, grColor[i], grMarkerStyle[i]);
    legend->AddEntry(gr[i],Form("q=%d",i+3),"p");
    mg->Add(gr[i]);
  }

  TCanvas* c = new TCanvas("c","Drawing multigraph",10,10,900,850);
  c->SetRightMargin(0.05);
  std::cout<<"N=";
  std::cin>>N;
  std::cout<<"R=";
  std::cin>>R;
  mg->SetTitle(Form("<|m|> vs p (N=%d, R=%d);p;<|m|>", N, R));
  mg->GetXaxis()->SetRangeUser(0.0,0.7);
  mg->Draw("AP");
  legend->Draw();

  c->SaveAs("multigraph.png");
}

void histereza(int q, float p_min = 0.0, float p_max = 0.7){

  std::ifstream iFile1;
  std::ifstream iFile2;

  Float_t p1[200], p2[200];
  Float_t m1[200], m2[200];

  Int_t n1 = 0, n2 = 0;

  Float_t im, ip;

  int color1;
  int color2;

  int markerStyle1;
  int markerStyle2;

  TMultiGraph* mg = new TMultiGraph();
  auto legend = new TLegend(0.65,0.9,0.95,0.75);
  TGraph* gr1;
  TGraph* gr2;

  TCanvas* c;

  switch(q){
    case 3:
      color1 = 1;
      color2 = 9;
      markerStyle1 = 20;
      markerStyle2 = 24;
      break;
    case 4:
      color1 = 2;
      color2 = 6;
      markerStyle1 = 21;
      markerStyle2 = 25;
      break;
    case 5:
      color1 = 3;
      color2 = 8;
      markerStyle1 = 29;
      markerStyle2 = 30;
      break;
    case 6:
      color1 = 4;
      color2 = 7;
      markerStyle1 = 33;
      markerStyle2 = 27;
      break;
  }

  iFile1.open(Form("./output/50-50/data_q%d.dat",q));
  if(iFile1.is_open()){
    while(iFile1>>ip){
      p1[n1] = ip;
      iFile1>>im;
      m1[n1] = abs(im);
      //std::cout<<"1: "<<ip<<" "<<im<<std::endl;
      n1++;
    }
  }else{
    std::cout<<"ERROR: no file "<<Form("./output/50-50/data_q%d.dat",q)<<std::endl;
    return;
  }
  iFile1.close();

  iFile2.open(Form("./output/95-05/data_q%d.dat",q));
  if(iFile2.is_open()){
    while(iFile2>>ip){
      p2[n2] = ip;
      iFile2>>im;
      m2[n2] = abs(im);
      //std::cout<<ip<<" "<<im<<std::endl;
      n2++;
    }
  }else{
    std::cout<<"ERROR: no file"<<Form("./output/95-05/data_q%d.dat",q)<<std::endl;
    return;
  }
  iFile2.close();

  gr1 = new TGraph(n1,p1,m1);
  setGraph(gr1, q, color1, markerStyle1);
  gr1->SetTitle(Form("q=%d, losowa;p;<|m|>",q));
  gr1->SetName(Form("gr1_q%d",q));

  gr2 = new TGraph(n2,p2,m2);
  setGraph(gr2, q, color2, markerStyle2);
  gr2->SetTitle(Form("q=%d, spolaryzowana;p;<|m|>",q));
  gr2->SetName(Form("gr2_q%d",q));

  mg->Add(gr1);
  legend->AddEntry(gr1,gr1->GetTitle(),"p");
  mg->Add(gr2);
  legend->AddEntry(gr2,gr2->GetTitle(),"p");

  c = new TCanvas("c","Drawing multigraph",10,10,900,850);
  c->SetRightMargin(0.05);

  mg->SetTitle("Przejscie z losowa lub spolaryzowana magnetyzacja;p;<|m|>");
  mg->GetXaxis()->SetRangeUser(p_min,p_max);

  mg->Draw("AP");
  legend->Draw();

  c->SaveAs(Form("histereza_q%d.png",q));

}
