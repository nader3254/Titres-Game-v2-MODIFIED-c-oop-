run: 
	g++ -c bootscreen.cpp -o bootscreen.o 
	g++ -c configscreen.cpp -o configscreen.o 
	g++ -c engine.cpp -o engine.o 
	ar rcs libboot.a bootscreen.o  
	ar rcs libconfig.a configscreen.o 
	ar rcs libengie.a engine.o 
	g++ -c main.cpp 
	g++ -o bin.exe main.o  -L.  -lboot  -lengie  -lconfig
	@echo   "The Game has compiled successfuly......."
	@echo   "enjoy Hell and play the game "
	@echo   " A7A programmer ....."
	
clean:
	rm  bootscreen.o
	rm  configscreen.o
	rm  libboot.a
	rm  libconfig.a
	rm  libengie.a
	rm  engine.o
	rm  main.o
	rm  bin.exe	
