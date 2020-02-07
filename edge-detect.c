#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* --- BEGING SETTINGS --- */
#define FRAME_HEIGHT 1080
#define FRAME_WIDTH 1920
// the work that would have to be done to make the filter support 
// more than 8 bit depth is more than it's worth at this time
#define FRAME_BUF_SIZE FRAME_WIDTH*FRAME_HEIGHT*3

#define BLACK_BORDER_TOLERANCE_R 50
#define BLACK_BORDER_TOLERANCE_G 50
#define BLACK_BORDER_TOLERANCE_B 50

// #D49C72
#define ANNE_SKIN_R 0xD4
#define ANNE_SKIN_G 0x9C
#define ANNE_SKIN_B 0x72
#define ANNE_SKIN_TOLERANCE_R 17
#define ANNE_SKIN_TOLERANCE_G 17
#define ANNE_SKIN_TOLERANCE_B 17
#define ANNE_SKIN_MATCHING 3

// #B17C63
#define ANNE_SKIN_SHADE_R 0xB1
#define ANNE_SKIN_SHADE_G 0x7C
#define ANNE_SKIN_SHADE_B 0x63
#define ANNE_SKIN_SHADE_TOLERANCE_R 17
#define ANNE_SKIN_SHADE_TOLERANCE_G 17
#define ANNE_SKIN_SHADE_TOLERANCE_B 17
#define ANNE_SKIN_SHADE_MATCHING 3

// #6F3C37
#define ANNE_HAIR_R 0x6F
#define ANNE_HAIR_G 0x3C
#define ANNE_HAIR_B 0x37
#define ANNE_HAIR_TOLERANCE_R 15
#define ANNE_HAIR_TOLERANCE_G 15
#define ANNE_HAIR_TOLERANCE_B 15
#define ANNE_HAIR_MATCHING 3

// #5B2E30
#define ANNE_HAIR_SHADE_R 0x5B
#define ANNE_HAIR_SHADE_G 0x2E
#define ANNE_HAIR_SHADE_B 0x30
#define ANNE_HAIR_SHADE_TOLERANCE_R 15
#define ANNE_HAIR_SHADE_TOLERANCE_G 15
#define ANNE_HAIR_SHADE_TOLERANCE_B 15
#define ANNE_HAIR_SHADE_MATCHING 3
/* ---   END SETTINGS  --- */

int main(void) {
	// Create frame buffers
	static uint8_t origFrame[FRAME_BUF_SIZE];
	static uint8_t lineFrame[FRAME_BUF_SIZE];

	// Load our raw RGB file into a uint8_t array "origFrame"
	FILE *rgbFile = fopen("963.rgb", "rb");
	fread(origFrame, sizeof(uint8_t), FRAME_BUF_SIZE, rgbFile);
	fclose(rgbFile);

	printf("--- Anne's skin tolerances ---\r\nRed: >= %X && <= %X\r\nGreen: >= %X && <= %X\r\nBlue: >= %X && <= %X\r\n#%X%X%X -> #%X%X%X\r\n", 
		ANNE_SKIN_R - ANNE_SKIN_TOLERANCE_R, ANNE_SKIN_R + ANNE_SKIN_TOLERANCE_R,
		ANNE_SKIN_G - ANNE_SKIN_TOLERANCE_G, ANNE_SKIN_G + ANNE_SKIN_TOLERANCE_G,
		ANNE_SKIN_B - ANNE_SKIN_TOLERANCE_B, ANNE_SKIN_B + ANNE_SKIN_TOLERANCE_B,
		ANNE_SKIN_R - ANNE_SKIN_TOLERANCE_R, ANNE_SKIN_G - ANNE_SKIN_TOLERANCE_G, ANNE_SKIN_B - ANNE_SKIN_TOLERANCE_B,
		ANNE_SKIN_R + ANNE_SKIN_TOLERANCE_R, ANNE_SKIN_G + ANNE_SKIN_TOLERANCE_G, ANNE_SKIN_B + ANNE_SKIN_TOLERANCE_B);

	printf("--- Anne's hair tolerances ---\r\nRed: >= %X && <= %X\r\nGreen: >= %X && <= %X\r\nBlue: >= %X && <= %X\r\n#%X%X%X -> #%X%X%X\r\n", 
		ANNE_HAIR_R - ANNE_HAIR_TOLERANCE_R, ANNE_HAIR_R + ANNE_HAIR_TOLERANCE_R,
		ANNE_HAIR_G - ANNE_HAIR_TOLERANCE_G, ANNE_HAIR_G + ANNE_HAIR_TOLERANCE_G,
		ANNE_HAIR_B - ANNE_HAIR_TOLERANCE_B, ANNE_HAIR_B + ANNE_HAIR_TOLERANCE_B,
		ANNE_HAIR_R - ANNE_HAIR_TOLERANCE_R, ANNE_HAIR_G - ANNE_HAIR_TOLERANCE_G, ANNE_HAIR_B - ANNE_HAIR_TOLERANCE_B,
		ANNE_HAIR_R + ANNE_HAIR_TOLERANCE_R, ANNE_HAIR_G + ANNE_HAIR_TOLERANCE_G, ANNE_HAIR_B + ANNE_HAIR_TOLERANCE_B);

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

			// Get all pixels at the border of a 4x4 square from this location
			static size_t surroundingPixels[32];
			surroundingPixels[0] = (((pixel/3) - (FRAME_WIDTH*1))*3) - (3*4);
			surroundingPixels[1] = (((pixel/3) - (FRAME_WIDTH*2))*3) - (3*4);
			surroundingPixels[2] = (((pixel/3) - (FRAME_WIDTH*3))*3) - (3*4);
			surroundingPixels[3] = (((pixel/3) - (FRAME_WIDTH*4))*3) - (3*4);
			surroundingPixels[4] = (((pixel/3) - (FRAME_WIDTH*4))*3) - (3*3);
			surroundingPixels[5] = (((pixel/3) - (FRAME_WIDTH*4))*3) - (3*2);
			surroundingPixels[6] = (((pixel/3) - (FRAME_WIDTH*4))*3) - (3*1);
			surroundingPixels[7] = (((pixel/3) - (FRAME_WIDTH*4))*3);
			surroundingPixels[8] = (((pixel/3) - (FRAME_WIDTH*4))*3) + (3*1);
			surroundingPixels[9] = (((pixel/3) - (FRAME_WIDTH*4))*3) + (3*2);
			surroundingPixels[10] = (((pixel/3) - (FRAME_WIDTH*4))*3) + (3*3);
			surroundingPixels[11] = (((pixel/3) - (FRAME_WIDTH*4))*3) + (3*4);
			surroundingPixels[12] = (((pixel/3) - (FRAME_WIDTH*3))*3) + (3*4);
			surroundingPixels[13] = (((pixel/3) - (FRAME_WIDTH*2))*3) + (3*4);
			surroundingPixels[14] = (((pixel/3) - (FRAME_WIDTH*1))*3) + (3*4);
			surroundingPixels[15] =  pixel - (3*4);
			surroundingPixels[16] =  pixel + (3*4);
			surroundingPixels[17] = (((pixel/3) + (FRAME_WIDTH*1))*3) - (3*4);
			surroundingPixels[18] = (((pixel/3) + (FRAME_WIDTH*2))*3) - (3*4);
			surroundingPixels[19] = (((pixel/3) + (FRAME_WIDTH*3))*3) - (3*4);
			surroundingPixels[20] = (((pixel/3) + (FRAME_WIDTH*4))*3) - (3*4);
			surroundingPixels[21] = (((pixel/3) + (FRAME_WIDTH*4))*3) - (3*3);
			surroundingPixels[22] = (((pixel/3) + (FRAME_WIDTH*4))*3) - (3*2);
			surroundingPixels[23] = (((pixel/3) + (FRAME_WIDTH*4))*3) - (3*1);
			surroundingPixels[24] = (((pixel/3) + (FRAME_WIDTH*4))*3);
			surroundingPixels[25] = (((pixel/3) + (FRAME_WIDTH*4))*3) + (3*1);
			surroundingPixels[26] = (((pixel/3) + (FRAME_WIDTH*4))*3) + (3*2);
			surroundingPixels[27] = (((pixel/3) + (FRAME_WIDTH*4))*3) + (3*3);
			surroundingPixels[28] = (((pixel/3) + (FRAME_WIDTH*4))*3) + (3*4);
			surroundingPixels[29] = (((pixel/3) + (FRAME_WIDTH*3))*3) + (3*4);
			surroundingPixels[30] = (((pixel/3) + (FRAME_WIDTH*2))*3) + (3*4);
			surroundingPixels[31] = (((pixel/3) + (FRAME_WIDTH*1))*3) + (3*4);

			uint8_t matched = 0;
			// Check if a surrounding pixel matches Anne's skin tone, accounting for tolerance settings
			for (size_t sPixel = 0; sPixel < sizeof(surroundingPixels)/sizeof(size_t); sPixel++) {
				//lineFrame[surroundingPixels[sPixel]] = 255;
				if (	origFrame[surroundingPixels[sPixel]  ] <= ANNE_SKIN_R + ANNE_SKIN_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] <= ANNE_SKIN_G + ANNE_SKIN_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] <= ANNE_SKIN_B + ANNE_SKIN_TOLERANCE_B &&
						origFrame[surroundingPixels[sPixel]  ] >= ANNE_SKIN_R - ANNE_SKIN_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] >= ANNE_SKIN_G - ANNE_SKIN_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] >= ANNE_SKIN_B - ANNE_SKIN_TOLERANCE_B) {
					matched++;
				}
			}
			if (matched >= ANNE_SKIN_MATCHING) {
				lineFrame[pixel] = 0;
				lineFrame[pixel+1] = 0;
				lineFrame[pixel+2] = 255;
			}
			matched = 0;

			// Check if a surrounding pixel matches Anne's skin shade colour, accounting for tolerance settings
			for (size_t sPixel = 0; sPixel < sizeof(surroundingPixels)/sizeof(size_t); sPixel++) {
				//lineFrame[surroundingPixels[sPixel]] = 255;
				if (	origFrame[surroundingPixels[sPixel]  ] <= ANNE_SKIN_SHADE_R + ANNE_SKIN_SHADE_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] <= ANNE_SKIN_SHADE_G + ANNE_SKIN_SHADE_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] <= ANNE_SKIN_SHADE_B + ANNE_SKIN_SHADE_TOLERANCE_B &&
						origFrame[surroundingPixels[sPixel]  ] >= ANNE_SKIN_SHADE_R - ANNE_SKIN_SHADE_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] >= ANNE_SKIN_SHADE_G - ANNE_SKIN_SHADE_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] >= ANNE_SKIN_SHADE_B - ANNE_SKIN_SHADE_TOLERANCE_B) {
					matched++;
				}
			}
			if (matched >= ANNE_SKIN_SHADE_MATCHING) {
				lineFrame[pixel] = 0;
				lineFrame[pixel+1] = 0;
				lineFrame[pixel+2] = 255;
			}
			matched = 0;

			// Check if a surrounding pixel matches Anne's hair colour, accounting for tolerance settings
			for (size_t sPixel = 0; sPixel < sizeof(surroundingPixels)/sizeof(size_t); sPixel++) {
				//lineFrame[surroundingPixels[sPixel]] = 255;
				if (	origFrame[surroundingPixels[sPixel]  ] <= ANNE_HAIR_R + ANNE_HAIR_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] <= ANNE_HAIR_G + ANNE_HAIR_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] <= ANNE_HAIR_B + ANNE_HAIR_TOLERANCE_B &&
						origFrame[surroundingPixels[sPixel]  ] >= ANNE_HAIR_R - ANNE_HAIR_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] >= ANNE_HAIR_G - ANNE_HAIR_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] >= ANNE_HAIR_B - ANNE_HAIR_TOLERANCE_B) {
					matched++;
				}
			}
			if (matched >= ANNE_HAIR_MATCHING) {
				lineFrame[pixel] = 0;
				lineFrame[pixel+1] = 255;
				lineFrame[pixel+2] = 0;
			}
			matched = 0;

			// Check if a surrounding pixel matches Anne's hair shade colour, accounting for tolerance settings
			for (size_t sPixel = 0; sPixel < sizeof(surroundingPixels)/sizeof(size_t); sPixel++) {
				//lineFrame[surroundingPixels[sPixel]] = 255;
				if (	origFrame[surroundingPixels[sPixel]  ] <= ANNE_HAIR_SHADE_R + ANNE_HAIR_SHADE_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] <= ANNE_HAIR_SHADE_G + ANNE_HAIR_SHADE_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] <= ANNE_HAIR_SHADE_B + ANNE_HAIR_SHADE_TOLERANCE_B &&
						origFrame[surroundingPixels[sPixel]  ] >= ANNE_HAIR_SHADE_R - ANNE_HAIR_SHADE_TOLERANCE_R && 
						origFrame[surroundingPixels[sPixel]+1] >= ANNE_HAIR_SHADE_G - ANNE_HAIR_SHADE_TOLERANCE_G && 
						origFrame[surroundingPixels[sPixel]+2] >= ANNE_HAIR_SHADE_B - ANNE_HAIR_SHADE_TOLERANCE_B) {
					matched++;
				}
			}
			if (matched >= ANNE_HAIR_SHADE_MATCHING) {
				lineFrame[pixel] = 0;
				lineFrame[pixel+1] = 255;
				lineFrame[pixel+2] = 0;
			}
			matched = 0;
		}
	}

	// Save our line file
	FILE *lineFile = fopen("line.rgb", "wb");
	fwrite(lineFrame, sizeof(uint8_t), FRAME_BUF_SIZE, lineFile);
	fclose(lineFile);

	return 0;
}