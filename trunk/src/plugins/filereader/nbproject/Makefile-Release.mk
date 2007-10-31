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
	${OBJECTDIR}/_ext/root/jnext/src/plugins/database/SQLite3/../../common/plugin.o \
	${OBJECTDIR}/SQLite3.o \
	${OBJECTDIR}/_ext/root/jnext/src/plugins/database/SQLite3/../Database.o \
	${OBJECTDIR}/sqlite3.o

# C Compiler Flags
CFLAGS=-fPIC

# CC Compiler Flags
CCFLAGS=-fPIC
CXXFLAGS=-fPIC

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=\
	-L/root/soci-2.2.0/src/core/.libs \
	-L/root/soci-2.2.0/src/backends/sqlite3/.libs \
	-lsoci_core-gcc-g-2_2 \
	-lsoci_sqlite3-gcc-g-2_2

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} /usr/lib/firefox/plugins/jnext/SQLite3.so

/usr/lib/firefox/plugins/jnext/SQLite3.so: ${OBJECTFILES}
	${MKDIR} -p /usr/lib/firefox/plugins/jnext
	${LINK.cc} -fno-rtti -fno-exceptions -rdynamic -lrt -shared -o /usr/lib/firefox/plugins/jnext/SQLite3.so -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/root/jnext/src/plugins/database/SQLite3/../../common/plugin.o: ../../common/plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/root/jnext/src/plugins/database/SQLite3/../../common
	$(COMPILE.cc) -O2 -I../../../../../soci-2.2.0/src/core -I../../../../../soci-2.2.0/src/backends/sqlite3 -o ${OBJECTDIR}/_ext/root/jnext/src/plugins/database/SQLite3/../../common/plugin.o ../../common/plugin.cpp

${OBJECTDIR}/SQLite3.o: SQLite3.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -I../../../../../soci-2.2.0/src/core -I../../../../../soci-2.2.0/src/backends/sqlite3 -o ${OBJECTDIR}/SQLite3.o SQLite3.cpp

${OBJECTDIR}/_ext/root/jnext/src/plugins/database/SQLite3/../Database.o: ../Database.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/root/jnext/src/plugins/database/SQLite3/..
	$(COMPILE.cc) -O2 -I../../../../../soci-2.2.0/src/core -I../../../../../soci-2.2.0/src/backends/sqlite3 -o ${OBJECTDIR}/_ext/root/jnext/src/plugins/database/SQLite3/../Database.o ../Database.cpp

${OBJECTDIR}/sqlite3.o: sqlite3.c 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -o ${OBJECTDIR}/sqlite3.o sqlite3.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} /usr/lib/firefox/plugins/jnext/SQLite3.so

# Subprojects
.clean-subprojects:
