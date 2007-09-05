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
	${OBJECTDIR}/sockets.o

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
	/root/pwlib/lib/libpt_linux_x86_d_s.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} /usr/lib/firefox/plugins/js2n/Sockets.so

/usr/lib/firefox/plugins/js2n/Sockets.so: ${OBJECTFILES}
	${MKDIR} -p /usr/lib/firefox/plugins/js2n
	${LINK.cc} -fno-rtti -fno-exceptions -rdynamic -shared -o /usr/lib/firefox/plugins/js2n/Sockets.so ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/sockets.o: sockets.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -g -I/root/pwlib/include -o ${OBJECTDIR}/sockets.o sockets.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} /usr/lib/firefox/plugins/js2n/Sockets.so

# Subprojects
.clean-subprojects:
