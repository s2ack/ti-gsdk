// This file was created by Filewrap 1.1
// Little endian mode
// DO NOT EDIT

#include "../PVRTMemoryFileSystem.h"

// using 32 bit to guarantee alignment.
#ifndef A32BIT
 #define A32BIT static const unsigned int
#endif

// ******** Start: DefaultVertShader.vsh ********

// File data
static const char _DefaultVertShader_vsh[] = 
	"attribute highp   vec3 inVertex;\r\n"
	"attribute mediump vec2 inTexCoord;\r\n"
	"\r\n"
	"uniform highp   mat4 MVPMatrix;\r\n"
	"uniform float\tfUOffset;\r\n"
	"\r\n"
	"varying mediump vec2  TexCoord;\r\n"
	"\r\n"
	"void main()\r\n"
	"{\r\n"
	"\tgl_Position = MVPMatrix * vec4(inVertex, 1.0);\r\n"
	"\r\n"
	"\t// Pass through texcoords\r\n"
	"\tTexCoord = inTexCoord;\r\n"
	"\tTexCoord.x += fUOffset;\r\n"
	"}\r\n"
	" ";

// Register DefaultVertShader.vsh in memory file system at application startup time
static CPVRTMemoryFileSystem RegisterFile_DefaultVertShader_vsh("DefaultVertShader.vsh", _DefaultVertShader_vsh, 317);

// ******** End: DefaultVertShader.vsh ********

