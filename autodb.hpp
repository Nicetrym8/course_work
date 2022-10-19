
#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include "types.hpp"

namespace autodb
{

    class Controller
    {
    private:
        int argc;
        char **argv;
        size_t index;
        std::fstream db;
        std::fstream temp;
        template <typename T>
        void errWrapper(T &&);
        template <typename T>
        void readWrapper(T &&);
        template <typename T>
        void writeWrapper(std::shared_ptr<IVehicle>, std::fstream &, T &&);
        void print_head();
        void search(std::string);
        void show_element(std::string);
        void delete_element(std::string);
        void add_element();
        void print_all();

    public:
        ~Controller();
        Controller(int, char **);
        void exec();
    };

}

#endif