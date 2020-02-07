#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define frame_buf_size 1920*1080*3

int main(void) {
	// Create frame buffers
	//uint8_t *origFrame = malloc(frame_buf_size);
	uint8_t *origFrame = calloc(frame_buf_size, sizeof(uint8_t)); // <== HACK, calloc clears memory which shouldn't be neccessary
	uint8_t *lineFrame = calloc(frame_buf_size, sizeof(uint8_t));

	// Load our raw RGB file into a uint8_t array "origFrame"
	FILE *rgbFile = fopen("963.rgb", "rb");
	fread(origFrame, sizeof(uint8_t), frame_buf_size, rgbFile);
	fclose(rgbFile);

	for (size_t pixel = 0; pixel < (frame_buf_size); pixel += 3) {
		// Detect if the pixel's R, G, and B values are all <= 30
		if (origFrame[pixel] <= 30 && origFrame[pixel+1] <= 30 && origFrame[pixel+2] <= 30) {
			// Write a white pixel to the line frame buffer for this pixel
			lineFrame[pixel] = 255;
			lineFrame[pixel+1] = 255;
			lineFrame[pixel+2] = 255;
		}
	}

	// Save our line file
	FILE *lineFile = fopen("line.rgb", "wb");
	fwrite(lineFrame, sizeof(uint8_t), frame_buf_size, lineFile);
	fclose(lineFile);

	// Free buffers
	free(origFrame);
	free(lineFrame);

	return 0;
}