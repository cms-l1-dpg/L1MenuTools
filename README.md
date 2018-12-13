# my-project
Requirements- This tool need only root library

So In lxplus it is expected to be ran under a CMSSW environment

Log in to your lxplus account 

Go to any CMSSW scram area i.e cd $CMSSW_BASE/src

git clone https://github.com/alokekumardas/my-project.git

cd my-project

make

./fileread menu_v2_1_0_rate.txt > output.txt  (This is an example table one need to change the table name accroding to their table)

"piechart.pdf" and "output.txt" is saved in current directory
 
To see the category of each seed one should check the "output.txt" file

