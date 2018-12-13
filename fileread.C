#include <iostream>
#include <fstream>
#include <string>
#include <TCanvas.h>
#include <TPie.h>
#include <TString.h>
#include <sstream>
#include <TLegend.h>
#include <TStyle.h>
using namespace std;
int fileread(int arc, char** arv)

{
        string fString,line;
        string line1,seedname,sign;
        int index;
        double prescale,rate,error,pure,prop;

        ifstream myfile;
        //string fileLocation;

        //opens the file
        //myfile.open("output_aloke.txt");
        myfile.open(arv[1]);

        //makes sure that the file opened properly
        while(myfile.fail())
        {
                cout <<  " failed to open" << endl;
        }
        bool pietable = false;
        int count=0;
        double murate=0,mupurerate=0,muproprate=0;
        double EGrate=0,EGpurerate=0,EGproprate=0;
        double Jetrate=0,Jetpurerate=0,Jetproprate=0;
        double Taurate=0,Taupurerate=0,Tauproprate=0;
        double Sumsrate=0,Sumspurerate=0,Sumsproprate=0;
        double MuEGrate=0,MuEGpurerate=0,MuEGproprate=0;
        double MuJetSumsrate=0,MuJetSumspurerate=0,MuJetSumsproprate=0;
        double MuTaurate=0,MuTaupurerate=0,MuTauproprate=0;
        double EGJetSumsrate=0,EGJetSumspurerate=0,EGJetSumsproprate=0;
        double EGTaurate=0,EGTaupurerate=0,EGTauproprate=0;
        double TauJetSumsrate=0,TauJetSumspurerate=0,TauJetSumsproprate=0;
        double Otherrate=0,Otherpurerate=0,Otherproprate=0;
        double ZeroBiasrate=0,ZeroBiaspurerate=0,ZeroBiasproprate=0;
        while(getline(myfile,line))
        {
                if(line.find("L1Bit")!=std::string::npos)
                  {
                   pietable = true;
                  }
                if(line.find("Total rate")!=std::string::npos)
                  {
                  pietable = false;
                  }
                 if(pietable)
                  {
                 if(line.find("L1Bit")!=std::string::npos) continue;
                 line1=line;
                 //cout<<line1<<endl;
                 stringstream aloke(line1);
                 aloke >> index >> seedname >> prescale >> rate /*>> sign*/ >> error >> pure >> prop ;
                 if(rate==0 && pure==0 && prop==0) continue;
                 //cout<<index<<" "<<seedname<<" "<<prescale<<" "<<rate<<" "<<error<<" "<<pure<<" "<<prop<<endl;
               //  cout<<index<<" "<<seedname<<" "<<prescale<<" "<<rate<<" "<<sign<<" "<<error<<" "<<pure<<" "<<prop<<endl;
                 bool issums=false, ismu=false, isjet=false, isEG=false,isTau=false,isZerobias=false; 
                 if(seedname.find("ETM")!=string::npos || seedname.find("HTT")!=string::npos || seedname.find("ETT")!=string::npos || seedname.find("ETMHF")!=string::npos)     issums=true;
                 if(seedname.find("Mu")!=string::npos) ismu=true;
                 if(seedname.find("EG")!=string::npos) isEG=true;
                 if(seedname.find("Jet")!=string::npos)isjet=true;
                 if(seedname.find("Tau")!=string::npos)isTau=true;
                 if(seedname.find("ZeroBias")!=string::npos && seedname.find("copy")==string::npos) isZerobias=true;
                 if(ismu && !isjet && !isEG && !isTau && !issums && !isZerobias)
                 {
                         murate=murate + rate;
                         mupurerate = mupurerate + pure;
                         muproprate = muproprate + prop;
                         cout<<seedname<<"  is Mu"<<endl;        
                  }
                 else if(!ismu && !isjet && isEG && !isTau && !issums && !isZerobias)
                 {
                         EGrate=EGrate + rate;
                         EGpurerate = EGpurerate + pure;
                         EGproprate = EGproprate + prop;
                         cout<<seedname<<"  is EG"<<endl;        
                  }
                 else if(!ismu && isjet && !isEG && !isTau && !issums && !isZerobias)
                 {
                         Jetrate=Jetrate + rate;
                         Jetpurerate = Jetpurerate + pure;
                         Jetproprate = Jetproprate + prop;
                         cout<<seedname<<"  is Jet"<<endl;        
                  }
                 else if(!ismu && !isjet && !isEG && isTau && !issums && !isZerobias)
                 {
                         Taurate=Taurate + rate;
                         Taupurerate = Taupurerate + pure;
                         Tauproprate = Tauproprate + prop;
                         cout<<seedname<<"  is Tau"<<endl;        
                  }
                 else if(!ismu && !isjet && !isEG && !isTau && issums && !isZerobias)
                  {
                       Sumsrate=Sumsrate + rate;
                       Sumspurerate = Sumspurerate + pure;
                       Sumsproprate = Sumsproprate + prop;
                         cout<<seedname<<"  is Sums"<<endl;        
                  }
                 else if(ismu && !isjet && isEG && !isTau && !issums && !isZerobias)
                  {
                       MuEGrate=MuEGrate + rate;
                       MuEGpurerate = MuEGpurerate + pure;
                       MuEGproprate = MuEGproprate + prop;
                         cout<<seedname<<"  is MuEG"<<endl;        
                  }
                 else if(ismu && (isjet || issums ) && !isEG && !isTau && !isZerobias)
                  {
                       MuJetSumsrate=MuJetSumsrate + rate;
                       MuJetSumspurerate = MuJetSumspurerate + pure;
                       MuJetSumsproprate = MuJetSumsproprate + prop;
                         cout<<seedname<<"  is Mu+Jet/Sums"<<endl;        
                  }
                 else if(ismu && !isjet && !isEG && isTau && !issums && !isZerobias)
                  {
                       MuTaurate=MuTaurate + rate;
                       MuTaupurerate = MuTaupurerate + pure;
                       MuTauproprate = MuTauproprate + prop;
                         cout<<seedname<<"  is Mu+Tau"<<endl;        
                  }
                 else if(!ismu  && isEG && !isTau && (isjet || issums) && !isZerobias)
                  {
                       EGJetSumsrate=EGJetSumsrate + rate;
                       EGJetSumspurerate = EGJetSumspurerate + pure;
                       EGJetSumsproprate = EGJetSumsproprate + prop;
                         cout<<seedname<<"  is EG+Jet/Sums"<<endl;        
                  }
                 else if(!ismu && !isjet && isEG && isTau && !issums && !isZerobias )
                  {
                       EGTaurate=EGTaurate + rate;
                       EGTaupurerate = EGTaupurerate + pure;
                       EGTauproprate = EGTauproprate + prop;
                         cout<<seedname<<"  is EG+Tau"<<endl;        
                  }
                 else if(!ismu && (isjet || issums ) && !isEG && isTau && !isZerobias)
                  {
                       TauJetSumsrate=TauJetSumsrate + rate;
                       TauJetSumspurerate = TauJetSumspurerate + pure;
                       TauJetSumsproprate = TauJetSumsproprate + prop;
                         cout<<seedname<<"  is Tau+Jet/sums"<<endl;        
                  }
                 else if(!ismu && !isjet && !isEG && !isTau && !issums && isZerobias )
                  {
                       ZeroBiasrate=ZeroBiasrate + rate;
                       ZeroBiaspurerate = ZeroBiaspurerate + pure;
                       ZeroBiasproprate = ZeroBiasproprate + prop;
                         cout<<seedname<<"  is ZeroBias"<<endl;        
                  }
                 else
                   {
                         Otherrate=Otherrate + rate;
                         Otherpurerate = Otherpurerate + pure;
                         Otherproprate = Otherproprate + prop;
                         cout<<seedname<<"  is OtherSeeds"<<endl;        
                   }
                }
        }
        myfile.close();
        cout<<"Mu rate  "<<murate<<"  "<<mupurerate<<" "<<muproprate<<endl;
        cout<<"EG rate  "<<EGrate<<"  "<<EGpurerate<<" "<<EGproprate<<endl;
        cout<<"Jet rate  "<<Jetrate<<"  "<<Jetpurerate<<" "<<Jetproprate<<endl;
        cout<<"Tau rate  "<<Taurate<<"  "<<Taupurerate<<" "<<Tauproprate<<endl;
        cout<<"Sums rate  "<<Sumsrate<<"  "<<Sumspurerate<<" "<<Sumsproprate<<endl;
        cout<<"MuEG rate  "<<MuEGrate<<"  "<<MuEGpurerate<<" "<<MuEGproprate<<endl;
        cout<<"MuJetSums rate  "<<MuJetSumsrate<<"  "<<MuJetSumspurerate<<" "<<MuJetSumsproprate<<endl;
        cout<<"MuTau rate  "<<MuTaurate<<"  "<<MuTaupurerate<<" "<<MuTauproprate<<endl;
        cout<<"EGJetSums rate  "<<EGJetSumsrate<<"  "<<EGJetSumspurerate<<" "<<EGJetSumsproprate<<endl;
        cout<<"EGTau rate  "<<EGTaurate<<"  "<<EGTaupurerate<<" "<<EGTauproprate<<endl;
        cout<<"TauJetSums rate  "<<TauJetSumsrate<<"  "<<TauJetSumspurerate<<" "<<TauJetSumsproprate<<endl;
        cout<<"Other rate  "<<Otherrate<<"  "<<Otherpurerate<<" "<<Otherproprate<<endl;
        cout<<"ZerBias rate  "<<ZeroBiasrate<<"  "<<ZeroBiaspurerate<<" "<<ZeroBiasproprate<<endl;
       // double crossrate= MuEGproprate+MuJetSumsproprate+MuTauproprate+EGJetSumsproprate+EGTauproprate+TauJetSumsproprate;
       // double vals[]= {muproprate,EGproprate,Jetproprate,Tauproprate,Sumsproprate,crossrate,Otherproprate};
        double vals[]= {muproprate,EGproprate,Jetproprate,Tauproprate,Sumsproprate,MuEGproprate,MuJetSumsproprate,MuTauproprate,EGJetSumsproprate,EGTauproprate,TauJetSumsproprate,ZeroBiasproprate,/*Otherproprate*/};
   Int_t colors[] = {2,3,4,5,6,7,8,9,30,40,43,46};
  // Int_t colors[] = {2,3,4,5,6,7,8,9};
   Int_t nvals = sizeof(vals)/sizeof(vals[0]);
   TCanvas *cpie = new TCanvas("cpie","TPie test",2000,2000);
     //cpie->Divide(2,1);
     //cpie->cd(1);
   TPie *pie4 = new TPie("pie4",
      "rate disbritution between different seeds",nvals,vals,colors);
   //pie4->SetY(.32);
   pie4->SetRadius(.20);
   pie4->SetLabelsOffset(-0.08);
 //  pie4->SetEntryFillStyle(1,3030);
   pie4->SetLabelFormat("%perc %txt");
   //pie4->SetLabelFormat("%txt");
 //  pie4->SetLabelColor(1);
   //pie4->SetLabelFormat("#splitline{ (%perc)}{%txt}");
   pie4->SetTextSize(.010);
   pie4->SetTextColor(1);
   pie4->SetEntryLabel(0,"Mu");
   pie4->SetEntryLabel(1,"EG");
   pie4->SetEntryLabel(2,"Jet");
   pie4->SetEntryLabel(3,"Tau");
   pie4->SetEntryLabel(4,"Sums");
   pie4->SetEntryLabel(5,"Mu+EG");
   pie4->SetEntryLabel(6,"Mu+Jet/Sum");
   pie4->SetEntryLabel(7,"Mu+Tau");
   pie4->SetEntryLabel(8,"EG+Jet/Sum");
   pie4->SetEntryLabel(9,"EG+Tau");
   pie4->SetEntryLabel(10,"Tau+Jet/Sum");
   pie4->SetEntryLabel(11,"ZeroBias");
 //  pie4->SetEntryLabel(12,"Other");
 /* pie4->SetEntryFillStyle(1,3031);
  pie4->SetEntryFillStyle(2,3031);
  pie4->SetEntryFillStyle(3,3031);
  pie4->SetEntryFillStyle(4,3031);
  pie4->SetEntryFillStyle(5,3031);
  pie4->SetEntryFillStyle(6,3031);
  pie4->SetEntryFillStyle(7,3031);
  pie4->SetEntryFillStyle(8,3031);
  pie4->SetEntryFillStyle(9,3031);
  pie4->SetEntryFillStyle(10,3031);
  pie4->SetEntryFillStyle(11,3031);*/
 // pie4->SetFillStyle(12,3031);

   pie4->Draw("nol rs");
   TLegend *pieleg = pie4->MakeLegend();
   //TLegend *pieleg =(TLegend*)pie4->GetLegend();
   // pieleg->SetNColumns(2);
   // pieleg->SetEntrySeparation(.01f);
  // cpie->cd(2);
    pieleg->Draw();
 /*  gStyle->SetLegendBorderSize(1);
   gStyle->SetLegendFillColor(0);
   gStyle->SetLegendFont(42);
   gStyle->SetLegendTextSize(0.);*/
  // pieleg->SetY1(.56); pieleg->SetY2(.86);
   //pie4->Draw("nol <");

   cpie->SaveAs("piechart.pdf");
        //cin.ignore();
        return 0;
}


using namespace std;
int main(int arc, char** argv)
{
int l=0;
l=fileread(arc,argv);
return 0;
}
