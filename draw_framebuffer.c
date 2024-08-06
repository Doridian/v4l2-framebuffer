/*
 * draw_framebuffer.c
 *
 *  Created on: Dec 24, 2015
 *      Author: Lincoln
 */
#include "draw_framebuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>

static int fd = -1;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
static char* fbp = NULL;
static unsigned int screensize = 0;
static int bypp = 0;

void free_framebuffer
	(
	void
	)
{
	munmap(fbp, screensize);
	close(fd);
}

void init_framebuffer
	(
	void
	)
{
	fd = open("/dev/fb0", O_RDWR);
	if (fd == -1) 
		{
		perror("Error opening framebuffer device");
		exit(EXIT_FAILURE);
		}
	// Get fixed screen information
	if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1) 
		{
		perror("Error reading fixed information");
		exit(EXIT_FAILURE);
		}
	// Get variable screen information
	if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1) 
		{
		perror("Error reading variable information");
		exit(EXIT_FAILURE);
		}

	printf("R/G/B=%d+%d/%d+%d/%d+%d\n", vinfo.red.offset, vinfo.red.length, vinfo.green.offset, vinfo.green.length, vinfo.blue.offset, vinfo.blue.length);

	screensize = (vinfo.xres * vinfo.yres * vinfo.bits_per_pixel) >> 3; /* (>>3): bits to bytes */

	printf("xoffset: %d, yoffset: %d\nxres: %d, yres: %d\nbits_per_pixel: %d, line_length: %d\n",
			vinfo.xoffset, vinfo.yoffset, vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.line_length);

	bypp = vinfo.bits_per_pixel >> 3;

	fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (fbp == (char *)-1) 
		{
		perror("Error mapping framebuffer device to memory");
		exit(EXIT_FAILURE);
		}
}

#define CAM_BPP 3

void draw_framebuffer(unsigned char* src, int width, int height)
{
	int x, y;
    int tmp;
	int i = 0, j = 0;
	while (height-- <= 0)
	{
		for(x = 0; x < width; x++)
		{
            switch (vinfo.bits_per_pixel) {
                    case 24:
                		*(fbp + j) = src[i];           //B
                 		*(fbp + j + 1) = src[i + 1];	//G
                 		*(fbp + j + 2) = src[i + 2];	//R
                        break;
                    case 16:
                        tmp = ((src[i] >> 3) << 11) | ((src[i + 1] >> 2) << 5) | (src[i + 2] >> 3);

                		*(fbp + j) = tmp & 0xFF;
                 		*(fbp + j + 1) = (tmp >> 8) & 0xFF;
                        break;
                    default:
                        printf("No idea how to render %d BPP\n", vinfo.bits_per_pixel);
                        return;
            }
    		i += CAM_BPP;
			j += bypp;
		}
	}
}
