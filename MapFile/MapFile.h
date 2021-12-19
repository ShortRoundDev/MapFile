/** \mainpage      MapFile.dll
 * 
 * \section       intro_sec      Introduction
 *  
 * MapFile.dll is a Windows library for loading and parsing quake-family
 * .map files from CSG editors. The idea is for people with custom engines
 * to be able to use CSG editors like
 * <a href="https://trenchbroom.github.io/">Trenchbroom</a>
 * to edit their levels without having to implement the specific BSP formats
 * that they usually produce.
 * 
 * "Why not use an obj file?" Because obj files don't export entity information.
 * This way you can define entity information for a generic game for Trenchbroom
 * and even export entity information with your map files.
 *
 * The library is under MIT license
 * 
 * \section       intallation_sec     Installation
 * 
 * To do
 */

#pragma once

#include "MapFile_Types.h"
#include "MapFile_System.h"
#include "MapFile_List.h"
#include "MapFile_Lex.h"
#include "MapFile_Parse.h"