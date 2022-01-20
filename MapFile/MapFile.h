/** \mainpage      MapFile.dll
 * 
 * \section		intro_sec		What is this
 * MapFile.dll is a Windows library for parsing through .map files created by CSG editors like
 * [Trenchbroom](https://trenchbroom.github.io/).  It deserializes the text of the file into
 * a set of C structs which can be used to generate actual world geometry meshes or serve as
 * tokens for creating entities.
 * 
 * See the source code at https://github.com/shortrounddev/mapfile
 * 
 * \subsection		use_sec			What's the use case?
 * 1. For those who don't want to re-invent the wheel by developing their own CSG editors for
 * their own custom engine, but also don't want to implement the specificities of a .bsp file
 * 
 * 2. For those who don't like their current engine's CSG editors (such as Unity) and are more
 * familiar with the quake/valve world of editors

 * \subsection		map_sec			What does a .map file look like
 * This

 * ```c
 * // Game: Generic
 * // Format: Standard
 * // entity 0
 * {
 * "classname" "worldspawn"
 * "_tb_textures" "textures"
 * // brush 0
 * {
 * ( -64 -64 -16 ) ( -64 -63 -16 ) ( -64 -64 -15 ) __TB_empty 0 0 0 1 1
 * ( -64 -64 -16 ) ( -64 -64 -15 ) ( -63 -64 -16 ) __TB_empty 0 0 0 1 1
 * ( -64 -64 -16 ) ( -63 -64 -16 ) ( -64 -63 -16 ) __TB_empty 0 0 0 1 1
 * ( 64 64 16 ) ( 64 65 16 ) ( 65 64 16 ) __TB_empty 0 0 0 1 1
 * ( 64 64 16 ) ( 65 64 16 ) ( 64 64 17 ) __TB_empty 0 0 0 1 1
 * ( 64 64 16 ) ( 64 64 17 ) ( 64 65 16 ) __TB_empty 0 0 0 1 1
 * }
 * // brush 1
 * {
 * ( -16 -48 80 ) ( -16 32 16 ) ( -16 32 80 ) __TB_empty 0.25 0.5 33.5 2.3 4.6
 * ( 16 -48 80 ) ( -16 -48 16 ) ( -16 -48 80 ) __TB_empty 0.25 0.5 33.5 2.3 4.6
 * ( 16 32 16 ) ( -16 -48 16 ) ( 16 -48 16 ) __TB_empty 0.25 0.5 33.5 2.3 4.6
 * ( 16 32 80 ) ( -16 -48 80 ) ( -16 32 80 ) __TB_empty 0.25 0.5 33.5 2.3 4.6
 * ( 16 32 80 ) ( -16 32 16 ) ( 16 32 16 ) __TB_empty 0.25 0.5 33.5 2.3 4.6
 * ( 16 32 80 ) ( 16 -48 16 ) ( 16 -48 80 ) __TB_empty 0.25 0.5 33.5 2.3 4.6
 * }
 * }
 * // entity 1
 * {
 * "classname" "info_player_start"
 * "origin" "-48 0 40"
 * "customKey" "1234565"
 * }
 * ```

 * Everything in a map file is an entity of some kind (a dictionary of string-string key value pairs), but some entities have brushes in them, which are bounding planes which define the faces of each brush as well as the texture information for each face

 * If a map needs some kind of global variable, it's just a singleton entity with attributes on it (water_lod_control in Source engine, for example)

 * \subsection		installation_sec		Installation
 * Todo Upon release

 * \subsection		usage_sec				Usage

 * MapFile currently can parse brush information into structs. As an example of that, check out the example in HelloMapFile:
 * ```c
 * #include "MapFile.h"
 * #include <stdio.h>
 *
 * // runs MF_Parse and prints the first attribute of the first entity,
 * // or the error code, if there is an error 
 * void parse_and_print(char* text);
 *
 * int main(int argc, char** argv)
 * {
 *     MF_Init();
 *
 *     parse_and_print("\"classname\" \"entity\"}");  // missing opening bracket; should print error
 *     parse_and_print("{\"classname\" \"entity\"}"); // correct format
 *
 *     return 0;
 * }
 *
 * void parse_and_print(char* text)
 * {
 *     MF_Map_t map;
 *     BOOL success = MF_Parse(text, &map);
 *     if (success) {
 *         printf("Success!\n");
 *         printf("Total Entities: %zu\n", map.totalItems);
 *
 *         MF_KeyValuePair_t attr1 = map.items[0].attributes[0];
 *
 *         printf("Entity #1 attribute: %s = %s\n\n", attr1.key, attr1.value);
 *     }
 *     else {
 *         printf("Failure!\n");
 *         int error_code = MF_GetErrCode();
 *         char buffer[1024] = { 0 };
 *         MF_GetErrMessage(buffer);
 *         printf("Error [0x%x]: %s\n\n", error_code, buffer);
 *     }
 *     MF_ResetErr();
 * }
 * ```
 *
 * This will print the following :
 * ```
 * Failure!
 * Error[0x2000001]: No entities in map file counted
 *
 * Success!
 * Total Entities : 1
 * Entity #1 attribute: classname = entity
 * ```
 * 
 * \subsection		roadmap_sec		Roadmap / What's next
 *
 * 1. Implement MF_Map destructor.
 * 2. Add the geometry functions required to create a point cloud or mesh from the intersections of the brushes
 * 3. Add C# Bindings(and other bindings if anyone wants them)
 * \subsection		faq_sec			FAQ
 * > Why not just export a wavefront.obj file ?
 *
 * You can do that for geometry, but it doesn't include entity information
 *
 * > I'm an unreal user; why shouldn't I just use [HAMMUER](https://nte.itch.io/hammuer)?
 *
 * You probably should! That can convert compiled BSPsand, as far as I know, includes light computations as well.
 * Additionally, it's not just a dll file that you need to implement functionality for yourself: it's a full software
 * package which does the importing for you. Howevever, it's $42, and MapFile.dll is free. Also, MapFile.dll is MIT licensed
 * so if you need to modify it in some way, you can
 */

#pragma once

#include "MapFile_Types.h"
#include "MapFile_System.h"
#include "MapFile_List.h"
#include "MapFile_Lex.h"
#include "MapFile_Parse.h"
#include "MapFile_Geometry.h"