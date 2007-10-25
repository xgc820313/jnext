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
	${OBJECTDIR}/SQLite3.o \
	${OBJECTDIR}/_ext/root/jnext/src/plugins/SQLite3/../common/plugin.o

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
.build-conf: ${BUILD_SUBPROJECTS} /usr/lib/firefox/plugins/jnext/SQLite3.so

/usr/lib/firefox/plugins/jnext/SQLite3.so: ${OBJECTFILES}
	${MKDIR} -p /usr/lib/firefox/plugins/jnext
	${LINK.cc} -fno-rtti -fno-exceptions -rdynamic -lrt -shared -o /usr/lib/firefox/plugins/jnext/SQLite3.so ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/SQLite3.o: SQLite3.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -o ${OBJECTDIR}/SQLite3.o SQLite3.cpp

${OBJECTDIR}/_ext/root/jnext/src/plugins/SQLite3/../common/plugin.o: ../common/plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/root/jnext/src/plugins/SQLite3/../common
	$(COMPILE.cc) -g -o ${OBJECTDIR}/_ext/root/jnext/src/plugins/SQLite3/../common/plugin.o ../common/plugin.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} /usr/lib/firefox/plugins/jnext/SQLite3.so

# Subprojects
.clean-subprojects:
