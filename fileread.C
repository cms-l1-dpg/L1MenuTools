#include <iostream>
#include <fstream>
#include <string>
#include <TCanvas.h>
#include <TPie.h>
#include <TString.h>
#include <sstream>
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
        double Otherrate=0,Otherpurerate=0,Otherproprate=0;
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
                 aloke >> index >> seedname >> prescale >> rate >> sign >> error >> pure >> prop ;
                 if(rate==0 && pure==0 && prop==0) continue;
                 cout<<index<<" "<<seedname<<" "<<prescale<<" "<<rate<<" "<<sign<<" "<<error<<" "<<pure<<" "<<prop<<endl;
                 if(seedname.find("Mu")!=string::npos && seedname.find("EG")==string::npos && seedname.find("Jet")==string::npos && seedname.find("Tau")==string::npos)
                 {
                         murate=murate + rate;
                         mupurerate = mupurerate + pure;
                         muproprate = muproprate + prop;
                  }
                 else if(seedname.find("Mu")==string::npos && seedname.find("EG")!=string::npos && seedname.find("Jet")==string::npos && seedname.find("Tau")==string::npos)
                 {
                         EGrate=EGrate + rate;
                         EGpurerate = EGpurerate + pure;
                         EGproprate = EGproprate + prop;
                  }
                 else if(seedname.find("Mu")==string::npos && seedname.find("EG")==string::npos && seedname.find("Jet")!=string::npos && seedname.find("Tau")==string::npos)
                 {
                         Jetrate=Jetrate + rate;
                         Jetpurerate = Jetpurerate + pure;
                         Jetproprate = Jetproprate + prop;
                  }
                 else if(seedname.find("Mu")==string::npos && seedname.find("EG")==string::npos && seedname.find("Jet")==string::npos && seedname.find("Tau")!=string::npos)
                 {
                         Taurate=Taurate + rate;
                         Taupurerate = Taupurerate + pure;
                         Tauproprate = Tauproprate + prop;
                  }
                   else
                   {
                         Otherrate=Otherrate + rate;
                         Otherpurerate = Otherpurerate + pure;
                         Otherproprate = Otherproprate + prop;
                   }
                }
        }
        myfile.close();
        cout<<"Mu rate  "<<murate<<"  "<<mupurerate<<" "<<muproprate<<endl;
        cout<<"EG rate  "<<EGrate<<"  "<<EGpurerate<<" "<<EGproprate<<endl;
        cout<<"Jet rate  "<<Jetrate<<"  "<<Jetpurerate<<" "<<Jetproprate<<endl;
        cout<<"Tau rate  "<<Taurate<<"  "<<Taupurerate<<" "<<Tauproprate<<endl;
        cout<<"Other rate  "<<Otherrate<<"  "<<Otherpurerate<<" "<<Otherproprate<<endl;
        double vals[]= {muproprate,EGproprate,Jetproprate,Tauproprate,Otherproprate};
   Int_t colors[] = {2,4,6,7,9};
   Int_t nvals = sizeof(vals)/sizeof(vals[0]);
   TCanvas *cpie = new TCanvas("cpie","TPie test",1000,1000);
   TPie *pie4 = new TPie("pie4",
      "Pie with verbose labels",nvals,vals,colors);
   pie4->SetRadius(.3);
   pie4->SetLabelsOffset(0);
   pie4->SetLabelFormat("#splitline{ (%perc)}{%txt}");
   pie4->SetEntryLabel(0,"Mu");
   pie4->SetEntryLabel(1,"EG");
   pie4->SetEntryLabel(2,"Jet");
   pie4->SetEntryLabel(3,"Tau");
   pie4->SetEntryLabel(4,"Other");
   pie4->Draw("nol <");

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
