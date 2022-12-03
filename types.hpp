#ifndef AUTODB_HPP_
#define AUTODB_HPP_

#include "depencies.hpp"

namespace autodb
{
    inline std::string truncate(std::string &str);

    class descriptive_exception : public std::exception
    {
    public:
        descriptive_exception(std::string const &message) : msg_(message) {}
        virtual char const *what() const noexcept { return msg_.c_str(); }

    private:
        std::string msg_;
    };
    class Vehicle
    {
        friend class Controller;

    protected:
        std::string name = "";
        std::string model = "";
        std::string vehicle_number = "";
        virtual void stream(std::ostream &);
        virtual void stream_table(std::ostream &);

    public:
        Vehicle() {}
        Vehicle(std::istream &in, std::ostream &out);
        virtual ~Vehicle() {}
        template <class Archive>
        void serialize(Archive &ar)
        {
            ar(name, model, vehicle_number);
        }
        template <class T>
        friend std::ostream &operator<<(std::ostream &str, T &obj)
        {
            obj.stream(str);
            return str;
        }
        bool operator<(Vehicle &obj)
        {
            return this->name < obj.name;
        }
    };

    class PersonalTransport : public Vehicle
    {

    protected:
        void stream(std::ostream &) override;
        void stream_table(std::ostream &) override;
        std::string owners_firstname = "";
        std::string owners_lastname = "";

    public:
        PersonalTransport(std::istream &, std::ostream &);
        PersonalTransport(std::string, std::string, std::string, std::string, std::string);
        PersonalTransport(){};
        template <class Archive>
        void serialize(Archive &ar)
        {
            ar(cereal::base_class<Vehicle>(this), owners_firstname, owners_lastname);
        }
    };
    class OrganizationTransport : public Vehicle
    {

    protected:
        void stream_table(std::ostream &) override;
        void stream(std::ostream &) override;
        std::string organization;

    public:
        OrganizationTransport(std::istream &, std::ostream &);
        OrganizationTransport(std::string, std::string, std::string, std::string);
        OrganizationTransport(){};
        template <class Archive>
        void serialize(Archive &ar)
        {
            ar(cereal::base_class<Vehicle>(this), organization);
        }
    };
    class IssuedTransport : public PersonalTransport
    {

    protected:
        void stream_table(std::ostream &) override;
        void stream(std::ostream &) override;
        std::string organization;

    public:
        IssuedTransport() {}
        IssuedTransport(std::istream &, std::ostream &);
        template <class Archive>
        void serialize(Archive &ar)
        {
            ar(cereal::base_class<PersonalTransport>(this), organization);
        }
    };
}
#endif