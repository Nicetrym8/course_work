#include "types.hpp"
#include <random>

int main()
{
    std::srand(std::time(0));
    size_t i = 200000;
    auto s = std::ofstream("test_data.bin", std::ios::binary);

    while (--i)
    {
        cereal::BinaryOutputArchive oarchive(s);
        std::shared_ptr<autodb::Vehicle> p = std::make_shared<autodb::PersonalTransport>(std::to_string(rand() % 10000), std::to_string(rand() % 10000), std::to_string(rand() % 10000), std::to_string(rand() % 10000), std::to_string(rand() % 10000));
        p.get()->stream_table(std::cout);
        oarchive(p);
    }
}
