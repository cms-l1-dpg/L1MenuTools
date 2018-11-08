fileread : fileread.C
	g++ `root-config --cflags` fileread.C -o fileread `root-config --libs`
