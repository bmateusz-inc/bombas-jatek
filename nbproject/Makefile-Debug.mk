#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/client.o \
	${OBJECTDIR}/config.o \
	${OBJECTDIR}/flame.o \
	${OBJECTDIR}/game.o \
	${OBJECTDIR}/gamemanager.o \
	${OBJECTDIR}/gametable.o \
	${OBJECTDIR}/goods.o \
	${OBJECTDIR}/input.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/menu.o \
	${OBJECTDIR}/monster.o \
	${OBJECTDIR}/msgbox.o \
	${OBJECTDIR}/network.o \
	${OBJECTDIR}/player.o \
	${OBJECTDIR}/scoreboard.o \
	${OBJECTDIR}/select.o \
	${OBJECTDIR}/server.o \
	${OBJECTDIR}/textinput.o \
	${OBJECTDIR}/translate.o \
	${OBJECTDIR}/utils.o \
	${OBJECTDIR}/view/animfile.o \
	${OBJECTDIR}/view/gles.o \
	${OBJECTDIR}/view/lodepng/lodepng.o \
	${OBJECTDIR}/view/multi.o \
	${OBJECTDIR}/view/sprite.o \
	${OBJECTDIR}/view/text.o \
	${OBJECTDIR}/view/view.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lOpenGL32 -lSDL -lSDL_net icon.res

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dyna.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dyna.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dyna ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/client.o: client.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/client.o client.cpp

${OBJECTDIR}/config.o: config.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/config.o config.cpp

${OBJECTDIR}/flame.o: flame.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/flame.o flame.cpp

${OBJECTDIR}/game.o: game.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/game.o game.cpp

${OBJECTDIR}/gamemanager.o: gamemanager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gamemanager.o gamemanager.cpp

${OBJECTDIR}/gametable.o: gametable.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gametable.o gametable.cpp

${OBJECTDIR}/goods.o: goods.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/goods.o goods.cpp

${OBJECTDIR}/input.o: input.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/input.o input.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/menu.o: menu.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/menu.o menu.cpp

${OBJECTDIR}/monster.o: monster.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/monster.o monster.cpp

${OBJECTDIR}/msgbox.o: msgbox.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/msgbox.o msgbox.cpp

${OBJECTDIR}/network.o: network.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/network.o network.cpp

${OBJECTDIR}/player.o: player.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/player.o player.cpp

${OBJECTDIR}/scoreboard.o: scoreboard.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/scoreboard.o scoreboard.cpp

${OBJECTDIR}/select.o: select.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/select.o select.cpp

${OBJECTDIR}/server.o: server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/server.o server.cpp

${OBJECTDIR}/textinput.o: textinput.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/textinput.o textinput.cpp

${OBJECTDIR}/translate.o: translate.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/translate.o translate.cpp

${OBJECTDIR}/utils.o: utils.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utils.o utils.cpp

${OBJECTDIR}/view/animfile.o: view/animfile.cpp 
	${MKDIR} -p ${OBJECTDIR}/view
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/view/animfile.o view/animfile.cpp

${OBJECTDIR}/view/gles.o: view/gles.cpp 
	${MKDIR} -p ${OBJECTDIR}/view
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/view/gles.o view/gles.cpp

${OBJECTDIR}/view/lodepng/lodepng.o: view/lodepng/lodepng.cpp 
	${MKDIR} -p ${OBJECTDIR}/view/lodepng
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/view/lodepng/lodepng.o view/lodepng/lodepng.cpp

${OBJECTDIR}/view/multi.o: view/multi.cpp 
	${MKDIR} -p ${OBJECTDIR}/view
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/view/multi.o view/multi.cpp

${OBJECTDIR}/view/sprite.o: view/sprite.cpp 
	${MKDIR} -p ${OBJECTDIR}/view
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/view/sprite.o view/sprite.cpp

${OBJECTDIR}/view/text.o: view/text.cpp 
	${MKDIR} -p ${OBJECTDIR}/view
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/view/text.o view/text.cpp

${OBJECTDIR}/view/view.o: view/view.cpp 
	${MKDIR} -p ${OBJECTDIR}/view
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I. -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/view/view.o view/view.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dyna.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
