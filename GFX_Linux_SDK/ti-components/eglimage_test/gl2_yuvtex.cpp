/* gl2_yuv.cpp
 * This is an example/sample test application to show simple use case of eglimage extension
 * This is being reused from RobClark's eglimage work.
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <sched.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

static void printGLString(const char *name, GLenum s) {
	// fprintf(stderr, "printGLString %s, %d\n", name, s);
	const char *v = (const char *) glGetString(s);
	// int error = glGetError();
	// fprintf(stderr, "glGetError() = %d, result of glGetString = %x\n", error,
	//        (unsigned int) v);
	// if ((v < (const char*) 0) || (v > (const char*) 0x10000))
	//    fprintf(stderr, "GL %s = %s\n", name, v);
	// else
	//    fprintf(stderr, "GL %s = (null) 0x%08x\n", name, (unsigned int) v);
	fprintf(stderr, "GL %s = %s\n", name, v);
}

static void checkEglError(const char* op, EGLBoolean returnVal = EGL_TRUE) {
	if (returnVal != EGL_TRUE) {
		fprintf(stderr, "%s() returned %d\n", op, returnVal);
	}

	for (EGLint error = eglGetError(); error != EGL_SUCCESS; error = eglGetError()) {
		fprintf(stderr, "after %s() eglError 0x%x\n", op, error);
	}
}

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		fprintf(stderr, "after %s() glError (0x%x)\n", op, error);
	}
}

static const char gVertexShader[] =
        "attribute vec4 vPosition;\n"
		"varying vec2 yuvTexCoords;\n"
		"void main() {\n"
		"  yuvTexCoords = vPosition.xy + vec2(0.5, 0.5);\n"
		"  gl_Position = vPosition;\n"
		"}\n";

static const char gFragmentShader[] =
        "#extension GL_OES_EGL_image_external : require\n"
		"precision mediump float;\n"
		"uniform samplerExternalOES yuvTexSampler;\n"
		"varying vec2 yuvTexCoords;\n"
		"void main() {\n"
		"  gl_FragColor = texture2D(yuvTexSampler, yuvTexCoords);\n"
		"}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
	GLuint shader = glCreateShader(shaderType);
	if (shader) {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					fprintf(stderr, "Could not compile shader %d:\n%s\n",
							shaderType, buf);
					free(buf);
				}
			} else {
				fprintf(stderr, "Guessing at GL_INFO_LOG_LENGTH size\n");
				char* buf = (char*) malloc(0x1000);
				if (buf) {
					glGetShaderInfoLog(shader, 0x1000, NULL, buf);
					fprintf(stderr, "Could not compile shader %d:\n%s\n",
							shaderType, buf);
					free(buf);
				}
			}
			glDeleteShader(shader);
			shader = 0;
		}
	}
	return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		checkGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					fprintf(stderr, "Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}

GLuint gProgram;
GLint gvPositionHandle;
GLint gYuvTexSamplerHandle;

bool setupGraphics(int w, int h) {
	gProgram = createProgram(gVertexShader, gFragmentShader);
	if (!gProgram) {
		return false;
	}
	gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	checkGlError("glGetAttribLocation");
	fprintf(stderr, "glGetAttribLocation(\"vPosition\") = %d\n",
			gvPositionHandle);
	gYuvTexSamplerHandle = glGetUniformLocation(gProgram, "yuvTexSampler");
	checkGlError("glGetUniformLocation");
	fprintf(stderr, "glGetUniformLocation(\"yuvTexSampler\") = %d\n",
			gYuvTexSamplerHandle);

	glViewport(0, 0, w, h);
	checkGlError("glViewport");
	return true;
}

int align(int x, int a) {
	return (x + (a-1)) & (~(a-1));
}

static GLuint yuvTex;

#define FOURCC(a, b, c, d) ((uint32_t)(uint8_t)(a) | ((uint32_t)(uint8_t)(b) << 8) | ((uint32_t)(uint8_t)(c) << 16) | ((uint32_t)(uint8_t)(d) << 24 ))
#define FOURCC_STR(str)    FOURCC(str[0], str[1], str[2], str[3])

#define WIN_WIDTH  500
#define WIN_HEIGHT 500
#define VID_WIDTH  1024
#define VID_HEIGHT 1024
#define VID_SIZE   (VID_WIDTH * VID_HEIGHT * 3) / 2
#define PAGE_SIZE  4096

#ifndef EGL_TI_raw_video
#  define EGL_TI_raw_video 1
#  define EGL_RAW_VIDEO_TI						0x333A	/* eglCreateImageKHR target */
#  define EGL_GL_VIDEO_FOURCC_TI				0x3331	/* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_WIDTH_TI					0x3332	/* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_HEIGHT_TI				0x3333	/* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_BYTE_STRIDE_TI			0x3334	/* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_BYTE_SIZE_TI				0x3335	/* eglCreateImageKHR attribute */
#  define EGL_GL_VIDEO_YUV_FLAGS_TI				0x3336	/* eglCreateImageKHR attribute */
#endif

#ifndef EGLIMAGE_FLAGS_YUV_CONFORMANT_RANGE
#  define EGLIMAGE_FLAGS_YUV_CONFORMANT_RANGE (0 << 0)
#  define EGLIMAGE_FLAGS_YUV_FULL_RANGE       (1 << 0)
#  define EGLIMAGE_FLAGS_YUV_BT601            (0 << 1)
#  define EGLIMAGE_FLAGS_YUV_BT709            (1 << 1)
#endif

extern "C" {
/* swap these for big endian.. */
#define RED   2
#define GREEN 1
#define BLUE  0

static void fill420(unsigned char *y, unsigned char *u, unsigned char *v,
		int cs /*chroma pixel stride */,
		int n, int width, int height, int stride)
{
	int i, j;

	/* paint the buffer with colored tiles, in blocks of 2x2 */
	for (j = 0; j < height; j+=2) {
		unsigned char *y1p = y + j * stride;
		unsigned char *y2p = y1p + stride;
		unsigned char *up = u + (j/2) * stride * cs / 2;
		unsigned char *vp = v + (j/2) * stride * cs / 2;

		for (i = 0; i < width; i+=2) {
			div_t d = div(n+i+j, width);
			uint32_t rgb = 0x00130502 * (d.quot >> 6) + 0x000a1120 * (d.rem >> 6);
			unsigned char *rgbp = (unsigned char *)&rgb;
			unsigned char y = (0.299 * rgbp[RED]) + (0.587 * rgbp[GREEN]) + (0.114 * rgbp[BLUE]);

			*(y2p++) = *(y1p++) = y;
			*(y2p++) = *(y1p++) = y;

			*up = (rgbp[BLUE] - y) * 0.565 + 128;
			*vp = (rgbp[RED] - y) * 0.713 + 128;
			up += cs;
			vp += cs;
		}
	}
}
}

//#define USE_TILER

#ifdef USE_TILER
extern "C" {
#  include <tiler.h>
#  include <tilermem.h>
#  include <memmgr.h>
}
#endif


bool setupYuvTexSurface(EGLDisplay dpy, EGLContext context) {
    EGLint attr[] = {
            EGL_GL_VIDEO_FOURCC_TI,      FOURCC_STR("NV12"),
            EGL_GL_VIDEO_WIDTH_TI,       VID_WIDTH,
            EGL_GL_VIDEO_HEIGHT_TI,      VID_HEIGHT,
            EGL_GL_VIDEO_BYTE_STRIDE_TI, VID_WIDTH,
            EGL_GL_VIDEO_BYTE_SIZE_TI,   VID_SIZE,
            // TODO: pick proper YUV flags..
            EGL_GL_VIDEO_YUV_FLAGS_TI,   EGLIMAGE_FLAGS_YUV_CONFORMANT_RANGE |
            EGLIMAGE_FLAGS_YUV_BT601,
            EGL_NONE
    };
#ifdef USE_TILER
    MemAllocBlock block;
    memset(&block, 0, sizeof(block));
    block.pixelFormat = PIXEL_FMT_PAGE;
    block.dim.len = VID_SIZE;
    unsigned char *ptr = (unsigned char *)MemMgr_Alloc (&block, 1);
#else
    unsigned char *ptr = (unsigned char *)malloc(VID_SIZE + PAGE_SIZE);
    ptr = (unsigned char *)((uint32_t)ptr & ~(PAGE_SIZE - 1));
#endif
    unsigned char *y = ptr;
    // XXX deal with multiple bo case
    unsigned char *u = y + (VID_HEIGHT * VID_WIDTH);
    unsigned char *v = u + 1;

    fill420(y, u, v, 2, 0, VID_WIDTH, VID_HEIGHT, VID_WIDTH);

    PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR =
            (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
    PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES =
            (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)eglGetProcAddress("glEGLImageTargetTexture2DOES");

    EGLImageKHR img = eglCreateImageKHR(dpy, EGL_NO_CONTEXT, EGL_RAW_VIDEO_TI, ptr, attr);
    checkEglError("eglCreateImageKHR");
    if (img == EGL_NO_IMAGE_KHR) {
        printf("eglCreateImageKHR failed\n");
        return false;
    }

    glGenTextures(1, &yuvTex);
    checkGlError("glGenTextures");
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, yuvTex);
    checkGlError("glBindTexture");
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkGlError("glTexParameteri");
    glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES)img);
    checkGlError("glEGLImageTargetTexture2DOES");

    return true;
}

const GLfloat gTriangleVertices[] = {
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
};

void renderFrame() {
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	checkGlError("glClearColor");
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	glUseProgram(gProgram);
	checkGlError("glUseProgram");

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(gvPositionHandle);
	checkGlError("glEnableVertexAttribArray");

	glUniform1i(gYuvTexSamplerHandle, 0);
	checkGlError("glUniform1i");
	glBindTexture(GL_TEXTURE_EXTERNAL_OES, yuvTex);
	checkGlError("glBindTexture");

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	checkGlError("glDrawArrays");
}

void printEGLConfiguration(EGLDisplay dpy, EGLConfig config) {

#define X(VAL) {VAL, #VAL}
	struct {
		EGLint attribute;
		const char* name;
	} names[] = {
			X(EGL_BUFFER_SIZE),
			X(EGL_ALPHA_SIZE),
			X(EGL_BLUE_SIZE),
			X(EGL_GREEN_SIZE),
			X(EGL_RED_SIZE),
			X(EGL_DEPTH_SIZE),
			X(EGL_STENCIL_SIZE),
			X(EGL_CONFIG_CAVEAT),
			X(EGL_CONFIG_ID),
			X(EGL_LEVEL),
			X(EGL_MAX_PBUFFER_HEIGHT),
			X(EGL_MAX_PBUFFER_PIXELS),
			X(EGL_MAX_PBUFFER_WIDTH),
			X(EGL_NATIVE_RENDERABLE),
			X(EGL_NATIVE_VISUAL_ID),
			X(EGL_NATIVE_VISUAL_TYPE),
			X(EGL_SAMPLES),
			X(EGL_SAMPLE_BUFFERS),
			X(EGL_SURFACE_TYPE),
			X(EGL_TRANSPARENT_TYPE),
			X(EGL_TRANSPARENT_RED_VALUE),
			X(EGL_TRANSPARENT_GREEN_VALUE),
			X(EGL_TRANSPARENT_BLUE_VALUE),
			X(EGL_BIND_TO_TEXTURE_RGB),
			X(EGL_BIND_TO_TEXTURE_RGBA),
			X(EGL_MIN_SWAP_INTERVAL),
			X(EGL_MAX_SWAP_INTERVAL),
			X(EGL_LUMINANCE_SIZE),
			X(EGL_ALPHA_MASK_SIZE),
			X(EGL_COLOR_BUFFER_TYPE),
			X(EGL_RENDERABLE_TYPE),
			X(EGL_CONFORMANT),
	};
#undef X

	for (size_t j = 0; j < sizeof(names) / sizeof(names[0]); j++) {
		EGLint value = -1;
		EGLint returnVal = eglGetConfigAttrib(dpy, config, names[j].attribute, &value);
		EGLint error = eglGetError();
		if (returnVal && error == EGL_SUCCESS) {
			printf(" %s: ", names[j].name);
			printf("%d (0x%x)", value, value);
		}
	}
	printf("\n");
}

int main(int argc, char** argv) {
	static const EGLint attribs[] = {
#if 0
			EGL_RED_SIZE, 1,
			EGL_GREEN_SIZE, 1,
			EGL_BLUE_SIZE, 1,
			EGL_DEPTH_SIZE, 1,
#endif
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE
	};
	EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	EGLConfig config;
	EGLint num_configs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLContext context;
	EGLSurface surface;
	EGLint w, h;

	EGLDisplay edpy;
	Display *xdpy;
	Window xwin;

	int ret;

    xdpy = XOpenDisplay(NULL);

	checkEglError("<init>");
	edpy = eglGetDisplay((EGLNativeDisplayType)xdpy);
	checkEglError("eglGetDisplay");
	if (edpy == EGL_NO_DISPLAY) {
		printf("eglGetDisplay returned EGL_NO_DISPLAY.\n");
		return 0;
	}

	ret = eglInitialize(edpy, &majorVersion, &minorVersion);
	checkEglError("eglInitialize", ret);
	fprintf(stderr, "EGL version %d.%d\n", majorVersion, minorVersion);
	if (ret != EGL_TRUE) {
		printf("eglInitialize failed\n");
		return 0;
	}

	if (!eglChooseConfig(edpy, attribs, &config, 1, &num_configs)) {
		printf("Error: couldn't get an EGL visual config\n");
		exit(1);
	}

    xwin = XCreateSimpleWindow(xdpy, RootWindow(xdpy, 0), 1, 1,
    		WIN_WIDTH, WIN_HEIGHT, 0, BlackPixel (xdpy, 0),
    		BlackPixel(xdpy, 0));
    XMapWindow(xdpy, xwin);
    XFlush(xdpy);

    surface = eglCreateWindowSurface(edpy, config, (EGLNativeWindowType)xwin, NULL);
	checkEglError("eglCreateWindowSurface");
	if (surface == EGL_NO_SURFACE) {
		printf("gelCreateWindowSurface failed.\n");
		return 1;
	}

    context = eglCreateContext(edpy, config, EGL_NO_CONTEXT, context_attribs);
	checkEglError("eglCreateContext");
	if (context == EGL_NO_CONTEXT) {
		printf("eglCreateContext failed\n");
		return 1;
	}

	ret = eglMakeCurrent(edpy, surface, surface, context);
	checkEglError("eglMakeCurrent", ret);
	if (ret != EGL_TRUE) {
		printf("eglMakeCurrent failed\n");
		return 1;
	}

	eglQuerySurface(edpy, surface, EGL_WIDTH, &w);
	checkEglError("eglQuerySurface");
	eglQuerySurface(edpy, surface, EGL_HEIGHT, &h);
	checkEglError("eglQuerySurface");
	GLint dim = w < h ? w : h;

	fprintf(stderr, "Window dimensions: %d x %d\n", w, h);

	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);

	if(!setupYuvTexSurface(edpy, context)) {
		fprintf(stderr, "Could not set up texture surface.\n");
		return 1;
	}

	if(!setupGraphics(w, h)) {
		fprintf(stderr, "Could not set up graphics.\n");
		return 1;
	}

	for (;;) {
		renderFrame();
		eglSwapBuffers(edpy, surface);
		checkEglError("eglSwapBuffers");
	}

	return 0;
}
