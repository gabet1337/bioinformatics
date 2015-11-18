# Defining the compiler:
ifeq ($(OS),Windows_NT)
CC = cl
LINK = link
SWITCHES= -nologo -MT -D_CRT_SECURE_NO_DEPRECATE -EHsc -DPTW32_STATIC_LIB
OPTIMIZATION_LEVEL=-O2
DEBUG=-WX #-pg
OBJECTFLAG =-Fo
COMPILEFLAG =-c
LINK_OPTIONS =-nologo
LINKER_COMMANDS =-out:
LIBRARIES = pthreadVC2S.lib
else
CC = g++
LINK = g++
OPTIMIZATION_LEVEL=-O3 -msse2 #-g
DEBUG= -Wall #-g #-pg
OBJECTFLAG =-o
COMPILEFLAG =-c
LIBRARIES =-lpthread
LINK_OPTIONS = $(SWITCHES)
LINKER_COMMANDS =-o 
endif

CUDACC = nvcc 
SRCPATH=src
OBJPATH=obj
LIBPATH=lib
BINPATH=bin

DEBUG_CUDA = #-DTIMING #-g --ptxas-options=-v #-deviceemu 

rapidnj_cuda : SWITCHES += -DENABLEGPU
INCLUDES= -Isrc/distanceCalculation -Isrc/
rapidnj_cuda : INCLUDES += -I /usr/local/cuda/include -Isrc/distanceCalculation/gpu
#use -pg for gprof profiling in both steps

# Defining the object files:
objects = $(OBJPATH)/main.o $(OBJPATH)/node.o $(OBJPATH)/distMatrixReader.o $(OBJPATH)/polytree.o $(OBJPATH)/diskMatrix.o $(OBJPATH)/rdDataInitialiser.o $(OBJPATH)/rapidNJ.o $(OBJPATH)/rapidNJDisk.o $(OBJPATH)/rapidNJMem.o $(OBJPATH)/simpleNJ.o $(OBJPATH)/testNJ.o $(OBJPATH)/distanceCalculation/dnaBitString.o $(OBJPATH)/distanceCalculation/dataloaderPhylip.o $(OBJPATH)/distanceCalculation/hammingDistance.o $(OBJPATH)/distanceCalculation/bitDistanceGap.o $(OBJPATH)/distanceCalculation/JCdistance.o $(OBJPATH)/distanceCalculation/simpleDistanceCalculator.o $(OBJPATH)/distanceCalculation/dataloader.o $(OBJPATH)/distanceCalculation/KimuraDistance.o $(OBJPATH)/distanceCalculation/bitDistanceProtein.o $(OBJPATH)/getopt_pp/getopt_pp.o $(OBJPATH)/distanceCalculation/DistanceEstimate.o $(OBJPATH)/ProgressBar.o

objects_cuda = $(OBJPATH)/distanceCalculation/gpu/distanceCalculatorDNA_gpu.o $(OBJPATH)/distanceCalculation/gpu/distanceCalculatorProtein_gpu.o $(OBJPATH)/distanceCalculation/gpu/kimuraDNA_gpu.o

all: rapidnj
	echo $(LINK_OPTIONS)
	echo all: make complete

32 : SWITCHES=-m32
32 : all

64 : SWITCHES=-m64
64 : all

rapidnj: $(objects)
	$(LINK) $(DEBUG) $(LINK_OPTIONS) $(LINKER_COMMANDS)$(BINPATH)/$@ $+ $(LIBRARIES)

rapidnj_cuda: $(objects_cuda) $(objects)
	$(CUDACC) $(DEBUG_CUDA) $(OPTIMIZATION_LEVEL) $(LIBRARIES) -o $(BINPATH)/$@ $+

$(OBJPATH)/%.o: $(SRCPATH)/%.cu
	$(CUDACC) $(DEBUG_CUDA) $(INCLUDES) $(OPTIMIZATION_LEVEL) -o $@ -c $<   

# compile to objectfiles
$(OBJPATH)/%.o: $(SRCPATH)/%.cpp
	$(CC) $(DEBUG) $(SWITCHES) $(OPTIMIZATION_LEVEL) $(INCLUDES) $(COMPILEFLAG) $(OBJECTFLAG)$@ $<   

# clean target
clean: 	
	 -rm $(OBJPATH)/*.o
	 -rm $(OBJPATH)/distanceCalculation/*.o
	 -rm $(OBJPATH)/getopt_pp/*.o
	 -rm $(BINPATH)/*
	 echo clean: make complete

release:
	mkdir rapidNJ
	cp -R src rapidNJ/
	cp -R obj rapidNJ/
	cp -R bin rapidNJ/
	cp Makefile rapidNJ/
	find . -name "*.~" -exec rm {} \;
	zip rapidNJ.zip rapidNJ/INSTALL rapidNJ/Makefile rapidNJ/src/* rapidNJ/src/distanceCalculation/* rapidNJ/src/distanceCalculation/gpu/* rapidNJ/src/getopt_pp/* rapidNJ/obj rapidNJ/obj/distanceCalculation rapidNJ/obj/distanceCalculation/gpu rapidNJ/obj/getopt_pp rapidNJ/bin
	rm -Rf rapidNJ/

util:	$(SRCPATH)/distanceCalculation/sim_seq.cpp
	$(CC) $(DEBUG) $(SWITCHES) $(OPTIMIZATION_LEVEL) $(INCLUDES) $(COMPILEFLAG) $(OBJECTFLAG)$(OBJPATH)/distanceCalculation/sim_seq.o $<
	$(LINK) $(DEBUG) $(LINK_OPTIONS) $(LINKER_COMMANDS)$(BINPATH)/sim_seq $(OBJPATH)/distanceCalculation/sim_seq.o

#Make sure that make rebuilds files if included headers change:
$(objects): $(SRCPATH)/stdinclude.h $(SRCPATH)/polytree.h $(SRCPATH)/diskMatrix.h $(SRCPATH)/rdDataInitialiser.h $(SRCPATH)/distMatrixReader.hpp $(SRCPATH)/node.h $(SRCPATH)/rapidNJ.h $(SRCPATH)/rapidNJDisk.h $(SRCPATH)/rapidNJMem.hpp $(SRCPATH)/simpleNJ.h $(SRCPATH)/testNJ.h $(SRCPATH)/distanceCalculation/dataloader.hpp $(SRCPATH)/distanceCalculation/hammingDistance.hpp $(SRCPATH)/distanceCalculation/bitDistanceGap.hpp $(SRCPATH)/distanceCalculation/dnaBitString.hpp $(SRCPATH)/distanceCalculation/JCdistance.hpp $(SRCPATH)/distanceCalculation/simpleDistanceCalculator.hpp $(SRCPATH)/distanceCalculation/dataloaderStockholm.hpp $(SRCPATH)/distanceCalculation/dataloaderPhylip.hpp $(SRCPATH)/distanceCalculation/KimuraDistance.hpp $(SRCPATH)/distanceCalculation/bitStringUtils.hpp $(SRCPATH)/distanceCalculation/bitDistanceProtein.hpp $(SRCPATH)/distanceCalculation/dataloaderMemory.hpp $(SRCPATH)/distanceCalculation/dataloaderFasta.hpp $(SRCPATH)/distanceCalculation/gpu/constants.hpp $(SRCPATH)/getopt_pp/getopt_pp.h $(SRCPATH)/distanceCalculation/DistanceEstimate.hpp $(SRCPATH)/ProgressBar.hpp
