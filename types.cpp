#include "types.hpp"

using namespace autodb;
inline std::string autodb::truncate(std::string &str)
{
    if (str.length() > 15)
        return str.substr(0, 12) + "...";
    return str;
}

Vehicle::Vehicle(std::istream &in, std::ostream &out)
{
    out << "Vendor: ";
    in >> name;
    out << "Model: ";
    in >> model;
    out << "Registration number: ";
    in >> vehicle_number;
}
void Vehicle::stream(std::ostream &str)
{
    str << "Vendor: " << name << '\n'
        << "Model: " << model << '\n'
        << "Registration number: " << vehicle_number << '\n';
}
void Vehicle::stream_table(std::ostream &str)
{
    str << "| " << std::setw(15) << truncate(name) << " | " << std::setw(15) << truncate(model) << " | " << std::setw(15) << truncate(vehicle_number) << " |";
}
PersonalTransport::PersonalTransport(std::string name, std::string model, std::string vehicle_number, std::string owners_firstname, std::string owners_lastname)
{
    this->owners_lastname = owners_lastname;
    this->owners_firstname = owners_firstname;
    Vehicle::model = model;
    Vehicle::name = name;
    Vehicle::vehicle_number = vehicle_number;
}
PersonalTransport::PersonalTransport(std::istream &in, std::ostream &out) : Vehicle(in, out)
{
    out << "Owner's firstname: ";
    in >> owners_firstname;
    out << "Owner's lastname: ";
    in >> owners_lastname;
}
void PersonalTransport::stream(std::ostream &str)
{
    Vehicle::stream(str);
    str << "Owner's firstname: " << owners_firstname << '\n'
        << "Owner's lastname: " << owners_lastname << "\n";
}
void PersonalTransport::stream_table(std::ostream &str)
{
    Vehicle::stream_table(str);
    str << std::setw(15) << truncate(owners_firstname) << " |" << std::setw(15) << truncate(owners_lastname) << " | " << std::setw(17) << " |" << '\n';
}

OrganizationTransport::OrganizationTransport(std::string name, std::string model, std::string vehicle_number, std::string organization)
{
    this->organization = organization;
    Vehicle::model = model;
    Vehicle::name = name;
    Vehicle::vehicle_number = vehicle_number;
}

OrganizationTransport::OrganizationTransport(std::istream &in, std::ostream &out) : Vehicle(in, out)
{

    out << "Owner organization: ";
    in >> organization;
}
void OrganizationTransport::stream(std::ostream &str)
{
    Vehicle::stream(str);
    str << "Owner organization: "
        << organization << "\n";
}
void OrganizationTransport::stream_table(std::ostream &str)
{
    Vehicle::stream_table(str);
    str << std::setw(18) << " | " << std::setw(17) << " | " << std::setw(15) << truncate(organization) << " |" << '\n';
}
IssuedTransport::IssuedTransport(std::istream &in, std::ostream &out) : PersonalTransport(in, out)
{
    out << "Owner organization: ";
    in >> organization;
}
void IssuedTransport::stream_table(std::ostream &str)
{
    Vehicle::stream_table(str);
    str << std::setw(15) << truncate(owners_firstname) << " |" << std::setw(15) << truncate(owners_lastname) << " | " << std::setw(15) << truncate(organization) << " |" << '\n';
}
void IssuedTransport::stream(std::ostream &str)
{
    PersonalTransport::stream(str);
    str << "Owner organization: "
        << organization << "\n";
}
CEREAL_REGISTER_TYPE(PersonalTransport);
CEREAL_REGISTER_TYPE(OrganizationTransport);
CEREAL_REGISTER_TYPE(IssuedTransport);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Vehicle, PersonalTransport);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Vehicle, OrganizationTransport);
CEREAL_REGISTER_POLYMORPHIC_RELATION(PersonalTransport, IssuedTransport);