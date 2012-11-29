CC=g++
CFLAGS=-g -O2 -Wall

SERIAL_SRC=Image.cpp Bitmap.cpp Motif.cpp
SERIAL_OBJ=Image.o Bitmap.o Motif.o
SERIAL_OUT=motif-serial

OMP_SRC=Image.cpp Bitmap.cpp Motif-omp.cpp Driver-omp.cpp
OMP_OBJ=Image.o Bitmap.o Motif-omp.o Driver-omp.o
OMP_OUT=motif-parallel

TEST_SRC=image-test.cpp tester-driver.cpp
TEST_OBJ=image-test.o tester-driver.o
TEST_OUT=motif-test

serial: ${SERIAL_OBJ}
	${CC} ${CFLAGS} ${SERIAL_SRC} Driver.cpp -o ${SERIAL_OUT}

parallel: ${OMP_OBJ}
	${CC} ${CFLAGS} ${OMP_SRC} -fopenmp -o ${OMP_OUT}

test: ${TEST_OBJ}
	${CC} ${CFLAGS} ${SERIAL_SRC} ${TEST_OBJ} -o ${TEST_OUT}

image-tester.o:
	${CC} -c ${CFLAGS} image-test.cpp 

tester-driver.o:
	${CC} -c ${CFLAGS} tester-driver.cpp

clean:
	rm -rf *.o
