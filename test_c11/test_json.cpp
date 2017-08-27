#include <string>
#include <iostream>
#include "ajson.hpp"

using namespace std;
using namespace ajson;

struct demo
{
    uint64_t hello;
    string world;
    string data;
};
// AJSON(demo);
// AJSON(demo, v.hello, v.world, v.data);

struct Education
{
    string	School;
    double	GPA;
};

struct Person
{
    string	Name;
    int		Age;
    vector<Education> Educations;
};

// AJSON(Education, v.School, v.GPA)
// AJSON(Person, v.Name, v.Age, v.Educations)
// AJSON(Person);
struct JSON_BaseProtocol
{
    unsigned char protid;
    unsigned char msgid;
    string data;
};
AJSON(JSON_BaseProtocol, v.protid, v.msgid, v.data);

// namespace ajson{
// 
//     template<>
//     struct json_impl < demo, void >
//     {
//         static inline detail::filed_list& this_filed_list()
//         {
//             static auto fields = detail::split_fields(STRINGFY_LIST(__VA_ARGS__));
//             return fields;
//         }
//         static inline void read(reader& rd, demo& v)
//         {
//             auto& fields = this_filed_list();
//             if (rd.expect('{') == false){ rd.error("read object must start with {!"); }
//             rd.next();
//             if (rd.expect('}'))
//                 return;
//             auto mber = rd.peek();
//             do
//             {
//                 if (mber.type != token::t_string){ rd.error("object key must be string"); }
//                 rd.next();
//                 if (rd.expect(':') == false){ rd.error("invalid json document!"); }
//                 rd.next();
//                 if (read_members(rd, &fields[0], mber.str, 0, __VA_ARGS__) == 0)
//                 {
//                     skip(rd);
//                 }
//                 if (rd.expect('}'))
//                 {
//                     rd.next();
//                     return;
//                 }
//                 else if (rd.expect(','))
//                 {
//                     rd.next();
//                     mber = rd.peek();
//                     continue;
//                 }
//                 rd.error("invalid json document!");
//             } while (true);
//         }
//         template<typename write_ty>
//         static inline void write(write_ty& wt, demo const& v)
//         {
//             auto& fields = this_filed_list();
//             wt.putc('{');
//             ::ajson::write_members(wt, &fields[0], 0, __VA_ARGS__);
//             wt.putc('}');
//         }
//     };
// }

namespace json {

    void main()
    {
//         char * buff = "{\"hello\" : 123113123123, \"world\" : \"world.\",\"data\":\"{\"hello\" : 123113123123, \"world\" : \"world.\"}\" }";
        char * buff = "{\"hello\" : 123113123123, \"world\" : \"world.\",\"data\":\"this is data\" }";
//         demo the_demo;
//         json_decode(the_demo, buff);
//         cout << the_demo.hello << " " << the_demo.world << std::endl;
        cin.get();
    }

    void main2()
    {
        Person person;
//         person.Name = "Bob";
//         person.Age = 28;
//         person.Educations.push_back(Education("MIT", 600));
//         json_stream jsons;
//         json_encode(jsons, person);
//         std::cout << jsons.str() << std::endl;
    }
}
static void trim(string& str)
{
    auto last = str.find_last_of(' ');
    if (last != string::npos)
        str.erase(last + 1, string::npos);    //È¥µô×Ö·û´®Ä©Î²¿Õ¸ñ

    auto first = str.find_first_of(' ');
    if (first != string::npos)
        str.erase(0, first);    //È¥µô×Ö·û´®Ê×¿Õ¸ñ 
}

#include "base64.hpp"
void json_main()
{
    string aa("  asdb");
    std::cout << aa << std::endl;
    trim(aa);
    std::cout << aa << std::endl;

    string str("{\"dwUsrID\":2007, \"wGroup\" : 2, \"dwVersion\" : 20161111, \"strPassword\" : \"123\", \"vecLoginExtendInfo\" : []}");
    auto base64 = websocketpp::base64_encode(str);
    cout << str.size() << endl;
    cout << base64.size() << endl;
    str = websocketpp::base64_decode(base64);

    str = string("{\"byCltType\":1, \"qwRoleID\" : 9007199254742999, \"dwSessionKey\" : 1005}");
    base64 = websocketpp::base64_encode(str);
    str = websocketpp::base64_decode(base64);

    json::main();
    json::main2();
}