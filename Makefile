CC = g++
FLAGS = -g

AUTO=Auto.cpp
AUTOOUT=auto.out

BMP=BitmaptoTextConversion.cpp
BMPOUT=bmptotext.out

B2B=BittoByteConversion.cpp
B2BOUT=bit2bite.out

PI=primitivi.cpp
PIOUT=primitivi.out

all:auto bmptotext bit2bite primitivi

auto:${AUTO}
	${CC} ${FLAGS} ${AUTO} -o ${AUTOOUT}

bmp2text:${BMP}
	${CC} ${FLAGS} ${BMP} -o ${BMPOUT}

bit2bite:${B2B}
	${CC} ${FLAGS} ${B2B} -o ${B2BOUT}

primitivi:${PI}
	${CC} ${FLAGS} ${PI} -O ${PIOUT}

clean:
	rm *.o
