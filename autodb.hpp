
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
        size_t count_pages();
        template <typename T>
        void errWrapper(T &&);
        template <typename T>
        void readWrapper(T &&);
        template <typename T>
        void writeWrapper(std::shared_ptr<Vehicle>, std::fstream &, T &&);
        void print_head();
        void search(std::string);
        void show_element(std::string);
        void delete_element(std::string);
        void show_page(char *);
        void add_element();
        void print_all();

    public:
        ~Controller();
        Controller(int, char **);
        void exec();
    };

}

#endif