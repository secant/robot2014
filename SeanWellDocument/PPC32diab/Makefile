# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = PPC32diab
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
WS_ROOT_DIR := C:/Users/mhsrobotics/Desktop/robot2014
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/SeanWellDocument



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
EXPAND_DBG = 0


#BuildSpec specific Build Macros
VX_CPU_FAMILY = ppc
CPU = PPC32
TOOL_FAMILY = diab
TOOL = diab
TOOL_PATH = 
CC_ARCH_SPEC = -tPPCFH:vxworks63 -Xstmw-slow -DPPC32_fp60x
LIBPATH = 
LIBS = 

IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 

IDE_LIBRARIES = 

IDE_DEFINES = 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -g
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
else
DEBUGFLAGS_C-Compiler = -XO -Xsize-opt
DEBUGFLAGS_C++-Compiler = -XO -Xsize-opt
DEBUGFLAGS_Linker = -XO -Xsize-opt
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -XO -Xsize-opt
endif


#Project Targets
PROJECT_TARGETS = SeanWellDocument/$(MODE_DIR)/SeanWellDocument.out \
	 SeanWellDocument_partialImage/$(MODE_DIR)/SeanWellDocument_partialImage.o


#Rules

# SeanWellDocument
ifeq ($(DEBUG_MODE),1)
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -XO -Xsize-opt
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -XO -Xsize-opt
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -XO -Xsize-opt
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
SeanWellDocument/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -XO -Xsize-opt
endif
SeanWellDocument/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
SeanWellDocument/$(MODE_DIR)/% : IDE_LIBRARIES = 
SeanWellDocument/$(MODE_DIR)/% : IDE_DEFINES = 
SeanWellDocument/$(MODE_DIR)/% : PROJECT_TYPE = DKM
SeanWellDocument/$(MODE_DIR)/% : DEFINES = 
SeanWellDocument/$(MODE_DIR)/% : EXPAND_DBG = 0
SeanWellDocument/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
SeanWellDocument/$(MODE_DIR)/% : CPU = PPC32
SeanWellDocument/$(MODE_DIR)/% : TOOL_FAMILY = diab
SeanWellDocument/$(MODE_DIR)/% : TOOL = diab
SeanWellDocument/$(MODE_DIR)/% : TOOL_PATH = 
SeanWellDocument/$(MODE_DIR)/% : CC_ARCH_SPEC = -tPPCFH:vxworks63 -Xstmw-slow -DPPC32_fp60x
SeanWellDocument/$(MODE_DIR)/% : LIBPATH = 
SeanWellDocument/$(MODE_DIR)/% : LIBS = 
SeanWellDocument/$(MODE_DIR)/% : OBJ_DIR := SeanWellDocument/$(MODE_DIR)

OBJECTS_SeanWellDocument = SeanWellDocument_partialImage/$(MODE_DIR)/SeanWellDocument_partialImage.o

SeanWellDocument/$(MODE_DIR)/SeanWellDocument.out : $(OBJECTS_SeanWellDocument)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";ddump -Ng $(OBJECTS_SeanWellDocument) $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)dcc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -Xdollar-in-ident -Xforce-declarations $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)dld -tPPCFH:vxworks63 -X -r5 -r4 -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_SeanWellDocument) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS)  $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

SeanWellDocument/$(MODE_DIR)/SeanWellDocument_compile_file : $(FILE) ;

_clean :: SeanWellDocument/$(MODE_DIR)/SeanWellDocument_clean

SeanWellDocument/$(MODE_DIR)/SeanWellDocument_clean : 
	$(TRACE_FLAG)if [ -d "SeanWellDocument" ]; then cd "SeanWellDocument"; rm -rf $(MODE_DIR); fi


# SeanWellDocument_partialImage
ifeq ($(DEBUG_MODE),1)
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -XO -Xsize-opt
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -XO -Xsize-opt
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -XO -Xsize-opt
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -XO -Xsize-opt
endif
SeanWellDocument_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
SeanWellDocument_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : IDE_DEFINES = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
SeanWellDocument_partialImage/$(MODE_DIR)/% : DEFINES = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
SeanWellDocument_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
SeanWellDocument_partialImage/$(MODE_DIR)/% : CPU = PPC32
SeanWellDocument_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = diab
SeanWellDocument_partialImage/$(MODE_DIR)/% : TOOL = diab
SeanWellDocument_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -tPPCFH:vxworks63 -Xstmw-slow -DPPC32_fp60x
SeanWellDocument_partialImage/$(MODE_DIR)/% : LIBPATH = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : LIBS = 
SeanWellDocument_partialImage/$(MODE_DIR)/% : OBJ_DIR := SeanWellDocument_partialImage/$(MODE_DIR)

OBJECTS_SeanWellDocument_partialImage =

SeanWellDocument_partialImage/$(MODE_DIR)/SeanWellDocument_partialImage.o : $(OBJECTS_SeanWellDocument_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dld -tPPCFH:vxworks63 -X -r5  -o "$@" $(OBJECTS_SeanWellDocument_partialImage) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)  && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

SeanWellDocument_partialImage/$(MODE_DIR)/SeanWellDocument_partialImage_compile_file : $(FILE) ;

_clean :: SeanWellDocument_partialImage/$(MODE_DIR)/SeanWellDocument_partialImage_clean

SeanWellDocument_partialImage/$(MODE_DIR)/SeanWellDocument_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "SeanWellDocument_partialImage" ]; then cd "SeanWellDocument_partialImage"; rm -rf $(MODE_DIR); fi

force : 



-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

main_all : external_build $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""
