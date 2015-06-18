/*****************************************************************************
 * main.c
 *   Sample application using PIXMAP 
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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "common.h"


/* Vertices for rectagle covering the entire display resolution */
GLfloat rect_vertices[6][3] =
{   // x     y     z
 
   /* 1st Traingle */
    {-1.0,  1.0,  0.0}, // 0  left    First Strip
    {-1.0, -1.0,  0.0}, // 1
    { 1.0,  1.0,  0.0}, // 2
  
   /* 2nd Traingle */
    { 1.0,  1.0,  0.0}, // 1
    {-1.0, -1.0,  0.0}, // 0
    { 1.0, -1.0,  0.0}, // 2
};

GLfloat color_values[6][4] =
{   // x     y     z  alpha

   /* 1st Traingle */
    { 1.0, 0.0,  1.0, 1.0},
    { 1.0, 0.0,  1.0, 1.0},
    { 1.0, 0.0,  1.0, 1.0},

   /* 2nd Traingle */
    { 0.0,  1.0,  0.0, 1.0}, 
    { 0.0,  1.0,  0.0, 1.0},
    { 0.0,  1.0,  0.0, 1.0},

};

int inSurfaceType =0;
NATIVE_PIXMAP_STRUCT* pNativePixmap = NULL;
int inWidth = 0;
int inHeight = 0;

void drawRect()
{

    glClear(GL_COLOR_BUFFER_BIT);    
    
    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    /*  Set pointers to the vertices and color arrays */
    glVertexPointer(3, GL_FLOAT, 0, rect_vertices);
    glColorPointer(4, GL_FLOAT, 0, color_values);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();    
    

    if(inSurfaceType == SGX_SURFACE_TYPE_WINDOW)
        eglSwapBuffers(dpy, surface);   
    else if(inSurfaceType == SGX_SURFACE_TYPE_PIXMAP_16 || inSurfaceType == SGX_SURFACE_TYPE_PIXMAP_32)
        eglWaitGL();
}

const char *helpString = \
"           Usage: program surfaceType width height \n\
                  surfaceType = type of surface(0 = WindowSurface, 1 = pixmapSurface_16b, 2 = pixmapSurface_32b)\n\
                  if (surfaceType > 0) width = pixmap surface width and pixmap height = surface height";

FILE *fp;
int numbytes;

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf (" Invalid Number of Arguments \n");
        printf (helpString);     
        exit (-1);
    }

    inSurfaceType = atol(argv[1]);
    printf (" inSurfaceType: %d\n", inSurfaceType);
   
    if (inSurfaceType > 0) {
        if (argc < 4) {
            printf ("Invalid Number of Arguments \n");
            printf (helpString);
            exit (-1);
        }
        inWidth  = atol(argv[2]);
        inHeight = atol(argv[3]);
        printf (" inWidth: %d\n", inWidth);
        printf (" inHeight: %d\n",inHeight);
    }
     
    signal(SIGINT, signalHandler);

    if (initEGL(inSurfaceType, inWidth, inHeight, &pNativePixmap, NULL, NULL)) {
        printf("ERROR: init EGL failed\n");
        goto err_ret;
    }
 
    while (!gQuit) {

        drawRect();
    }

    if(inSurfaceType == SGX_SURFACE_TYPE_PIXMAP_16 || inSurfaceType == SGX_SURFACE_TYPE_PIXMAP_32)
    {
        /* Use this for checking out the output in the pixmap */
        /* For DEBUGGING ONLY */
        fp = fopen("pixmap.raw", "wb");
        if(inSurfaceType == SGX_SURFACE_TYPE_PIXMAP_32) numbytes = 4; //ARGB8888
        if(inSurfaceType == SGX_SURFACE_TYPE_PIXMAP_16) numbytes = 2; //RGB565
        printf("Writing pixmap data to file pixmap.raw\n");
        fwrite((void*)pNativePixmap->lAddress, 1, inWidth*inHeight*numbytes, fp);
        fclose(fp);
    }

err_ret:
    deInitEGL(pNativePixmap);
    printf(" DONE \n");
    return 0;
}
