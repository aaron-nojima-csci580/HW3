/*   CS580 HW1 display functions to be completed   */

#include   "stdafx.h"  
#include	"Gz.h"
#include	"disp.h"


int GzNewFrameBuffer(char** framebuffer, int width, int height)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- pass back pointer 
 */
	// Byte = unsigned char (same data size ~ 8 bits)
	// framebuffer pointer references pointer to allocated memory
	*framebuffer = (char *)malloc(3 * sizeof(char) * width * height);
	return GZ_SUCCESS;
}

int GzNewDisplay(GzDisplay	**display, int xRes, int yRes)
{
/* HW1.2 create a display:
  -- allocate memory for indicated resolution
  -- pass back pointer to GzDisplay object in display
*/
	// allocate memory for GzDisplay display
	*display = (GzDisplay *)malloc(sizeof(GzDisplay));
	
	// set resolution fields
	(*display)->xres = xRes;
	(*display)->yres = yRes;

	// allocate memory for framebuffer array (note that this is not 2D)
	(*display)->fbuf = (GzPixel *)malloc(sizeof(GzPixel) * yRes * xRes);
	return GZ_SUCCESS;
}


int GzFreeDisplay(GzDisplay	*display)
{
/* HW1.3 clean up, free memory */
	if (display != NULL)
	{
		free(display);
	}
	return GZ_SUCCESS;
}


int GzGetDisplayParams(GzDisplay *display, int *xRes, int *yRes)
{
/* HW1.4 pass back values for a display */
	if (display != NULL)
	{
		// load resolution values into input integer pointers
		*xRes = display->xres;
		*yRes = display->yres;
		return GZ_SUCCESS;
	}
	return GZ_FAILURE;
}


int GzInitDisplay(GzDisplay	*display)
{
/* HW1.5 set everything to some default values - start a new frame */
	if (display != NULL)
	{
		for (int i = 0; i < display->xres; ++i)
		{
			for (int j = 0; j < display->yres; j++)
			{
				GzPixel * pixel = &display->fbuf[ARRAY(i, j)];
				// arbitrary values chosen (this is going to be the background
				pixel->red = 2000;
				pixel->green = 2000;
				pixel->blue = 2000;
				pixel->alpha = 4095;
				pixel->z = MAXINT;
			}
		}
		return GZ_SUCCESS;
	}
	return GZ_FAILURE;
}


int GzPutDisplay(GzDisplay *display, int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.6 write pixel values into the display */
	// clamp rgb values between 0 and 4095
	r = max(0, min(4095, r));
	b = max(0, min(4095, b));
	g = max(0, min(4095, g));
	if (display != NULL)
	{
		// don't bother updating framebuffer if out of bounds
		if (i >= 0 && i < display->xres && j >= 0 && j < display->yres)
		{
			// set pixel values
			GzPixel * pixel = &display->fbuf[ARRAY(i, j)];
			pixel->red = r;
			pixel->green = g;
			pixel->blue = b;
			pixel->alpha = a;
			pixel->z = z;
		}
		return GZ_SUCCESS;
	}
	return GZ_FAILURE;
}


int GzGetDisplay(GzDisplay *display, int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.7 pass back a pixel value to the display */
	if (display != NULL)
	{
		// don't bother for out of bounds values
		if (i >= 0 && i < display->xres && j >= 0 && j < display->yres)
		{
			// get pixel values
			GzPixel * pixel = &display->fbuf[ARRAY(i, j)];
			*r = pixel->red;
			*g = pixel->green;
			*b = pixel->blue;
			*a = pixel->alpha;
			*z = pixel->z;
		}
		return GZ_SUCCESS;
	}
	return GZ_FAILURE;
}


int GzFlushDisplay2File(FILE* outfile, GzDisplay *display)
{

/* HW1.8 write pixels to ppm file -- "P6 %d %d 255\r" */
	if (display != NULL && outfile != NULL)
	{
		int xRes = display->xres;
		int yRes = display->yres;
		// header file for ppm
		fprintf(outfile, "P6 %d %d 255\r", xRes, yRes);
		char r, g, b;
		for (int j = 0; j < yRes; ++j)
		{
			for (int i = 0; i < xRes; ++i)
			{
				GzPixel * pixel = &display->fbuf[ARRAY(i, j)];
				r = pixel->red >> 4;
				g = pixel->green >> 4;
				b = pixel->blue >> 4;
				// Binary ASCII values (aka char values from right-shifted byte value)
				fprintf(outfile, "%c%c%c", r, g, b);
			}
		}
		return GZ_SUCCESS;
	}
	return GZ_FAILURE;
}

int GzFlushDisplay2FrameBuffer(char* framebuffer, GzDisplay *display)
{

/* HW1.9 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
*/
	if (display != NULL && framebuffer != NULL)
	{
		int xRes = display->xres;
		int yRes = display->yres;
		char r, g, b;
		for (int i = 0; i < xRes; ++i)
		{
			for (int j = 0; j < yRes; ++j)
			{
				int index = ARRAY(i, j);
				GzPixel * pixel = &display->fbuf[index];
				r = pixel->red >> 4;
				g = pixel->green >> 4;
				b = pixel->blue >> 4;
				// Binary ASCII values (aka char values from right-shifted byte value)
				// since there are 3 char values per pixel in the framebuffer
				// also noting the order of bgr instead
				framebuffer[3 * index + 0] = b;
				framebuffer[3 * index + 1] = g;
				framebuffer[3 * index + 2] = r;
			}
		}
		return GZ_SUCCESS;
	}
	return GZ_FAILURE;
}