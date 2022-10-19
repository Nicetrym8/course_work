#ifndef CONSTS_HPP_
#define CONSTS_HPP_

namespace autodb
{
    const char manual[] = "Syntax: autodb [key] <arg>  file\nKeys:\n-h              Manual\n-s <arg>        Search in db\n-a              Add new record\n-d <arg>        Delete record\n-t              Print all records\n-p <arg>        Show record by registration number";
    const char args_array[][3] = {
        "-h",
        "-t",
        "-a",
        "-s",
        "-d",
        "-p",
    };
    enum eArgs
    {
        A_HELP,
        A_TABLE,
        A_ADD,
        A_SEARCH,
        A_DEL,
        A_SHOW,

    };
    enum eVehicleTypes
    {
        VT_PERSONAL,
        VT_PUBLIC,
    };
}
#endif
