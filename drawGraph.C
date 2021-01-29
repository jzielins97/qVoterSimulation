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
  gr->GetYaxis()->SetTitle("<m>");
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
    
      
    
