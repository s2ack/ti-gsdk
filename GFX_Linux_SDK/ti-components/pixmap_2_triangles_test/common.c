/*****************************************************************************
 * common.c
 *
 *    common functions - egl initialize
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *   
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *   
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/fb.h>
#include "common.h"
#include <cmem.h>


int gQuit = FALSE;

EGLDisplay dpy;
EGLSurface surface = EGL_NO_SURFACE;
static EGLContext context = EGL_NO_CONTEXT;

void signalHandler(int signum) { (void)signum; gQuit=TRUE; }

static CMEM_AllocParams params = { CMEM_POOL, CMEM_NONCACHED, 4096 };

extern NATIVE_PIXMAP_STRUCT* pNativePixmap;

void create_native_pixmap(unsigned long pixmapFormat, unsigned long pixmapwidth,
                          unsigned long pixmapHeight, NATIVE_PIXMAP_STRUCT **pNativePixmapPtr
                          )
{
    //The cmem module should be inserted before running this application
    //Create a contiguous buffer of required size for texture, and get userspace address
    *pNativePixmapPtr = (NATIVE_PIXMAP_STRUCT*)malloc(sizeof(NATIVE_PIXMAP_STRUCT));
    if (pixmapFormat == SGX_RGB565)
        (*pNativePixmapPtr)->ePixelFormat = 0;
    else if (pixmapFormat == SGX_ARGB8888)
        (*pNativePixmapPtr)->ePixelFormat = 2;
    else {
        printf("Invalid pixmap format type %ld\n", pixmapFormat);
        exit(-1);
    }
    (*pNativePixmapPtr)->eRotation = 0;

    printf ("pixmapwidth: %d\n",pixmapwidth);
    printf ("pixmapHeight: %d\n",pixmapHeight); 
    (*pNativePixmapPtr)->lWidth = pixmapwidth;
    (*pNativePixmapPtr)->lHeight = pixmapHeight;
        if(pixmapFormat == SGX_RGB565)
                (*pNativePixmapPtr)->lStride = (*pNativePixmapPtr)->lWidth* 16/8; //bitwidth/8
        else if(pixmapFormat == SGX_ARGB8888)
                (*pNativePixmapPtr)->lStride = (*pNativePixmapPtr)->lWidth* 32/8; //bitwidth/8
    (*pNativePixmapPtr)->lSizeInBytes = (*pNativePixmapPtr)->lHeight * (*pNativePixmapPtr)->lStride;
    (*pNativePixmapPtr)->lAddress = (long) CMEM_alloc((*pNativePixmapPtr)->lSizeInBytes, &params);
    if(!(*pNativePixmapPtr)->lAddress)
    {
        printf("CMEM_alloc returned NULL\n");
        exit(-1);
    }
    //Get the physical page corresponding to the above cmem buffer
    (*pNativePixmapPtr)->pvAddress = CMEM_getPhys((void*)(*pNativePixmapPtr)->lAddress);
    printf("Physical address = %x\n", (*pNativePixmapPtr)->pvAddress);
    if((*pNativePixmapPtr)->pvAddress & 0xFFF)
                printf("PVR2DMemWrap may have issues with this non-aligned address!\n");
}
void delete_native_pixmap(
                        NATIVE_PIXMAP_STRUCT *pNativePixmap)
{
    if(!pNativePixmap) return;
    if(pNativePixmap->lAddress)
        CMEM_free((void*)pNativePixmap->lAddress, &params);
    //delete the pixmap itself
    free(pNativePixmap);
}



int get_disp_resolution(int *w, int *h)
{
    int fb_fd, ret = -1;
    struct fb_var_screeninfo vinfo;

    if ((fb_fd = open("/dev/fb0", O_RDONLY)) < 0) {
        printf("failed to open fb0 device\n");
        return ret;
    }

    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("FBIOGET_VSCREENINFO");
        goto exit;
    }

    *w = vinfo.xres;
    *h = vinfo.yres;

    printf(" \n Display Resolution: %dx%d\n\n", *w, *h);
    if (*w && *h)
        ret = 0;

exit:
    close(fb_fd);
    return ret;
}

static void print_err(char *name)
{
    char *err_str[] = {
          "EGL_SUCCESS",
          "EGL_NOT_INITIALIZED",
          "EGL_BAD_ACCESS",
          "EGL_BAD_ALLOC",
          "EGL_BAD_ATTRIBUTE",    
          "EGL_BAD_CONFIG",
          "EGL_BAD_CONTEXT",   
          "EGL_BAD_CURRENT_SURFACE",
          "EGL_BAD_DISPLAY",
          "EGL_BAD_MATCH",
          "EGL_BAD_NATIVE_PIXMAP",
          "EGL_BAD_NATIVE_WINDOW",
          "EGL_BAD_PARAMETER",
          "EGL_BAD_SURFACE" };

    EGLint ecode = eglGetError();

    printf("'%s': egl error '%s' (0x%x)\n",
           name, err_str[ecode-EGL_SUCCESS], ecode);
}

void deInitEGL(NATIVE_PIXMAP_STRUCT* pNativePixmap)
{

    eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    printf (" eglMakeCurrent EGL_NO_SURFACE \n");
    if(pNativePixmap)
        delete_native_pixmap(pNativePixmap);
    if (context != EGL_NO_CONTEXT) {
        eglDestroyContext(dpy, context);
        printf (" eglDestroyContext \n");
    }
    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(dpy, surface);
        printf (" eglDestroySurface \n");
    }
    eglTerminate(dpy);
    printf (" eglTerminate \n");
}


int initEGL( int surfaceType, int inW, int inH, NATIVE_PIXMAP_STRUCT** pNativePixmapPtr, int *surf_w, int *surf_h)
{
    typedef NativeDisplayType EGLNativeDisplayType;
    typedef NativeWindowType EGLNativeWindowType;

    EGLint            disp_w, disp_h;
    EGLNativeDisplayType disp_type;
    EGLNativeWindowType  window;
    EGLConfig         cfgs[2];
    EGLint            n_cfgs;
    
    EGLint            egl_attr[] = {
                         EGL_BUFFER_SIZE, EGL_DONT_CARE,
                         EGL_RED_SIZE,    8,
                         EGL_GREEN_SIZE,  8,
                         EGL_BLUE_SIZE,   8,
                         EGL_DEPTH_SIZE,  8,
                         EGL_SURFACE_TYPE, EGL_PIXMAP_BIT,  // EGL_WINDOW_BIT
                         EGL_NONE };



    if (surfaceType == SGX_SURFACE_TYPE_WINDOW) {
        if (get_disp_resolution(&disp_w, &disp_h)) {
            printf("ERROR: get display resolution failed\n");
            return -1;
        }
    } else {
        if (CMEM_init() == -1) {
            printf("CMEM_init() failed\n");
            return -1;
        }
        disp_w = inW;
        disp_h = inH;
    }

    disp_type = (EGLNativeDisplayType)EGL_DEFAULT_DISPLAY;
    window  = 0;

    dpy = eglGetDisplay(disp_type);
    printf (" eglGetDisplay \n");
   
    if (eglInitialize(dpy, NULL, NULL) != EGL_TRUE) {
        print_err("eglInitialize");
        return -1;
    }

    printf (" eglInitialize \n");

    if (eglGetConfigs(dpy, cfgs, 2, &n_cfgs) != EGL_TRUE) {
        print_err("eglGetConfigs");
        goto cleanup;
    }

    printf (" eglGetConfigs \n");
 
    if (eglChooseConfig(dpy, egl_attr, cfgs, 2, &n_cfgs) != EGL_TRUE) {
        print_err("eglChooseConfig");
        goto cleanup;
    }

    printf (" eglChooseConfig \n");

    printf (" n_cfgs: %d\n", n_cfgs);

    if(surfaceType == SGX_SURFACE_TYPE_WINDOW)
        surface = eglCreateWindowSurface(dpy, cfgs[0],(void*) NULL, NULL);  
    else if(surfaceType == SGX_SURFACE_TYPE_PIXMAP_16)
    {
        create_native_pixmap(SGX_RGB565, disp_w, disp_h, pNativePixmapPtr);
        surface = eglCreatePixmapSurface(dpy, cfgs[0], *pNativePixmapPtr, NULL);

    }
    else if(surfaceType == SGX_SURFACE_TYPE_PIXMAP_32)
    {
        printf (" SGX_SURFACE_TYPE_PIXMAP_32 \n ");
        create_native_pixmap(SGX_ARGB8888, disp_w, disp_h, pNativePixmapPtr);
        surface = eglCreatePixmapSurface (dpy, cfgs[0], *pNativePixmapPtr, NULL);
    }

    if (surface == EGL_NO_SURFACE) {
        print_err("eglCreateWindowSurface");
        printf (" Exitiing \n");
        exit (-1);
        goto cleanup;
    }

    if (surf_w && surf_h) {
        *surf_w = disp_w;
        *surf_h = disp_h;
    }

    context = eglCreateContext(dpy, cfgs[0], EGL_NO_CONTEXT, NULL);
    
    if (context == EGL_NO_CONTEXT) {
        print_err("eglCreateContext");
        goto cleanup;
    }

    if (eglMakeCurrent(dpy, surface, surface, context) != EGL_TRUE) {
        print_err("eglMakeCurrent");
        goto cleanup;
    }
  
    return 0;

cleanup:
    deInitEGL(pNativePixmap);
    return -1;
}

