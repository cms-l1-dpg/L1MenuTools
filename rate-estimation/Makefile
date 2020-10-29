######### GENERAL STUFF: DON NOT CONSIDER
ODIR     =  objs
CXX      =  g++
CXXFLAGS =  -std=c++11
CXXFLAGS += -Wall -fPIC -O3
#CXXFLAGS  += -g -pg
#CXXFLAGS  += -Wextra
#CXXFLAGS += -Wextra -Weffc++ #Asking for troubles
#CXXFLAGS += -Wno-reorder #Dirty fix
CXXFLAGS  += $(subst -I, -isystem , $(shell root-config --cflags))

LD         = g++
LDFLAGS    =
LIBS       = $(shell root-config --libs) 

ifneq ($(shell echo $$CMSSW_BASE), )
  CXXFLAGS  += -isystem $(CMSSW_BASE)/src/
  CXXFLAGS  += -isystem $(CMSSW_RELEASE_BASE)/src
  CXXFLAGS  += -isystem $(shell scram tool info boost | awk -F"=" '/INCLUDE=(.*)/{print $$NF}')
  LIBS += -L$(CMSSW_BASE)/lib/$(SCRAM_ARCH)/ -L$(CMSSW_RELEASE_BASE)/lib/$(SCRAM_ARCH)/ \
		  -lFWCoreFWLite -lDataFormatsL1TGlobal
  LIBS += -L$(shell scram tool info boost | awk -F"=" '/LIBDIR=(.*)/{print $$NF}') \
		  -lboost_program_options -lboost_system -lboost_filesystem
endif

TestOBJS = include/L1Ntuple include/L1AlgoFactory include/L1Menu2016 include/L1Plot include/L1TnP include/L1uGT include/PreColumn

ifeq ($(wildcard menulib.cc)$(wildcard menulib.hh), menulib.ccmenulib.hh)
  CXXFLAGS += -DUTM_MENULIB
  TestOBJS += menulib
endif

## Whether to use Muon Eta/Phi at Vtx
CXXFLAGS += -DL1NTUPLE_MUONCORATVTX

OBJS := $(TestOBJS:%=$(ODIR)/%.o)

PROGRAM = testMenu2016

MKBIN = $(CXX) $(CXXFLAGS) `root-config  --libs --cflags` -lMinuit -lGenVector

$(ODIR)/%.o : %.C
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

$(ODIR)/%.o : %.cc
	$(CXX) $(CXXFLAGS) $(CXXDEPFLAGS) -o $@ -c $<

all : $(PROGRAM)

testMenu2016 : $(ODIR)/testMenu2016.o $(OBJS)
	@echo "Linking $(PROGRAM) ..."
	$(LD) $^ $(LIBS)  -o $@
	@echo "done"

clean:
	rm -f *exe *.o $(PROGRAM) $(OBJS) $(ODIR)/*.o


