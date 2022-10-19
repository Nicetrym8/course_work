#include "autodb.hpp"
using namespace autodb;

template <typename T>
void Controller::errWrapper(T &&func)
{
    try
    {
        func();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl
                  << "To show manual use: autodb -h\n";
        std::exit(-1);
    }
};
template <typename T>
void Controller::readWrapper(T &&logic)
{
    static cereal::BinaryInputArchive iarchive(db);
    while (true)
    {
        try
        {
            std::shared_ptr<IVehicle> ptr;
            iarchive(ptr);
            logic(ptr);
        }
        catch (cereal::Exception &e)
        {
            break;
        }
    }
}
template <typename T>
void Controller::writeWrapper(std::shared_ptr<IVehicle> ptr, std::fstream &s, T &&logic)
{
    if (logic(ptr))
    {
        static cereal::BinaryOutputArchive oarchive(s);
        oarchive(ptr);
    }
}
void Controller::search(std::string querry)
{
    print_head();
    readWrapper([querry](std::shared_ptr<IVehicle> ptr)
                {std::stringstream ss;
                    ss << *ptr.get();
         if (ss.str().find(querry.c_str()) != std::string::npos) ptr.get()->stream_table(std::cout); });
}
void Controller::show_element(std::string number)
{
    readWrapper([number](std::shared_ptr<IVehicle> ptr)
                { if (ptr.get()->vehicle_number == number) std::cout << *ptr.get(); });
}
void Controller::delete_element(std::string number)
{
    readWrapper([number, this](std::shared_ptr<IVehicle> ptr)
                { this->writeWrapper(ptr, this->temp, [number](std::shared_ptr<IVehicle> p)
                                     { return !(p.get()->vehicle_number == number); }); });
    temp.close();
    db.close();
    std::remove(argv[argc - 1]);
    std::rename((std::string(argv[argc - 1]) + ".temp").c_str(), argv[argc - 1]);
}
void Controller::add_element()
{
    int type = 0;
    std::shared_ptr<IVehicle> ptr;
    errWrapper([&type]()
               {
        std::cout << "Choose type of transport (Personal - 0, Public - 1): ";
        std::cin >> type;
        if(type > VT_PUBLIC)throw descriptive_exception("Incorrect data type"); });

    switch (type)
    {
    case VT_PERSONAL:
        ptr = std::make_shared<PersonalTransport>(std::cin, std::cout);
        break;
    case VT_PUBLIC:
        ptr = std::make_shared<PublicTransport>(std::cin, std::cout);
        break;
    }
    writeWrapper(ptr, db, [](std::shared_ptr<IVehicle> ptr)
                 { return true; });
}

void Controller::print_all()
{
    print_head();
    readWrapper([](std::shared_ptr<IVehicle> ptr)
                { ptr.get()->stream_table(std::cout); });
}
void Controller::print_head()
{
    std::cout << std::string(107, '-') << '\n'
              << "| " << std::setw(15) << "Vendor"
              << " | " << std::setw(15) << "Model"
              << " | " << std::setw(15) << "Number"
              << " |" << std::setw(15) << "Firstname"
              << " |" << std::setw(15) << "Lastname"
              << " | " << std::setw(15) << "Organization"
              << " |\n"
              << std::string(107, '-') << '\n';
}

Controller::~Controller()
{
    if (temp.is_open())
        temp.close();
    if (db.is_open())
        db.close();
}
void Controller::exec()
{
    switch (index)
    {
    case A_HELP:
        std::cout << manual << std::endl;
        break;
    case A_TABLE:
        print_all();
        break;
    case A_ADD:
        add_element();
        break;
    case A_SEARCH:
        search(argv[2]);
        break;
    case A_DEL:
        delete_element(argv[2]);
        break;
    case A_SHOW:
        show_element(argv[2]);
        break;
    }
}
Controller::Controller(int argc, char **argv) : argc(argc), argv(argv)
{
    // print_head();
    index = 0;
    errWrapper([argc, argv, this]()
               {
                if (argc < 2) throw descriptive_exception("Too few arguments"); 
                   for (const auto& t : args_array){
                        if((this->index && argc<3) || (this->index > 2 && argc <4))throw descriptive_exception("Too few arguments");
                        if(std::string(t) == std::string(argv[1]))return;
                        this->index++;
                    }
                throw descriptive_exception("This key is not in the parameter list"); });

    if (argc > 2)
    {
        db = std::fstream(argv[argc - 1], std::ios::in | std::ios::app | std::ios::binary);
        if (std::string(argv[1]) == args_array[A_DEL])
            temp = std::fstream(std::string(argv[argc - 1]) + ".temp", std::ios::out | std::ios::binary);
    }
}
