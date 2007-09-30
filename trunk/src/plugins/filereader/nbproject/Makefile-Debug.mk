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
	${OBJECTDIR}/filereader.o \
	${OBJECTDIR}/_ext/root/jnext/src/plugins/filereader/../common/plugin.o

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

${OBJECTDIR}/filereader.o: filereader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -o ${OBJECTDIR}/filereader.o filereader.cpp

${OBJECTDIR}/_ext/root/jnext/src/plugins/filereader/../common/plugin.o: ../common/plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/root/jnext/src/plugins/filereader/../common
	$(COMPILE.cc) -g -o ${OBJECTDIR}/_ext/root/jnext/src/plugins/filereader/../common/plugin.o ../common/plugin.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} /usr/lib/firefox/plugins/jnext/FileReader.so

# Subprojects
.clean-subprojects:
