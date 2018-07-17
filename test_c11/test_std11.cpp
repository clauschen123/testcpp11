/* The following code example is taken from the book
* "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
* by Nicolai M. Josuttis, Addison-Wesley, 2012
*
* (C) Copyright Nicolai M. Josuttis 2012.
* Permission to copy, use, modify, sell and distribute this software
* is granted provided this copyright notice appears in all copies.
* This software is provided "as is" without express or implied
* warranty, and with no claim as to its suitability for any purpose.
*/

#include "test_std11.hpp"
#include <boost/progress.hpp>

using namespace std;

namespace algo
{
    struct demo_t
    {
        int id;
        int m;
        int n;
        demo_t() = default;
        demo_t(int _id, int _m, int _n) :id(_id), m(_m), n(_n)
        {
        }

        //     demo_t operator+ (const demo_t& item)
        //     {
        //         demo_t a;
        //         a.m = m + item.m;
        //         return a;
        //     }

        //     demo_t operator() (const demo_t& item)
        //     {
        //         return (*this) + item;
        //     }
        bool operator== (const demo_t& right)
        {
            return id == right.id;
        }
        bool operator< (const demo_t& right)
        {
            return id < right.id;
        }
    };

    inline demo_t operator+ (const demo_t& l, const demo_t& r)
    {
        demo_t ret;
        ret.m = l.m + r.m;
        return ret;
    }

    void main0()
    {
        cout << "\n------ " << __FUNCTION__ << ": accumulate -------------" << endl;

        vector<int> coll;

        INSERT_ELEMENTS(coll, 1, 9);
        PRINT_ELEMENTS(coll);

        // process sum of elements
        cout << "sum: "
            << accumulate(coll.cbegin(), coll.cend(),  // range
            0)                           // initial value
            << endl;

        // process sum of elements less 100
        cout << "sum: "
            << accumulate(coll.cbegin(), coll.cend(),  // range
            -100)                        // initial value
            << endl;

        // process product of elements
        cout << "product: "
            << accumulate(coll.cbegin(), coll.cend(),  // range
            1,                           // initial value
            multiplies<int>())           // operation
            << endl;

        // process product of elements (use 0 as initial value)
        cout << "product: "
            << accumulate(coll.cbegin(), coll.cend(),  // range
            0,                           // initial value
            multiplies<int>())           // operation
            << endl;

        ////////////////////////////////////////////////////////
        cout << "Test:" << endl;
        vector<demo_t> va(1, demo_t());
        demo_t a;
        a.m = 11;
//         demo_t b = a + a;
        cout << "  struct accmulate = " 
            << accumulate(va.begin(), va.end(), a, plus<demo_t>()).m
            << endl;

    }

    void main1()
    {
        cout << "\n------ " << __FUNCTION__ << ": adjacent_difference -------------" << endl;

        deque<int> coll;

        INSERT_ELEMENTS(coll, 1, 6);
        PRINT_ELEMENTS(coll);

        // print all differences between elements
        adjacent_difference(coll.cbegin(), coll.cend(),       // source
            ostream_iterator<int>(cout, " ")); // destination
        cout << endl;

        // print all sums with the predecessors
        adjacent_difference(coll.cbegin(), coll.cend(),       // source
            ostream_iterator<int>(cout, " "),  // destination
            plus<int>());                     // operation
        cout << endl;

        // print all products between elements
        adjacent_difference(coll.cbegin(), coll.cend(),       // source
            ostream_iterator<int>(cout, " "),  // destination
            multiplies<int>());               // operation
        cout << endl;
    }
    // return whether the second object has double the value of the first
    bool doubled(int elem1, int elem2)
    {
        return elem1 * 2 == elem2;
    }
    void main2()
    {
        cout << "\n------ " << __FUNCTION__ << ": adjacent_find -------------" << endl;

        vector<int> coll;

        coll.push_back(1);
        coll.push_back(3);
        coll.push_back(2);
        coll.push_back(4);
        coll.push_back(5);
        coll.push_back(5);
        coll.push_back(0);

        PRINT_ELEMENTS(coll, "coll: ");

        // search first two elements with equal value
        vector<int>::iterator pos;
        pos = adjacent_find(coll.begin(), coll.end()); // default pred is equal_to<>

        if (pos != coll.end()) {
            cout << "first two elements with equal value have position "
                << distance(coll.begin(), pos) + 1
                << endl;
        }

        // search first two elements for which the second has double the value of the first
        pos = adjacent_find(coll.begin(), coll.end(),   // range
            doubled);                   // criterion

        if (pos != coll.end()) {
            cout << "first two elements with second value twice the "
                << "first have pos. "
                << distance(coll.begin(), pos) + 1
                << endl;
        }
    }
    void main3()
    {
        cout << "\n------ "<<__FUNCTION__<<": all/any/none -------------" << endl;

        vector<int> coll;
        vector<int>::iterator pos;

        INSERT_ELEMENTS(coll, 1, 9);
        PRINT_ELEMENTS(coll, "coll: ");

        // define an object for the predicate (using a lambda)
        auto isEven = [](int elem) {
            return elem % 2 == 0;
        };

        // print whether all, any, or none of the elements are/is even
        cout << boolalpha << "all even?:  "
            << all_of(coll.cbegin(), coll.cend(), isEven) << endl;
        cout << "any even?:  "
            << any_of(coll.cbegin(), coll.cend(), isEven) << endl;
        cout << "none even?: "
            << none_of(coll.cbegin(), coll.cend(), isEven) << endl;
    }
    void main4()
    {
        cout << "\n------ " << __FUNCTION__ << ": binary_search -------------" << endl;

//        list<int> coll;
        vector<int> coll;

        INSERT_ELEMENTS(coll, 1, 9);
        PRINT_ELEMENTS(coll);

        // check existence of element with value 5
        if (binary_search(coll.cbegin(), coll.cend(), 5)) {
            cout << "5 is present" << endl;
        }
        else {
            cout << "5 is not present" << endl;
        }

        // check existence of element with value 42
        if (binary_search(coll.cbegin(), coll.cend(), 42)) {
            cout << "42 is present" << endl;
        }
        else {
            cout << "42 is not present" << endl;
        }
    }

    void main5()
    {
        cout << "\n------ " << __FUNCTION__ << ": copy -------------" << endl;

        // copy1
        vector<string> coll1 = { "Hello", "this", "is", "an", "example" };
        list<string> coll2;

        // copy elements of coll1 into coll2
        // - use back inserter to insert instead of overwrite
        copy(coll1.cbegin(), coll1.cend(),         // source range
            back_inserter(coll2));                // destination range

        // print elements of coll2
        // - copy elements to cout using an ostream iterator
        copy(coll2.cbegin(), coll2.cend(),         // source range
            ostream_iterator<string>(cout, " "));  // destination range
        cout << endl;

        // copy elements of coll1 into coll2 in reverse order
        // - now overwriting
        //claus coll2 must have same size with coll1 here
        copy(coll1.crbegin(), coll1.crend(),       // source range
            coll2.begin());                       // destination range

        // print elements of coll2 again
        copy(coll2.cbegin(), coll2.cend(),         // source range
            ostream_iterator<string>(cout, " "));  // destination range
        cout << endl;

        // copy2
        // initialize source collection with "..........abcdef.........."
        vector<char> source(10, '.');
        for (int c = 'a'; c <= 'f'; c++) {
            source.push_back(c);
        }
        source.insert(source.end(), 10, '.');
        PRINT_ELEMENTS(source, "source: ");

        // copy all letters three elements in front of the 'a'
        vector<char> c1(source.cbegin(), source.cend());
        copy(c1.cbegin() + 10, c1.cbegin() + 16,           // source range
            c1.begin() + 7);                            // destination range
        PRINT_ELEMENTS(c1, "c1:     ");

        // copy all letters three elements behind the 'f'
        vector<char> c2(source.cbegin(), source.cend());
        copy_backward(c2.cbegin() + 10, c2.cbegin() + 16,  // source range
            c2.begin() + 6);                  // destination range
//          c2.begin() + 19);                  // destination range
        PRINT_ELEMENTS(c2, "c2:     ");

        // copy3
//         copy(istream_iterator<string>(cin),         // beginning of source
//             istream_iterator<string>(),            // end of source
//             ostream_iterator<string>(cout, "\n"));  // destination
    }

    void main6()
    {
        cout << "\n------ " << __FUNCTION__ << ": count -------------" << endl;
        vector<int> coll;
        int num;
        INSERT_ELEMENTS(coll, 1, 9);
        PRINT_ELEMENTS(coll, "coll: ");

        // count elements with value 4
        num = count(coll.cbegin(), coll.cend(),     // range
            4);                             // value
        cout << "number of elements equal to 4:      " << num << endl;

        // count elements with even value
        num = count_if(coll.cbegin(), coll.cend(),  // range
            [](int elem){                // criterion
            return elem % 2 == 0;
        });
        cout << "number of elements with even value: " << num << endl;

        // count elements that are greater than value 4
        num = count_if(coll.cbegin(), coll.cend(),  // range
            [](int elem){                // criterion
            return elem > 4;
        });
        cout << "number of elements greater than 4:  " << num << endl;

        cout << "Test:" << endl;
        vector<demo_t> va(2, demo_t());
        demo_t a;
        a.m = 11;
        //         demo_t b = a + a;
        cout << "  count_if struct demo_t: "
             << count_if(va.begin(), va.end(), [](const demo_t elem){
                    return elem.m == 0;
                })
             << endl;
    }
    bool bothEvenOrOdd(int elem1, int elem2)
    {
        return elem1 % 2 == elem2 % 2;
    }

    void main7()
    {
        cout << "\n------ " << __FUNCTION__ << ": equal / equal_range -------------" << endl;

        vector<int> coll1;
        list<int> coll2;

        INSERT_ELEMENTS(coll1, 1, 7);
        INSERT_ELEMENTS(coll2, 3, 9);

        PRINT_ELEMENTS(coll1, "coll1: ");
        PRINT_ELEMENTS(coll2, "coll2: ");

        // check whether both collections are equal
        if (equal(coll1.begin(), coll1.end(),  // first range
            coll2.begin())) {            // second range
            cout << "coll1 == coll2" << endl;
        }
        else {
            cout << "coll1 != coll2" << endl;
        }

        // check for corresponding even and odd elements
        if (equal(coll1.begin(), coll1.end(),  // first range
            coll2.begin(),               // second range
            bothEvenOrOdd)) {            // comparison criterion
            cout << "even and odd elements correspond" << endl;
        }
        else {
            cout << "even and odd elements do not correspond" << endl;
        }
        {
            list<int> coll;

            INSERT_ELEMENTS(coll, 1, 9);
            INSERT_ELEMENTS(coll, 1, 9);
            coll.sort();
            PRINT_ELEMENTS(coll);

            // print first and last position 5 could get inserted
            pair<list<int>::const_iterator, list<int>::const_iterator> range;
            range = equal_range(coll.cbegin(), coll.cend(),
                5);

            cout << "5 could get position "
                << distance(coll.cbegin(), range.first) + 1
                << " up to "
                << distance(coll.cbegin(), range.second) + 1
                << " without breaking the sorting" << endl;
        }
    }
    void main8()
    {
        cout << "\n------ " << __FUNCTION__ << ": fill -------------" << endl;
        // print ten times 7.7
        fill_n(ostream_iterator<float>(cout, " "), // beginning of destination
            10,                                 // count
            7.7);                               // new value
        cout << endl;

        list<string> coll;

        // insert "hello" nine times
        fill_n(back_inserter(coll),       // beginning of destination
            9,                         // count
            "hello");                  // new value
        PRINT_ELEMENTS(coll, "coll: ");

        // overwrite all elements with "again"
        fill(coll.begin(), coll.end(),    // destination
            "again");                    // new value
        PRINT_ELEMENTS(coll, "coll: ");

        // replace all but two elements with "hi"
        fill_n(coll.begin(),              // beginning of destination
            coll.size() - 2,             // count
            "hi");                     // new value
        PRINT_ELEMENTS(coll, "coll: ");

        // replace the second and up to the last element but one with "hmmm"
        list<string>::iterator pos1, pos2;
        pos1 = coll.begin();
        pos2 = coll.end();
        fill(++pos1, --pos2,              // destination
            "hmmm");                     // new value
        PRINT_ELEMENTS(coll, "coll: ");
    }
    void main9()
    {
        cout << "\n------ " << __FUNCTION__ << ": find_if / find_if_not /find_end / find_of -------------" << endl;

        vector<int> coll;
        vector<int>::iterator pos;

        INSERT_ELEMENTS(coll, 1, 9);
        PRINT_ELEMENTS(coll, "coll: ");

        // find first element greater than 3
        pos = find_if(coll.begin(), coll.end(),    // range
            bind(greater<int>(), std::placeholders::_1, 3));  // criterion

        // print its position
        cout << "the "
            << distance(coll.begin(), pos) + 1
            << ". element is the first greater than 3" << endl;

        // find first element divisible by 3
        pos = find_if(coll.begin(), coll.end(),
            [](int elem){
            return elem % 3 == 0;
        });

        // print its position
        cout << "the "
            << distance(coll.begin(), pos) + 1
            << ". element is the first divisible by 3" << endl;

        // find first element not <5
        pos = find_if_not(coll.begin(), coll.end(),
            bind(less<int>(), std::placeholders::_1, 5));
        cout << "first value >=5: " << *pos << endl;

        // find_end
        {
            deque<int> coll;
            list<int> subcoll;

            INSERT_ELEMENTS(coll, 1, 7);
            INSERT_ELEMENTS(coll, 1, 7);

            INSERT_ELEMENTS(subcoll, 3, 6);

            PRINT_ELEMENTS(coll, "coll:    ");
            PRINT_ELEMENTS(subcoll, "subcoll: ");

            // search LAST occurrence of subcoll in coll
            deque<int>::iterator pos;
            pos = find_end(coll.begin(), coll.end(),         // range
                subcoll.begin(), subcoll.end());  // subrange

            // loop while subcoll found as subrange of coll
            deque<int>::iterator end(coll.end());
            while (pos != end) {
                // print position of first element
                cout << "subcoll found starting with element "
                    << distance(coll.begin(), pos) + 1
                    << endl;

                // search next occurrence of subcoll
                end = pos;
                pos = find_end(coll.begin(), end,               // range
                    subcoll.begin(), subcoll.end()); // subrange
            }
        }
        // find_first_of
        {
            vector<int> coll;
            list<int> searchcoll;

            INSERT_ELEMENTS(coll, 1, 11);
            INSERT_ELEMENTS(searchcoll, 3, 5);

            PRINT_ELEMENTS(coll, "coll:       ");
            PRINT_ELEMENTS(searchcoll, "searchcoll: ");

            // search first occurrence of an element of searchcoll in coll
            vector<int>::iterator pos;
            pos = find_first_of(coll.begin(), coll.end(),     // range
                searchcoll.begin(),   // beginning of search set
                searchcoll.end());    // end of search set
            cout << "first element of searchcoll in coll is element "
                << distance(coll.begin(), pos) + 1
                << endl;

            // search last occurrence of an element of searchcoll in coll
            vector<int>::reverse_iterator rpos;
            rpos = find_first_of(coll.rbegin(), coll.rend(),  // range
                searchcoll.begin(),  // beginning of search set
                searchcoll.end());   // end of search set
            cout << "last element of searchcoll in coll is element "
                << distance(coll.begin(), rpos.base())
                << endl;
        }
    }

    class MeanValue {
    private:
        long num;    // number of elements
        long sum;    // sum of all element values
    public:
        // constructor
        MeanValue() : num(0), sum(0) {
        }

        // function call
        // - process one more element of the sequence
        void operator() (int elem) {
            num++;          // increment count
            sum += elem;    // add value
        }

        // return mean value (implicit type conversion)
        operator double() {
            return static_cast<double>(sum) / static_cast<double>(num);
        }
    };
    void main10()
    {
        cout << "\n------ " << __FUNCTION__ << ": for_each -------------" << endl;

        vector<int> coll;

        INSERT_ELEMENTS(coll, 1, 8);

        // process and print mean value
        double mv = for_each(coll.begin(), coll.end(),  // range
            MeanValue());              // operation
        cout << "mean value: " << mv << endl;
    }
    void main11()
    {
        cout << "\n------ " << __FUNCTION__ << ": generate_n -------------" << endl;

        list<int> coll;

        // insert five random numbers
        generate_n(back_inserter(coll),      // beginning of destination range
            5,                        // count
            rand);                    // new value generator
        PRINT_ELEMENTS(coll);

        // overwrite with five new random numbers
        generate(coll.begin(), coll.end(),   // destination range
            rand);                      // new value generator
        PRINT_ELEMENTS(coll);
    }
    void main12()
    {
        cout << "\n------ " << __FUNCTION__ << ": make_heap/push_heap/pop_heap -------------" << endl;

        vector<int> coll;

        INSERT_ELEMENTS(coll, 3, 7);
        INSERT_ELEMENTS(coll, 5, 9);
        INSERT_ELEMENTS(coll, 1, 4);

        PRINT_ELEMENTS(coll, "on entry:           ");

        // convert collection into a heap
        make_heap(coll.begin(), coll.end());

        PRINT_ELEMENTS(coll, "after make_heap():  ");

        // pop next element out of the heap
        pop_heap(coll.begin(), coll.end());
        coll.pop_back();

        PRINT_ELEMENTS(coll, "after pop_heap():   ");

        // push new element into the heap
        coll.push_back(17);
        push_heap(coll.begin(), coll.end());

        PRINT_ELEMENTS(coll, "after push_heap():  ");

        // convert heap into a sorted collection
        // - NOTE: after the call it is no longer a heap
        sort_heap(coll.begin(), coll.end());

        PRINT_ELEMENTS(coll, "after sort_heap():  ");
    }
    void main13()
    {
        cout << "\n------ " << __FUNCTION__ << ": bounds -------------" << endl;

        list<int> coll;

        INSERT_ELEMENTS(coll, 1, 9);
        INSERT_ELEMENTS(coll, 1, 9);
        coll.sort();
        PRINT_ELEMENTS(coll);

        // print first and last position 5 could get inserted
        auto pos1 = lower_bound(coll.cbegin(), coll.cend(),
            5);
        auto pos2 = upper_bound(coll.cbegin(), coll.cend(),
            5);

        cout << "5 could get position "
            << distance(coll.cbegin(), pos1) + 1
            << " up to "
            << distance(coll.cbegin(), pos2) + 1
            << " without breaking the sorting" << endl;

        // insert 3 at the first possible position without breaking the sorting
        coll.insert(lower_bound(coll.begin(), coll.end(),
            3),
            3);

        // insert 7 at the last possible position without breaking the sorting
        coll.insert(upper_bound(coll.begin(), coll.end(),
            7),
            7);

        PRINT_ELEMENTS(coll);
    }
    void main14()
    {
        cout << "\n------ " << __FUNCTION__ << ": includes -------------" << endl;

        list<int> coll;
        vector<int> search;

        INSERT_ELEMENTS(coll, 1, 9);
        PRINT_ELEMENTS(coll, "coll:   ");

        search.push_back(3);
        search.push_back(4);
        search.push_back(7);
        PRINT_ELEMENTS(search, "search: ");

        // check whether all elements in search are also in coll
        if (includes(coll.cbegin(), coll.cend(),
            search.cbegin(), search.cend())) {
            cout << "all elements of search are also in coll"
                << endl;
        }
        else {
            cout << "not all elements of search are also in coll"
                << endl;
        }
    }

    void main15()
    {
        cout << "\n------ " << __FUNCTION__ << ": inner_product -------------" << endl;

        list<int> coll;

        INSERT_ELEMENTS(coll, 1, 6);
        PRINT_ELEMENTS(coll);

        // process sum of all products
        // (0 + 1*1 + 2*2 + 3*3 + 4*4 + 5*5 + 6*6)
        cout << "inner product: "
            << inner_product(coll.cbegin(), coll.cend(),  // first range
                                coll.cbegin(),               // second range
                                0)                           // initial value
            << endl;

        // process sum of 1*6 ... 6*1
        // (0 + 1*6 + 2*5 + 3*4 + 4*3 + 5*2 + 6*1)
        cout << "inner reverse product: "
            << inner_product(coll.cbegin(), coll.cend(),  // first range
            coll.crbegin(),              // second range
            0)                           // initial value
            << endl;

        // process product of all sums
        // (1 * 1+1 * 2+2 * 3+3 * 4+4 * 5+5 * 6+6)
        cout << "product of sums: "
            << inner_product(coll.cbegin(), coll.cend(),  // first range
            coll.cbegin(),               // second range
            1,                           // initial value
            multiplies<int>(),           // outer operation
            plus<int>())                 // inner operation
            << endl;
    }
    void main16()
    {
        cout << "\n------ " << __FUNCTION__ << ": inplace_merge -------------" << endl;

        list<int> coll;

        // insert two sorted sequences
        INSERT_ELEMENTS(coll, 1, 7);
        INSERT_ELEMENTS(coll, 1, 8);
        PRINT_ELEMENTS(coll);

        // find beginning of second part (element after 7)
        list<int>::iterator pos;
        pos = find(coll.begin(), coll.end(),    // range
            7);                          // value
        ++pos;

        // merge into one sorted range
        inplace_merge(coll.begin(), pos, coll.end());

        PRINT_ELEMENTS(coll);
    }
    void main17()
    {
        cout << "\n------ " << __FUNCTION__ << ": iota -------------" << endl;

        array<int, 10> coll;

        iota(coll.begin(), coll.end(),  // destination range
            42);                       // start value

        PRINT_ELEMENTS(coll, "coll: ");
    }
    void main18()
    {
        cout << "\n------ " << __FUNCTION__ << ": is_heap -------------" << endl;

        vector<int> coll1 = { 9, 8, 7, 7, 7, 5, 4, 2, 1 };
        vector<int> coll2 = { 5, 3, 2, 1, 4, 7, 9, 8, 6 };
        PRINT_ELEMENTS(coll1, "coll1: ");
        PRINT_ELEMENTS(coll2, "coll2: ");

        // check whether the collections are heaps
        cout << boolalpha << "coll1 is heap: "
            << is_heap(coll1.cbegin(), coll1.cend()) << endl;
        cout << "coll2 is heap: "
            << is_heap(coll2.cbegin(), coll2.cend()) << endl;

        // print the first element that is not a heap in coll2
        auto pos = is_heap_until(coll2.cbegin(), coll2.cend());
        if (pos != coll2.end()) {
            cout << "first non-heap element: " << *pos << endl;
        }
    }

    void main19()
    {
        cout << "\n------ " << __FUNCTION__ << ": is_partitioned / partition_point -------------" << endl;

        vector<int> coll = { 5, 3, 9, 1, 3, 4, 8, 2, 6 };
        PRINT_ELEMENTS(coll, "coll: ");

        // define predicate: check whether element is odd:
        auto isOdd = [](int elem) {
            return elem % 2 == 1;
        };

        // check whether coll is partitioned in odd and even elements
        if (is_partitioned(coll.cbegin(), coll.cend(),  // range
            isOdd)) {                    // predicate
            cout << "coll is partitioned" << endl;

            // find first even element:
            auto pos = partition_point(coll.cbegin(), coll.cend(),
                isOdd);
            cout << "first even element: " << *pos << endl;
        }
        else {
            cout << "coll is not partitioned" << endl;
        }
    }
    void main20()
    {
        cout << "\n------ " << __FUNCTION__ << ": is_permutation  -------------" << endl;

        vector<int> coll1;
        list<int> coll2;
        deque<int> coll3;

        coll1 = { 1, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        coll2 = { 1, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        coll3 = { 11, 12, 13, 19, 18, 17, 16, 15, 14, 11 };

        PRINT_ELEMENTS(coll1, "coll1: ");
        PRINT_ELEMENTS(coll2, "coll2: ");
        PRINT_ELEMENTS(coll3, "coll3: ");

        // check whether both collections have equal elements in any order
        if (is_permutation(coll1.cbegin(), coll1.cend(), // first range
            coll2.cbegin())) {            // second range
            cout << "coll1 and coll2 have equal elements" << endl;
        }
        else {
            cout << "coll1 and coll2 don't have equal elements" << endl;
        }

        // check for corresponding number of even and odd elements
        if (is_permutation(coll1.cbegin(), coll1.cend(), // first range
            coll3.cbegin(),               // second range
            bothEvenOrOdd)) {             // comparison criterion
            cout << "numbers of even and odd elements match" << endl;
        }
        else {
            cout << "numbers of even and odd elements don't match" << endl;
        }
    }
    void main21()
    {
        cout << "\n------ " << __FUNCTION__ << ": is_sorted/is_sorted_until  -------------" << endl;

        vector<int> coll1 = { 1, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        PRINT_ELEMENTS(coll1, "coll1: ");

        // check whether coll1 is sorted
        if (is_sorted(coll1.begin(), coll1.end())) {
            cout << "coll1 is sorted" << endl;
        }
        else {
            cout << "coll1 is not sorted" << endl;
        }

        map<int, string> coll2 = { { 1, "Bill" }, { 2, "Jim" }, { 3, "Nico" }, { 4, "Liu" }, { 5, "Ai" } };

        PRINT_MAPPED_ELEMENTS(coll2, "coll2: ");

        // define predicate to compare names
        auto compareName = [](const pair<int, string>& e1,
            const pair<int, string>& e2){
            return e1.second < e2.second;
        };

        // check whether the names in coll2 are sorted
        if (is_sorted(coll2.cbegin(), coll2.cend(),
            compareName)) {
            cout << "names in coll2 are sorted" << endl;
        }
        else {
            cout << "names in coll2 are not sorted" << endl;
        }

        // print first unsorted name
        auto pos = is_sorted_until(coll2.cbegin(), coll2.cend(),
            compareName);
        if (pos != coll2.end()) {
            cout << "first unsorted name: " << pos->second << endl;
        }
    }
    void printCollection(const list<int>& l)
    {
        PRINT_ELEMENTS(l);
    }

    bool lessForCollection(const list<int>& l1, const list<int>& l2)
    {
        return lexicographical_compare
            (l1.cbegin(), l1.cend(),   // first range
            l2.cbegin(), l2.cend());  // second range
    }

    void main22()
    {
        cout << "\n------ " << __FUNCTION__ << ": lexicographical_compare  -------------" << endl;

        list<int> c1, c2, c3, c4;

        // fill all collections with the same starting values
        INSERT_ELEMENTS(c1, 1, 5);
        c4 = c3 = c2 = c1;

        // and now some differences
        c1.push_back(7);
        c3.push_back(2);
        c3.push_back(0);
        c4.push_back(2);

        // create collection of collections
        vector<list<int>> cc;
        cc.insert(cc.begin(), { c1, c2, c3, c4, c3, c1, c4, c2 });

        // print all collections
        for_each(cc.cbegin(), cc.cend(),
            printCollection);
        cout << endl;

        // sort collection lexicographically
        sort(cc.begin(), cc.end(),    // range
            lessForCollection);      // sorting criterion

        // print all collections again
        for_each(cc.cbegin(), cc.cend(),
            printCollection);
    }
    void main23()
    {
        cout << "\n------ " << __FUNCTION__ << ": merge  -------------" << endl;

        list<int> coll1;
        set<int> coll2;

        // fill both collections with some sorted elements
        INSERT_ELEMENTS(coll1, 1, 6);
        INSERT_ELEMENTS(coll2, 3, 8);

        PRINT_ELEMENTS(coll1, "coll1:  ");
        PRINT_ELEMENTS(coll2, "coll2:  ");

        // print merged sequence
        cout << "merged: ";
        merge(coll1.cbegin(), coll1.cend(),
            coll2.cbegin(), coll2.cend(),
            ostream_iterator<int>(cout, " "));
        cout << endl;
    }
    bool absLess(int elem1, int elem2)
    {
        return abs(elem1) < abs(elem2);
    }

    void main24()
    {
        cout << "\n------ " << __FUNCTION__ << ": min , max  -------------" << endl;

        deque<int> coll;

        INSERT_ELEMENTS(coll, 2, 6);
        INSERT_ELEMENTS(coll, -3, 6);

        PRINT_ELEMENTS(coll);

        // process and print minimum and maximum
        cout << "minimum: "
            << *min_element(coll.cbegin(), coll.cend())
            << endl;
        cout << "maximum: "
            << *max_element(coll.cbegin(), coll.cend())
            << endl;

        // print min and max and their distance using minmax_element()
        auto mm = minmax_element(coll.cbegin(), coll.cend());
        cout << "min: " << *(mm.first) << endl;    // print minimum
        cout << "max: " << *(mm.second) << endl;   // print maximum
        cout << "distance: " << distance(mm.first, mm.second) << endl;

        // process and print minimum and maximum of absolute values
        cout << "minimum of absolute values: "
            << *min_element(coll.cbegin(), coll.cend(),
            absLess)
            << endl;
        cout << "maximum of absolute values: "
            << *max_element(coll.cbegin(), coll.cend(),
            absLess)
            << endl;
    }
    void main25()
    {
        cout << "\n------ " << __FUNCTION__ << ": mismatch -------------" << endl;

        vector<int> coll1 = { 1, 2, 3, 4, 5, 6 };
        list<int>   coll2 = { 1, 2, 4, 8, 16, 3 };

        PRINT_ELEMENTS(coll1, "coll1: ");
        PRINT_ELEMENTS(coll2, "coll2: ");

        // find first mismatch
        auto values = mismatch(coll1.cbegin(), coll1.cend(),  // first range
            coll2.cbegin());               // second range
        if (values.first == coll1.end()) {
            cout << "no mismatch" << endl;
        }
        else {
            cout << "first mismatch: "
                << *values.first << " and "
                << *values.second << endl;
        }

        // find first position where the element of coll1 is not
        // less than the corresponding element of coll2
        values = mismatch(coll1.cbegin(), coll1.cend(),       // first range
            coll2.cbegin(),                     // second range
            less_equal<int>());                 // criterion
        if (values.first == coll1.end()) {
            cout << "always less-or-equal" << endl;
        }
        else {
            cout << "not less-or-equal: "
                << *values.first << " and "
                << *values.second << endl;
        }
    }
    void main26()
    {
        cout << "\n------ " << __FUNCTION__ << ": move -------------" << endl;

        vector<string> coll1 = { "Hello", "this", "is", "an", "example" };
        list<string> coll2;

        // copy elements of coll1 into coll2
        // - use back inserter to insert instead of overwrite
        // - use copy() because the elements in coll1 are used again
        copy(coll1.cbegin(), coll1.cend(),         // source range
            back_inserter(coll2));                // destination range

        // print elements of coll2
        // - copy elements to cout using an ostream iterator
        // - use move() because these elements in coll2 are not used again
        move(coll2.cbegin(), coll2.cend(),         // source range
            ostream_iterator<string>(cout, " "));  // destination range
        cout << endl;

        // copy elements of coll1 into coll2 in reverse order
        // - now overwriting (coll2.size() still fits)
        // - use move() because the elements in coll1 are not used again
        move(coll1.crbegin(), coll1.crend(),       // source range
            coll2.begin());                       // destination range

        // print elements of coll2 again
        // - use move() because the elements in coll2 are not used again
        move(coll2.cbegin(), coll2.cend(),         // source range
            ostream_iterator<string>(cout, " "));  // destination range
        cout << endl;
    }
    void main27()
    {
        cout << "\n------ " << __FUNCTION__ << ": nth_element -------------" << endl;

        deque<int> coll;

        INSERT_ELEMENTS(coll, 3, 7);
        INSERT_ELEMENTS(coll, 2, 6);
        INSERT_ELEMENTS(coll, 1, 5);
        PRINT_ELEMENTS(coll);

        // extract the four lowest elements
        nth_element(coll.begin(),     // beginning of range
            coll.begin() + 3,   // element that should be sorted correctly
            coll.end());      // end of range

        // print them
        cout << "the four lowest elements are:  ";
        copy(coll.cbegin(), coll.cbegin() + 4,
            ostream_iterator<int>(cout, " "));
        cout << endl;

        // extract the four highest elements
        nth_element(coll.begin(),     // beginning of range
            coll.end() - 4,     // element that should be sorted correctly
            coll.end());      // end of range

        // print them
        cout << "the four highest elements are: ";
        copy(coll.cend() - 4, coll.cend(),
            ostream_iterator<int>(cout, " "));
        cout << endl;

        // extract the four highest elements (second version)
        nth_element(coll.begin(),     // beginning of range
            coll.begin() + 3,   // element that should be sorted correctly
            coll.end(),       // end of range
            greater<int>());  // sorting criterion

        // print them
        cout << "the four highest elements are: ";
        copy(coll.cbegin(), coll.cbegin() + 4,
            ostream_iterator<int>(cout, " "));
        cout << endl;
    }

    void main28()
    {
        cout << "\n------ " << __FUNCTION__ << ": partial_sort / partial_sort_copy -------------" << endl;

        deque<int> coll;

        INSERT_ELEMENTS(coll, 3, 7);
        INSERT_ELEMENTS(coll, 2, 6);
        INSERT_ELEMENTS(coll, 1, 5);
        PRINT_ELEMENTS(coll);

        // sort until the first five elements are sorted
        partial_sort(coll.begin(),      // beginning of the range
            coll.begin() + 5,    // end of sorted range
            coll.end());       // end of full range
        PRINT_ELEMENTS(coll);

        // sort inversely until the first five elements are sorted
        partial_sort(coll.begin(),      // beginning of the range
            coll.begin() + 5,    // end of sorted range
            coll.end(),        // end of full range
            greater<int>());   // sorting criterion
        PRINT_ELEMENTS(coll);

        // sort all elements
        partial_sort(coll.begin(),      // beginning of the range
            coll.end(),        // end of sorted range
            coll.end());       // end of full range
        PRINT_ELEMENTS(coll);

        {
            deque<int> coll1;
            vector<int> coll6(6);      // initialize with 6 elements
            vector<int> coll30(30);    // initialize with 30 elements

            INSERT_ELEMENTS(coll1, 3, 7);
            INSERT_ELEMENTS(coll1, 2, 6);
            INSERT_ELEMENTS(coll1, 1, 5);
            PRINT_ELEMENTS(coll1);

            // copy elements of coll1 sorted into coll6
            vector<int>::const_iterator pos6;
            pos6 = partial_sort_copy(coll1.cbegin(), coll1.cend(),
                coll6.begin(), coll6.end());

            // print all copied elements
            copy(coll6.cbegin(), pos6,
                ostream_iterator<int>(cout, " "));
            cout << endl;

            // copy elements of coll1 sorted into coll30
            vector<int>::const_iterator pos30;
            pos30 = partial_sort_copy(coll1.cbegin(), coll1.cend(),
                coll30.begin(), coll30.end(),
                greater<int>());

            // print all copied elements
            copy(coll30.cbegin(), pos30,
                ostream_iterator<int>(cout, " "));
            cout << endl; 
        }
    }
    void main29()
    {
        cout << "\n------ " << __FUNCTION__ << ": partial_sum -------------" << endl;
        vector<int> coll;

        INSERT_ELEMENTS(coll, 1, 6);
        PRINT_ELEMENTS(coll);

        // print all partial sums
        partial_sum(coll.cbegin(), coll.cend(),        // source range
            ostream_iterator<int>(cout, " "));  // destination
        cout << endl;

        // print all partial products
        partial_sum(coll.cbegin(), coll.cend(),        // source range
            ostream_iterator<int>(cout, " "),   // destination
            multiplies<int>());                // operation
        cout << endl;
    }

    void main30()
    {
        cout << "\n------ " << __FUNCTION__ << ": partition / stable_partition -------------" << endl;
        vector<int> coll1;
        vector<int> coll2;

        INSERT_ELEMENTS(coll1, 1, 9);
        INSERT_ELEMENTS(coll2, 1, 9);
        PRINT_ELEMENTS(coll1, "coll1: ");
        PRINT_ELEMENTS(coll2, "coll2: ");
        cout << endl;

        // move all even elements to the front
        vector<int>::iterator pos1, pos2;
        pos1 = partition(coll1.begin(), coll1.end(),         // range
            [](int elem){                        // criterion
            return elem % 2 == 0;
        });
        pos2 = stable_partition(coll2.begin(), coll2.end(),  // range
            [](int elem){                // criterion
            return elem % 2 == 0;
        });

        // print collections and first odd element
        PRINT_ELEMENTS(coll1, "coll1: ");
        cout << "first odd element: " << *pos1 << endl;
        PRINT_ELEMENTS(coll2, "coll2: ");
        cout << "first odd element: " << *pos2 << endl;
    }
    void main31()
    {
        cout << "\n------ " << __FUNCTION__ << ": partition_copy  -------------" << endl;
        vector<int> coll = { 1, 6, 33, 7, 22, 4, 11, 33, 2, 7, 0, 42, 5 };
        PRINT_ELEMENTS(coll, "coll: ");

        // destination collections:
        vector<int> evenColl;
        vector<int> oddColl;

        // copy all elements partitioned accordingly into even and odd elements
        partition_copy(coll.cbegin(), coll.cend(), // source range
            back_inserter(evenColl),   // destination for even elements
            back_inserter(oddColl),    // destination for odd elements
            [](int elem){              // predicate: check for even elements
            return elem % 2 == 0;
        });

        PRINT_ELEMENTS(evenColl, "evenColl: ");
        PRINT_ELEMENTS(oddColl, "oddColl:  ");
    }
    void main32()
    {
        cout << "\n------ " << __FUNCTION__ << ": next_permutation / prev_permutation  -------------" << endl;
        vector<int> coll;
        INSERT_ELEMENTS(coll, 1, 3);
        PRINT_ELEMENTS(coll, "on entry:  ");

        // permute elements until they are sorted
        // - runs through all permutations because the elements are sorted now
        while (next_permutation(coll.begin(), coll.end())) {
            PRINT_ELEMENTS(coll, " ");
        }
        PRINT_ELEMENTS(coll, "afterward: ");

        // permute until descending sorted
        // - this is the next permutation after ascending sorting
        // - so the loop ends immediately
        while (prev_permutation(coll.begin(), coll.end())) {
            PRINT_ELEMENTS(coll, " ");
        }
        PRINT_ELEMENTS(coll, "now:       ");

        // permute elements until they are sorted in descending order
        // - runs through all permutations because the elements are sorted in descending order now
        while (prev_permutation(coll.begin(), coll.end())) {
            PRINT_ELEMENTS(coll, " ");
        }
        PRINT_ELEMENTS(coll, "afterward: ");
    }
    class MyRandom {
    public:
        ptrdiff_t operator() (ptrdiff_t max) {
            double tmp;
            tmp = static_cast<double>(rand())
                / static_cast<double>(RAND_MAX);
            return static_cast<ptrdiff_t>(tmp * max);
        }
    };

    void main33()
    {
        cout << "\n------ " << __FUNCTION__ << ": random_shuffle /shuffle-------------" << endl;
        vector<int> coll;

        INSERT_ELEMENTS(coll, 1, 9);
        PRINT_ELEMENTS(coll, "coll:     ");

        // shuffle elements with self-written random-number generator
        MyRandom rd;
        random_shuffle(coll.begin(), coll.end(),    // range
            rd);                 // random-number generator

        PRINT_ELEMENTS(coll, "shuffled: ");

        {
            vector<int> coll;

            INSERT_ELEMENTS(coll, 1, 9);
            PRINT_ELEMENTS(coll, "coll:     ");

            // shuffle all elements randomly
            random_shuffle(coll.begin(), coll.end());

            PRINT_ELEMENTS(coll, "shuffled: ");

            // sort them again
            sort(coll.begin(), coll.end());
            PRINT_ELEMENTS(coll, "sorted:   ");

            // shuffle elements with default engine
            default_random_engine dre;
            shuffle(coll.begin(), coll.end(),  // range
                dre);                      // random-number generator

            PRINT_ELEMENTS(coll, "shuffled: ");
        }

    }
    void main34()
    {
        cout << "\n------ " << __FUNCTION__ << ": adjacent_difference / partial_sum -------------" << endl;
        vector<int> coll = { 17, -3, 22, 13, 13, -9 };
        PRINT_ELEMENTS(coll, "coll:     ");

        // convert into relative values
        adjacent_difference(coll.cbegin(), coll.cend(),  // source
            coll.begin());               // destination
        PRINT_ELEMENTS(coll, "relative: ");

        // convert into absolute values
        partial_sum(coll.cbegin(), coll.cend(),          // source
            coll.begin());                       // destination
        PRINT_ELEMENTS(coll, "absolute: ");
    }
    void main35()
    {
        cout << "\n------ " << __FUNCTION__ << ": remove / remove_copy_if -------------" << endl;
        vector<int> coll;

        INSERT_ELEMENTS(coll, 2, 6);
        INSERT_ELEMENTS(coll, 4, 9);
        INSERT_ELEMENTS(coll, 1, 7);
        PRINT_ELEMENTS(coll, "coll:               ");

        // remove all elements with value 5
        vector<int>::iterator pos;
        pos = remove(coll.begin(), coll.end(),         // range
            5);                               // value to remove

        PRINT_ELEMENTS(coll, "size not changed:   ");

        // erase the "removed" elements in the container
        coll.erase(pos, coll.end());
        PRINT_ELEMENTS(coll, "size changed:       ");

        // remove all elements less than 4
        coll.erase(remove_if(coll.begin(), coll.end(), // range
            [](int elem){             // remove criterion
            return elem < 4;
        }),
            coll.end());
        PRINT_ELEMENTS(coll, "<4 removed:         ");

        {
            list<int> coll1;

            INSERT_ELEMENTS(coll1, 1, 6);
            INSERT_ELEMENTS(coll1, 1, 9);
            PRINT_ELEMENTS(coll1);

            // print elements without those having the value 3
            remove_copy(coll1.cbegin(), coll1.cend(),       // source
                ostream_iterator<int>(cout, " "),    // destination
                3);                                 // removed value
            cout << endl;

            // print elements without those having a value greater than 4
            remove_copy_if(coll1.cbegin(), coll1.cend(),    // source
                ostream_iterator<int>(cout, " "), // destination
                [](int elem){      // criterion for elements NOT copied
                return elem > 4;
            });
            cout << endl;

            // copy all elements not less than 4 into a multiset
            multiset<int> coll2;
            remove_copy_if(coll1.cbegin(), coll1.cend(),    // source
                inserter(coll2, coll2.end()),     // destination
                bind(less<int>(), std::placeholders::_1, 4));         // elements NOT copied
            PRINT_ELEMENTS(coll2);
        }
    }

    void main36()
    {
        cout << "\n------ " << __FUNCTION__ << ": replace / replace_if / replace_copy_if -------------" << endl;
        list<int> coll;

        INSERT_ELEMENTS(coll, 2, 7);
        INSERT_ELEMENTS(coll, 4, 9);
        PRINT_ELEMENTS(coll, "coll: ");

        // replace all elements with value 6 with 42
        replace(coll.begin(), coll.end(),     // range
            6,                            // old value
            42);                          // new value
        PRINT_ELEMENTS(coll, "coll: ");

        // replace all elements with value less than 5 with 0
        replace_if(coll.begin(), coll.end(),  // range
            [](int elem){              // criterion for replacement
            return elem < 5;
        },
            0);                        // new value
        PRINT_ELEMENTS(coll, "coll: ");
        {
            list<int> coll;

            INSERT_ELEMENTS(coll, 2, 6);
            INSERT_ELEMENTS(coll, 4, 9);
            PRINT_ELEMENTS(coll);

            // print all elements with value 5 replaced with 55
            replace_copy(coll.cbegin(), coll.cend(),         // source
                ostream_iterator<int>(cout, " "),    // destination
                5,                                  // old value
                55);                                // new value
            cout << endl;

            // print all elements with a value less than 5 replaced with 42
            replace_copy_if(coll.cbegin(), coll.cend(),      // source
                ostream_iterator<int>(cout, " "), // destination
                bind(less<int>(), std::placeholders::_1, 5),          // replacement criterion
                42);                             // new value
            cout << endl;

            // print each element while each odd element is replaced with 0
            replace_copy_if(coll.cbegin(), coll.cend(),      // source
                ostream_iterator<int>(cout, " "), // destination
                [](int elem){                    // replacement criterion
                return elem % 2 == 1;
            },
                0);                              // new value
            cout << endl;
        }

    }
    void main37()
    {
        cout << "\n------ " << __FUNCTION__ << ": reverse / reverse_copy -------------" << endl;
        vector<int> coll;

        INSERT_ELEMENTS(coll, 1, 9);
        PRINT_ELEMENTS(coll, "coll: ");

        // reverse order of elements
        reverse(coll.begin(), coll.end());
        PRINT_ELEMENTS(coll, "coll: ");

        // reverse order from second to last element but one
        reverse(coll.begin() + 1, coll.end() - 1);
        PRINT_ELEMENTS(coll, "coll: ");

        // print all of them in reverse order
        reverse_copy(coll.cbegin(), coll.cend(),         // source
            ostream_iterator<int>(cout, " "));   // destination
        cout << endl;
    }
    void main38()
    {
        cout << "\n------ " << __FUNCTION__ << ": rotate / rotate_copy -------------" << endl;
        vector<int> coll;

        INSERT_ELEMENTS(coll, 1, 9);
        PRINT_ELEMENTS(coll, "coll:      ");

        // rotate one element to the left
        rotate(coll.begin(),      // beginning of range
            coll.begin() + 1,  // new first element
            coll.end());       // end of range
        PRINT_ELEMENTS(coll, "one left:  ");

        // rotate two elements to the right
        rotate(coll.begin(),      // beginning of range
            coll.end() - 2,    // new first element
            coll.end());       // end of range
        PRINT_ELEMENTS(coll, "two right: ");

        // rotate so that element with value 4 is the beginning
        rotate(coll.begin(),                     // beginning of range
            find(coll.begin(), coll.end(), 4),  // new first element
            coll.end());                      // end of range
        PRINT_ELEMENTS(coll, "4 first:   ");
        {
            set<int> coll;

            INSERT_ELEMENTS(coll, 1, 9);
            PRINT_ELEMENTS(coll);

            // print elements rotated one element to the left
            set<int>::const_iterator pos = next(coll.cbegin());
            rotate_copy(coll.cbegin(),                    // beginning of source
                pos,                              // new first element
                coll.cend(),                      // end of source
                ostream_iterator<int>(cout, " ")); // destination
            cout << endl;

            // print elements rotated two elements to the right
            pos = coll.cend();
            advance(pos, -2);
            rotate_copy(coll.cbegin(),                    // beginning of source
                pos,                              // new first element
                coll.cend(),                      // end of source
                ostream_iterator<int>(cout, " ")); // destination
            cout << endl;

            // print elements rotated so that element with value 4 is the beginning
            rotate_copy(coll.cbegin(),                    // beginning of source
                coll.find(4),                     // new first element
                coll.cend(),                      // end of source
                ostream_iterator<int>(cout, " ")); // destination
            cout << endl;
        }

    }

    bool checkEven(int elem, bool even)
    {
        if (even) {
            return elem % 2 == 0;
        }
        else {
            return elem % 2 == 1;
        }
    }
    void main39()
    {
        cout << "\n------ " << __FUNCTION__ << ": search / search_n -------------" << endl;
        deque<int> coll;
        list<int> subcoll;

        INSERT_ELEMENTS(coll, 1, 7);
        INSERT_ELEMENTS(coll, 1, 7);
        INSERT_ELEMENTS(subcoll, 3, 6);

        PRINT_ELEMENTS(coll, "coll:    ");
        PRINT_ELEMENTS(subcoll, "subcoll: ");

        // search first occurrence of subcoll in coll
        deque<int>::iterator pos;
        pos = search(coll.begin(), coll.end(),         // range
            subcoll.begin(), subcoll.end());  // subrange

        // loop while subcoll found as subrange of coll
        while (pos != coll.end()) {
            // print position of first element
            cout << "subcoll found starting with element "
                << distance(coll.begin(), pos) + 1
                << endl;

            // search next occurrence of subcoll
            ++pos;
            pos = search(pos, coll.end(),                  // range
                subcoll.begin(), subcoll.end());  // subrange
        }

        {
            vector<int> coll;

            INSERT_ELEMENTS(coll, 1, 9);
            PRINT_ELEMENTS(coll, "coll: ");

            // arguments for checkEven()
            // - check for: "even odd even"
            bool checkEvenArgs[3] = { true, false, true };

            // search first subrange in coll
            vector<int>::iterator pos;
            pos = search(coll.begin(), coll.end(),       // range
                checkEvenArgs, checkEvenArgs + 3, // subrange values
                checkEven);                     // subrange criterion

            // loop while subrange found
            while (pos != coll.end()) {
                // print position of first element
                cout << "subrange found starting with element "
                    << distance(coll.begin(), pos) + 1
                    << endl;

                // search next subrange in coll
                pos = search(++pos, coll.end(),              // range
                    checkEvenArgs, checkEvenArgs + 3, // subr. values
                    checkEven);                     // subr. criterion
            }
        }
        {
            deque<int> coll;

            coll = { 1, 2, 7, 7, 6, 3, 9, 5, 7, 7, 7, 3, 6 };
            PRINT_ELEMENTS(coll);

            // find three consecutive elements with value 7
            deque<int>::iterator pos;
            pos = search_n(coll.begin(), coll.end(),    // range
                3,                           // count
                7);                          // value

            // print result
            if (pos != coll.end()) {
                cout << "three consecutive elements with value 7 "
                    << "start with " << distance(coll.begin(), pos) + 1
                    << ". element" << endl;
            }
            else {
                cout << "no four consecutive elements with value 7 found"
                    << endl;
            }

            // find four consecutive odd elements
            pos = search_n(coll.begin(), coll.end(),    // range
                4,                           // count
                0,                           // value
                [](int elem, int value){     // criterion
                return elem % 2 == 1;
            });

            // print result
            if (pos != coll.end()) {
                cout << "first four consecutive odd elements are: ";
                for (int i = 0; i < 4; ++i, ++pos) {
                    cout << *pos << ' ';
                }
            }
            else {
                cout << "no four consecutive elements with value > 3 found";
            }
            cout << endl;
        }

    }
    bool lessLength(const string& s1, const string& s2)
    {
        return s1.length() < s2.length();
    }

    void main40()
    {
        cout << "\n------ " << __FUNCTION__ << ": stable_sort -------------" << endl;
        // fill two collections with the same elements
        vector<string> coll1 = { "1xxx", "2x", "3x", "4x", "5xx", "6xxxx",
            "7xx", "8xxx", "9xx", "10xxx", "11", "12",
            "13", "14xx", "15", "16", "17" };
        vector<string> coll2(coll1);

        PRINT_ELEMENTS(coll1, "on entry:\n ");

        // sort (according to the length of the strings)
        sort(coll1.begin(), coll1.end(),           // range
            lessLength);                          // criterion
        stable_sort(coll2.begin(), coll2.end(),    // range
            lessLength);                   // criterion

        PRINT_ELEMENTS(coll1, "\nwith sort():\n ");
        PRINT_ELEMENTS(coll2, "\nwith stable_sort():\n ");
    }

    void main41()
    {
        cout << "\n------ " << __FUNCTION__ << ": swap_ranges -------------" << endl;
        vector<int> coll1;
        deque<int> coll2;

        INSERT_ELEMENTS(coll1, 1, 9);
        INSERT_ELEMENTS(coll2, 11, 23);

        PRINT_ELEMENTS(coll1, "coll1: ");
        PRINT_ELEMENTS(coll2, "coll2: ");

        // swap elements of coll1 with corresponding elements of coll2
        deque<int>::iterator pos;
        pos = swap_ranges(coll1.begin(), coll1.end(),  // first range
            coll2.begin());              // second range

        PRINT_ELEMENTS(coll1, "\ncoll1: ");
        PRINT_ELEMENTS(coll2, "coll2: ");
        if (pos != coll2.end()) {
            cout << "first element not modified: "
                << *pos << endl;
        }

        // mirror first three with last three elements in coll2
        swap_ranges(coll2.begin(), coll2.begin() + 3,    // first range
            coll2.rbegin());                   // second range

        PRINT_ELEMENTS(coll2, "\ncoll2: ");
    }
    void main42()
    {
        cout << "\n------ " << __FUNCTION__ << ": transform -------------" << endl;
        vector<int> coll1;
        list<int> coll2;

        INSERT_ELEMENTS(coll1, 1, 9);
        PRINT_ELEMENTS(coll1, "coll1:   ");

        // negate all elements in coll1
        transform(coll1.cbegin(), coll1.cend(),    // source range
            coll1.begin(),                   // destination range
            negate<int>());                  // operation
        PRINT_ELEMENTS(coll1, "negated: ");

        // transform elements of coll1 into coll2 with ten times their value
        transform(coll1.cbegin(), coll1.cend(),    // source range
            back_inserter(coll2),            // destination range
            bind(multiplies<int>(), std::placeholders::_1, 10));  // operation
        PRINT_ELEMENTS(coll2, "coll2:   ");

        // print coll2 negatively and in reverse order
        transform(coll2.crbegin(), coll2.crend(),  // source range
            ostream_iterator<int>(cout, " "), // destination range
            [](int elem){                    // operation
            return -elem;
        });
        cout << endl;

        {
            vector<int> coll1;
            list<int> coll2;

            INSERT_ELEMENTS(coll1, 1, 9);
            PRINT_ELEMENTS(coll1, "coll1:   ");

            // square each element
            transform(coll1.cbegin(), coll1.cend(),     // first source range
                coll1.cbegin(),                   // second source range
                coll1.begin(),                    // destination range
                multiplies<int>());               // operation
            PRINT_ELEMENTS(coll1, "squared: ");

            // add each element traversed forward with each element traversed backward
            // and insert result into coll2
            transform(coll1.cbegin(), coll1.cend(),     // first source range
                coll1.crbegin(),                  // second source range
                back_inserter(coll2),             // destination range
                plus<int>());                     // operation
            PRINT_ELEMENTS(coll2, "coll2:   ");

            // print differences of two corresponding elements
            cout << "diff:    ";
            transform(coll1.cbegin(), coll1.cend(),     // first source range
                coll2.cbegin(),                   // second source range
                ostream_iterator<int>(cout, " "), // destination range
                minus<int>());                    // operation
            cout << endl;
        }

    }
    bool differenceOne(int elem1, int elem2)
    {
        return elem1 + 1 == elem2 || elem1 - 1 == elem2;
    }
    bool bothSpaces(char elem1, char elem2)
    {
        return elem1 == ' ' && elem2 == ' ';
    }
    void main43()
    {
        cout << "\n------ " << __FUNCTION__ << ": unique/ unique_copy -------------" << endl;

        vector<demo_t> vdemo;
        vdemo.push_back(demo_t(1 ,1 ,0));
        vdemo.push_back(demo_t(2, 2, 0));
        vdemo.push_back(demo_t(1, 3, 0));
        vdemo.push_back(demo_t(2, 4, 0));
        sort(vdemo.begin(), vdemo.end());

        auto pos1 = unique(vdemo.begin(), vdemo.end()
            , []( demo_t& l,  demo_t& r)
            { return l.id == r.id; }
        );
        if (pos1 == vdemo.end())
            cout << " No unique elem" << endl;
        else
        {

        }

        // source data
        int source[] = { 1, 4, 2 };
        //list<int> coll;
        vector<int> coll{ 1, 2, 1, 2 };

        // initialize coll with elements from source
//         copy(begin(source), end(source),         // source
//             back_inserter(coll));               // destination
        PRINT_ELEMENTS(coll);

        // remove consecutive duplicates
        auto pos = unique(coll.begin(), coll.end());
        if (pos == coll.end())
            cout << " No unique elem" << endl;
        else
            cout << " Have unique elem" << endl;
        // print elements not removed
        // - use new logical end
        copy(coll.begin(), pos,                  // source
            ostream_iterator<int>(cout, " "));   // destination
        cout << "\n\n";

        // reinitialize coll with elements from source
        copy(begin(source), end(source),         // source
            coll.begin());                      // destination
        PRINT_ELEMENTS(coll);

        // remove elements if there was a previous greater element
        coll.erase(unique(coll.begin(), coll.end(),
            greater<int>()),
            coll.end());
        PRINT_ELEMENTS(coll);

        {
            // source data
            int source[] = { 1, 4, 4, 6, 1, 2, 2, 3, 1, 6, 6, 6, 5, 7, 5, 4, 4 };

            // initialize coll with elements from source
            list<int> coll;
            copy(begin(source), end(source),               // source
                back_inserter(coll));                     // destination
            PRINT_ELEMENTS(coll);

            // print elements with consecutive duplicates removed
            unique_copy(coll.cbegin(), coll.cend(),        // source
                ostream_iterator<int>(cout, " "));  // destination
            cout << endl;

            // print elements without consecutive entries that differ by one
            unique_copy(coll.cbegin(), coll.cend(),        // source
                ostream_iterator<int>(cout, " "),   // destination
                differenceOne);                    // duplicates criterion
            cout << endl;
        }
        {
            // don't skip leading whitespaces by default
            cin.unsetf(ios::skipws);

            // copy standard input to standard output
            // - while compressing spaces
//             unique_copy(istream_iterator<char>(cin),  // beginning of source: cin
//                 istream_iterator<char>(),     // end of source: end-of-file
//                 ostream_iterator<char>(cout), // destination: cout
//                 bothSpaces);                  // duplicate criterion
        }
    }

    void main44()
    {
        cout << "\n------ " << __FUNCTION__ << ": set_union / set_intersection/set_difference/set_symmetric_difference -------------" << endl;
        vector<int> c1 = { 1, 2, 2, 4, 6, 7, 7, 9 };

        deque<int>  c2 = { 2, 2, 2, 3, 6, 6, 8, 9 };

        // print source ranges
        cout << "c1:                         ";
        copy(c1.cbegin(), c1.cend(),
            ostream_iterator<int>(cout, " "));
        cout << endl;
        cout << "c2:                         ";
        copy(c2.cbegin(), c2.cend(),
            ostream_iterator<int>(cout, " "));
        cout << '\n' << endl;

        // sum the ranges by using merge()
        cout << "merge():                    ";
        merge(c1.cbegin(), c1.cend(),
            c2.cbegin(), c2.cend(),
            ostream_iterator<int>(cout, " "));
        cout << endl;

        // unite the ranges by using set_union()
        cout << "set_union():                ";
        set_union(c1.cbegin(), c1.cend(),
            c2.cbegin(), c2.cend(),
            ostream_iterator<int>(cout, " "));
        cout << endl;

        // intersect the ranges by using set_intersection()
        cout << "set_intersection():         ";
        set_intersection(c1.cbegin(), c1.cend(),
            c2.cbegin(), c2.cend(),
            ostream_iterator<int>(cout, " "));
        cout << endl;

        // determine elements of first range without elements of second range
        // by using set_difference()
        cout << "set_difference():           ";
        set_difference(c2.cbegin(), c2.cend(),
            c1.cbegin(), c1.cend(),
            ostream_iterator<int>(cout, " "));
        cout << endl;

        // determine difference the ranges with set_symmetric_difference()
        cout << "set_symmetric_difference(): ";
        set_symmetric_difference(c1.cbegin(), c1.cend(),
            c2.cbegin(), c2.cend(),
            ostream_iterator<int>(cout, " "));
        cout << endl;
    }
}

#include <future>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>
#include <condition_variable>
#include <queue>
namespace async_
{
    int doSomething(char c)
    {
        // random-number generator (use c as seed to get different sequences)
        std::default_random_engine dre(c);
        dre.seed(time(nullptr));
        std::uniform_int_distribution<int> id(10, 1000);

        cout << id(dre) << endl;
        // loop to print character after a random period of time
        for (int i = 0; i < 10; ++i) {
            this_thread::sleep_for(chrono::milliseconds(id(dre)));
            cout.put(c).flush();
        }

        return c;
    }

    int func1()
    {
        return doSomething('.');
    }

    int func2()
    {
        return doSomething('+');
    }

    void main0()
    {
        cout << "\n------ " << __FUNCTION__ << ": future / async -------------" << endl;

        std::cout << "  starting func1() in background"
            << " and func2() in foreground:" << std::endl;

        // start func1() asynchronously (now or later or never):
        std::future<int> result1(std::async(func1));

        cout << "Get result2 of func2..." << endl;
        int result2 = func2();    // call func2() synchronously (here and now)

        // print result (wait for func1() to finish and add its result to result2
        int result = result1.get() + result2;

        std::cout << "\n  result of func1()+func2(): " << result
            << std::endl;
    }
    void task1()
    {
        // endless insertion and memory allocation
        // - will sooner or later raise an exception
        // - BEWARE: this is bad practice
        list<int> v;
        cout << "task1: current thread id: " << this_thread::get_id() << endl;
        while (true) {
            for (int i = 0; i < 10000; ++i) {
                v.push_back(i);
            }
            cout.put('.').flush();
        }
    }

    void main1()
    {
        cout << "\n------ " << __FUNCTION__ << ": async -------------" << endl;

        cout << "starting 2 tasks" << endl;
        cout << "- task1: process endless loop of memory consumption" << endl;
        cout << "- task2: wait for <return> and then for task1" << endl;

        auto f1 = async(task1);  // start task1() asynchronously (now or later or never)

        cin.get();  // read a character (like getchar())

        cout << "\nwait for the end of task1: " << endl;
        try {
            cout << "main1: current thread id: " << this_thread::get_id() << endl;

            f1.get();  // wait for task1() to finish (raises exception if any)
        }
        catch (const exception& e) {
            cerr << "EXCEPTION: " << e.what() << endl;
        }
    }

    void doSomething2(char c)
    {
        // random-number generator (use c as seed to get different sequences)
        default_random_engine dre(c);
        uniform_int_distribution<int> id(10, 1000);

        // loop to print character after a random period of time
        for (int i = 0; i < 10; ++i) {
            this_thread::sleep_for(chrono::milliseconds(id(dre)));
            cout.put(c).flush();
        }
    }

    void main2()
    {
        cout << "\n------ " << __FUNCTION__ << ": future / wait_for -------------" << endl;

        cout << "  starting 2 operations asynchronously" << endl;

        // start two loops in the background printing characters . or +
        auto f1 = async([]{ doSomething2('.'); });
        auto f2 = async([]{ doSomething2('+'); });

        // if at least one of the background tasks is running
        if (f1.wait_for(chrono::seconds(0)) != future_status::deferred ||
            f2.wait_for(chrono::seconds(0)) != future_status::deferred) {
            // poll until at least one of the loops finished
            while (f1.wait_for(chrono::seconds(0)) != future_status::ready &&
                f2.wait_for(chrono::seconds(0)) != future_status::ready) {
                //...;
                this_thread::yield();  // hint to reschedule to the next thread
            }
        }
        cout.put('\n').flush();

        // wait for all loops to be finished and process any exception
        try {
            f1.get();
            f2.get();
        }
        catch (const exception& e) {
            cout << "\nEXCEPTION: " << e.what() << endl;
        }
        cout << "\ndone" << endl;
    }

    long data;
    std::atomic<bool> readyFlag(false);

    void provider()
    {
        // after reading a character
        std::cout << "<return>" << std::endl;
        std::cin.get();

        // provide some data
        data = 42;

        // and signal readiness
        readyFlag.store(true);
    }

    void consumer()
    {
        // wait for readiness and do something else
        while (!readyFlag.load()) {
            std::cout.put('.').flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        // and process provided data
        std::cout << "\nvalue : " << data << std::endl;
    }

    void main3()
    {
        cout << "\n------ " << __FUNCTION__ << ": atom -------------" << endl;
        // start provider and consumer
        auto p = std::async(std::launch::async, provider);
        auto c = std::async(std::launch::async, consumer);
    }

    bool readyFlag2;
    std::mutex readyMutex;
    std::condition_variable readyCondVar;

    void thread1()
    {
        // do something thread2 needs as preparation
        std::cout << "<return>" << std::endl;
        std::cin.get();

        // signal that thread1 has prepared a condition
        {
            std::lock_guard<std::mutex> lg(readyMutex);
            readyFlag2 = true;
        } // release lock
        readyCondVar.notify_one();
    }

    void thread2()
    {
        // wait until thread1 is ready (readyFlag is true)
        {
            std::unique_lock<std::mutex> ul(readyMutex);
            readyCondVar.wait(ul, []{ return readyFlag2; });
        } // release lock

        // do whatever shall happen after thread1 has prepared things
        std::cout << "done" << std::endl;
    }

    void main4()
    {
        cout << "\n------ " << __FUNCTION__ << ": condition_var -------------" << endl;
        auto f1 = std::async(std::launch::async, thread1);
        auto f2 = std::async(std::launch::async, thread2);
    }
    std::queue<int> queue;
    std::mutex queueMutex;
    std::condition_variable queueCondVar;

    void provider2(int val)
    {
        // push different values (val til val+5 with timeouts of val milliseconds into the queue
        for (int i = 0; i < 6; ++i) {
            {
                std::lock_guard<std::mutex> lg(queueMutex);
                queue.push(val + i);
            } // release lock
            queueCondVar.notify_one();

            std::this_thread::sleep_for(std::chrono::milliseconds(val));
        }
    }

    void consumer2(int num)
    {
        // pop values if available (num identifies the consumer)
        while (true) {
            int val;
            {
                std::unique_lock<std::mutex> ul(queueMutex);
                queueCondVar.wait(ul, []{ return !queue.empty(); });
                val = queue.front();
                queue.pop();
            } // release lock
            std::cout << "consumer " << num << ": " << val << std::endl;
        }
    }

    void main5()
    {
        cout << "\n------ " << __FUNCTION__ << ": condition_var2 -------------" << endl;
        // start three providers for values 100+, 300+, and 500+
        auto p1 = std::async(std::launch::async, provider2, 100);
        auto p2 = std::async(std::launch::async, provider2, 300);
        auto p3 = std::async(std::launch::async, provider2, 500);

        // start two consumers printing the values
        auto c1 = std::async(std::launch::async, consumer2, 1);
        auto c2 = std::async(std::launch::async, consumer2, 2);
    }

    std::mutex printMutex;  // enable synchronized output with print()

    void print(const std::string& s)
    {
        std::lock_guard<std::mutex> l(printMutex);
        for (char c : s) {
            std::cout.put(c);
        }
        std::cout << std::endl;
    }

    void main6()
    {
        cout << "\n------ " << __FUNCTION__ << ": mutex -------------" << endl;
        auto f1 = std::async(std::launch::async,
            print, "Hello from a first thread");
        auto f2 = std::async(std::launch::async,
            print, "Hello from a second thread");
        print("Hello from the main thread");
    }

    void doSomething3(std::promise<std::string>& p)
    {
        try {
            // read character and throw exception if 'x'
            std::cout << "read char ('x' for exception): ";
            char c = std::cin.get();
            if (c == 'x') {
                throw std::runtime_error(std::string("char ") + c + " read");
            }
            //...
            std::string s = std::string("char ") + c + " processed";
            p.set_value(std::move(s));    // store result
        }
        catch (...) {
            p.set_exception(std::current_exception());  // store exception
        }
    }

    void main7()
    {
        cout << "\n------ " << __FUNCTION__ << ": promise -------------" << endl;
        try {
            // start thread using a promise to store the outcome
            std::promise<std::string> p;
            std::thread t(doSomething3, std::ref(p));
            t.detach();
            //...

            // create a future to process the outcome
            std::future<std::string> f(p.get_future());

            // process the outcome
            std::cout << "result: " << f.get() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "EXCEPTION: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "EXCEPTION " << std::endl;
        }
    }
    int queryNumber()
    {
        // read number
        cout << "read number: ";
        int num;
        cin >> num;

        // throw exception if none
        if (!cin) {
            throw runtime_error("no number read");
        }

        return num;
    }

    void doSomething6(char c, shared_future<int> f)
    {
        try {
            // wait for number of characters to print
            int num = f.get();  // get result of queryNumber()

            for (int i = 0; i < num; ++i) {
                this_thread::sleep_for(chrono::milliseconds(100));
                cout.put(c).flush();
            }
        }
        catch (const exception& e) {
            cerr << "EXCEPTION in thread " << this_thread::get_id()
                << ": " << e.what() << endl;
        }
    }

    void main8()
    {
        cout << "\n------ " << __FUNCTION__ << ": shared_future -------------" << endl;
        try {
            // start one thread to query a number
            shared_future<int> f = async(queryNumber);

            // start three threads each processing this number in a loop
            auto f1 = async(launch::async, doSomething6, '.', f);
            auto f2 = async(launch::async, doSomething6, '+', f);
            auto f3 = async(launch::async, doSomething6, '*', f);

            // wait for all loops to be finished
            f1.get();
            f2.get();
            f3.get();
        }
        catch (const exception& e) {
            cout << "\nEXCEPTION: " << e.what() << endl;
        }
        cout << "\ndone" << endl;
    }
    void doSomething7(int num, char c)
    {
        try {
            // random-number generator (use c as seed to get different sequences)
            default_random_engine dre(42 * c);
            uniform_int_distribution<int> id(10, 1000);
            for (int i = 0; i < num; ++i) {
                this_thread::sleep_for(chrono::milliseconds(id(dre)));
                cout.put(c).flush();
                //...
            }
        }
        // make sure no exception leaves the thread and terminates the program
        catch (const exception& e) {
            cerr << "THREAD-EXCEPTION (thread "
                << this_thread::get_id() << "): " << e.what() << endl;
        }
        catch (...) {
            cerr << "THREAD-EXCEPTION (thread "
                << this_thread::get_id() << ")" << endl;
        }
    }

    void main9()
    {
        cout << "\n------ " << __FUNCTION__ << ": thread -------------" << endl;
        try {
            thread t1(doSomething7, 5, '.');  // print five dots in separate thread
            cout << "- started fg thread " << t1.get_id() << endl;

            // print other characters in other background threads
            for (int i = 0; i < 5; ++i) {
                thread t(doSomething7, 10, 'a' + i); // print 10 chars in separate thread
                cout << "- detach started bg thread " << t.get_id() << endl;
                t.detach();  // detach thread into the background
            }

            cin.get();  // wait for any input (return)

            cout << "- join fg thread " << t1.get_id() << endl;
            t1.join();  // wait for t1 to finish
        }
        catch (const exception& e) {
            cerr << "EXCEPTION: " << e.what() << endl;
        }
    }

    int task(int count)
    {
        // endless insertion and memory allocation
        // - will sooner or later raise an exception
        // - BEWARE: this is bad practice
        list<int> v;
        while (count--) {
            for (int i = 0; i < 10000; ++i) {
                v.push_back(i);
            }
            cout.put('.').flush();
        }
        return 100;
    }

    void test()
    {
        cout << "\n------ " << __FUNCTION__ << ": packaged_task -------------" << endl;

        packaged_task<int(int)> myPackaged(task);//首先创建packaged_task对象myPackaged其内部创建一个函数task和一个共享状态(用于返回task的结果)  
        future<int> myFuture = myPackaged.get_future();//通过packaged_task::get_future()返回一个future对象myFuture用于获取task的任务结果  
        thread myThread(move(myPackaged), 100/*"hello world"*/);//创建一个线程执行task任务，这里注意move语义强制将左值转为右值使用因为packaged_task禁止copy constructor，可以不创建线程，那么task任务的执行将和future结果的获取在同一个线程，这样就不叫异步了  
        
        //这里主线程可以做其它的操作  
        //cout << " main thread: " << task(10) << endl;
        int x = myFuture.get();//线程还可以在执行一些其它操作，直到其想获取task的结果时调用此语句  
        cout << " sub thread: " << x << endl;
    }

    //////// How to check a thread is end ///////////////////
    template<typename T>
    static T random()
    {
        std::random_device rd;
        std::mt19937 generator(rd());
        return (T)generator();
    }

    atomic<int> total_thread_create(0);
    atomic<int> total_thread_exit(0);
    struct Writer {
        bool _end = false;
        int _id;
        int count = 1;
        std::thread* _thread = nullptr;
        Writer(int c, int id) : count(c), _id(id) {
            _thread = new thread(bind(&Writer::Action, this, id));
            total_thread_create++;
        }
        ~Writer() {
            _thread->join();
            //         cout << "*****Thread " << _id << " exit and delete" << endl;
            total_thread_exit++;
            delete _thread;
        }

        void Action(int id) {
            while (count--) {
                random<int>();
            }
            //         cout << "******Writer_thread: " << id <<" all done" << endl;
            _end = true;
        }
    };
    vector<shared_ptr<Writer>> _writer_thread;
    
    // Test how to check a thread is end
    void my_task() {
        int count = 1000000;
        while (count--) {
            random<int>();
            if (count % 10000 == 0) {
                if (_writer_thread.empty() == false) {
                    auto it = _writer_thread.begin();
                    if ((*it)->_end == true) _writer_thread.erase(it);
                }
                _writer_thread.emplace_back(make_shared<Writer>(10000, count));
            }
        }
        cout << "******my task all done,left thread count=" << _writer_thread.size() << endl;
        cout << " thread create = " << total_thread_create << ", thread exit = " << total_thread_exit << endl;
        _writer_thread.clear();
        cout << " thread create = " << total_thread_create << ", thread exit = " << total_thread_exit << endl;

    }
    typedef uint32_t NonceType;
    struct Rec {
        NonceType c;
        NonceType i1;
        NonceType i2;
        Rec(NonceType n1, NonceType n2, NonceType n3) : c(n1), i1(n2), i2(n3) {}
        bool operator<(const Rec& rhs) const { return c < rhs.c; }
        bool operator==(const Rec& rhs) const { return c == rhs.c; }

        void print() { cout << "c:" << c << ",i1:" << i1 << ",i2" << i2 << endl; }
    };
    void my_sort() {
        int count = 100000;
        vector<Rec> v,v1, v2,v3;
        while (count--) { 
            v.emplace_back(random<uint32_t>(), random<uint32_t>(), random<uint32_t>());
        }
        {
            v1 = v;
            boost::progress_timer t;
            std::sort(v1.begin(), v1.end());
        }
        {
            v2 = v;
            boost::progress_timer t;
            std::stable_sort(v2.begin(), v2.end());
            assert(v2 == v1);
        }
        {
            v3 = v;
            boost::progress_timer t;
            std::qsort(&v3[0], v3.size(), sizeof Rec, [](const void* a, const void* b) {
                const Rec* arg1 = static_cast<const Rec*>(a);
                const Rec* arg2 = static_cast<const Rec*>(b);
                if (arg1->c < arg2->c) return -1;
                if (arg1->c > arg2->c) return 1;
                return 0;
            });
            assert(v3 == v1);
            assert(v3 == v2);

            for (auto&r : v3) {
                r.print();
            }
        }


    }
}

void std11_main()
{
    async_::my_sort();
//     async_::doSomething('+');
//     async_::test();
//     async_::main0();
//     async_::main1();
//     async_::main2();
//     async_::main3();
//     async_::main4();
//     async_::main5();
//     async_::main6();
//     async_::main7();
//     async_::main8();
//     async_::main9();
}