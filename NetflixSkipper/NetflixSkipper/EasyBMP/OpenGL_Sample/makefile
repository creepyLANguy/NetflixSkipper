CC = g++
CFLAGS = -funroll-all-loops -fomit-frame-pointer -ffast-math -O3 
OPENGL_FLAGS = -lopengl32 -lglut32 -lglu32 
		
Nixon_Text: EasyBMP_OpenGL_Sample.o EasyBMP.o EasyBMP_OpenGL.o
	$(CC) $(CFLAGS) -o Nixon_Test EasyBMP_OpenGL_Sample.o EasyBMP.o EasyBMP_OpenGL.o $(OPENGL_FLAGS)
	
EasyBMP.o: EasyBMP.cpp
	$(CC) $(CFLAGS) -c EasyBMP.cpp
		
EasyBMP_OpenGL_Sample.o: EasyBMP_OpenGL_Sample.cpp
	cp ../EasyBMP.h .
	cp ../EasyBMP.cpp .
	cp ../EasyBMP_DataStructures.h .
	cp ../EasyBMP_BMP.h .
	cp ../EasyBMP_VariousBMPutilities.h .
	cp ../EasyBMP_OpenGL.* .
	$(CC) $(CFLAGS) -c EasyBMP_OpenGL_Sample.cpp
	
EasyBMP_OpenGL.o: EasyBMP_OpenGL.cpp
	$(CC) $(CFLAGS) -c EasyBMP_OpenGL.cpp
	
clean:
	rm EasyBMP.h
	rm EasyBMP.cpp
	rm EasyBMP_DataStructures.h
	rm EasyBMP_BMP.h
	rm EasyBMP_VariousBMPutilities.h
	rm EasyBMP_OpenGL.*
	rm *.o
	rm frame*.bmp
	rm screenshot.bmp
