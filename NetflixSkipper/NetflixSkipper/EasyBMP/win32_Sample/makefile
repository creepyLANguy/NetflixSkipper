CC = g++
CFLAGS = -O3 -s -mwindows

OBJECTS = easybmp_win32_sample.o EasyBMP.o EasyBMP_Geometry.o EasyBMP_Font.o EasyBMP_win32.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) -o win32test $(OBJECTS)
	
easybmp_win32_sample.o: easybmp_win32_sample.cpp
	cp ../EasyBMP.h .
	cp ../EasyBMP.cpp .
	cp ../EasyBMP_DataStructures.h .
	cp ../EasyBMP_BMP.h .
	cp ../EasyBMP_VariousBMPutilities.h .
	cp ../EasyBMP_Geometry.* .
	cp ../EasyBMP_Font.* .
	cp ../EasyBMP_win32.* .
	$(CC) $(CFLAGS) -c easybmp_win32_sample.cpp
	
EasyBMP.o: EasyBMP.cpp
	$(CC) $(CFLAGS) -c EasyBMP.cpp
	
EasyBMP_Font.o: EasyBMP_Font.cpp
	$(CC) $(CFLAGS) -c EasyBMP_Font.cpp
	
EasyBMP_Geometry.o: EasyBMP_Geometry.cpp
	$(CC) $(CFLAGS) -c EasyBMP_Geometry.cpp
	
EasyBMP_win32.o: EasyBMP_win32.cpp
	$(CC) $(CFLAGS) -c EasyBMP_win32.cpp
	
clean:
	rm *.o
	rm EasyBMP.h
	rm EasyBMP.cpp
	rm EasyBMP_DataStructures.h
	rm EasyBMP_BMP.h
	rm EasyBMP_VariousBMPutilities.h
	rm EasyBMP_Geometry.*
	rm EasyBMP_Font.*
	rm EasyBMP_win32.*
	rm *.bmp
