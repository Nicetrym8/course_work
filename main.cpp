#include "autodb.hpp"

int main(int argc, char **argv)
{
    auto c = autodb::Controller(argc, argv);
    c.exec();
}
