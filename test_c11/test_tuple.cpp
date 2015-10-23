#include <tuple>
#include <iostream>
#include <string>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;
namespace Tuple
{
    struct S {
        int n;
        std::string s;
        float d;
        bool operator<(const S& rhs) const
        {
            // compares n to rhs.n,
            // then s to rhs.s,
            // then d to rhs.d
            return std::tie(n, s, d) < std::tie(rhs.n, rhs.s, rhs.d);
        }
    };

    int main1()
    {
        std::set<S> set_of_s; // S is LessThanComparable

        S value{ 42, "Test", 3.14 };
        std::set<S>::iterator iter;
        bool inserted;

        // unpacks the return value of insert into iter and inserted
        std::tie(iter, inserted) = set_of_s.insert(value);

        if (inserted)
            std::cout << "Value was inserted successfully\n";
        return 0;
    }
    /////////////////////////////////////////////////////////////////
    std::tuple<double, char, std::string> get_student(int id)
    {
        if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
        if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
        if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
        throw std::invalid_argument("id");
    }

    int main()
    {
        vector<tuple<double, char, std::string>> vectuple;
        auto student0 = get_student(0);
        vectuple.push_back(student0);

        std::cout << "ID: 0, "
            << "GPA: " << std::get<0>(student0) << ", "
            << "grade: " << std::get<1>(student0) << ", "
            << "name: " << std::get<2>(student0) << '\n';

        double gpa1;
        char grade1;
        std::string name1;
        std::tie(gpa1, grade1, name1) = get_student(1);
        std::cout << "ID: 1, "
            << "GPA: " << gpa1 << ", "
            << "grade: " << grade1 << ", "
            << "name: " << name1 << '\n';

        auto student1 = std::tie(gpa1, grade1, name1);
        vectuple.push_back(student1);

        auto it = find_if(vectuple.begin(), vectuple.end(), [](tuple<double, char, std::string> const & elem)
        {
            return std::get<0>(elem) == 3.8;
        });

        //for (auto it = vectuple.begin(); it != vectuple.end(); ++it)
        {
           cout << "GPA: " << std::get<0>(*it) << ", "
               << "grade: " << std::get<1>(*it) << ", "
               << "name: " << std::get<2>(*it) << '\n';
        }
        return 0;
    }
}

void tuple_main()
{
    Tuple::main();
}