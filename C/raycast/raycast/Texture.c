#include "Texture.h"
#include <stdio.h>

static const char* textureFileNames[NUM_TEXTURES] = 
{
	// WALL TEXTURES			[ARRAY] [MAP]
	"./images/redbrick.png",	// [0]	- 1
	"./images/purplestone.png",	// [1]	- 2
	"./images/mossystone.png",	// [2]	- 3
	"./images/graystone.png",	// [3]	- 4
	"./images/colorstone.png",	// [4]	- 5
	"./images/bluestone.png",	// [5]	- 6
	"./images/wood.png",		// [6]	- 7
	"./images/eagle.png",		// [7]	- 8
	"./images/blaskowicz2.png",	// [8]	- 9

	// SPRITE TEXTURES
	"./images/barrel.png",		//	[9]
	"./images/light.png",		// [10]
	"./images/table.png",		// [11]
	"./images/guard.png",		// [12]
	"./images/armor.png"		// [13]
};

void LoadTextures() {
	for (int i = 0; i < NUM_TEXTURES; i++) 
	{
		upng_t* upng = upng_new_from_file(textureFileNames[i]);

		if (upng != NULL) 
		{
			upng_decode(upng);
			if (upng_get_error(upng) == UPNG_EOK) {
				textures[i] = upng;
			}
			else
			{
				printf("Error decoding texture file %s \n", textureFileNames[i]);
			}
		}
		else
		{
			printf("Error loading texture %s \n", textureFileNames[i]);
		}
	}

}

void FreeTextures() {
	for (int i = 0; i < NUM_TEXTURES; i++) 
	{
		upng_free(textures[i]);
	}
}