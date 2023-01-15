#include <string>
#include "Utils.h"

void Utils::GetData(const char* info)
{
    if (info == nullptr)
        return;

    std::string line(info);
    do
    {
        size_t pos = line.find(')');
        if (pos != line.npos)
        {
            //sf::Vector3<float> vec = GetVector(info, 1, pos);
        }

    } while (line.empty());

}

sf::Vector3<float> Utils::GetVector(const char* info, size_t& start)
{
    if(info == nullptr)
        return sf::Vector3<float>();

    sf::Vector3<float> vec;

    std::string line(info);
    size_t start_pos = line.find('(');
    size_t end_pos = line.find(')');

    if (start_pos != line.npos && end_pos != line.npos)
    {
        std::string number;
        for (int i = 0; i < 3; i++)
        {
            for (size_t j = start_pos + 1; j < end_pos; j++)
            {
                if (info[j] != ',')
                    number.push_back(info[j]);
                else
                {
                    start_pos = j + 1;
                    break;
                }
            }

            if (i == 0)
                vec.x = GetFloat(info, start, start_pos);
            if (i == 1)
                vec.y = GetFloat(info, start, start_pos);
            if (i == 2)
                vec.z = GetFloat(info, start, end_pos - 1);

            start = start_pos - 1;

            number.clear();
        }
    }

    return vec;
}



float Utils::GetFloat(const char* info, int start, int end)
{
    if(info == nullptr)
        return 0.0f;
    float val = 0.0F;

    std::string line(info);
    size_t pos = line.find_last_of(')');
    if (pos != line.npos)
    {
        char c = '\0';
        std::string number;
        char* end{};

        for (size_t j = pos; j < pos; j++)
        {
            if (info[j] != ',' && info[j] != '\0' && info[j] != '\n')
                number.push_back(info[j]);
            else
            {
                start = j + 1;
                break;
            }
        }
        val = std::strtof(number.data(), &end);
    }

    return val;
}
