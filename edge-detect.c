#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* --- BEGING SETTINGS --- */
#define FRAME_HEIGHT 1080
#define FRAME_WIDTH 1920
// the work that would have to be done to make the filter support 
// more than 8 bit depth is more than it's worth at this time
#define FRAME_BUF_SIZE FRAME_WIDTH*FRAME_HEIGHT*3

#define BLACK_BORDER_TOLERANCE_R 30
#define BLACK_BORDER_TOLERANCE_G 30
#define BLACK_BORDER_TOLERANCE_B 30

// #D49C72
#define ANNE_SKIN_R 0xD4
#define ANNE_SKIN_G 0x9C
#define ANNE_SKIN_B 0x72
#define ANNE_SKIN_TOLERANCE_R 30
#define ANNE_SKIN_TOLERANCE_G 30
#define ANNE_SKIN_TOLERANCE_B 30

// #6F3C37
#define ANNE_HAIR_R 0x6F
#define ANNE_HAIR_G 0x3C
#define ANNE_HAIR_B 0x37
#define ANNE_HAIR_TOLERANCE_R 30
#define ANNE_HAIR_TOLERANCE_G 30
#define ANNE_HAIR_TOLERANCE_B 30
/* ---   END SETTINGS  --- */

int main(void) {
	// Create frame buffers
	static uint8_t origFrame[FRAME_BUF_SIZE];
	static uint8_t lineFrame[FRAME_BUF_SIZE];

	// Load our raw RGB file into a uint8_t array "origFrame"
	FILE *rgbFile = fopen("963.rgb", "rb");
	fread(origFrame, sizeof(uint8_t), FRAME_BUF_SIZE, rgbFile);
	fclose(rgbFile);

	printf("--- Anne's skin tolerances ---\r\nRed: >= %x && <= %x\r\nGreen: >= %x && <= %x\r\nBlue: >= %x && <= %x\r\n", 
		ANNE_SKIN_R - ANNE_SKIN_TOLERANCE_R, ANNE_SKIN_R + ANNE_SKIN_TOLERANCE_R,
		ANNE_SKIN_G - ANNE_SKIN_TOLERANCE_G, ANNE_SKIN_G + ANNE_SKIN_TOLERANCE_G,
		ANNE_SKIN_B - ANNE_SKIN_TOLERANCE_B, ANNE_SKIN_B + ANNE_SKIN_TOLERANCE_B);

	// For each pixel in the frame (keep in mind each pixel is 3 uint8_ts long)
	for (size_t pixel = 0; pixel < (FRAME_BUF_SIZE); pixel += 3) {
		// Detect if the pixel's R, G, and B values are all <= 30
		if (	origFrame[pixel] <= BLACK_BORDER_TOLERANCE_R && 
				origFrame[pixel+1] <= BLACK_BORDER_TOLERANCE_G && 
				origFrame[pixel+2] <= BLACK_BORDER_TOLERANCE_B) {
			// Write a white pixel to the line frame buffer for this pixel
			lineFrame[pixel] = 255;
			lineFrame[pixel+1] = 255;
			lineFrame[pixel+2] = 255;

			// Get locations of all 8 surrounding pixels to this pixel
			static size_t surroundingPixels[8];
			surroundingPixels[0] = (pixel - FRAME_WIDTH) - 3;
			surroundingPixels[1] = (pixel - FRAME_WIDTH);
			surroundingPixels[2] = (pixel - FRAME_WIDTH) + 3;
			surroundingPixels[3] = pixel - 3;
			surroundingPixels[4] = pixel + 3;
			surroundingPixels[5] = (pixel + FRAME_WIDTH) - 3;
			surroundingPixels[6] = (pixel + FRAME_WIDTH);
			surroundingPixels[7] = (pixel + FRAME_WIDTH) + 3;

			// Check if a surrounding pixel matches Anne's skin tone, accounting for tolerance settings
			for (size_t sPixel = 0; sPixel < sizeof(surroundingPixels); sPixel++) {
				printf("%d\r\n", surroundingPixels[sPixel]);
				lineFrame[surroundingPixels[sPixel]] = 255;
				if (	origFrame[surroundingPixels[sPixel]  ] <= ANNE_SKIN_R + ANNE_SKIN_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] <= ANNE_SKIN_G + ANNE_SKIN_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] <= ANNE_SKIN_B + ANNE_SKIN_TOLERANCE_B &&
						origFrame[surroundingPixels[sPixel]  ] >= ANNE_SKIN_R - ANNE_SKIN_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] >= ANNE_SKIN_G - ANNE_SKIN_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] >= ANNE_SKIN_B - ANNE_SKIN_TOLERANCE_B) {
					lineFrame[pixel] = 0;
					lineFrame[pixel+1] = 0;
					lineFrame[pixel+2] = 255;
				}
			}
		}
	}

	// Save our line file
	FILE *lineFile = fopen("line.rgb", "wb");
	fwrite(lineFrame, sizeof(uint8_t), FRAME_BUF_SIZE, lineFile);
	fclose(lineFile);

	return 0;
}