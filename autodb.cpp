#include "autodb.hpp"
using namespace autodb;

template <typename T>
void Controller::errWrapper(T &&logic)
{
    try
    {
        logic();
    }
    catch (const std::exception &e)
    {
        ostr << e.what() << std::endl
             << "To show manual use: " << argv[0] << " -h\n";
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
            std::shared_ptr<Vehicle> ptr;
            iarchive(ptr);
            logic(ptr);
        }
        catch (cereal::Exception &e)
        {
            if (std::string(e.what()).find("Error while trying to deserialize a smart pointer") != std::string::npos)
                ostr << "This file is corrupted" << std::endl;
            db.seekg(std::ios::beg);
            break;
        }
    }
}
template <typename T>
void Controller::writeWrapper(std::shared_ptr<Vehicle> ptr, std::fstream &s, T &&logic)
{
    if (logic(ptr))
    {
        cereal::BinaryOutputArchive oarchive(s);
        oarchive(ptr);
    }
}
void Controller::search(std::string query)
{
    print_head();
    readWrapper([query, this](std::shared_ptr<Vehicle> ptr)
                {std::stringstream ss;
                    ss << *ptr.get();
         if (ss.str().find(query.c_str()) != std::string::npos) ptr->stream_table(ostr); });
}
void Controller::show_element(std::string number)
{
    readWrapper([number, this](std::shared_ptr<Vehicle> ptr)
                { if (ptr->vehicle_number == number){
                    ostr << *ptr.get();
                    throw cereal::Exception("Result found");
                } });
}
void Controller::delete_element(std::string number)
{
    readWrapper([number, this](std::shared_ptr<Vehicle> ptr)
                { this->writeWrapper(ptr, this->temp, [number](std::shared_ptr<Vehicle> p)
                                     { return !(p.get()->vehicle_number == number); }); });
    temp.close();
    db.close();
    std::remove(argv[argc - 1]);
    std::rename((std::string(argv[argc - 1]) + ".temp").c_str(), argv[argc - 1]);
}
void Controller::add_element()
{
    int type = 0;
    std::shared_ptr<Vehicle> ptr;

    errWrapper([&type, this]()
               {
        ostr << "Choose type of transport (Personal - 0, Organization - 1, Issued - 2): ";
        if(!(istr >> type) ||type > VT_ISSUED)throw descriptive_exception("Incorrect data type"); });

    switch (type)
    {
    case VT_PERSONAL:
        ptr = std::make_shared<PersonalTransport>(istr, ostr);
        break;
    case VT_PUBLIC:
        ptr = std::make_shared<OrganizationTransport>(istr, ostr);
        break;
    case VT_ISSUED:
        ptr = std::make_shared<IssuedTransport>(istr, ostr);
        break;
    }
    writeWrapper(ptr, db, [](std::shared_ptr<Vehicle> ptr)
                 { return true; });
}
size_t Controller::count_pages()
{
    size_t counter = 0;
    readWrapper([&counter](std::shared_ptr<Vehicle> ptr)
                { counter++; });
    return counter / 10 + 1;
}
void Controller::show_page(char *arg)
{

    errWrapper([arg, this]()
               {
        std::vector<std::shared_ptr<Vehicle>> vec;
        size_t index = atoi(arg);
        size_t counter = 0;
         if (index && index <= count_pages())
        {
        index *= 10;
        readWrapper([&vec, &counter, index](std::shared_ptr<Vehicle> ptr)
                    { 
                      if(counter >= index-10) vec.push_back(ptr); 
                      counter++;
                      if (counter >= index)throw cereal::Exception("Result found");});
        std::sort(vec.begin(), vec.end(), [](std::shared_ptr<Vehicle> a, std::shared_ptr<Vehicle> b)
                  { return a.get()->operator<(*(b.get())); });
        print_head();
        for (const auto &t : vec)
            t.get()->stream_table(ostr);
        return;
     }
        throw descriptive_exception("Wrong page index"); });
}
void Controller::print_all()
{
    print_head();
    readWrapper([this](std::shared_ptr<Vehicle> ptr)
                { ptr->stream_table(ostr); });
}
void Controller::print_head()
{
    ostr << std::string(107, '-') << '\n'
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
        ostr << "Syntax: " << argv[0] << manual << std::endl;
        break;
    case A_TABLE:
        print_all();
        break;
    case A_QUANTITY:
        ostr << "Total page quantity: " << count_pages() << std::endl;
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
    case A_PAGE:
        show_page(argv[2]);
        break;
    }
}

Controller::Controller(int argc, char **argv, std::istream &istr, std::ostream &ostr) : argc(argc), argv(argv), ostr(ostr), istr(istr)
{
    index = 0;
    errWrapper([argc, argv, this]()
               {
                if (argc < 2) throw descriptive_exception("Too few arguments"); 
                   for (const auto& t : args_array){
                        if((this->index && argc<3) || (this->index > 3 && argc <4))throw descriptive_exception("Too few arguments");
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
