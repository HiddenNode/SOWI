src = ./source
hdr = ./header
obj = ./object
bin = ./binary
INCLUDE = -I $(hdr)/ -I C:/Users/kothm/Documents/libs/req/API2/include
LIB = -L C:/Users/kothm/Documents/libs/req/API2/lib
compiler = C:/msys64/mingw64/bin/g++.exe
options = -std=c++20 -g

lighting: $(obj)/main.o $(obj)/program.o $(obj)/shader.o $(obj)/shader.o $(obj)/camera.o $(obj)/light.o $(obj)/resourceHandler.o $(obj)/eventHandler.o $(obj)/helper.o $(obj)/commands.o $(obj)/commandHandler.o $(obj)/texture.o
	g++ $(options) $(obj)/main.o $(obj)/program.o $(obj)/shader.o $(obj)/camera.o $(obj)/light.o $(obj)/resourceHandler.o $(obj)/eventHandler.o $(obj)/helper.o $(obj)/commands.o -o $(bin)/lighting.exe $(obj)/commandHandler.o -lsfml-window-d -lsfml-system-d -lglew32 -lopengl32 -lwinmm -lgdi32 -luser32 $(LIB)
$(obj)/main.o: $(src)/main.cpp $(hdr)/program.hpp $(hdr)/helper.hpp
	g++ $(options) -c $(src)/main.cpp -o $(obj)/main.o $(INCLUDE)
$(obj)/program.o: $(src)/program.cpp $(hdr)/program.hpp $(hdr)/shader.hpp $(hdr)/eventHandler.hpp
	g++ $(options) -c $(src)/program.cpp -o $(obj)/program.o $(INCLUDE)
$(obj)/shader.o: $(src)/shader.cpp $(hdr)/shader.hpp
	g++ $(options) -c $(src)/shader.cpp -o $(obj)/shader.o $(INCLUDE)
$(obj)/helper.o: $(src)/helper.cpp $(hdr)/helper.hpp
	g++ $(options) -c $(src)/helper.cpp -o $(obj)/helper.o $(INCLUDE)
$(obj)/camera.o: $(src)/camera.cpp $(hdr)/camera.hpp
	g++ $(options) -c $(src)/camera.cpp -o $(obj)/camera.o $(INCLUDE)
$(obj)/light.o: $(src)/light.cpp $(hdr)/light.hpp
	g++ $(options) -c $(src)/light.cpp -o $(obj)/light.o $(INCLUDE)
$(obj)/resourceHandler.o: $(src)/resourceHandler.cpp $(hdr)/resourceHandler.hpp $(hdr)/texture.hpp
	g++ $(options) -c $(src)/resourceHandler.cpp -o $(obj)/resourceHandler.o $(INCLUDE)
$(obj)/eventHandler.o: $(src)/eventHandler.cpp $(hdr)/eventHandler.hpp
	g++ $(options) -c $(src)/eventHandler.cpp -o $(obj)/eventHandler.o $(INCLUDE)
$(obj)/commands.o: $(src)/commands.cpp $(hdr)/commands.hpp $(hdr)/templateHelper.hpp
	g++ $(options) -c $(src)/commands.cpp -o $(obj)/commands.o $(INCLUDE)
$(obj)/commandHandler.o: $(src)/commandHandler.cpp $(hdr)/commandHandler.hpp $(hdr)/commands.hpp
	g++ $(options) -c $(src)/commandHandler.cpp -o $(obj)/commandHandler.o $(INCLUDE)
$(obj)/texture.o: $(src)/texture.cpp $(hdr)/texture.hpp $(hdr)/stb_image.h
	g++ $(options) -c $(src)/texture.cpp -o $(obj)/texture.o $(INCLUDE)
test:
	g++ $(options) $(src)/test.cpp -o test.exe