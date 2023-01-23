#include <string>
#include "Utils.h"

glm::vec3 Utils::GetVector(const char** info)
{
    if(info == nullptr)
        return glm::vec3(0.0F);

    glm::vec3 vec(0.0F);
    int coord = 0;

    while (**info != ')' && **info != '\0' && **info != '\n')
    {
        if (**info == '(' || **info == ',' || **info == ' ')
        {
            (*info)++;
            continue;
        }
        else
        {
            vec[coord] = GetFloat(info);
            coord++;
        }

    }

    (*info)++;
    return vec;
}



float Utils::GetFloat(const char** info)
{
    if(info == nullptr)
        return 0.0f;

    float val = 0.0F;
    std::string number;
    char* end{};

    while (**info != ',' && **info != ')' && **info != '\0' && **info != '\n')
    {
        number.push_back(**info);
        (*info)++;
    }

    val = std::strtof(number.data(), &end);

    return val;
}
