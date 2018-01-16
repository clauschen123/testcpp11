#include <functional>
#include <iostream>
#include <boost/function.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

const char * say_what(bool b) { return b ? "true" : "false"; }

//从VS的x64命令行进去编译64位的bjam，b2
//b2 address-model=64 --toolset=msvc-12.0 --with-coroutine debug
//./b2 address-model=64 --with-filesystem --stagedir = "/lib64" link = shared runtime - link = shared release
//./b2 address-model=64 --with-filesystem --stagedir = "/lib64" link = shared runtime - link = shared release
//bjam address-model=64 --toolset=msvc-14.0 --with-filesystem --stagedir = "/lib64" --build-type=complete stage 
/**
全部编译的命令行：bjam --toolset=msvc-14.0 --build-type=complete stage
　　部分编译的命令行：bjam --toolset=msvc-14.0 address-model=64 link=static runtime-link=static --with-thread
　　编译成64位的命令：bjam --toolset=msvc-14.0 address-model=64 --build-type=complete stage 
　　查看需要编译库的命令行：bjam --show-libraries
bjam --toolset=msvc-14.0 address-model=64 --with-date_time --with-thread  --with-chrono  link=static runtime-link=static --stagedir="./bin/vs15_64" release
**/	 
namespace Test {
    int main()
    {
        boost::lockfree::queue<boost::function<void(int)>*> queue(3);
        assert(queue.is_lock_free());

        for (int j = 0; j < 50; ++j) {
            auto function = [](int i) { std::cout << i << std::endl; };
            queue.push(new boost::function<void(int)>(function));
        }

        int i = 0;
        boost::function<void(int)> * functor;
        while (queue.pop(functor)) {
            functor->operator()(i++);
            delete functor;
        }
        return 1;
    }
}
void boost_main()
{
    Test::main();

    vector<int> v1 = { 1, 2,3,4,5};
    vector<int> v2(10,2);

    auto end = v1.end();
    if (v1.size() > 3)
        end = v1.begin() +3;
    v2.assign(v1.begin(), end);
    v2.assign(v1.begin(), v1.end());

    auto it = find(v1.begin(), v1.end(), 4);
    unsigned char idx = unsigned char(distance(v1.begin(), it)) + 1;

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