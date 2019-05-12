all:
	# g++ processList.cpp -o processList
	g++ processList.cpp -c
	g++ affinity.cpp -c
	g++ affinity.o processList.o -o affinity
	g++ payload.cpp -o payload