#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>

#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TSystem.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include  "time.h"
#include  "TGaxis.h"

#include "leggifile.cc"
#include "functionch.cc"




int main(){
   gROOT->Reset();
   gROOT->SetBatch(kTRUE);
 
    std::string DirData;
    
    getline(cin, DirData);
    
    /*for (DirData; std::getline(std::cin, DirData);) {
        std::cout << DirData << std::endl;
    }*/
    
  gSystem->Exec(("mkdir "+DirData+"/Plot").c_str());

  
  gSystem->Exec(("mkdir "+DirData+"/Plot/PlotEnergy").c_str());
  gSystem->Exec(("mkdir "+DirData+"/Plot/TimeVsTemp").c_str());
  gSystem->Exec(("mkdir "+DirData+"/Plot/PlotSigma").c_str());
 gSystem->Exec(("mkdir "+DirData+"/PlotSigma").c_str());
  gSystem->Exec(("mkdir "+DirData+"/PlotSigma/PlotTimeTemp").c_str());
 gSystem->Exec(("mkdir "+DirData+"/PlotMean").c_str());
 gSystem->Exec(("mkdir "+DirData+"/PlotEntr").c_str());
    
    
    
 vector<string> FileList;
  string Filename= DirData+"/Filelist.txt";
    std::cout << "Lista File Pedestal: "<< Filename<< std::endl;
  FileList=ReadData(Filename);


  Int_t Nch=350, size;
    Long64_t time1, time2, timeini;
    Double_t MeanTemp59, MeanTemp315, sigma59, sigma315, MeanPedCh59, MeanPedCh315, Mean2PedCh59, Mean2PedCh315, ErrMean59, ErrMean315;
   bool printFileList= true;
  bool printOpenedFileNumber= true;

  if(printFileList){
    
    for(int i=0;i<(int)FileList.size();i++){
      std::cout<<"File " << i << "   " << (DirData+"/"+FileList.at(i)).c_str() << std::endl;
    }//chiudo for
    
  }//chiudo if
 
  TFile* DataFile[2];
  TTree* DataTree[2];
  //TH1F* HistoT[(int)FileList.size()][Nch];
  //TH1F* Histo[(int)FileList.size()][Nch];
    
    
    
    
    
    
    size=(int)FileList.size();

    Double_t MeanValue[size][Nch];
    Double_t MeanValuePre[Nch];
    Double_t MeanValueAfter[Nch];
  
    Double_t MeanFit[size][Nch];
    Double_t MeanFitPre[Nch];
    Double_t MeanFitAfter[Nch];


    Double_t sigma[size][Nch];
    Double_t sigmaPre[Nch];
    Double_t sigmaAfter[Nch];


    double  NentrAppPre[Nch];
    double  NentrAppAft[Nch];
    double  Nentr[size][Nch];
    
    
    Double_t MeanTemp[size][Nch];
    Double_t MeanTempPre[Nch];
    Double_t MeanTempAfter[Nch];
    
    
    
    TGraph *t[Nch];
    TGraph *t2[Nch];
    TGraph *s[Nch];
    TGraph *stmp[Nch] ;
    TGraph *g[Nch] ;
    TGraph *gt[Nch] ;
    TGraph *entries[Nch];
    TGraph *fits[Nch] ;
    TGraph *fitstmp[Nch] ;
    TGraphErrors *meant[Nch];
    TGraphErrors *meantemp[Nch];
    
    TCanvas* canvTempMean = new TCanvas("MeanVsTemp","MeanVsTemp",1200,600);
        canvTempMean->Divide(2,1);

	TCanvas* canvTimeCH = new TCanvas("ChVsTime","ChVsTime",1200,600);
	canvTimeCH->Divide(2,1);
	TCanvas* BrokenCH = new TCanvas("Broken","Broken",1200,600);
	//canvTimeCH->Divide(2,1);
       TCanvas* canvEntries[Nch];//= new TCanvas("Nentries", "Nentries", 1200,600);

 TMultiGraph *tm = new TMultiGraph();
 TMultiGraph *tm2 = new TMultiGraph();
 
 TMultiGraph *ch= new TMultiGraph();
//TMultiGraph *entr= new TMultiGraph();
    

        
    
    DataFile[0]=TFile::Open((DirData+"/"+FileList.at(0)).c_str());
    if(printOpenedFileNumber) std::cout << "FileOpened: " << std::endl;
    DataTree[0] = (TTree*)DataFile[0]->Get("data");
    timeini=GetTime(DataTree[0]);



    for(int i=0; i<size; i++){
     for(int k=0; k<Nch; k++){
         MeanValue[i][k]=0;
         MeanTemp[i][k]=0;
     }
    }
         
    
    for(int k=0; k<Nch; k++){
        g[k] = new TGraph(size);
	t[k] = new TGraph;
	t2[k] = new TGraph;
	entries[k] = new TGraph;
	 canvEntries[k]= new TCanvas(("EntriechNum"+to_string(k)).c_str(), ("EntriechNum"+to_string(k)).c_str(), 1200,600);

        
    }
    
    for(int i=0;i<(int)FileList.size()-1;i+=2){
        
        for(int j=0; j<2;j++){
            DataFile[j]=TFile::Open((DirData+"/"+FileList.at(i+j)).c_str());
            if(printOpenedFileNumber) std::cout << "FileOpened: " << i+j << std::endl;
            DataTree[j] = (TTree*)DataFile[j]->Get("data");
        }//chiudo for j
        
        
        GetPedestal(DataTree[0], MeanValuePre, NentrAppPre, MeanFitPre, sigmaPre);
        GetPedestal(DataTree[1], MeanValueAfter, NentrAppAft, MeanFitAfter, sigmaAfter);
        GetTemperature(DataTree[0], MeanTempPre);
        GetTemperature(DataTree[1], MeanTempAfter);
	time1=GetTime(DataTree[0]);
	time1-=timeini;
        time2=GetTime(DataTree[1]);
	time2-=timeini;
        
        for(int k=0; k<Nch; k++){


		Nentr[i][k]=NentrAppPre[k];
		Nentr[i+1][k]=NentrAppPre[k];
   		entries[k]-> SetPoint(i, time1, Nentr[i][k]);
	        entries[k]-> SetPoint(i+1, time2, Nentr[i+1][k]);
		//std::cout<<k<<Nentr[i][k]<<std::endl;
                
            if (MeanValuePre[k]!=0 &&MeanValueAfter[k] != 0){
                MeanValue[i][k]= (MeanValuePre[k]+MeanValueAfter[k])/2;
                MeanTemp[i][k]= (MeanTempPre[k]+MeanTempAfter[k])/2;
		MeanFit[i][k]= (MeanFitPre[k]+MeanFitAfter[k])/2;
		sigma[i][k]= sqrt(sigmaPre[k]);
		
		//sigma[i][k]= sigmaPre[k];
            }
            
            
            if(MeanValue[i][k]!=0){
                g[k]-> SetPoint (i , MeanTemp[i][k] , MeanValue[i][k]);
		t[k]-> SetPoint (i, time1, k);
            }else if ( (k<64 )|| (k>255&&k<320)){
 		t2[k]-> SetPoint (i, time1, k);
			}
           
            
        }
        
        
    


	for(int j=0;j<2;j++){
            DataFile[j]->Close("R");
            std::cout << "FileClose: " << i+j << std::endl;
 	}// chiudo for j
        
    
    }//chiudo for i
    
    /*for(int k=0; k<Nch; k++){
        
        
        g[k] = new TGraph(size);
        
        
        
        for(int i=0; i<size; i+=2){
        
            
            if (MeanValuePre[k]!=0 &&MeanValueAfter[k] != 0){
                MeanValue[k]= (MeanValuePre[k]+MeanValueAfter[k])/2;
                MeanTemp[k]= (MeanTempPre[k]+MeanTempAfter[k])/2;
            }
            
            
            if(MeanValue[k]!=0){
                g[k]-> SetPoint (i , MeanTemp[k] , MeanValue[k]);
            }
            
            
            
            
            std::cout<<"Media= "<<MeanValue[k]<<std::endl;
        }
 
    }// chiudo for k*/
    


  for(int k=0; k<Nch;k++){
	
    if(k<64){
        canvTempMean->cd(1)->SetGrid();
        canvTempMean->cd(1);
        
        //g[k]->SetMaximum(37);
        //g[k]->SetMinimum(20);
        g[k]->SetTitle("TemperatureVsMean");
        g[k]->GetXaxis()->SetTitle("temperature [#circC]");
        g[k]->GetYaxis()->SetTitle("Mean");
        g[k]->GetXaxis()->SetLimits(24,37);
        g[k]->SetMarkerStyle(7);
        g[k]->SetMarkerColor(kBlack);
        tm->Add(g[k]);
        tm->SetMaximum(280);
        tm->SetMinimum(130);
        tm->SetTitle("TemperatureVsMean");
        tm->GetXaxis()->SetTitle("temperature [#circC]");
        tm->GetYaxis()->SetTitle("Mean");
        tm->GetXaxis()->SetLimits(24,37);
        tm->Draw("AP");
        
    } else if(k>255&&k<320){


        canvTempMean->cd(2)->SetGrid();
        canvTempMean->cd(2);
        //t[k+64]->SetMaximum(37);
        //t[k+64]->SetMinimum(20);
        g[k]->SetTitle("TemperatureVsMean");
        g[k]->GetXaxis()->SetLimits(24,37);
        g[k]->GetXaxis()->SetTitle("temperature [#circC]");
        g[k]->GetYaxis()->SetTitle("Mean");
        g[k]->SetMarkerStyle(7);
        g[k]->SetMarkerColor(kBlack);
        tm2->Add(g[k]);
        tm2->SetMaximum(240);
        tm2->SetMinimum(100);
        tm2->SetTitle("TemperatureVsMean");
        tm2->GetXaxis()->SetTitle("temperature [#circC]");
        tm2->GetYaxis()->SetTitle("Mean");
        tm2->GetXaxis()->SetLimits(24,37);
        tm2->Draw("AP");
        }
      
      }//chiudo for*/

    
    
  /*  canvTimeTemp->cd(1);
    g[0]->SetTitle("TimeVsTemperature channelID==59");
    g[0]->GetYaxis()->SetTitle("temperature [#circC]");
    g[0]->SetMarkerStyle(7);
    g[0]->SetMarkerColor(kBlack);
     g[0]->Draw("AP");*/
    canvTempMean->SaveAs((DirData+"/Plot/PlotEnergy/MeanVsTemp.png").c_str());


    for(int k=0; k<Nch;k++){
	if((k<64)||(k>255&&k<320)){
	entries[k]->SetMarkerStyle(7);
	entries[k]->SetMarkerColor(kRed);
	entries[k]->Draw("AP");
	canvEntries[k]->SaveAs((DirData+"/PlotEntr/EntriechNum"+to_string(k)+".png").c_str());
		}
	}


for(int k=0; k<Nch;k++){
	
    if(k<64){
        //canvTimeCH->cd(1)->SetGrid();
        canvTimeCH->cd(1);
        
        t[k]->SetMaximum(65);
        t[k]->SetMinimum(-2);
        t[k]->SetTitle("Allch");
        t[k]->GetXaxis()->SetTitle("time [s]");
        t[k]->GetYaxis()->SetTitle("ch");
        //t[k]->GetXaxis()->SetLimits(24,37);
        t[k]->SetMarkerStyle(7);
        t[k]->SetMarkerColor(kBlack);
	t2[k]->SetMarkerStyle(7);
	t2[k]->SetMarkerColor(kRed);
	if(k==0){
        t[k]->Draw("AP");
	t2[k]->Draw("SAMEP");
		}else {t[k]->Draw("SAMEP");
			t2[k]->Draw("SAMEP");}
         //ch->Add(t2[k]);
    } else if(k>255&&k<320){


       // canvTimeCH->cd(2)->SetGrid();
        canvTimeCH->cd(2);
        t[k]->SetMaximum(321);
        t[k]->SetMinimum(255);
        t[k]->SetTitle("Allch");
        //t[k]->GetXaxis()->SetLimits(24,37);
        t[k]->GetXaxis()->SetTitle("time [s]");
        t[k]->GetYaxis()->SetTitle("ch");
        t[k]->SetMarkerStyle(7);
        t[k]->SetMarkerColor(kBlack);
	t2[k]->SetMarkerStyle(7);
	t2[k]->SetMarkerColor(kRed);
        //ch->Add(t2[k]);
	if(k==256){
        t[k]->Draw("AP");
	t2[k]->Draw("SAMEP");
		}else {t[k]->Draw("SAMEP");
			t2[k]->Draw("SAMEP");}
        
        }
      
      }//chiudo for*/
canvTimeCH->SaveAs((DirData+"/Plot/PlotEnergy/chVstime.png").c_str());





for(int k=0; k<Nch;k++){
	
    if(k<64){
	ch->Add(t2[k]);
	}
}

BrokenCH->cd(0);
ch->SetMinimum(10);
ch->Draw("AP");
    BrokenCH->SaveAs((DirData+"/Plot/PlotEnergy/BrokenVstime.png").c_str());




}//chiudo analisi



/*g[k]->GetXaxis()->SetTitle("time[h]");
 g[k]->GetXaxis()->SetTimeDisplay(1);
 g[k]->GetXaxis()->SetTimeFormat("%I");*/


    /*g[k+64]->GetXaxis()->SetTitle("time[h]");
     g[k+64]->GetXaxis()->SetTimeDisplay(1);
     g[k+64]->GetXaxis()->SetTimeFormat("%I");*/



/*t[k] = new TGraph;
 s[k] = new TGraph;
 stmp[k] = new TGraph ;*/

/* gt[k] = new TGraph;
 fits[k] = new TGraph;
 fitstmp[k] = new TGraph;
 meant[k] = new TGraphErrors;
 meantemp[k] = new TGraphErrors;
 
 t[k+64] = new TGraph;
 s[k+64] = new TGraph;
 stmp[k+64] = new TGraph ;
 g[k+64] = new TGraph ;
 gt[k+64] = new TGraph;
 fits[k+64] = new TGraph;
 fitstmp[k+64] = new TGraph;
 meant[k+64] = new TGraphErrors;
 meantemp[k+64] = new TGraphErrors;
 
 
 
 
 
 stmp[k+64]->SetPoint(i, MeanTemp315, sigma315);
 
 
 
 
 MeanTemp59 =HistoT[i+1][k]->GetMean();
 MeanTemp315 =HistoT[i+1][k+64]->GetMean();
 sigma59=Histo[i+1][k]->GetRMS();
 sigma315=Histo[i+1][k+64]->GetRMS();
 
 
 
 Mean2PedCh59=Histo[i+1][k]->GetMean();
 Mean2PedCh315=Histo[i+1][k+64]->GetMean();
 
 Mean2PedCh59-=MeanPedCh59;
 Mean2PedCh315-=MeanPedCh315;
 
 MeanPedCh59=(Histo[i][k]->GetMean()+Histo[i+1][k]->GetMean())/2;
 MeanPedCh315=(Histo[i][k+64]->GetMean()+Histo[i+1][k+64]->GetMean())/2;
 MeanTemp59=(HistoT[i][k]->GetMean()+HistoT[i+1][k]->GetMean())/2;
 MeanTemp315=(HistoT[i][k+64]->GetMean()+HistoT[i+1][k+64]->GetMean())/2;
 
 
 ErrMean59=sqrt(Histo[i][k]->GetMeanError()*Histo[i][k]->GetMeanError()+Histo[i+1][k]->GetMeanError()*Histo[i+1][k]->GetMeanError());
 ErrMean315=sqrt(Histo[i][k+64]->GetMeanError()*Histo[i][k+64]->GetMeanError()+Histo[i+1][k+64]->GetMeanError()*Histo[i+1][k+64]->GetMeanError());
 
 
 // g[k]->SetPoint(i, time1 , MeanPedCh59);
 gt[k]->SetPoint(i, MeanTemp59, MeanPedCh59);
 // meant[k]->SetPoint(i, time1, Mean2PedCh59);
 meant[k]->SetPointError(i, 0, ErrMean59);
 meantemp[k]->SetPoint(i, MeanTemp59, Mean2PedCh59);
 meantemp[k]->SetPointError(i, 0, ErrMean59);
 
 
 // g[k+64]->SetPoint(i, time1 , MeanPedCh315);
 gt[k+64]->SetPoint(i, MeanTemp315, MeanPedCh315);
 //meant[k+64]->SetPoint(i, time1, Mean2PedCh315);
 meant[k+64]->SetPointError(i, 0, ErrMean315);
 meantemp[k+64]->SetPoint(i, MeanTemp315, Mean2PedCh315);
 meantemp[k+64]->SetPointError(i, 0, ErrMean315);*/
