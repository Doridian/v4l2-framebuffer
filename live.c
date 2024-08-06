/*
 ============================================================================
 Name        : live.c
 Author      : LincolnHard
 Version     :
 Copyright   : free and open
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "draw_framebuffer.h"
#include "video_capture.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* check the supported webcam resolutions using $v4l2-ctl --list-formats-ext */

int main(int argc, char** argv) {
	int width = 0, height = 0;

	if (argc >= 2) width = atoi(argv[1]);
	if (argc >= 3) height = atoi(argv[2]);

	if (width == 0) width = 1280;
	if (height == 0) height = 1024;

	printf("Using size: %dx%d.\n", width, height);
	usleep(1 * 1000000); // sleep one second

    unsigned char src_image[width * height * 4];
    printf("Init FB\n");
    init_framebuffer();
    printf("Init VC\n");
    init_video_capture(width, height);
    printf("Init Done\n");

	int did_capture = 0;
	for(; ;){
		did_capture = video_capture(src_image, width, height);
		if (did_capture > 0) {
			draw_framebuffer(src_image, width, height);
		}
	}
    free_video_capture();
	free_framebuffer();
	return EXIT_SUCCESS;
}
