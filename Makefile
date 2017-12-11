
CMP = g++
CLASS = spaceship
MAIN = project
EXEC = project

$(EXEC): $(CLASS).o $(MAIN).o 
	$(CMP) $(CLASS).o $(MAIN).o gfx.o -lX11 -o $(EXEC) 

$(CLASS).o: $(CLASS).cpp $(CLASS).h
	$(CMP) $(CLASS).cpp -c -std=c++11 -o $(CLASS).o 

$(MAIN).o: $(MAIN).cpp $(CLASS).h 
	$(CMP) $(MAIN).cpp -c -std=c++11 -o $(MAIN).o

clean:
	rm $(CLASS).o
	rm $(MAIN).o
	rm $(EXEC)

