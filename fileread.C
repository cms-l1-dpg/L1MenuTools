//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              Author - Aloke Kumar Das                                                //
//                           Email - da21saloke@gmail.com                                               //
//                                                                                                      //
//                       It makes Piechart from the ratetable                                           //
//                                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <TCanvas.h>
#include <TPie.h>
#include <TString.h>
#include <sstream>
#include <TLegend.h>
#include <TStyle.h>
#include <TLatex.h>
using namespace std;
int fileread(int arc, char** arv)

{       cout<<"no of tables "<<arc-2<<endl;
        string fString,line;
        string line1,seedname,sign;
        int index,index2;
 
       double prescale,rate,error,pure,prop;

double murate=0,mupurerate=0,muproprate=0;
        double singlemurate=0,singlemupurerate=0,singlemuproprate=0;
        double multimurate=0,multimupurerate=0,multimuproprate=0;
        double EGrate=0,EGpurerate=0,EGproprate=0;
        double singleEGrate=0,singleEGpurerate=0,singleEGproprate=0;
        double multiEGrate=0,multiEGpurerate=0,multiEGproprate=0;
        double Jetrate=0,Jetpurerate=0,Jetproprate=0;
        double Taurate=0,Taupurerate=0,Tauproprate=0;
        double Sumsrate=0,Sumspurerate=0,Sumsproprate=0;
        double MuEGrate=0,MuEGpurerate=0,MuEGproprate=0;
        double MuJetSumsrate=0,MuJetSumspurerate=0,MuJetSumsproprate=0;
      // double MuTaurate=0,MuTaupurerate=0,MuTauproprate=0;
       double EGJetSumsrate=0,EGJetSumspurerate=0,EGJetSumsproprate=0;
       // double EGTaurate=0,EGTaupurerate=0,EGTauproprate=0;
       // double TauJetSumsrate=0,TauJetSumspurerate=0,TauJetSumsproprate=0;
       double TauMuEGJetSumsrate=0,TauMuEGJetSumspurerate=0,TauMuEGJetSumsproprate=0;
       double JetSumsrate=0,JetSumspurerate=0,JetSumsproprate=0;
       double Otherrate=0,Otherpurerate=0,Otherproprate=0;
       double ZeroBiasrate=0,ZeroBiaspurerate=0,ZeroBiasproprate=0;
       double Calibrationrate=0,Calibrationpurerate=0,Calibrationproprate=0;
 for(int ntable=2;ntable<arc;ntable++)
{
        ifstream myfile;
        //string fileLocation;

        //opens the file
        //myfile.open("output_aloke.txt");
        myfile.open(arv[ntable]);

        //makes sure that the file opened properly
        while(myfile.fail())
        {
                cout <<  " failed to open" << endl;
        }
        bool pietable = false;
        int count=0;
       /* double murate=0,mupurerate=0,muproprate=0;
        double singlemurate=0,singlemupurerate=0,singlemuproprate=0;
        double multimurate=0,multimupurerate=0,multimuproprate=0;
        double EGrate=0,EGpurerate=0,EGproprate=0;
        double singleEGrate=0,singleEGpurerate=0,singleEGproprate=0;
        double multiEGrate=0,multiEGpurerate=0,multiEGproprate=0;
        double Jetrate=0,Jetpurerate=0,Jetproprate=0;
        double Taurate=0,Taupurerate=0,Tauproprate=0;
        double Sumsrate=0,Sumspurerate=0,Sumsproprate=0;
        double MuEGrate=0,MuEGpurerate=0,MuEGproprate=0;
        double MuJetSumsrate=0,MuJetSumspurerate=0,MuJetSumsproprate=0;
       // double MuTaurate=0,MuTaupurerate=0,MuTauproprate=0;
        double EGJetSumsrate=0,EGJetSumspurerate=0,EGJetSumsproprate=0;
       // double EGTaurate=0,EGTaupurerate=0,EGTauproprate=0;
       // double TauJetSumsrate=0,TauJetSumspurerate=0,TauJetSumsproprate=0;
        double TauMuEGJetSumsrate=0,TauMuEGJetSumspurerate=0,TauMuEGJetSumsproprate=0;
        double JetSumsrate=0,JetSumspurerate=0,JetSumsproprate=0;
        double Otherrate=0,Otherpurerate=0,Otherproprate=0;
        double ZeroBiasrate=0,ZeroBiaspurerate=0,ZeroBiasproprate=0;
        double Calibrationrate=0,Calibrationpurerate=0,Calibrationproprate=0;*/
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
             //    aloke >> index >> seedname >> prescale >> rate >> error >> pure >> prop >>index2;
                 aloke >> index >> seedname >> prescale >> rate >> sign >> error >> pure >> prop ;
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
                 if(prescale >1)
                 {
                  Calibrationrate=Calibrationrate+ rate;  
                  Calibrationpurerate=Calibrationpurerate+ rate;  
                  Calibrationproprate=Calibrationproprate+ rate;  
                  }
                 if(ismu && !isjet && !isEG && !isTau && !issums && !isZerobias)
                 {
                         murate=murate + rate;
                         mupurerate = mupurerate + pure;
                         muproprate = muproprate + prop;
                     //    cout<<seedname<<"  is Mu"<<endl;        
                    if(seedname.find("Single")!=string::npos) 
                            {
                         singlemurate=singlemurate + rate;
                         singlemupurerate = singlemupurerate + pure;
                         singlemuproprate = singlemuproprate + prop;
                         cout<<seedname<<"  is SingleMu and rate " <<singlemuproprate<<endl;        
                            
                            } 
                    else 
                          {
                          
                         multimurate=multimurate + rate;
                         multimupurerate = multimupurerate + pure;
                         multimuproprate = multimuproprate + prop;
                        cout<<seedname<<"  is MultiMu"<<endl;        
                           }
                  }
                 else if(!ismu && !isjet && isEG && !isTau && !issums && !isZerobias)
                 {
                         EGrate=EGrate + rate;
                         EGpurerate = EGpurerate + pure;
                         EGproprate = EGproprate + prop;
                        // cout<<seedname<<"  is EG"<<endl;        
                         if(seedname.find("Single")!=string::npos)
                         {
                         singleEGrate=singleEGrate + rate;
                         singleEGpurerate = singleEGpurerate + pure;
                         singleEGproprate = singleEGproprate + prop;
                         cout<<seedname<<"  is SingleEG"<<endl;        
                          }
                          else
                          {
                         multiEGrate=multiEGrate + rate;
                         multiEGpurerate = multiEGpurerate + pure;
                         multiEGproprate = multiEGproprate + prop;
                         cout<<seedname<<"  is multiEG"<<endl;        
                           
                           
                          }
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
                 else if(!ismu /*&& !isjet*/ && !isEG && !isTau && issums && !isZerobias)
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
                 /*else if(ismu && !isjet && !isEG && isTau && !issums && !isZerobias)
                  {
                       MuTaurate=MuTaurate + rate;
                       MuTaupurerate = MuTaupurerate + pure;
                       MuTauproprate = MuTauproprate + prop;
                         cout<<seedname<<"  is Mu+Tau"<<endl;        
                  }*/
                 else if(!ismu  && isEG && !isTau && (isjet || issums) && !isZerobias)
                  {
                       EGJetSumsrate=EGJetSumsrate + rate;
                       EGJetSumspurerate = EGJetSumspurerate + pure;
                       EGJetSumsproprate = EGJetSumsproprate + prop;
                         cout<<seedname<<"  is EG+Jet/Sums"<<endl;        
                  }
                /* else if(!ismu && !isjet && isEG && isTau && !issums && !isZerobias )
                  {
                       EGTaurate=EGTaurate + rate;
                       EGTaupurerate = EGTaupurerate + pure;
                       EGTauproprate = EGTauproprate + prop;
                         cout<<seedname<<"  is EG+Tau"<<endl;        
                  }*/
                /* else if(!ismu && (isjet || issums ) && !isEG && isTau && !isZerobias)
                  {
                       TauJetSumsrate=TauJetSumsrate + rate;
                       TauJetSumspurerate = TauJetSumspurerate + pure;
                       TauJetSumsproprate = TauJetSumsproprate + prop;
                         cout<<seedname<<"  is Tau+Jet/sums"<<endl;        
                  }*/
                 else if(isTau && (isjet || issums || ismu || isEG) && !isZerobias)
                  {
                       TauMuEGJetSumsrate=TauMuEGJetSumsrate + rate;
                       TauMuEGJetSumspurerate = TauMuEGJetSumspurerate + pure;
                       TauMuEGJetSumsproprate = TauMuEGJetSumsproprate + prop;
                         cout<<seedname<<"  is Tau+Mu/EG/Jet/sums"<<endl;        
                  }
                 else if(!ismu && !isjet && !isEG && !isTau && !issums && isZerobias )
                  {
                       ZeroBiasrate=ZeroBiasrate + rate;
                       ZeroBiaspurerate = ZeroBiaspurerate + pure;
                       ZeroBiasproprate = ZeroBiasproprate + prop;
                         cout<<seedname<<"  is ZeroBias"<<endl;        
                  }
                 else if(!ismu  && !isEG && !isTau && isjet && issums && !isZerobias)
                  {
                       JetSumsrate=JetSumsrate + rate;
                       JetSumspurerate = JetSumspurerate + pure;
                       JetSumsproprate = JetSumsproprate + prop;
                         cout<<seedname<<"  is Jet+Sums"<<endl;        
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
}
        //cout<<"Mu rate  "<<murate<<"  "<<mupurerate<<" "<<muproprate<<endl;
        cout<<"SingleMu rate  "<<singlemurate<<"  "<<singlemupurerate<<" "<<singlemuproprate<<endl;
        cout<<"MultuMu rate  "<<multimurate<<"  "<<multimupurerate<<" "<<multimuproprate<<endl;
        //cout<<"EG rate  "<<EGrate<<"  "<<EGpurerate<<" "<<EGproprate<<endl;
        cout<<"SingleEG rate  "<<singleEGrate<<"  "<<singleEGpurerate<<" "<<singleEGproprate<<endl;
        cout<<"MultiEG rate  "<<multiEGrate<<"  "<<multiEGpurerate<<" "<<multiEGproprate<<endl;
        cout<<"Jet rate  "<<Jetrate<<"  "<<Jetpurerate<<" "<<Jetproprate<<endl;
        cout<<"Tau rate  "<<Taurate<<"  "<<Taupurerate<<" "<<Tauproprate<<endl;
        cout<<"Sums rate  "<<Sumsrate<<"  "<<Sumspurerate<<" "<<Sumsproprate<<endl;
        cout<<"MuEG rate  "<<MuEGrate<<"  "<<MuEGpurerate<<" "<<MuEGproprate<<endl;
        cout<<"MuJetSums rate  "<<MuJetSumsrate<<"  "<<MuJetSumspurerate<<" "<<MuJetSumsproprate<<endl;
        //cout<<"MuTau rate  "<<MuTaurate<<"  "<<MuTaupurerate<<" "<<MuTauproprate<<endl;
        cout<<"EGJetSums rate  "<<EGJetSumsrate<<"  "<<EGJetSumspurerate<<" "<<EGJetSumsproprate<<endl;
        //cout<<"EGTau rate  "<<EGTaurate<<"  "<<EGTaupurerate<<" "<<EGTauproprate<<endl;
       // cout<<"TauJetSums rate  "<<TauJetSumsrate<<"  "<<TauJetSumspurerate<<" "<<TauJetSumsproprate<<endl;
        cout<<"TauMuEGJetSums rate  "<<TauMuEGJetSumsrate<<"  "<<TauMuEGJetSumspurerate<<" "<<TauMuEGJetSumsproprate<<endl;
        //cout<<"JetSums rate  "<<JetSumsrate<<"  "<<JetSumspurerate<<" "<<JetSumsproprate<<endl;
        cout<<"Other rate  "<<Otherrate<<"  "<<Otherpurerate<<" "<<Otherproprate<<endl;
        cout<<"ZerBias rate  "<<ZeroBiasrate<<"  "<<ZeroBiaspurerate<<" "<<ZeroBiasproprate<<endl;
        cout<<"Calibration rate   "<<Calibrationrate<<"  "<<Calibrationpurerate<<" "<<Calibrationproprate<<endl;
       // double crossrate= MuEGproprate+MuJetSumsproprate+MuTauproprate+EGJetSumsproprate+EGTauproprate+TauJetSumsproprate;
       // double vals[]= {muproprate,EGproprate,Jetproprate,Tauproprate,Sumsproprate,crossrate,Otherproprate};
      //  double vals[]= {muproprate,EGproprate,Jetproprate,Tauproprate,Sumsproprate,MuEGproprate,MuJetSumsproprate,MuTauproprate,EGJetSumsproprate,EGTauproprate,TauJetSumsproprate,ZeroBiasproprate,/*Otherproprate*/};
        double vals[]= {singlemuproprate,multimuproprate,singleEGproprate,multiEGproprate,Jetproprate,Tauproprate,Sumsproprate,MuEGproprate,MuJetSumsproprate,EGJetSumsproprate,/*JetSumsproprate,*/TauMuEGJetSumsproprate,Calibrationproprate/*ZeroBiasproprate,Otherproprate*/};
   Int_t colors[] = {16,14,865,38,802,819,9,798,30,40,/*43,*/49,13};
   //Int_t colors[] = {2,46,4,38,6,36,8,9,30,40,/*43,*/49,13};
  // Int_t colors[] = {2,3,4,5,6,7,8,9};
   Int_t nvals = sizeof(vals)/sizeof(vals[0]);
   TCanvas *cpie = new TCanvas("cpie","TPie test",2000,2000);
     //cpie->Divide(2,1);
     //cpie->cd(1);
   TPie *pie4 = new TPie("pie4",
      " ",nvals,vals,colors);
   //pie4->SetY(.32);
   pie4->SetRadius(.3);
   pie4->SetLabelsOffset(-0.23);
 //  pie4->SetEntryFillStyle(1,3030);
   pie4->SetLabelFormat("%perc %txt");
   //pie4->SetLabelFormat("%txt");
 //  pie4->SetLabelColor(1);
   //pie4->SetLabelFormat("#splitline{ (%perc)}{%txt}");
   pie4->SetTextSize(.02);
   pie4->SetTextColor(19);
   //pie4->SetEntryFillColor(1,1);
   //pie4->SetEntryFillColor(3,1);
   //pie4->SetEntryFillColor(5,1);
   pie4->SetEntryLabel(0,"Single #mu");
   pie4->SetEntryLabel(1,"Multi #mu");
   pie4->SetEntryLabel(2,"Single e/#gamma");
   pie4->SetEntryLabel(3,"Multi e/#gamma");
   pie4->SetEntryLabel(4,"Jets");
   pie4->SetEntryLabel(5,"#tau");
   pie4->SetEntryLabel(6,"Sums");
   pie4->SetEntryLabel(7,"#mu+e/#gamma");
   pie4->SetEntryLabel(8,"#mu+Jets/Sums");
   pie4->SetEntryLabel(9,"e/#gamma+Jets/Sums");
   //pie4->SetEntryLabel(1,"EG+Tau");
   //pie4->SetEntryLabel(10,"Tau+Jet/Sum");
  // pie4->SetEntryLabel(10,"Jets+sums");
   pie4->SetEntryLabel(10,"#tau+#mu/e/#gamma/Jets/Sums");
//   pie4->SetEntryLabel(11,"ZeroBias");
   pie4->SetEntryLabel(11,"Calibration");
 //  pie4->SetEntryLabel(12,"Other");
   pie4->Draw("nol rs");
  TLegend *pieleg = pie4->MakeLegend();
pieleg->SetFillStyle(0);
pieleg->SetY1(.25); pieleg->SetY2(.75);
   pieleg->SetX1(.00); pieleg->SetX2(.20);
   pieleg->SetBorderSize(0);
   //TLegend *pieleg =(TLegend*)pie4->GetLegend();
   // pieleg->SetNColumns(2);
   // pieleg->SetEntrySeparation(.01f);
  // cpie->cd(2);
   // pieleg->Draw();
 /*  gStyle->SetLegendBorderSize(1);
   gStyle->SetLegendFillColor(0);
   gStyle->SetLegendFont(42);
   gStyle->SetLegendTextSize(0.);*/
  // pieleg->SetY1(.56); pieleg->SetY2(.86);
   //pie4->Draw("nol <");

   //cpie->SaveAs("piechart.pdf");
   string s1,s2,s3;
   s1=arv[1];
   s2="_piechart.pdf";
   s3=s1+s2;
   cpie->SaveAs(s3.c_str());
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
