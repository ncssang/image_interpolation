#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

int main()
{
    std::ifstream input_file_stream;
    input_file_stream.open("/home/nq/Desktop/c1.txt");
    std::vector<std::string> word;
    if (input_file_stream.is_open())
    {
        std::string line;
        while (std::getline(input_file_stream, line))
        {
            word.push_back(line);            
        }
    }
    else
    {
        std::cout << "Find not found.\n";
    }
    input_file_stream.close();

    std::random_shuffle ( word.begin(), word.end() );

    std::ofstream output_file_stream;
    output_file_stream.open("c1-output.txt");
    for (size_t i = 0; i < word.size(); ++i)
    {
        output_file_stream << word[i] << std::endl;
    }
    output_file_stream.close();
    return 0;
}