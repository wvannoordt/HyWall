include ../../utils/testsetup.mk
main:
	${CC} ${IFLAGS} ${MAIN} -o ${EXE} ${LFLAGS}

clean:
	-rm ${EXE}

test: clean main
	./${EXE}
