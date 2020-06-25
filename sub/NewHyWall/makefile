LIB_NAME := HyWall

ifndef PARALLEL
PARALLEL := MPI
endif

ifndef ALLOW_DEBUG_EXT
ALLOW_DEBUG_EXT := 0
endif

ifndef OPTLEVEL
OPTLEVEL := 0
endif

ifndef CUDA_ENABLE
CUDA_ENABLE := 0
endif

ifndef PROBLEM_DIMENSION
PROBLEM_DIMENSION := 2
endif

ifndef DEBUG_LEVEL
DEBUG_LEVEL := 0
endif

HYWALL_BASEIDIR   = $(shell pwd)
HYWALL_SRC_DIR   := ${HYWALL_BASEIDIR}/src
HYWALL_LIB_DIR   := ${HYWALL_BASEIDIR}/lib
HYWALL_OBJ_DIR   := ${HYWALL_BASEIDIR}/obj
HYWALL_HDR_DIR   := ${HYWALL_BASEIDIR}/include
HYWALL_DOC_DIR   := ${HYWALL_BASEIDIR}/docs
HYWALL_TST_DIR   := ${HYWALL_BASEIDIR}/testing
HYWALL_HDRHX_DIR := ${HYWALL_BASEIDIR}/includex


HYWALL_IFLAGS := -I${HYWALL_HDR_DIR} -I${HYWALL_HDRHX_DIR}

export WMLIB_INCLUDE_CONFIG=-I${HYWALL_HDR_DIR} -L${HYWALL_LIB_DIR} -l${LIB_NAME}

SRC_FILES_HYBRID_H      := $(shell find ${HYWALL_SRC_DIR} -name *.cppx)
SRC_FILES_HYBRID_D      := $(shell find ${HYWALL_SRC_DIR} -name *.cppx)
SRC_FILES_HOST          := $(shell find ${HYWALL_SRC_DIR} -name *.cpp)
SRC_FILES_CUDA          := $(shell find ${HYWALL_SRC_DIR} -name *.cu)

HEADER_FILES    := $(shell find ${HYWALL_SRC_DIR} -name *.h)
HEADER_FILES_HX := $(shell find ${HYWALL_SRC_DIR} -name *.hx)


TARGET := ${HYWALL_LIB_DIR}/lib${LIB_NAME}.a

PY_EXE := $(shell which python3)
CC_HOST := $(shell which mpicxx)
CC_DEVICE :=  $(shell which nvcc) -ccbin=${CC_HOST}
ifneq (${PARALLEL}, MPI)
CC_HOST := g++
CC_DEVICE := none
endif

CU_O_TARGET_NAME := ${HYWALL_OBJ_DIR}/CU_dlink.o
ifeq (${CUDA_ENABLE}, 1)
LINK_STEP := link_step
CU_O_TARGET := ${CU_O_TARGET_NAME}
LCUDA := -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -lcudadevrt -lcudart
else
LINK_STEP :=
OBJ_FILES_HYBRID_DEVICE :=
CUDA_ENABLE := 0
OBJ_FILES_CUDA :=
CU_O_TARGET :=
LCUDA :=
endif

COMPILE_TIME_OPT += -DCUDA_ENABLE=${CUDA_ENABLE}
COMPILE_TIME_OPT += -DPROBLEM_DIMENSION=${PROBLEM_DIMENSION}
COMPILE_TIME_OPT += -DDEBUG_LEVEL=${DEBUG_LEVEL}
COMPILE_TIME_OPT += -DALLOW_DEBUG_EXT=${ALLOW_DEBUG_EXT}
COMPILE_TIME_OPT += -DPARALLEL=${PARALLEL}
COMPILE_TIME_OPT += -DMAX_BUFFERS=1024
COMPILE_TIME_OPT += -DTRACK_OUTPUT_ORIGINS=1


DEVICE_FLAGS := -O${OPTLEVEL} -x cu -rdc=true -Xcompiler -fPIC ${COMPILE_TIME_OPT} -dc
DEVICE_DLINK_FLAGS := -Xcompiler -fPIC -rdc=true -dlink
HOST_FLAGS := -O${OPTLEVEL} -x c++ -Wno-unknown-pragmas -fPIC -fpermissive -std=c++11 -c ${LCUDA}

export CUDA_ENABLE

LZLIB :=
ifeq (${ALLOW_DEBUG_EXT}, 1)
LZLIB := -lz
endif


DO_CLEAN :=
ifeq (1, ${CUDA_ENABLE})
ifeq (,$(wildcard ${CU_O_TARGET_NAME}))
DO_CLEAN := clean
endif
endif

ifeq (0, ${CUDA_ENABLE})
ifneq (,$(wildcard ${CU_O_TARGET_NAME}))
DO_CLEAN := clean
endif
endif

ifeq (0, ${CUDA_ENABLE})
SRC_FILES_HYBRID_D :=
endif


TARGETNAME_HYBRID_H := $(addprefix ${HYWALL_OBJ_DIR}/,$(addsuffix .o,$(notdir ${SRC_FILES_HYBRID_H})))
TARGETNAME_HYBRID_D := $(addprefix ${HYWALL_OBJ_DIR}/,$(addsuffix .o,$(notdir ${SRC_FILES_HYBRID_D})))
TARGETNAME_HOST     := $(addprefix ${HYWALL_OBJ_DIR}/,$(addsuffix .o,$(notdir ${SRC_FILES_HOST})))
TARGETNAME_CUDA     := $(addprefix ${HYWALL_OBJ_DIR}/,$(addsuffix .o,$(notdir ${SRC_FILES_CUDA})))

SRC_FILES_HOST_DIR := $(dir ${SRC_FILES_HOST})
MP:=%

ifeq (,${TARGETNAME_HYBRID_D})
ifeq (,${TARGETNAME_CUDA})
LINK_STEP := 
endif
endif

.PHONY: final

final: ${DO_CLEAN} setup ${TARGETNAME_CUDA} ${TARGETNAME_HYBRID_D} ${LINK_STEP} ${TARGETNAME_HYBRID_H} ${TARGETNAME_HOST}
	${CC_HOST} -fPIC -shared ${HYWALL_OBJ_DIR}/*.o ${HYWALL_IFLAGS} ${COMPILE_TIME_OPT} ${LZLIB} ${LCUDA} -o ${TARGET}

.SECONDEXPANSION:
${TARGETNAME_HYBRID_D}: ${HYWALL_OBJ_DIR}/%.o : $$(filter $$(MP)/$$*,$$(SRC_FILES_HYBRID_D))
	${CC_DEVICE} ${DEVICE_FLAGS} ${COMPILE_TIME_OPT} ${HYWALL_IFLAGS} $< -o $@

.SECONDEXPANSION:
${TARGETNAME_HYBRID_H}: ${HYWALL_OBJ_DIR}/%.o : $$(filter $$(MP)/$$*,$$(SRC_FILES_HYBRID_H))
	${CC_HOST} ${HOST_FLAGS} ${COMPILE_TIME_OPT} ${HYWALL_IFLAGS} $< -o $@

.SECONDEXPANSION:
${TARGETNAME_HOST}: ${HYWALL_OBJ_DIR}/%.o : $$(filter $$(MP)/$$*,$$(SRC_FILES_HOST))
	${CC_HOST} ${HOST_FLAGS} ${COMPILE_TIME_OPT} ${HYWALL_IFLAGS} $< -o $@

.SECONDEXPANSION:
${TARGETNAME_CUDA}: ${HYWALL_OBJ_DIR}/%.o : $$(filter $$(MP)/$$*,$$(SRC_FILES_CUDA))
	${CC_DEVICE} ${DEVICE_FLAGS} ${COMPILE_TIME_OPT} ${HYWALL_IFLAGS} $< -o $@

${LINK_STEP}:
	${CC_DEVICE} ${DEVICE_DLINK_FLAGS} ${COMPILE_TIME_OPT} ${OBJ_FILES_HYBRID_DEVICE} ${OBJ_FILES_CUDA} -o ${CU_O_TARGET} -lcudadevrt

setup:
	echo ${AA}
	-rm -r ${HYWALL_HDR_DIR}
	-rm -r ${HYWALL_HDRHX_DIR}
	mkdir -p ${HYWALL_LIB_DIR}
	mkdir -p ${HYWALL_OBJ_DIR}
	mkdir -p ${HYWALL_HDR_DIR}
	mkdir -p ${HYWALL_HDRHX_DIR}
	@for hdr in ${HEADER_FILES} ; do\
		echo "Linking $${hdr}:";\
		ln -s $${hdr} -t ${HYWALL_HDR_DIR};\
	done
	@for hdr in ${HEADER_FILES_HX} ; do\
		echo "Linking $${hdr}:";\
		ln -s $${hdr} -t ${HYWALL_HDRHX_DIR};\
	done

clean:
	-rm -r ${HYWALL_LIB_DIR}
	-rm -r ${HYWALL_OBJ_DIR}
	-rm -r ${HYWALL_HDR_DIR}
	-rm -r ${HYWALL_HDRHX_DIR}

test: final
	@echo "NO TEST TARGET"
