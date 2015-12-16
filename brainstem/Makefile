CFLAGS=-g -Wall 
OPTIONS = -Wall
sacu: src/sacu.cpp
	gcc -g -c src/sacu.cpp $(OPTIONS) -I./RoBoIO/include
	gcc sacu.o -o sacu -L./RoBoIO -lRBIO -lstdc++

sacu_c: sacu.c
	gcc -g -c -Wall src/sacu.c -o sacu.o -I./RoBoIO/include
	gcc sacu.o -o sacu -L./RoBoIO -lRBIO

sacu_old: sacu.c
	gcc -g -c -Wall sacu.c -o sacu.o
	gcc sacu.o -o sacu

ultrasonics: ultrasonics.c
	gcc -g -c -Wall ultrasonics.c -o ultrasonics.o
	gcc ultrasonics.o -o ultrasonics


# Type
#     "make"
# to get hmc5843.exe adxl345.exe idg650.exe 
#

.PHONY : rbmodule
rbmodule: hmc5843.exe adxl345.exe idg650.exe

hmc5843.exe : hmc5843.o
	gcc -o hmc5843 hmc5843.o -L./RoBoIO -lRBIO -lstdc++

hmc5843.o : src/hmc5843.cpp
	gcc -c src/hmc5843.cpp $(OPTIONS) -I./RoBoIO/include

adxl345.exe : adxl345.o
	gcc -o adxl345 adxl345.o -L./RoBoIO -lRBIO -lstdc++

adxl345.o : src/adxl345.cpp
	gcc -c src/adxl345.cpp $(OPTIONS) -I./RoBoIO/include

idg650.exe : idg650.o
	gcc -o idg650 idg650.o -L./RoBoIO -lRBIO -lstdc++

idg650.o : src/idg650.cpp
	gcc -c src/idg650.cpp $(OPTIONS) -I./RoBoIO/include

.PHONY : clean
clean :
	-rm hmc5843 hmc5843.o

	-rm adxl345 adxl345.o

	-rm idg650 idg650.o
