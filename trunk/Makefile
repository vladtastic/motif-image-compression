CC=g++
CFLAGS=-g -O2 -Wall

SERIAL_SRC=Image.cpp Bitmap.cpp Motif.cpp Driver.cpp
SERIAL_OUT=motif-serial

OMP_SRC=Image.cpp Bitmap.cpp Motif-omp.cpp Driver-omp.cpp
OMP_OUT=motif-parallel

TEST_SRC=image-tester.cpp tester-driver.cpp
TEST_OUT=motif-test

serial: ${SERIAL_SRC}
	${CC} ${CFLAGS} ${SERIAL_SRC} -o ${SERIAL_OUT}

parallel: ${OMP_SRC}
	${CC} ${CFLAGS} ${OMP_SRC} -fopenmp -o ${OMP_OUT}

test: ${TEST_SRC}
	${CC} ${CFLAGS} ${SERIAL_SRC} ${TEST_SRC} -o ${TEST_OUT}

clean:
	rm -rf *.o
