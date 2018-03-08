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

#include <functional>
#include <iostream>
#include <boost/function.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

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


#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <typeinfo>
using boost::multi_index_container;
using namespace boost::multi_index;

namespace mi_sample1 { //basic

    /* an employee record holds its ID, name and age */
    struct employee
    {
        int         id;
        std::string name;
        int         age;

        employee(int id_, std::string name_, int age_) :id(id_), name(name_), age(age_) {}

        friend std::ostream& operator<<(std::ostream& os, const employee& e)
        {
            os << e.id << " " << e.name << " " << e.age << std::endl;
            return os;
        }
    };

    /* tags for accessing the corresponding indices of employee_set */

    struct id {};
    struct name {};
    struct age {};

    /* see Compiler specifics: Use of member_offset for info on
    * BOOST_MULTI_INDEX_MEMBER
    */

    /* Define a multi_index_container of employees with following indices:
    *   - a unique index sorted by employee::int,
    *   - a non-unique index sorted by employee::name,
    *   - a non-unique index sorted by employee::age.
    */

    typedef multi_index_container<
        employee,
        indexed_by<
            ordered_unique< tag<id>, BOOST_MULTI_INDEX_MEMBER(employee, int, id)>,
            ordered_non_unique< tag<name>, BOOST_MULTI_INDEX_MEMBER(employee, std::string, name)>,
            ordered_non_unique< tag<age>, BOOST_MULTI_INDEX_MEMBER(employee, int, age)> 
        >
    > employee_set;

    template<typename Tag, typename MultiIndexContainer>
    void print_out_by(const MultiIndexContainer& s)
    {
        /* obtain a reference to the index tagged by Tag */

        const typename boost::multi_index::index<MultiIndexContainer, Tag>::type& i = get<Tag>(s);
//         std::cout << "i type is : " << typeid(i).name() << std::endl;

        typedef typename MultiIndexContainer::value_type value_type;
//         std::cout << "value_type is : " << typeid(value_type).name() << std::endl;
        /* dump the elements of the index to cout */

        std::copy(i.begin(), i.end(), std::ostream_iterator<value_type>(std::cout));
    }

    int main()
    {
        employee_set es;

        es.insert(employee(0, "Joe", 31));
        es.insert(employee(1, "Robert", 27));
        es.insert(employee(2, "John", 40));

        /* next insertion will fail, as there is an employee with
        * the same ID
        */

        es.insert(employee(2, "Aristotle", 2387));

        es.insert(employee(3, "Albert", 20));
        es.insert(employee(4, "John", 57));

        /* list the employees sorted by ID, name and age */

        std::cout << "by ID" << std::endl;
        print_out_by<id>(es);
        std::cout << std::endl;

//         std::cout << "by name" << std::endl;
//         print_out_by<name>(es);
//         std::cout << std::endl;
// 
//         std::cout << "by age" << std::endl;
//         print_out_by<age>(es);
//         std::cout << std::endl;

        return 0;
    }
}

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/global_fun.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <iostream>
#include <string>
using boost::multi_index_container;
using namespace boost::multi_index;

namespace mi_sample2 { //using functions as keys
    /* A name record consists of the given name (e.g. "Charlie")
    * and the family name (e.g. "Brown"). The full name, calculated
    * by name_record::name() is laid out in the "phonebook order"
    * family name + given_name.
    */
    struct name_record
    {
        name_record(std::string given_name_, std::string family_name_) :
            given_name(given_name_), family_name(family_name_)
        {}

        std::string name()const
        {
            std::string str = family_name;
            str += " ";
            str += given_name;
            return str;
        }

    private:
        std::string given_name;
        std::string family_name;
    };

    std::string::size_type name_record_length(const name_record& r)
    {
        return r.name().size();
    }

    /* multi_index_container with indices based on name_record::name()
    * and name_record_length().
    * See Compiler specifics: Use of const_mem_fun_explicit and
    * mem_fun_explicit for info on BOOST_MULTI_INDEX_CONST_MEM_FUN.
    */

    typedef multi_index_container<
        name_record,
        indexed_by<
            ordered_unique< BOOST_MULTI_INDEX_CONST_MEM_FUN(name_record, std::string, name)  >,
            ordered_non_unique< global_fun<const name_record&, std::string::size_type, name_record_length> >
        >
    > name_record_set;

    int main()
    {
        name_record_set ns;

        ns.insert(name_record("Joe", "Smith"));
        ns.insert(name_record("Robert", "Nightingale"));
        ns.insert(name_record("Robert", "Brown"));
        ns.insert(name_record("Marc", "Tuxedo"));

        /* list the names in ns in phonebook order */

        std::cout << "Phonenook order\n" << "---------------" << std::endl;
        for (name_record_set::iterator it = ns.begin(); it != ns.end(); ++it) {
            std::cout << it->name() << std::endl;
        }

        /* list the names in ns according to their length*/

        std::cout << "\nLength order\n" << "------------" << std::endl;
        for (nth_index<name_record_set, 1>::type::iterator it1 = get<1>(ns).begin();
            it1 != get<1>(ns).end(); ++it1) {
            std::cout << it1->name() << std::endl;
        }

        return 0;
    }
}

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
using boost::multi_index_container;
using namespace boost::multi_index;

namespace mi_sample3 { // constructing multi_index_containers with ctor_args_list
    /* modulo_less order numbers according to their division residual.
    * For instance, if modulo==10 then 22 is less than 15 as 22%10==2 and
    * 15%10==5.
    */
    template<typename IntegralType>
    struct modulo_less
    {
        modulo_less(IntegralType m) :modulo(m) {}

        bool operator()(IntegralType x, IntegralType y)const
        {
            return (x%modulo)<(y%modulo);
        }

    private:
        IntegralType modulo;
    };

    /* multi_index_container of unsigned ints holding a "natural" index plus
    * an ordering based on modulo_less.
    */

    typedef multi_index_container<
        unsigned int,
        indexed_by<
          ordered_unique<identity<unsigned int> >,
          ordered_non_unique<identity<unsigned int>, modulo_less<unsigned int> >
        >
    > modulo_indexed_set;

    int main()
    {
        /* define a modulo_indexed_set with modulo==10 */

        modulo_indexed_set::ctor_args_list args_list =
            boost::make_tuple(
                /* ctor_args for index #0 is default constructible */
                nth_index<modulo_indexed_set, 0>::type::ctor_args(),

                /* first parm is key_from_value, second is our sought for key_compare */
                boost::make_tuple(identity<unsigned int>(), modulo_less<unsigned int>(10))
            );

        modulo_indexed_set m(args_list);
        /* this could have be written online without the args_list variable,
        * left as it is for explanatory purposes. */

        /* insert some numbers */

        unsigned int       numbers[] = { 0,1,20,40,33,68,11,101,60,34,88,230,21,4,7,17 };
        const std::size_t  numbers_length(sizeof(numbers) / sizeof(numbers[0]));

        m.insert(&numbers[0], &numbers[numbers_length]);

        /* lists all numbers in order, along with their "equivalence class", that is,
        * the equivalent numbers under modulo_less
        */

        for (modulo_indexed_set::iterator it = m.begin(); it != m.end(); ++it) {
            std::cout << *it << " -> ( ";

            nth_index<modulo_indexed_set, 1>::type::iterator it0, it1;
            boost::tie(it0, it1) = get<1>(m).equal_range(*it);
            std::copy(it0, it1, std::ostream_iterator<unsigned int>(std::cout, " "));

            std::cout << ")" << std::endl;
        }

        return 0;
    }
}

#include <boost/variant.hpp>

namespace test_variant {
    class var_visitor : public boost::static_visitor<void>
    {
    public:
        void operator()(int& i) const
        {
            std::cout << i << std::endl;
        }

        void operator()(std::string& str) const
        {
            std::cout << str << std::endl;
        }
    };
    void main() 
    {
        boost::variant<int, std::string> v;
        v = "Hello world";
        //way 1
        std::cout << boost::get<std::string>(v) << std::endl;
        //way 2
        boost::apply_visitor(var_visitor(), v);
    }
}
void boost_main()
{
    test_variant::main();
    
}