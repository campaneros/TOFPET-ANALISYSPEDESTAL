void GetPedestal(TTree* tree, Double_t* pedchip, double* entries, Double_t* Mean, Double_t* sigma){
  
  Float_t energy;
  UShort_t chID;

    
    

  tree->SetBranchAddress("energy",&energy);
  tree->SetBranchAddress("channelID",&chID);
   
  /*for(int i=0; i<tree->GetEntries(); i++){
    tree->GetEntry(i);
    if(chID== k) {  histoCh59->Fill(energy);
      //  std::cout<<chID<<std::endl;
    }//chiudo if
    else if (chID==(64*4+k)){ histoCh315->Fill(energy); }//chiudo else
  }//chiudo for*/
    
    TH1F* Histo[350];
    TF1* gauss[350];
    
    for (int k=0; k<350; k++){
        Histo[k]= new TH1F (("histo"+to_string(k)).c_str(),("histo"+to_string(k)).c_str(), 600,0,600);
	gauss[k] = new TF1(("gauss"+to_string(k)).c_str(),"gaus",100,300);
    }
    
    for (int i=0; i<tree->GetEntries(); i++){
        tree->GetEntry(i);
        if (energy !=0) Histo[chID]->Fill(energy);
        
    }
        
        
        

    
    
    for (int k=0; k<350; k++){
        entries[k]=Histo[k]->GetEntries();
        if ((Histo[k]!=NULL) && (Histo[k]->GetEntries()>90)){
	Histo[k]->Fit(("gauss"+to_string(k)).c_str(), "N");
	Mean[k]=gauss[k]->GetParameter(1);
	sigma[k]=gauss[k]->GetParameter(2);
            pedchip[k]=Histo[k]->GetMean();}
        delete Histo[k];
    }
    
  
  std::cout << "Filled"<< std::endl;

  
}




void GetTemperature(TTree* tree, Double_t* pedtemp){
    
     UShort_t chID;
    Double_t temp2, temp3;
    
    tree->SetBranchAddress("temp2",&temp2);
    tree->SetBranchAddress("temp3",&temp3);
    tree->SetBranchAddress("channelID",&chID);
    
    TH1F* Histo[350];
    
    for (int k=0; k<350; k++){
        Histo[k]= new TH1F (("histot"+to_string(k)).c_str(),("histot"+to_string(k)).c_str(), 50,0,50);
    }
    
    for (int i=0; i<tree->GetEntries(); i++){
        tree->GetEntry(i);
        if (chID<64) {Histo[chID]->Fill(temp3);
            //std::cout<<temp3<<std::endl;
			}
        else  {Histo[chID]->Fill(temp2);}
    
    }
    std::cout<<"prova"<<std::endl;
    for (int k=0; k<350; k++){
        if ((Histo[k]!=NULL)&& (Histo[k]->GetEntries()>99)){
            pedtemp[k]=Histo[k]->GetMean();}
        delete Histo[k];
    }
    

    
    std::cout << "FilledTemp"<< std::endl;
    //std::cout<<"prova"<<std::endl;
    
    
}


/* // std::cout<<"ciao"<<std::endl;
 for(int i=0; i<tree->GetEntries(); i++){
 tree->GetEntry(i);
 if(chID==k) { histoCh59->Fill(temp3);
 //std::cout<<temp3<<std::endl;
 }//chiudo if
 else if (chID==(64*4+k)){ histoCh315->Fill(temp2);}//chiudo else
 
 }//chiudo for*/



long int GetTime(TTree* tree){
   Long64_t time1;
   tree->SetBranchAddress("unixTime", &time1);
   //std::cout<<"ciao"<<std::endl;
  // int j=0;
   tree->GetEntry(0);
   // if (1==0)
    std::cout<<time1<<std::endl;
  return time1;



}



