#include "Textures.h"
#include <stdio.h>

static const char* textureFileNames[NUM_TEXTURES] = 
{
	"./images/redbrick.png",
	"./images/purplestone.png",
	"./images/mossystone.png",
	"./images/graystone.png",
	"./images/colorstone.png",
	"./images/bluestone.png",
	"./images/wood.png",
	"./images/eagle.png",
	"./images/blaskowicz2.png"
};

void LoadWallTextures() {
	for (int i = 0; i < NUM_TEXTURES; i++) 
	{
		upng_t* upng;

		upng = upng_new_from_file(textureFileNames[i]);
		if (upng != NULL) 
		{
			upng_decode(upng);
			if (upng_get_error(upng) == UPNG_EOK) {
				WallTextures[i].upngTexture = upng;
				WallTextures[i].width = upng_get_width(upng);
				WallTextures[i].height = upng_get_height(upng);
				WallTextures[i].texture_buffer = (uint32_t*)upng_get_buffer(upng);
			}
		}
	}

}

void FreeWallTextures() {
	for (int i = 0; i < NUM_TEXTURES; i++) 
	{
		upng_free(WallTextures[i].upngTexture);
	}
}