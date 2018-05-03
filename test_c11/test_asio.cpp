#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif


#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <boost/coroutine/coroutine.hpp>
#include <boost/asio/yield.hpp>

using namespace boost::asio;
io_service service;


#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)


namespace asio
{
    /** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers
    - server disconnects any client that hasn't pinged for 5 seconds

    Possible requests:
    - gets a list of all connected clients
    - ping: the server answers either with "ping ok" or "ping client_list_changed"
    */
    class talk_to_svr : public boost::enable_shared_from_this<talk_to_svr>
        , public coroutine, boost::noncopyable {
        typedef talk_to_svr self_type;
        talk_to_svr(const std::string & username)
            : sock_(service), started_(false), username_(username), timer_(service) {}
        void start(ip::tcp::endpoint ep) {
            sock_.async_connect(ep, MEM_FN2(step, _1, 0));
        }
    public:
        typedef boost::system::error_code error_code;
        typedef boost::shared_ptr<talk_to_svr> ptr;

        static ptr start(ip::tcp::endpoint ep, const std::string & username) {
            ptr new_(new talk_to_svr(username));
            new_->start(ep); // start ourselves
            return new_;
        }
        void step(const error_code & err = error_code(), size_t bytes = 0) {
            reenter(this) {
                for (;;) {
                    if (!started_) {
                        started_ = true;
                        std::ostream out(&write_buffer_);
                        out << "login " << username_ << "\n";
                    }
                    yield async_write(sock_, write_buffer_, MEM_FN2(step, _1, _2));
                    yield async_read_until(sock_, read_buffer_, "\n", MEM_FN2(step, _1, _2));
                    yield service.post(MEM_FN(on_answer_from_server));
                }
            }
        }
    private:
        void on_answer_from_server() {
            std::istream in(&read_buffer_);
            std::string word; in >> word;
            if (word == "login") on_login();
            else if (word == "ping") on_ping();
            else if (word == "clients") on_clients();
            else std::cerr << "invalid msg " << std::endl;
            read_buffer_.consume(read_buffer_.size());
            if (write_buffer_.size() > 0)
                service.post(MEM_FN2(step, error_code(), 0));
        }

        void on_login() {
            std::cout << username_ << " logged in" << std::endl;
            do_ask_clients();
        }
        void on_ping() {
            std::istream in(&read_buffer_);
            std::string answer;
            in >> answer;
            if (answer == "client_list_changed") do_ask_clients();
            else postpone_ping();
        }
        void on_clients() {
            std::ostringstream clients;
            clients << &read_buffer_;
            std::cout << username_ << ", new client list:" << clients.str();
            postpone_ping();
        }

        void do_ping() {
            std::ostream out(&write_buffer_); out << "ping\n";
            service.post(MEM_FN2(step, error_code(), 0));
        }
        void postpone_ping() {
            // note: even though the server wants a ping every 5 secs, we randomly 
            // don't ping that fast - so that the server will randomly disconnect us
            int millis = rand() % 3000;
            std::cout << username_ << " postponing ping " << millis
                << " millis" << std::endl;
            timer_.expires_from_now(boost::posix_time::millisec(millis));
            timer_.async_wait(MEM_FN(do_ping));
        }
        void do_ask_clients() {
            std::ostream out(&write_buffer_); out << "ask_clients\n";
        }

    private:
        ip::tcp::socket sock_;
        streambuf read_buffer_, write_buffer_;
        bool started_;
        std::string username_;
        deadline_timer timer_;
    };

    void main() {
        ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
        talk_to_svr::start(ep, "John");
        talk_to_svr::start(ep, "Suzie");
        service.run();
    }
}
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <istream>
#include <iostream>
#include <ostream>

#include "icmp_header.hpp"
#include "ipv4_header.hpp"

using boost::asio::ip::icmp;
using boost::asio::deadline_timer;
namespace posix_time = boost::posix_time;
namespace ping {

    class pinger
    {
    public:
        pinger(boost::asio::io_service& io_service, const char* destination)
            : resolver_(io_service), socket_(io_service, icmp::v4()),
            timer_(io_service), sequence_number_(0), num_replies_(0)
        {
            icmp::resolver::query query(icmp::v4(), destination, "");
            destination_ = *resolver_.resolve(query);

            start_send();
            start_receive();
        }

    private:
        void start_send()
        {
            std::string body("\"Hello!\" from Asio ping.");

            // Create an ICMP header for an echo request.
            icmp_header echo_request;
            echo_request.type(icmp_header::echo_request);
            echo_request.code(0);
            echo_request.identifier(get_identifier());
            echo_request.sequence_number(++sequence_number_);
            compute_checksum(echo_request, body.begin(), body.end());

            // Encode the request packet.
            boost::asio::streambuf request_buffer;
            std::ostream os(&request_buffer);
            os << echo_request << body;

            // Send the request.
            time_sent_ = posix_time::microsec_clock::universal_time();
            socket_.send_to(request_buffer.data(), destination_);

            // Wait up to five seconds for a reply.
            num_replies_ = 0;
            timer_.expires_at(time_sent_ + posix_time::seconds(5));
            timer_.async_wait(boost::bind(&pinger::handle_timeout, this));
        }

        void handle_timeout()
        {
            if (num_replies_ == 0)
                std::cout << "Request timed out" << std::endl;

            // Requests must be sent no less than one second apart.
            timer_.expires_at(time_sent_ + posix_time::seconds(1));
            timer_.async_wait(boost::bind(&pinger::start_send, this));
        }

        void start_receive()
        {
            // Discard any data already in the buffer.
            reply_buffer_.consume(reply_buffer_.size());

            // Wait for a reply. We prepare the buffer to receive up to 64KB.
            socket_.async_receive(reply_buffer_.prepare(65536),
                boost::bind(&pinger::handle_receive, this, _2));
        }

        void handle_receive(std::size_t length)
        {
            // The actual number of bytes received is committed to the buffer so that we
            // can extract it using a std::istream object.
            reply_buffer_.commit(length);

            // Decode the reply packet.
            std::istream is(&reply_buffer_);
            ipv4_header ipv4_hdr;
            icmp_header icmp_hdr;
            is >> ipv4_hdr >> icmp_hdr;

            // We can receive all ICMP packets received by the host, so we need to
            // filter out only the echo replies that match the our identifier and
            // expected sequence number.
            if (is && icmp_hdr.type() == icmp_header::echo_reply
                && icmp_hdr.identifier() == get_identifier()
                && icmp_hdr.sequence_number() == sequence_number_)
            {
                // If this is the first reply, interrupt the five second timeout.
                if (num_replies_++ == 0)
                    timer_.cancel();

                // Print out some information about the reply packet.
                posix_time::ptime now = posix_time::microsec_clock::universal_time();
                std::cout << length - ipv4_hdr.header_length()
                    << " bytes from " << ipv4_hdr.source_address()
                    << ": icmp_seq=" << icmp_hdr.sequence_number()
                    << ", ttl=" << ipv4_hdr.time_to_live()
                    << ", time=" << (now - time_sent_).total_milliseconds() << " ms"
                    << std::endl;
            }

            start_receive();
        }

        static unsigned short get_identifier()
        {
#if defined(BOOST_WINDOWS)
            return static_cast<unsigned short>(::GetCurrentProcessId());
#else
            return static_cast<unsigned short>(::getpid());
#endif
        }

        icmp::resolver resolver_;
        icmp::endpoint destination_;
        icmp::socket socket_;
        deadline_timer timer_;
        unsigned short sequence_number_;
        posix_time::ptime time_sent_;
        boost::asio::streambuf reply_buffer_;
        std::size_t num_replies_;
    };

    int main() {
        try
        {
//             if (argc != 2)
//             {
//                 std::cerr << "Usage: ping <host>" << std::endl;
// #if !defined(BOOST_WINDOWS)
//                 std::cerr << "(You may need to run this program as root.)" << std::endl;
// #endif
//                 return 1;
//             }

            boost::asio::io_service io_service;
            pinger p(io_service, "www.baidu.com");
            io_service.run();
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
        return 0;
    }
}
void asio_main()
{
    short a = 1, b = 2;
    int c = a << 16 | b;
//     asio::main();
    ping::main();
}