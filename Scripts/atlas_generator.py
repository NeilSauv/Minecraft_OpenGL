import os
from PIL import Image
import math

def create_texture_atlas(directory, output_file, code_file):
    images = [os.path.join(directory, f) for f in sorted(os.listdir(directory)) if f.endswith('.png')]
    images.sort()

    image_data = {}
    index_map = {}
    for img_path in images:
        img = Image.open(img_path)
        name = (os.path.splitext(os.path.basename(img_path)))[0]
        top = "_top" in name
        bottom = "_bottom" in name
        side = "_side" in name

        name = name.replace("_top", "")
        name = name.replace("_bottom", "")
        name = name.replace("_side", "")
        block_type = name.upper()

        if block_type not in image_data:
            image_data[block_type] = {}

        if top:
            suffix = "top"
        elif bottom:
            suffix = "bottom"
        elif side:
            suffix = "side"
        else:
            suffix = "none"
        image_data[block_type][suffix] = img

    # Déterminer la grille pour l'atlas
    num_images = sum(len(v) for v in image_data.values())
    num_columns = int(math.ceil(math.sqrt(num_images)))
    atlas_width = num_columns * 16  # Assuming each texture is 16 pixels wide
    atlas_height = int(math.ceil(num_images / num_columns)) * 16  # Assuming each texture is 16 pixels high

    atlas = Image.new('RGBA', (atlas_width, atlas_height))
    x, y = 0, 0
    index = 0

    for block, textures in image_data.items():
        for suffix, img in textures.items():
            atlas.paste(img, (x, y))
            index_map[f"{block}_{suffix}"] = index
            index += 1
            x += 16
            if x >= atlas_width:
                x = 0
                y += 16

    atlas.save(output_file, 'PNG')

    with open(code_file + ".c", 'w') as file:
        file.write('#include "Block.h"\n#include <Generators/Noises/NoiseStruct.h>\n#include <Textures/ColorMap.h>\n#include <Textures/BlockDef.h>\n#include <stdlib.h>\n\n')
        file.write('void InitBlockPattern(struct SimplexNoiseObj *noise)\n{\n    ColorScheme *colorScheme = noise->colorScheme;\n    int size = colorScheme->size;\n    BlockPattern **patterns = malloc(sizeof(BlockPattern *) * size);\n    colorScheme->patterns = patterns;\n\n')
        for block, textures in image_data.items():
            none = index_map.get(f"{block}_none", -1)
            top = index_map.get(f"{block}_top", none)
            bottom = index_map.get(f"{block}_bottom", top)
            side = index_map.get(f"{block}_side", none)

            file.write(f'    AddToBlockPattern(BLOCK_{block}, {top}, {bottom}, {side}, {side}, {side}, {side}, patterns);\n')
        file.write('}\n')

    with open(code_file + ".h", 'w') as file:
        file.write('#ifndef BLOCK_H\n#define BLOCK_H\n\nstruct SiplexNoiseObje;\n#include <Generators/Noises/NoiseStruct.h>\n\ntypedef enum BlockTypeEnum\n{\n')
        file.write(f'    BLOCK_AIR = -1,\n')
        for block in image_data:
            file.write(f'    BLOCK_{block},\n')
        file.write('} BlockTypeEnum;\n\nvoid InitBlockPattern(struct SimplexNoiseObj *noise);\n\n#endif // !BLOCK_H')



# Usage
create_texture_atlas('Assets/Block', 'Textures/Pictures/atlas.png', 'Textures/Block')
