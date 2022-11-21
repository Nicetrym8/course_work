#ifndef CONSTS_HPP_
#define CONSTS_HPP_

namespace autodb
{
    const char manual[] = " [key] <arg>  file\nKeys:\n-h              Manual\n-s <arg>        Search in db\n-a              Add new record\n-d <arg>        Delete record\n-t              Print all records\n-p <arg>        Show record by registration number\n-w <arg>        Print sorted page\n-q              Show page quantity";
    const char args_array[][3] = {
        "-h",
        "-t",
        "-a",
        "-q",
        "-d",
        "-p",
        "-w",
        "-s"};
    enum eArgs
    {
        A_HELP,
        A_TABLE,
        A_ADD,
        A_QUANTITY,
        A_DEL,
        A_SHOW,
        A_PAGE,
        A_SEARCH,
    };
    enum eVehicleTypes
    {
        VT_PERSONAL,
        VT_PUBLIC,
        VT_ISSUED,
    };
}
#endif
