all:
	g++ payload.cpp -o payload
	g++ processList.cpp -c -g
	g++ affinity.cpp -c -g
	g++ watchList.cpp -c -g
	# g++ processList.cpp -o processList
	# g++ affinity.o processList.o -o affinity
	# g++ watchList.o processList.o -o watchList -g