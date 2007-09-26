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
OBJECTDIR=build/Release/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/plugin.o \
	${OBJECTDIR}/filereader.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fPIC
CXXFLAGS=-fPIC

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} /usr/lib/firefox/plugins/jnext/FileReader.so

/usr/lib/firefox/plugins/jnext/FileReader.so: ${OBJECTFILES}
	${MKDIR} -p /usr/lib/firefox/plugins/jnext
	${LINK.cc} -fno-rtti -fno-exceptions -rdynamic -lrt -shared -o /usr/lib/firefox/plugins/jnext/FileReader.so ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/plugin.o: plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/plugin.o plugin.cpp

${OBJECTDIR}/filereader.o: filereader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -o ${OBJECTDIR}/filereader.o filereader.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} /usr/lib/firefox/plugins/jnext/FileReader.so

# Subprojects
.clean-subprojects:
