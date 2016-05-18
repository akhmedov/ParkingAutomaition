CXX = g++ $(if ${DEBUG},-Wall,)
OPENCV_FLAGS=`pkg-config --cflags opencv` -I /usr/local/include/opencv2
OPENCV_LIBS=`pkg-config --libs opencv`

list:
	find . -name '*.cpp' -o -name '*.h' | xargs wc -l

build: bin/recognition.o bin/main.o
	$(CXX) -o bin/parking $? $(OPENCV_LIBS)

clean:
	rm -f bin/*
	rm -fr bin/

bin/main.o: main.cpp
	mkdir -p bin/
	$(CXX) -o bin/main.o -c main.cpp	

bin/recognition.o: clust/recognition.cpp clust/recognition.h
	mkdir -p bin/
	$(CXX) $(OPENCV_FLAGS) -o bin/recognition.o -c clust/recognition.cpp
