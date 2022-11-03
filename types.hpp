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
    class IVehicle
    {
        friend class Controller;

    protected:
        std::string name = "";
        std::string model = "";
        std::string vehicle_number = "";

    public:
        IVehicle() {}
        IVehicle(std::istream &in, std::ostream &out);
        virtual ~IVehicle() {}
        virtual void stream(std::ostream &);
        virtual void stream_table(std::ostream &);
        template <class Archive>
        void serialize(Archive &ar);
        template <class T>
        friend std::ostream &operator<<(std::ostream &str, T &obj)
        {
            obj.stream(str);
            return str;
        }
        bool operator<(IVehicle &obj)
        {
            return this->name < obj.name;
        }
    };

    class PersonalTransport : public IVehicle
    {
    protected:
        std::string owners_firstname = "";
        std::string owners_lastname = "";

    public:
        PersonalTransport(std::istream &, std::ostream &);
        PersonalTransport(std::string, std::string, std::string, std::string, std::string);
        PersonalTransport(){};
        template <class Archive>
        void serialize(Archive &ar);
        void stream(std::ostream &) override;
        void stream_table(std::ostream &) override;
    };
    class OrganizationTransport : public IVehicle
    {
    protected:
        std::string organization;

    public:
        OrganizationTransport(std::istream &, std::ostream &);
        OrganizationTransport(std::string, std::string, std::string, std::string);
        OrganizationTransport(){};
        template <class Archive>
        void serialize(Archive &ar);
        void stream_table(std::ostream &) override;
        void stream(std::ostream &) override;
    };
    class IssuedTransport : public PersonalTransport
    {
        std::string organization;

    public:
        IssuedTransport() {}
        IssuedTransport(std::istream &, std::ostream &);
        template <class Archive>
        void serialize(Archive &ar);
        void stream_table(std::ostream &) override;
        void stream(std::ostream &) override;
    };
}
#endif