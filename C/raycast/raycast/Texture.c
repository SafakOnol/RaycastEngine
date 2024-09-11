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
	"./images/armor.png",		// [13]

	// RETRO PACK IMAGES
	"./images/retropack01/BRICK_1A.PNG",		// [14] - 15
	"./images/retropack01/BRICK_2B.PNG",		// [15] - 16
	"./images/retropack01/BRICK_3B.PNG",		// [16] - 17
	"./images/retropack01/BRICK_3D.PNG",		// [17] - 18
	"./images/retropack01/BRICK_4A.PNG",		// [18] - 19
	"./images/retropack01/BRICK_6D.PNG",		// [19] - 20
	"./images/retropack01/CONCRETE_3C.PNG",		// [20] - 21
	"./images/retropack01/CONCRETE_4A.PNG",		// [21] - 22
	"./images/retropack01/CONSOLE_1B.PNG",		// [22] - 23
	"./images/retropack01/CRATE_1D.PNG",		// [23] - 24
	"./images/retropack01/CRATE_1E.PNG",		// [24] - 25
	"./images/retropack01/CRATE_2C.PNG",		// [25] - 26
	"./images/retropack01/CRATE_2M.PNG",		// [26] - 27
	"./images/retropack01/DIRT_1A.PNG",			// [27] - 28
	"./images/retropack01/DOOR_1A.PNG",			// [28] - 29
	"./images/retropack01/DOOR_1C.PNG",			// [29] - 30
	"./images/retropack01/DOOR_1E.PNG",			// [30] - 31
	"./images/retropack01/DOOR_4A.PNG",			// [31] - 32
	"./images/retropack01/DOORTRIM_1A.PNG",		// [32] - 33
	"./images/retropack01/FLOOR_1A.PNG",		// [33] - 34
	"./images/retropack01/FLOOR_3A.PNG",		// [34] - 35
	"./images/retropack01/FLOOR_4A.PNG",		// [35] - 36
	"./images/retropack01/GRASS_1A.PNG",		// [36] - 37
	"./images/retropack01/GRID_1A.PNG",			// [37] - 38
	"./images/retropack01/GRID_2B.PNG",			// [38] - 39
	"./images/retropack01/HEDGE_1A.PNG",		// [39] - 40
	"./images/retropack01/LAB_2B.PNG",			// [40] - 41
	"./images/retropack01/LIGHT_1B.PNG",		// [41] - 42
	"./images/retropack01/PAPER_1B.PNG",		// [42] - 43
	"./images/retropack01/PAPER_1E.PNG",		// [43] - 44
	"./images/retropack01/PAPER_1F.PNG",		// [44] - 45
	"./images/retropack01/PIPES_1A.PNG",		// [45] - 46
	"./images/retropack01/PIPES_2B.PNG",		// [46] - 47
	"./images/retropack01/RIVET_1A.PNG",		// [47] - 48
	"./images/retropack01/SAND_1A.PNG",			// [48] - 49
	"./images/retropack01/SAND_1A.PNG",			// [49] - 50
	"./images/retropack01/SLUDGE_1A.PNG",		// [50] - 51
	"./images/retropack01/STEEL_1A.PNG",		// [51] - 52
	"./images/retropack01/SUPPORT_3A.PNG",		// [52] - 53
	"./images/retropack01/SUPPORT_4A.PNG",		// [53] - 54
	"./images/retropack01/TECH_1C.PNG",			// [54] - 55
	"./images/retropack01/TECH_1E.PNG",			// [55] - 56
	"./images/retropack01/TECH_2F.PNG",			// [56] - 57
	"./images/retropack01/TECH_3B.PNG",			// [57] - 58
	"./images/retropack01/TECH_4E.PNG",			// [58] - 59
	"./images/retropack01/TECH_4F.PNG",			// [59] - 60
	"./images/retropack01/TILE_1A.PNG",			// [60] - 61
	"./images/retropack01/TILE_2C.PNG",			// [61] - 62
	"./images/retropack01/WARN_1A.PNG",			// [62] - 63
	"./images/retropack01/WOOD_1C.PNG"			// [63] - 64
	
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