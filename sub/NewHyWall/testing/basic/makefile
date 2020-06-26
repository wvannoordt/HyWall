MAINCC := $(wildcard *.cc)
TARGET := test_$(basename $(notdir ${MAINCC}))
main:
	${CC_HOST} ${HYWALL_ICONFIG} ${MAINCC} -o ${TARGET} ${HYWALL_LCONFIG}

clean:
	-rm ${TARGET}

test: clean main
	./${TARGET}
