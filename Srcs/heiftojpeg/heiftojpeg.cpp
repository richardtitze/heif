#include "hevcimagefilereader.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if ( argc != 3 ) {
        cout << "usage: heiftojpeg <input_file_name> <output_file_name>";
        return 1;
    }

    char *input_file_name = argv[1];
    char *output_file_name = argv[2];

    cout << "Converting HEIF image " << input_file_name << " to JPEG " << output_file_name << "\n";

    cout << "reading " << input_file_name << "...\n";
    HevcImageFileReader reader;
    reader.initialize(input_file_name);
    const auto& properties = reader.getFileProperties();

    // Verify that the file has one or several images in the MetaBox
    if (not (properties.fileFeature.hasFeature(ImageFileReaderInterface::FileFeature::HasSingleImage) ||
        properties.fileFeature.hasFeature(ImageFileReaderInterface::FileFeature::HasImageCollection)))
    {
        cout << "No image property found.\n";
        return 1;
    }


    cout << "getting master image id...\n";
    const uint32_t contextId = properties.rootLevelMetaBoxProperties.contextId;
    ImageFileReaderInterface::IdVector itemIds;
    reader.getItemListByType(contextId, "master", itemIds);
    const uint32_t masterId = itemIds.at(0);

    cout << "master image found with id " << masterId << ", getting image data...\n";
    ImageFileReaderInterface::DataVector data;
    reader.getItemDataWithDecoderParameters(contextId, masterId, data);

    cout << "item data received with size " << data.size() << "\n";

    return 0;
}

