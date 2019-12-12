# Piechart tool

Requirements- This tool need only root library

So In lxplus it is expected to be ran under a CMSSW environment

Log in to your lxplus account 

Go to any CMSSW scram area i.e cd $CMSSW_BASE/src

git clone https://github.com/alokekumardas/my-project.git

cd my-project

make

./fileread <outputname> <input ratetable>  > output.txt  (This is an example table one need to change the table name accroding to their table)
 for example
 ./fileread run_323940 ratetable.txt > outputlog.txt (for better understanding see "piemake.sh")

"run_323940_piechart.pdf" and "outputlog.txt" is saved in current directory

To see the category of each seed one should check the "outputlog.txt" file

N.B - If your rate Table looks like 

L1Bit	L1SeedName	pre-scale 	rate@13TeV 	+/- 	error_rate@13TeV	pure@13TeV	prop@13TeV	Comments

 Then use line 74 instead of line 73      
   cout<<index<<" "<<seedname<<" "<<prescale<<" "<<rate<<" "<<sign<<" "<<error<<" "<<pure<<" "<<prop<<endl;


if your rate table looks like this 

L1Bit	L1SeedName	PS	PS_rate	error	pure_rate	prop_rate


Then use line 73 and comment out line 74

 cout<<index<<" "<<seedname<<" "<<prescale<<" "<<rate<<" "<<error<<" "<<pure<<" "<<prop<<endl;
