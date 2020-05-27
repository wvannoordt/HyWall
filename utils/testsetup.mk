CC := mpicxx
ifndef CUDA_ENABLE
CUDA_ENABLE := 0
endif
ifeq (${CUDA_ENABLE}, 1)
ICUDA := -I/usr/local/cuda/include
LCUDA := -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -lcudadevrt -lcudart
else
ICUDA :=
LCUDA :=
endif
IFLAGS := -I../../include ${ICUDA}
LFLAGS := -L/home/wvn/dirs/research/bitcart/src/HyWall/lib -lHyWall ${LCUDA}
MAIN := $(wildcard *.cc)
EXE := hywallTest
