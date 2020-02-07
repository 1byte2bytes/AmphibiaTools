#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define frame_buf_size 1920*1080*3

int main(void) {
	// Create frame buffers
	static uint8_t origFrame[frame_buf_size];
	static uint8_t lineFrame[frame_buf_size];

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

	return 0;
}