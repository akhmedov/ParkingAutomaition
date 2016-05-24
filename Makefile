CXX = g++ $(if ${DEBUG},-Wall -DDEBUG,) $(if ${OUTFILES}, -DOUTFILES,) -std=c++11 
OPENCV_FLAGS=`pkg-config --cflags opencv` -I /usr/local/include/opencv2
OPENCV_LIBS=`pkg-config --libs opencv`
TEST_SOURCES=$(wildcard test/*.cpp)
TEST_EXECS=$(TEST_SOURCES:test/%.cpp=bin/%)
SOURCES=main.cpp clust/recognition.cpp clust/imgutils.cpp \
	parking/camera.cpp parking/spot.cpp
MAIN_SRC=main.cpp
OBJECTS=$(SOURCES:%.cpp=bin/%.o)
MAIN_OBJS=$(MAIN_SRC:%.cpp=bin/%.o)

try_photo_cv:
	bin/singleFrameCVIT screenshot/map.png screenshot/2016-05-18_12\:09\:15.png

try_cv:
	bin/cvApproachIT screenshot/2016-05-18_12\:09\:15.png

try_render:
	bin/maskRenderingIT screenshot/map.png screenshot/2016-05-18_12\:09\:15.png

list:
	find . -name '*.cpp' -o -name '*.h' | xargs wc -l

test: $(TEST_EXECS)

build: $(OBJECTS)
	$(CXX) -o bin/server $? $(OPENCV_LIBS)

clean:
	rm -fr bin/
	rm -fr out/

# Next goals are for inside use only

bin/%.o: %.cpp
	@echo Build $@
	@mkdir -p $(@D)
	$(CXX) -c $< -o $@

bin/clust/%.o: clust/%.cpp clust/*.h
	@echo Build $@
	@mkdir -p $(@D)
	$(CXX) -c $(OPENCV_FLAGS) $< -o $@

bin/parking/%.o: parking/%.cpp parking/*.h
	@echo Build $@
	@mkdir -p $(@D)
	$(CXX) -c $(OPENCV_FLAGS) $< -o $@

# $(EXECUTABLE): $(SOURCES)

# qq:
# 	@echo $(MAIN_OBJS)
# 	@echo
# 	@echo $(filter-out $(MAIN_OBJS),$(OBJECTS))

$(TEST_EXECS): bin/%:test/%.cpp
	@echo Build $@
	@mkdir -p $(@D)
	@$(CXX) $(OPENCV_FLAGS) $^ $(OPENCV_LIBS) -o $@

$(TEST_EXECS): $(filter-out $(MAIN_OBJS),$(OBJECTS))
