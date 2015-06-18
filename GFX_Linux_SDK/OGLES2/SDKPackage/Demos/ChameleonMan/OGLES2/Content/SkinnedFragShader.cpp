// This file was created by Filewrap 1.1
// Little endian mode
// DO NOT EDIT

#include "../PVRTMemoryFileSystem.h"

// using 32 bit to guarantee alignment.
#ifndef A32BIT
 #define A32BIT static const unsigned int
#endif

// ******** Start: SkinnedFragShader.fsh ********

// File data
static const char _SkinnedFragShader_fsh[] = 
	"uniform sampler2D sTexture;\r\n"
	"uniform sampler2D sNormalMap;\r\n"
	"uniform bool bUseDot3;\r\n"
	"\r\n"
	"varying mediump vec2 TexCoord;\r\n"
	"varying mediump vec3 Light;\r\n"
	"\r\n"
	"void main()\r\n"
	"{\r\n"
	"\tif(bUseDot3)\r\n"
	"\t{\r\n"
	"\t\t/*\r\n"
	"\t\t\tNote:\r\n"
	"\t\t\tIn the normal map red = y, green = x, blue = z which is why when we get the normal\r\n"
	"\t\t\tfrom the texture we use the swizzle .grb so the colours are mapped to the correct\r\n"
	"\t\t\tco-ordinate variable.\r\n"
	"\t\t*/\r\n"
	"\r\n"
	"\t\tmediump vec3 fNormal = texture2D(sNormalMap, TexCoord).grb;\r\n"
	"\t\tmediump float fNDotL = dot((fNormal - 0.5) * 2.0, Light);\r\n"
	"\t\t\r\n"
	"\t\tgl_FragColor = texture2D(sTexture, TexCoord) * fNDotL;\r\n"
	"    }\r\n"
	"    else\r\n"
	"\t\tgl_FragColor = texture2D(sTexture, TexCoord) * Light.x;\r\n"
	"}\r\n";

// Register SkinnedFragShader.fsh in memory file system at application startup time
static CPVRTMemoryFileSystem RegisterFile_SkinnedFragShader_fsh("SkinnedFragShader.fsh", _SkinnedFragShader_fsh, 672);

// ******** End: SkinnedFragShader.fsh ********

