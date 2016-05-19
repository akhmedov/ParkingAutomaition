CXX = g++ $(if ${DEBUG},-Wall,)
OPENCV_FLAGS=`pkg-config --cflags opencv` -I /usr/local/include/opencv2
OPENCV_LIBS=`pkg-config --libs opencv`
TEST_SOURCES=$(wildcard test/*.cpp)
TEST_EXECS=$(TEST_SOURCES:test/%.cpp=bin/%)
SOURCES=main.cpp clust/recognition.cpp clust/imgutils.cpp
MAIN_SRC=main.cpp
OBJECTS=$(SOURCES:%.cpp=bin/%.o)
MAIN_OBJS=$(MAIN_SRC:%.cpp=bin/%.o)

list:
	find . -name '*.cpp' -o -name '*.h' | xargs wc -l

test: $(TEST_EXECS)

build: $(OBJECTS)
	$(CXX) -o bin/parking $? $(OPENCV_LIBS)

clean:
	rm -fr bin/

# Next goal are for inside use only

bin/%.o: %.cpp
	@echo Build $@
	@mkdir -p $(@D)
	$(CXX) -c $< -o $@

bin/clust/%.o: clust/%.cpp clust/*.h
	@echo Build $@
	@mkdir -p $(@D)
	$(CXX) -c $(OPENCV_FLAGS) $< -o $@

$(EXECUTABLE): $(SOURCES)

# qq:
# 	@echo $(MAIN_OBJS)
# 	@echo
# 	@echo $(filter-out $(MAIN_OBJS),$(OBJECTS))

$(TEST_EXECS):bin/%:test/%.cpp
	@echo Build $@
	@mkdir -p $(@D)
	@$(CXX) $(OPENCV_FLAGS) $^ $(OPENCV_LIBS) -o $@

$(TEST_EXECS): $(filter-out $(MAIN_OBJS),$(OBJECTS))
