/**
*	CONFIG.H FILE
*	Set the configuration of the project
*/

#pragma once

#ifndef CONFIG_H
#define CONFIG_H

/* Foam size */
#define xFoamSize x /* width */
#define yFoamSize y /* heigth */
#define zFoamSize z /* length */
#define foamSize(xFoamSize,yFoamSize,zFoamSize) foamSize(xFoamSize,yFoamSize,zFoamSize)

/* Foam Cutter size */
#define xCutterSize xCutter  /* width */
#define yCutterSize yCutter /* heigth */
#define zCutterSize zCutter /* length */
#define circleSize rCutter /* Size of the lazy Susan */
#define sizeCutter(xCutterSize,yCutterSize,zCutterSize,circleSize) sizeCutter(xCutterSize,yCutterSize,zCutterSize,circleSize)

/* GCODE actions */
// G actions
#define G0 fastMotion
#define G1 workMotion
#define G4 wait
#define xG28 xPosition
#define yG28 yPosition
#define uG28 uPosition
#define vG28 vPosition
#define G28(xG28,yG28,uG28,vG28) potision(xG28,yG28,uG28,vG28)
#define G90 absoluteCoordinate
#define G91 relativeCoordinate
#define G92 changeOrigin

// M actions
#define M0_M1(a) (a || b)
#define M2 endOfProgram
#define M3 heatWire
#define M5 stopHeatWire
#define M17 powerMotor
#define M18_M84 stopPowerMotor
#define M45 enableXZMirror
#define M46 disableXZMirror
#define M47 enableYEMirror
#define M48 disableYEMirror
#define M85 waitStopMotor
#define M114 returnPosition
#define M115 serialProtocol
#define M117 displayMeesage
#define M119 reportEndswitch
#define M120 disableEndstop
#define M121 enableEndStop
#define xM201 xMaxAcceleration
#define yM201 yMaxAcceleration
#define uM201 uMaxAcceleration
#define vM201 vMaxAcceleration
#define M201(xM201,yM201,uM201,vM201) maxAcceleration(xM201,yM201,uM201,vM201)
#define xM202 xAxisStep
#define yM202 yAxisStep
#define uM202 uAxisStep
#define vM202 vAxisStep
#define M202(xM202,yM202,uM202,vM202) axisStep(xM202,yM202,uM202,vM202)
#define xM203 xMaxFeedrate
#define yM203 yMaxFeedrate
#define uM203 uMaxFeedrate
#define vM203 vMaxFeedrate
#define M203(xM203,yM203,uM203,vM203) maxFeedRate(xM203,yM203,uM203,vM203)
#define xM206 xAddOffset
#define yM206 yAddOffset
#define uM206 uAddOffset
#define vM206 vAddOffset
#define M206(xM206,yM206,uM206,vM206) addOffset(xM206,yM206,uM206,vM206)
#define M220 speedFactor
#define M400 finishAllMoves

/* Rest of actions */
#define S heatData
#define F workData

#endif // CONFIG_H