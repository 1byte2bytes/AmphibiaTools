#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* --- BEGIN SETTINGS --- */
#define FRAME_HEIGHT 1080
#define FRAME_WIDTH 1920
// the work that would have to be done to make the filter support 
// more than 8 bit depth is more than it's worth at this time
#define FRAME_BUF_SIZE FRAME_WIDTH*FRAME_HEIGHT*3

#define BLACK_BORDER_TOLERANCE_R 50
#define BLACK_BORDER_TOLERANCE_G 50
#define BLACK_BORDER_TOLERANCE_B 50

// = Anne's skin =
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

// = Anne's hair =
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

// = Anne's clothing =
// #BDC6C0
#define ANNE_SHIRT_R 0xBD
#define ANNE_SHIRT_G 0xC6
#define ANNE_SHIRT_B 0xC0
#define ANNE_SHIRT_TOLERANCE_R 15
#define ANNE_SHIRT_TOLERANCE_G 15
#define ANNE_SHIRT_TOLERANCE_B 15
#define ANNE_SHIRT_MATCHING 3

// #9DA4AD
#define ANNE_SHIRT_SHADE_R 0x9D
#define ANNE_SHIRT_SHADE_G 0xA4
#define ANNE_SHIRT_SHADE_B 0xAD
#define ANNE_SHIRT_SHADE_TOLERANCE_R 15
#define ANNE_SHIRT_SHADE_TOLERANCE_G 15
#define ANNE_SHIRT_SHADE_TOLERANCE_B 15
#define ANNE_SHIRT_SHADE_MATCHING 2

// #A4708A <-- handles both skirt, collar, and purple in shirt badge
#define ANNE_SKIRT_R 0xA4
#define ANNE_SKIRT_G 0x70
#define ANNE_SKIRT_B 0x8A
#define ANNE_SKIRT_TOLERANCE_R 15
#define ANNE_SKIRT_TOLERANCE_G 15
#define ANNE_SKIRT_TOLERANCE_B 15
#define ANNE_SKIRT_MATCHING 3

// #8B5A7B
#define ANNE_SKIRT_SHADE_R 0x8B
#define ANNE_SKIRT_SHADE_G 0x5A
#define ANNE_SKIRT_SHADE_B 0x7B
#define ANNE_SKIRT_SHADE_TOLERANCE_R 15
#define ANNE_SKIRT_SHADE_TOLERANCE_G 15
#define ANNE_SKIRT_SHADE_TOLERANCE_B 15
#define ANNE_SKIRT_SHADE_MATCHING 1
/* ---  END SETTINGS  --- */

// R, G, B values, then R, G, B tolerances
// Helpder function to check if a pixel matches the colour values specified, within the tolerance values provided
int matchPixelColour(uint8_t* pixels, size_t pixel, uint8_t R, uint8_t G, uint8_t B, uint8_t R_t, uint8_t G_t, uint8_t B_t) {
	if (	pixels[pixel  ] <= R + R_t && 
			pixels[pixel+1] <= G + G_t && 
			pixels[pixel+2] <= B + B_t &&
			pixels[pixel  ] >= R - R_t && 
			pixels[pixel+1] >= G - G_t && 
			pixels[pixel+2] >= B - B_t) {
		return 1;
	}
	return 0;
}

int surroundingPixelMatching(size_t* checkPixels, size_t checkPixelC, uint8_t* pixels, uint8_t R, uint8_t G, uint8_t B, 
	uint8_t R_t, uint8_t G_t, uint8_t B_t, uint8_t matchCount) {
	// Check if a surrounding pixel matches specified colour, accounting for tolerance settings
	size_t matched = 0;
	for (size_t sPixel = 0; sPixel < checkPixelC; sPixel++) {
		matched += matchPixelColour(pixels, checkPixels[sPixel], 
			R, G, B, R_t, R_t, B_t);
	}
	if (matched >= matchCount) {
		return 1;
	}
	return 0;
}

int main(void) {
	// Create frame buffers
	static uint8_t origFrame[FRAME_BUF_SIZE];
	static uint8_t lineFrame[FRAME_BUF_SIZE];

	// Load our raw RGB file into a uint8_t array "origFrame"
	FILE *rgbFile = fopen("963.rgb", "rb");
	fread(origFrame, sizeof(uint8_t), FRAME_BUF_SIZE, rgbFile);
	fclose(rgbFile);

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

			// Check if the border pixel is around Anne's skin
			matched += surroundingPixelMatching(
				surroundingPixels, sizeof(surroundingPixels)/sizeof(size_t), origFrame,
				ANNE_SKIN_R, ANNE_SKIN_G, ANNE_SKIN_B, 
				ANNE_SKIN_TOLERANCE_R, ANNE_SKIN_TOLERANCE_G, ANNE_SKIN_TOLERANCE_B,
				ANNE_SKIN_MATCHING
			);
			matched += surroundingPixelMatching(
				surroundingPixels, sizeof(surroundingPixels)/sizeof(size_t), origFrame,
				ANNE_SKIN_SHADE_R, ANNE_SKIN_SHADE_G, ANNE_SKIN_SHADE_B, 
				ANNE_SKIN_SHADE_TOLERANCE_R, ANNE_SKIN_SHADE_TOLERANCE_G, ANNE_SKIN_SHADE_TOLERANCE_B,
				ANNE_SKIN_SHADE_MATCHING
			);
			if (matched >= 1) {
				lineFrame[pixel] = 0;
				lineFrame[pixel+1] = 0;
				lineFrame[pixel+2] = 255;
			}
			matched = 0;

			// Check if the border pixel is around Anne's hair
			matched += surroundingPixelMatching(
				surroundingPixels, sizeof(surroundingPixels)/sizeof(size_t), origFrame,
				ANNE_HAIR_R, ANNE_HAIR_G, ANNE_HAIR_B, 
				ANNE_HAIR_TOLERANCE_R, ANNE_HAIR_TOLERANCE_G, ANNE_HAIR_TOLERANCE_B,
				ANNE_HAIR_MATCHING
			);
			matched += surroundingPixelMatching(
				surroundingPixels, sizeof(surroundingPixels)/sizeof(size_t), origFrame,
				ANNE_HAIR_SHADE_R, ANNE_HAIR_SHADE_G, ANNE_HAIR_SHADE_B, 
				ANNE_HAIR_SHADE_TOLERANCE_R, ANNE_HAIR_SHADE_TOLERANCE_G, ANNE_HAIR_SHADE_TOLERANCE_B,
				ANNE_HAIR_SHADE_MATCHING
			);
			if (matched >= 1) {
				lineFrame[pixel] = 0;
				lineFrame[pixel+1] = 255;
				lineFrame[pixel+2] = 0;
			}
			matched = 0;

			// Check if the border pixel is around Anne's clothes
			matched += surroundingPixelMatching(
				surroundingPixels, sizeof(surroundingPixels)/sizeof(size_t), origFrame,
				ANNE_SHIRT_R, ANNE_SHIRT_G, ANNE_SHIRT_B, 
				ANNE_SHIRT_TOLERANCE_R, ANNE_SHIRT_TOLERANCE_G, ANNE_SHIRT_TOLERANCE_B,
				ANNE_SHIRT_MATCHING
			);
			matched += surroundingPixelMatching(
				surroundingPixels, sizeof(surroundingPixels)/sizeof(size_t), origFrame,
				ANNE_SHIRT_SHADE_R, ANNE_SHIRT_SHADE_G, ANNE_SHIRT_SHADE_B, 
				ANNE_SHIRT_SHADE_TOLERANCE_R, ANNE_SHIRT_SHADE_TOLERANCE_G, ANNE_SHIRT_SHADE_TOLERANCE_B,
				ANNE_SHIRT_SHADE_MATCHING
			);
			matched += surroundingPixelMatching(
				surroundingPixels, sizeof(surroundingPixels)/sizeof(size_t), origFrame,
				ANNE_SKIRT_R, ANNE_SKIRT_G, ANNE_SKIRT_B, 
				ANNE_SKIRT_TOLERANCE_R, ANNE_SKIRT_TOLERANCE_G, ANNE_SKIRT_TOLERANCE_B,
				ANNE_SKIRT_MATCHING
			);
			matched += surroundingPixelMatching(
				surroundingPixels, sizeof(surroundingPixels)/sizeof(size_t), origFrame,
				ANNE_SKIRT_SHADE_R, ANNE_SKIRT_SHADE_G, ANNE_SKIRT_SHADE_B, 
				ANNE_SKIRT_SHADE_TOLERANCE_R, ANNE_SKIRT_SHADE_TOLERANCE_G, ANNE_SKIRT_SHADE_TOLERANCE_B,
				ANNE_SKIRT_SHADE_MATCHING
			);
			if (matched >= 1) {
				lineFrame[pixel] = 255;
				lineFrame[pixel+1] = 0;
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