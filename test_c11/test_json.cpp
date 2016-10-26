#include <string>
#include <iostream>
#include "ajson.hpp"

using namespace std;
using namespace ajson;

struct demo
{
    string hello;
    string world;
};

AJSON(demo, v.hello, v.world);

struct Education
{
    string	School;
    double	GPA;

    Education() :GPA(0.0){}

    Education(const string& school, double gpa)
        :School(school), GPA(gpa)
    {
    }
};

struct Person
{
    string	Name;
    int		Age;
    vector<Education> Educations;
};

AJSON(Education, v.School, v.GPA)
AJSON(Person, v.Name, v.Age, v.Educations)
namespace json {

    void main()
    {
        char * buff = "{\"hello\" : \"Hello\", \"world\" : \"world.\"}";
        demo the_demo;
        load_from_buff(the_demo, buff);
        cout << the_demo.hello << " " << the_demo.world << std::endl;
        cin.get();
    }

    void main2()
    {
        Person person;
        person.Name = "Bob";
        person.Age = 28;
        person.Educations.push_back(Education("MIT", 600));
        string_stream ss;
        save_to(ss, person);
        std::cout << ss.str() << std::endl;
    }
}

void json_main()
{
    json::main();
    json::main2();
}