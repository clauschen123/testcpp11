#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/yield.hpp>
#include <iostream>
#if 0
namespace Coroutine{
    int foo(boost::asio::coroutine& ct)
    {
        std::cout << "before reenter" << std::endl;

        reenter(ct)
        {
            std::cout << "before yield1" << std::endl;
            yield std::cout << "yield1" << std::endl;
            std::cout << "before yield2" << std::endl;
            yield return 1;
        }

        std::cout << "after reenter" << std::endl;
        return 2;
    }

    int main()
    {
        boost::asio::coroutine ct;
        while (!ct.is_complete())
        {
            int ret = foo(ct);
            std::cout << "return:" << ret << std::endl;
        }
        return 0;
    }

    //asio sample
    boost::asio::io_service ios;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 1234);
    boost::asio::ip::tcp::acceptor acceptor(ios, endpoint);

    char data[1024];

    void handle(boost::asio::coroutine ct, boost::asio::ip::tcp::socket* sock,
        const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (error)
        {
            std::cout << "read发生错误：" << error.message() << std::endl;
            delete sock;
            return;
        }

        reenter(ct)
        {
            yield boost::asio::async_write(*sock, boost::asio::buffer(data, bytes_transferred),
                boost::bind(handle, ct, sock,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));

            yield sock->async_read_some(boost::asio::buffer(data),
                boost::bind(handle, boost::asio::coroutine(), sock,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }

    }

    void session(boost::asio::ip::tcp::socket* sock,
        const boost::system::error_code& error)
    {
        if (error)
        {
            delete sock;
        }
        else
        {
            std::cout << "有客户端连接" << std::endl;

            sock->async_read_some(boost::asio::buffer(data),
                boost::bind(handle, boost::asio::coroutine(), sock,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }

        boost::asio::ip::tcp::socket* s = new boost::asio::ip::tcp::socket(ios);
        acceptor.async_accept(*s, boost::bind(session, s,
            boost::asio::placeholders::error));
    }

    int main2()
    {
        boost::asio::ip::tcp::socket* s = new boost::asio::ip::tcp::socket(ios);
        acceptor.async_accept(*s, boost::bind(session, s, boost::asio::placeholders::error));

        ios.run();
        return 0;
    }
}

void coroutine_main()
{
    Coroutine::main();
}
#endif