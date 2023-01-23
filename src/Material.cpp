#include "Material.h"
#include "Utils.h"

void Material::ParseData(std::ifstream& file)
{
    char buffer[40] = { '\0' };
    do
    {
        file.getline(buffer, 40);
        if(buffer[0] == '\0' || buffer[0] == '#')
            break;

        //read diffuse
        std::string line(buffer);
        size_t found = line.find(' ');

        if (found != line.npos)
        {
            std::string type = line.substr(0, found);
            std::string vector = line.substr(found + 1u).data();
            const char* info = vector.data();
            if (type.compare("DIFFUSE") == 0)
                mDiffuse = Color(Utils::GetVector(&info));
        }

    } while (!file.eof());
}
