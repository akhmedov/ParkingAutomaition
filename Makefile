CXX = g++
CXX_FLAGS=$(if ${DEBUG},-Wall -DDEBUG,) $(if ${OUTFILES}, -DOUTFILES,) 
CXX_STD=-std=c++11

OPENCV_FLAGS=`pkg-config --cflags opencv` -I /usr/local/include/opencv2
OPENCV_LIBS=`pkg-config --libs opencv`

# MYSQL_FLAGS=-I /usr/include/mysql
MYSQL_FLAGS=`mysql_config --cflags`
MYSQL_LIBS =`mysql_config --libs`

TEST_SOURCES=$(wildcard test/*.cpp)
TEST_EXECS=$(TEST_SOURCES:test/%.cpp=bin/%)
SOURCES=main.cpp clust/recognition.cpp clust/imgutils.cpp \
	parking/camera.cpp parking/spot.cpp \
	logics/datafactory.h logics/datafactory.cpp
MAIN_SRC=main.cpp

OBJECTS=$(SOURCES:%.cpp=bin/%.o)
MAIN_OBJS=$(MAIN_SRC:%.cpp=bin/%.o)

.PHONY: $(clean) $(list)

try_photo_cv:
	bin/singleFrameCVIT screenshot/map.png screenshot/2016-05-18_12\:09\:15.png

try_cv:
	bin/cvApproachIT screenshot/2016-05-18_12\:09\:15.png

try_render:
	bin/maskRenderingIT screenshot/map.png screenshot/2016-05-18_12\:09\:15.png

list:
	find . -name '*.cpp' -o -name '*.h' -o -name '*.sql' -o -name 'Makefile' | xargs wc -l

test: $(TEST_EXECS)

build: $(OBJECTS)
	$(CXX) $(CXX_STD) $(CXX_FLAGS) -o bin/server $? $(OPENCV_LIBS) $(MYSQL_LIBS)

clean:
	@rm -fr bin/
	@rm -fr out/

# Next goals are for inside use only

bin/%.o: %.cpp
	@echo Build $@
	@mkdir -p $(@D)
	$(CXX) $(CXX_STD) $(CXX_FLAGS) -c $< -o $@

bin/clust/%.o: clust/%.cpp clust/*.h
	@echo Build $@
	@mkdir -p $(@D)
	$(CXX) $(CXX_STD) $(CXX_FLAGS) -c $(MYSQL_FLAGS) $(OPENCV_FLAGS) $< -o $@

bin/logics/%.o: logics/%.cpp logics/*.h
	@echo Build $@
	@mkdir -p $(@D)
	$(CXX) $(CXX_STD) $(CXX_FLAGS) -c $(MYSQL_FLAGS) $(OPENCV_FLAGS) $< -o $@	

bin/parking/%.o: parking/%.cpp parking/*.h
	@echo Build $@
	@mkdir -p $(@D)
	$(CXX) $(CXX_STD) $(CXX_FLAGS) -c $(OPENCV_FLAGS) $(MYSQL_FLAGS) $< -o $@

# $(EXECUTABLE): $(SOURCES)

# qq:
# 	@echo $(MAIN_OBJS)
# 	@echo
# 	@echo $(filter-out $(MAIN_OBJS),$(OBJECTS))

$(TEST_EXECS): bin/%:test/%.cpp
	@echo Build $@
	@mkdir -p $(@D)
	@$(CXX) $(CXX_STD) $(CXX_FLAGS) $(OPENCV_FLAGS) $(MYSQL_FLAGS) $^ $(OPENCV_LIBS) -o $@

$(TEST_EXECS): $(filter-out $(MAIN_OBJS),$(OBJECTS))
