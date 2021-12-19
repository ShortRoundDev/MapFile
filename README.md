# MapFile.dll

## What is this
MapFile.dll is a Windows library for parsing through .map files created by CSG editors like [Trenchbroom](https://trenchbroom.github.io/).  It deserializes the text of the file into a set of C structs which can be used to generate actual world geometry meshes or serve as tokens for creating entities.

## What's the use case?
1. For those who don't want to re-invent the wheel by developing their own CSG editors for their own custom engine, but also don't want to implement the specificities of a .bsp file

2. For those who don't like their current engine's CSG editors (such as Unity) and are more familiar with the Quake/Valve world of editors

## Installation
Todo Upon release

## FAQ
> Why not just export a wavefront .obj file?

You can do that for geometry, but it doesn't include entity information

> I'm an unreal user; why shouldn't I just use [HAMM**UE**R](https://nte.itch.io/hammuer)?

You probably should! That can convert compiled BSPs and, as far as I know, includes light computations as well. Additionally, it's not just a dll file that you need to implement functionality for yourself: it's a full software package which does the importing for you. Howevever, it's $42, and MapFile.dll is free. Also, MapFile.dll is MIT licensed so if you need to modify it in some way, you can