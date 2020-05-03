LIB_NAME := HyWall

ifndef PARALLEL
PARALLEL := MPI
endif

ifndef ALLOW_DEBUG_EXT
ALLOW_DEBUG_EXT := 0
endif

ifndef OPT_LEVEL
OPT_LEVEL := 0
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

WM_BASEIDIR = $(shell pwd)
WM_SRC_DIR := ${WM_BASEIDIR}/src

WM_LIB_DIR := ${WM_BASEIDIR}/lib
WM_OBJ_DIR := ${WM_BASEIDIR}/obj
WM_HDR_DIR := ${WM_BASEIDIR}/include
WM_PRP_DIR := ${WM_BASEIDIR}/maketools
PREPROCESSOR := ${WM_PRP_DIR}/hyWallPreProcess.py
POSTPROCESSOR := ${WM_PRP_DIR}/hyWallPostProcess.py

WM_IFLAGS := -I${WM_HDR_DIR}

export WMLIB_INCLUDE_CONFIG=-I${WM_HDR_DIR} -I${BITCART_SRC}/navier_stokes_newest -L${WM_LIB_DIR} -l${LIB_NAME}

SRC_FILES_HYBRID        := $(wildcard ${WM_SRC_DIR}/hybrid/*.cpp)
SRC_FILES_HOST          := $(wildcard ${WM_SRC_DIR}/host/*.cpp)
SRC_FILES_CUDA          := $(wildcard ${WM_SRC_DIR}/cuda/*.cu)

HDR_FILES_HYBRID        := $(wildcard ${WM_SRC_DIR}/hybrid/*.h)
HDR_FILES_HOST          := $(wildcard ${WM_SRC_DIR}/host/*.h)
HDR_FILES_CUDA          := $(wildcard ${WM_SRC_DIR}/cuda/*.h)

HEADER_FILES := ${HDR_FILES_HYBRID}
HEADER_FILES += ${HDR_FILES_HOST}
HEADER_FILES += ${HDR_FILES_CUDA}


OBJ_FILES_HYBRID_HOST   := $(patsubst ${WM_SRC_DIR}/numerics/%.cpp,$(WM_OBJ_DIR)/%.o,$(SRC_FILES_HYBRID))
OBJ_FILES_HYBRID_DEVICE := $(patsubst ${WM_SRC_DIR}/numerics/%.cpp,$(WM_OBJ_DIR)/K_%.o,$(SRC_FILES_HYBRID))
OBJ_FILES_HOST          := $(patsubst ${WM_SRC_DIR}/host/%.cpp,$(WM_OBJ_DIR)/%.o,$(SRC_FILES_HOST))
OBJ_FILES_CUDA          := $(patsubst ${WM_SRC_DIR}/kernel/%.cu,$(WM_OBJ_DIR)/%.o,$(SRC_FILES_CUDA))


TARGET := ${WM_LIB_DIR}/lib${LIB_NAME}.a

PY_EXE := $(shell which python3)
CC_HOST := mpicxx
CC_DEVICE := nvcc -ccbin=mpicxx
ifneq (${PARALLEL}, MPI)
CC_HOST := g++
CC_DEVICE := none
endif

CU_O_TARGET_NAME := ${WM_OBJ_DIR}/CU_dlink.o
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


DEVICE_FLAGS := -O${OPT_LEVEL} -x cu -rdc=true -Xcompiler -fPIC ${COMPILE_TIME_OPT} -dc
DEVICE_DLINK_FLAGS := -Xcompiler -fPIC -rdc=true -dlink
HOST_FLAGS := -O${OPT_LEVEL} -fPIC -fpermissive -std=c++11 -c ${LCUDA}

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

OBJ_FILES_HYBRID_HOST   := $(patsubst ${WM_SRC_DIR}/hybrid/%.cpp,$(WM_OBJ_DIR)/%.o,$(SRC_FILES_HYBRID))
OBJ_FILES_HYBRID_DEVICE := $(patsubst ${WM_SRC_DIR}/hybrid/%.cpp,$(WM_OBJ_DIR)/K_%.o,$(SRC_FILES_HYBRID))
OBJ_FILES_HOST          := $(patsubst ${WM_SRC_DIR}/host/%.cpp,$(WM_OBJ_DIR)/%.o,$(SRC_FILES_HOST))
OBJ_FILES_CUDA          := $(patsubst ${WM_SRC_DIR}/cuda/%.cu,$(WM_OBJ_DIR)/%.o,$(SRC_FILES_CUDA))

ifeq (0, ${CUDA_ENABLE})
OBJ_FILES_CUDA :=
OBJ_FILES_HYBRID_DEVICE :=
endif

LIB_OBJECTS := ${OBJ_FILES_HOST} ${OBJ_FILES_HYBRID_HOST} ${OBJ_FILES_CUDA} ${OBJ_FILES_HYBRID_DEVICE} ${CU_O_TARGET}
LIB_SOURCES := ${HEADER_FILES} ${SRC_FILES_HYBRID} ${SRC_FILES_HOST} ${SRC_FILES_CUDA}
export HWPP_OUT=${WM_HDR_DIR}/HWPP.hpp

.PHONY: final preprocess postprocess

postprocess: final
	${PY_EXE} ${POSTPROCESSOR}

final: ${DO_CLEAN} setup preprocess ${OBJ_FILES_HYBRID_HOST} ${OBJ_FILES_HOST} ${OBJ_FILES_HYBRID_DEVICE} ${OBJ_FILES_CUDA} ${LINK_STEP}
	${CC_HOST} -fPIC -shared ${LIB_OBJECTS} ${WM_IFLAGS} ${COMPILE_TIME_OPT} ${LZLIB} ${LCUDA} -o ${TARGET}

preprocess:
	${PY_EXE} ${PREPROCESSOR} ${LIB_SOURCES}

${OBJ_FILES_HYBRID_DEVICE}: ${WM_OBJ_DIR}/K_%.o : ${WM_SRC_DIR}/hybrid/%.cpp
	${CC_DEVICE} ${DEVICE_FLAGS} ${COMPILE_TIME_OPT} ${WM_IFLAGS} $< -o $@

${OBJ_FILES_HYBRID_HOST}: ${WM_OBJ_DIR}/%.o : ${WM_SRC_DIR}/hybrid/%.cpp
	${CC_HOST} ${HOST_FLAGS} ${COMPILE_TIME_OPT} ${WM_IFLAGS} $< -o $@

${OBJ_FILES_HOST}: ${WM_OBJ_DIR}/%.o : ${WM_SRC_DIR}/host/%.cpp
	${CC_HOST} ${HOST_FLAGS} ${COMPILE_TIME_OPT} ${WM_IFLAGS} $< -o $@

${LINK_STEP}:
	${CC_DEVICE} ${DEVICE_DLINK_FLAGS} ${COMPILE_TIME_OPT} ${OBJ_FILES_HYBRID_DEVICE} ${OBJ_FILES_CUDA} -o ${CU_O_TARGET} -lcudadevrt

${OBJ_FILES_CUDA}: ${WM_OBJ_DIR}/%.o : ${WM_SRC_DIR}/cuda/%.cu
	${CC_DEVICE} ${DEVICE_FLAGS} ${COMPILE_TIME_OPT} ${WM_IFLAGS} $< -o $@

syslink:
	ln -sf ${TARGET} ${LINK_LOCATION}

setup:
	-rm -r ${WM_HDR_DIR}
	mkdir -p ${WM_LIB_DIR}
	mkdir -p ${WM_OBJ_DIR}
	mkdir -p ${WM_HDR_DIR}
	@for hdr in ${HEADER_FILES} ; do \
		ln -s $${hdr} -t ${WM_HDR_DIR};\
	done

test: final
	@for fldr in testing/* ; do \
                ${MAKE} -C $${fldr} -f makefile -s test || exit 1; \
        done

clean:
	for fldr in testing/* ; do \
	            ${MAKE} -C $${fldr} -f makefile clean ; \
	    done
	-rm -r ${WM_LIB_DIR}
	-rm -r ${WM_OBJ_DIR}
	-rm -r ${WM_HDR_DIR}
