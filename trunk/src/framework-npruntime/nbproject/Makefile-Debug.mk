#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add custumized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/Debug/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/plugin.o \
	${OBJECTDIR}/nprn.o \
	${OBJECTDIR}/_ext/root/js2n/src/framework-npruntime/../common/binreloc.o \
	${OBJECTDIR}/npp_gate.o \
	${OBJECTDIR}/npn_gate.o \
	${OBJECTDIR}/np_entry.o \
	${OBJECTDIR}/_ext/root/js2n/src/framework-npruntime/../common/nativelogic.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fPIC
CXXFLAGS=-fPIC

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=\
	-L/root/pwlib/lib \
	-lpt_linux_x86_d_s

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} /usr/lib/firefox/plugins/nprn.so

/usr/lib/firefox/plugins/nprn.so: ${OBJECTFILES}
	${MKDIR} -p /usr/lib/firefox/plugins
	${LINK.cc} -fno-rtti -fno-exceptions -shared -o /usr/lib/firefox/plugins/nprn.so ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/plugin.o: plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DENABLE_BINRELOC -DXPCOM_GLUE -DMOZILLA_STRICT_API -DXP_UNIX -DMOZ_X11 -Igecko-sdk/include -I/root/pwlib/include -o ${OBJECTDIR}/plugin.o plugin.cpp

${OBJECTDIR}/nprn.o: nprn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DENABLE_BINRELOC -DXPCOM_GLUE -DMOZILLA_STRICT_API -DXP_UNIX -DMOZ_X11 -Igecko-sdk/include -I/root/pwlib/include -o ${OBJECTDIR}/nprn.o nprn.cpp

${OBJECTDIR}/_ext/root/js2n/src/framework-npruntime/../common/binreloc.o: ../common/binreloc.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/root/js2n/src/framework-npruntime/../common
	$(COMPILE.c) -g -DENABLE_BINRELOC -o ${OBJECTDIR}/_ext/root/js2n/src/framework-npruntime/../common/binreloc.o ../common/binreloc.c

${OBJECTDIR}/npp_gate.o: npp_gate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DENABLE_BINRELOC -DXPCOM_GLUE -DMOZILLA_STRICT_API -DXP_UNIX -DMOZ_X11 -Igecko-sdk/include -I/root/pwlib/include -o ${OBJECTDIR}/npp_gate.o npp_gate.cpp

${OBJECTDIR}/npn_gate.o: npn_gate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DENABLE_BINRELOC -DXPCOM_GLUE -DMOZILLA_STRICT_API -DXP_UNIX -DMOZ_X11 -Igecko-sdk/include -I/root/pwlib/include -o ${OBJECTDIR}/npn_gate.o npn_gate.cpp

${OBJECTDIR}/np_entry.o: np_entry.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -DENABLE_BINRELOC -DXPCOM_GLUE -DMOZILLA_STRICT_API -DXP_UNIX -DMOZ_X11 -Igecko-sdk/include -I/root/pwlib/include -o ${OBJECTDIR}/np_entry.o np_entry.cpp

${OBJECTDIR}/_ext/root/js2n/src/framework-npruntime/../common/nativelogic.o: ../common/nativelogic.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/root/js2n/src/framework-npruntime/../common
	$(COMPILE.cc) -g -DENABLE_BINRELOC -DXPCOM_GLUE -DMOZILLA_STRICT_API -DXP_UNIX -DMOZ_X11 -Igecko-sdk/include -I/root/pwlib/include -o ${OBJECTDIR}/_ext/root/js2n/src/framework-npruntime/../common/nativelogic.o ../common/nativelogic.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} /usr/lib/firefox/plugins/nprn.so

# Subprojects
.clean-subprojects:
