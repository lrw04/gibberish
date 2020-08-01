OBJS = main.o model.o word.o
CXX = g++
CXXOPTS = -O2 -std=c++17

gibberish: $(OBJS)
	$(CXX) $(CXXOPTS) -o gibberish $(OBJS)

main.o: main.cpp model.h
	$(CXX) $(CXXOPTS) -c main.cpp

model.o: model.cpp model.h word.h
	$(CXX) $(CXXOPTS) -c model.cpp

word.o: word.h
	$(CXX) $(CXXOPTS) -c word.cpp

.PHONY: clean

clean:
	rm gibberish $(OBJS)
