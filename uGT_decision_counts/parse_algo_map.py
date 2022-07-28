import sys

def main() :

	if len(sys.argv) < 2 : sys.exit("Error : too few arguments")
	
	infilename = sys.argv[1]
	
	infile = open(infilename)
	
	lines = infile.readlines()
	
	outfile = open("algo_map.py", "w")

	menu_name = infilename.split(".")
	outfile.write("#Menu used : "+menu_name[0]+"\n\n")
	
	outfile.write("algo_map = [\n")
	
	for line in lines[1:] :
		arr = line.split(',')
		arr = [arr[0],arr[1]]
		outfile.write(str(arr))
		outfile.write(",\n")
	
	outfile.write("]")
	outfile.close()

main()
