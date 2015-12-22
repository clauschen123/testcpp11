#include <iostream>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

const char * say_what(bool b) { return b ? "true" : "false"; }

//��VS��x64�����н�ȥ����64λ��bjam��b2
//b2 address-model=64 --toolset=msvc-12.0 --with-coroutine debug
//./b2 address-model=64 --with-filesystem --stagedir = "/lib64" link = shared runtime - link = shared release

void boost_main()
{
    int argc = 1;
    char* argv = "d://psiphon3.exe";

    path p(argv);

    try
    {
        std::time_t t = boost::filesystem::last_write_time(p);
        std::cout << t << std::endl;
        std::cout << std::ctime(&t) << std::endl;
    }
    catch (boost::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}