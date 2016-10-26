#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>
#include <set>
#include <unordered_map>
#include <boost/progress.hpp>

using namespace std;
//////////////////////////////////////////////////////////////////
//     __int64 a  = 9007199255300713;
//     double b = static_cast<double>(a);
// 
// vector<int> m_vecLevelRankFull{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
// vector<int> vecRank;
// 
// auto myself = find_if(m_vecLevelRankFull.begin(), m_vecLevelRankFull.end(),
//     [](int const& e){ return e == 6; });
// 
// int count = 9;
// auto totalcnt = count << 1;
// if (m_vecLevelRankFull.size() <= totalcnt + 1){
//     vecRank.insert(vecRank.begin(), m_vecLevelRankFull.begin(), myself);
//     vecRank.insert(vecRank.begin() + vecRank.size(), ++myself, m_vecLevelRankFull.end());
// }
// else{
//     int forcnt = distance(m_vecLevelRankFull.begin(), myself);
//     int backcnt = distance(myself, m_vecLevelRankFull.end()) - 1;
// 
//     if (forcnt < count) backcnt = totalcnt - forcnt;
//     else if (backcnt < count) forcnt = totalcnt - backcnt;
//     else forcnt = count, backcnt = count;
// 
//     vecRank.insert(vecRank.begin(), myself - forcnt, myself);
//     ++myself;
//     vecRank.insert(vecRank.end(), myself, myself + backcnt);
// }
////////////////////////////////////////////////////////////////
namespace TRIVAL_TEST
{
    struct B;
    struct A
    {
        B* pb = nullptr;
        A() { cout << "A()" << endl; }
        ~A() { cout << "~A() " << pb << endl; }
    };

    struct A1 :public A
    {
        A1() { cout << "A1()" << endl; }
        ~A1() { cout << "~A1()" << endl; }
    };

    struct B
    {
        int num = 100;
        int* p = nullptr;
        A a;
        B(){ cout << "B()" << endl; a.pb = this; }
        ~B(){ cout << "~B()" << num << endl; p = nullptr; }
    };
    void main0()
    {
        map<int, int> m;
        const int MAX_COUNT = 1000000;
        int count = MAX_COUNT;
        while (count--)
        {
            //m.insert(std::make_pair<int, int>(count, count));
            m[count] = count;
        }
        count = MAX_COUNT;
        {
            boost::progress_timer t;
            while (count--) if (m.empty());
        }
        count = MAX_COUNT;
        {
            boost::progress_timer t;
            while (count--) if (0 == m.size());
        }
    }
}

namespace Work
{
    ///////////////////////////////////
    /// Util
    ///////////////////////////////////
    template<class _FwdIt>
    bool contain(vector<_FwdIt>& v, _FwdIt it)
    {
        return (v.end() != find(v.begin(), v.end(), it));
    }

    template<class _FwdIt, class T>
    inline vector<_FwdIt> n_min_element(_FwdIt _First, _FwdIt _Last, int count, T type)
    {	// find smallest element, using _Pred
        vector<_FwdIt> v;

        //     while (!comparable(*_First))
        //     {
        //         _First++;
        //         if (_First == _Last)
        //             return std::move(v);
        //     }

        while (count)
        {
            _FwdIt _Found = _First;
            if (_First == _Last) break;

            {
                for (auto _It = _First; ++_It != _Last;)
                {
                    if (contain(v, _It))
                        continue;
                    //if (*_It < *_Found)
                    if (Less(*_It, *_Found, type))
                        _Found = _It;
                }

                if (comparable(*_Found) && count--)
                    v.push_back(_Found);

                if (_First == _Found)
                while (contain(v, ++_First) && _First != _Last);
            }
        }
        return std::move(v);
    }

    template<class E, class T>
    inline bool Less(const shared_ptr<E>& left, const shared_ptr<E>& right, T type)
    {
        if (left->Dead() || right->Dead()) return false;
        return (*left)(right, type);
    }

    template<class E>
    inline bool comparable(const shared_ptr<E>& left)
    {
        return left->NotDead();
    }

    enum EFindLowHP {};
    enum EFindByID  {};
    enum EFindNear  {};
    enum EFindFar   {};
    enum EFindNofilter {};

    class CBattleFighter;
    typedef shared_ptr<CBattleFighter> PBattleFighter;
    typedef vector<PBattleFighter> TVeCBattleFighters;

    class CBattleFighter
    {
        int hp_;
        int id_;
        int dis_;
        bool dead_;
    public:
        CBattleFighter(int hp, int id, int dis, bool dead) 
        : hp_(hp), id_(id), dis_(dis), dead_(dead) {}

        int GetID() const                       { return id_; }
        int GetHP() const                       { return hp_; }
        int GetDis(int dis) const               { return abs(dis_ - dis); }
        bool NotDead() const                    { return !dead_; }
        bool Dead() const                       { return dead_; }

        template<class Fighter>
        bool operator() (const Fighter& fighter, EFindLowHP x)
        {
            return GetHP() < fighter->GetHP();
        }
//         bool operator() (const CBattleFighter* fighter, EFindLowHP x)
//         {
//             return GetHP() < fighter->GetHP();
//         }

        bool operator() (const PBattleFighter fighter, EFindNear x)
        {
            return GetDis(x) < fighter->GetDis(x);
        }
        bool operator() (const CBattleFighter* fighter, EFindNear x)
        {
            return GetDis(x) < fighter->GetDis(x);
        }
        bool operator() (const CBattleFighter* fighter, EFindFar x)
        {
            return GetDis(x) > fighter->GetDis(x);
        }
        bool operator== (EFindByID findID)
        {
            return id_ == (int)findID;
        }
        bool operator() (const CBattleFighter* fighter, EFindNofilter)
        {
            return true;
        }
    };
    inline bool operator== (PBattleFighter left, EFindByID id)
    {
        return (*left) == id;
    }
    inline bool operator< (const PBattleFighter& left, const PBattleFighter& right)
    {
        return (*left)(right, EFindLowHP(0));
    }
    // inline bool operator< (const CBattleFighter*& left, const CBattleFighter*& right)
    // {
    //     return true;// (*left)(right, EFindLowHP(0));
    // }

    void main1()
    {
        int hp[] = { 7, 6, 2, 14, 6, 1, 8, 10, 5, 1 };
        int dis[] = { 7, 6, 2, 14, 6, 1, 8, 10, 5, 1 };
        TVeCBattleFighters fighters;
        vector<int> v1(hp, hp + 10);
        vector<int> v3(3, 4);
        vector<CBattleFighter*> v2;

        int n = v1.front();

        cout << typeid(TVeCBattleFighters::iterator::value_type::element_type*).name() << endl;
        cout << typeid(vector<int>).name() << endl;

        for (int i = 0; i < 10; ++i)
        {
            fighters.push_back(PBattleFighter(new CBattleFighter(hp[i], i, dis[i], (i % 5) == 0)));
            v2.push_back((new CBattleFighter(hp[i], i, dis[i], (i % 5) == 0)));
        }

        {
            auto it0 = max_element(fighters.begin(), fighters.end(), [](PBattleFighter right, PBattleFighter left)->bool{
                return (*left)(right, EFindLowHP(0)); 
                });
            cout << "Find low hp is " << (*it0)->GetHP() << ", id = " << (*it0)->GetID() << endl;

            auto it1 = max_element(v2.begin(), v2.end(), [](CBattleFighter* right , CBattleFighter* left)->bool{
                return (*left)(right, EFindLowHP(0));
            });
            cout << "Find low hp is " << (*it1)->GetHP() << ", id = " << (*it1)->GetID() << endl;
        }

        v2.erase(std::remove_if(v2.begin(), v2.end(), [](CBattleFighter* ft)->bool{
            return ft->Dead();
        }), v2.end());

        // search max element of mydis with lambda
        auto head = v2.begin();
        while ((*head)->Dead())
            ++head;
        int mydis = 14;
        if (head != v2.end())
        {
            auto it = std::max_element(head, v2.end(),
                [mydis](CBattleFighter* left, CBattleFighter* right)->bool{
                if (right->Dead()) return false;
                return (*right)(left, EFindFar(mydis));
            });
            CBattleFighter* ft = *it;
            cout << "Max element:" << ft->GetHP();
        }

        // sort with lambda
        std::sort(v2.begin(), v2.end(), [mydis](CBattleFighter* left, const CBattleFighter* right)->bool {
            return left->operator()(right, EFindNear(mydis));
        });

        std::sort(fighters.begin(), fighters.end());

        auto found1 = find(fighters.begin(), fighters.end(), EFindByID(6));
        if (found1 != fighters.end())
        {
            cout << (*found1)->GetID() << endl;
        }

        int count = 3;
        //     auto found = n_min_element<TVeCBattleFighters::iterator>(fighters.begin(), fighters.end(), count, EFindLowHP(0));
        auto found = n_min_element<TVeCBattleFighters::iterator>(fighters.begin(), fighters.end(), count, EFindNear(mydis));
        for (int i = 0; i < found.size(); ++i)
        {
            cout << (*found[i])->GetHP() << ", " << (*found[i])->GetID() << ", " << (*found[i])->Dead() << endl;
        }
        cout << endl;
    }
    //////////////////////////////////////////////////////////////
    template <int N>
    struct X
    {
        X() { cout << "X" << N << "()" << endl; }
        ~X() { cout << "~X" << N << "()" << endl; }
    };

    struct Y1
    {
        Y1() { cout << "Y1()" << endl; }
        ~Y1() { cout << "~Y1()" << endl; }
    };

    struct Y : public Y1
    {
        Y() { cout << "Y()" << endl; }
        ~Y() { cout << "~Y()" << endl; }

        X<1> x1;
        X<2> x2;
        X<3> x3;

    };

    void main2()
    {
        Y y;
    }

    void main3()
    {
        __int64  count = 10000000;

        {
            int a, b = -1233493;
            int c = -b;
            boost::progress_timer t;
            while (count--) a = abs(b);
        }
        {
            int a, b = -1233493;
            boost::progress_timer t;
            while (count--) a = b > 0 ? b : 0 - b;
        }
    }

    void main4()
    {
        set<PBattleFighter> fighters;
        typedef set<PBattleFighter>::iterator It;

        int hp[] = { 7, 6, 2, 4, 6, 1, 8, 10, 5, 1 };

        for (int i = 0; i < 5; ++i)
        {
            fighters.insert(PBattleFighter(new CBattleFighter(hp[i], i, hp[i], (i % 3) == 0)));
        }

        It end = fighters.end();
        for (auto it = fighters.begin(); it != end; ++it)
        {

            int i = 7;
            fighters.insert(PBattleFighter(new CBattleFighter(hp[i], i, hp[i], (i % 3) == 0)));
        }
    }

    struct C1;
    struct C
    {
        shared_ptr<C1>   ptr_;

        template<class T>
        void set_shared(T t)
        {
//            ptr_ = t->shared_from_this();
        }
    };

    struct C1 
    {};
    struct C2 : public C1/*, public enable_shared_from_this<C2>*/
    {
        void test(C& c)
        {
            c.set_shared(this);
        }
    };
    struct C3 : public C2, public enable_shared_from_this<C3>
    {
    };

    void main5()
    {
        C c;

        C3* pC3 = new C3;
        shared_ptr<C3> pc3(pC3);
        cout << pc3.use_count() << endl;
        //shared_ptr<C3> t = pC3->shared_from_this();

        pc3->test(c);
        //c.set_shared(pc3.get());
        cout << pc3.use_count() << endl;
    }
}
// int h()
// {
//     cout << "hhh" << endl;
//     return 100;
// }
// int g()
// {
//     cout << "ggg" << endl;
//     return 100;
// }
// void f(shared_ptr<int>,int,int)
// {
//     cout << " fff" << endl;
// }
//     f(shared_ptr<int>(new int),h(), g());
#include <array>
#include <algorithm>
#include <random>
#include <numeric>

namespace Recount
{
    //战斗伤害统计
    struct SDamageStatistics
    {
        std::string player; //玩家姓名
        int damage; //伤害值
        int pecent; //伤害百分比,n%
        SDamageStatistics() = default;
        SDamageStatistics(const std::string& player_ , int damage_ , int pecent_)
            :player(player_), damage(damage_), pecent(pecent_)
        {}
    };

    inline int operator+ (const SDamageStatistics& left,const SDamageStatistics& right)
    {
        return left.damage + right.damage;
    }

    inline bool operator< (const SDamageStatistics& left, const SDamageStatistics& right)
    {
        return left.damage > right.damage;
    }

    void main()
    {
        std::default_random_engine dre(99);
        array<SDamageStatistics, 11> dps;

//         for (int i = 0; i < 1000; ++i){
//             std::uniform_int_distribution<int> id(10, 1000);
//             dps[10] = SDamageStatistics("abc,", id(dre), 0);
//             std::sort(dps.begin(),dps.end());
//         }

        {
            set<SDamageStatistics> dps;
            for (int i = 0; i < 1000; ++i)
            {
                std::uniform_int_distribution<int> id(10, 1000);
                dps.insert(SDamageStatistics("abc,", 66/*id(dre)*/, 0));
                //std::sort(dps.begin(), dps.end());
            }

            //vector<SDamageStatistics> dp1(dps.begin(), dps.begin() + 10);

            SDamageStatistics const& s = *dps.begin();
            int acc = 0;
            for_each(dps.begin(), dps.end(), [&acc](SDamageStatistics const& l){

                acc += l.damage;
                //l.damage++;
            });
            cout << acc << endl;
        }


        for_each(dps.begin(), dps.end(), [](SDamageStatistics& l){
            cout << l.damage << " , " << l.pecent << endl;
        });

        int hurt = 934;
        int total_hurt = 1834;
        float percent0 = ((float)hurt / (float)total_hurt) ;
        float percent1 = ((float)(total_hurt - hurt) / (float)total_hurt);
        cout << percent0 << endl;
        cout << percent1 << endl;
    }
}

////////////////////////////////////////
namespace other{
    struct President
    {
        std::string name;
        std::string country;
        int year;

        President(std::string p_name, std::string p_country, int p_year)
            : name(std::move(p_name)), country(std::move(p_country)), year(p_year)
        {
            std::cout << "I am being constructed.\n";
        }
        President(President&& other)
            : name(std::move(other.name)), country(std::move(other.country)), year(other.year)
        {
            std::cout << "I am being moved.\n";
        }
        President& operator=(const President& other) = default;
    };

    void test(){
        std::vector<President> elections;
        std::cout << "emplace_back:\n";
        elections.emplace_back("Nelson Mandela", "South Africa", 1994);

        std::vector<President> reElections;
        std::cout << "\npush_back:\n";
        reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));

        std::cout << "\nContents:\n";
        for (President const& president : elections) {
            std::cout << president.name << " was elected president of "
                << president.country << " in " << president.year << ".\n";
        }
        for (President const& president : reElections) {
            std::cout << president.name << " was re-elected president of "
                << president.country << " in " << president.year << ".\n";
        }
    }
}

namespace 
{
//     strExecName = strDir.substr(pos+1, string::npos);
//     if ('D' == strExecName.at(strExecName.length() - 1))
//         strExecName.erase(strExecName.length() - 1);
//     strcasecmp(pOpen, "true") == 0

//     string a = "1234567";
//     strcpy((char*)a.data(), "1");
//     auto s = a.size();

//     struct C1{
//         int a;
//         int& get_a(){ 
//             return a; 
//         }
//     };
//     C1 c1;
//     c1.a = 10;
//     auto& b = c1.get_a();
//     auto b = c1.get_a();
//     b = 11;
}
enum EFighterAttr
{
    EFIGHTERATTR_MIN = 0, //最小值
    EFIGHTERATTR_PHYATK = 1, //物理攻击
    EFIGHTERATTR_MGCATK = 2, //法术攻击
    EFIGHTERATTR_PHYDEF = 3, //物理防御
    EFIGHTERATTR_MGCDEF = 4, //法术防御
};
template<unsigned char POLY>
class CCrc8
{
public:
    static unsigned char CalcCrc(const void* buf, size_t len)
    {
        return s_Instance.CalcCrcTable(buf, len);
    }
    CCrc8()
    {
        memset(&crcTable, 0, sizeof(crcTable));
        for (int i = 0; i<256; i++)
        {
            unsigned char byResult = (unsigned char)i;
            for (int j = 8; j>0; j--)
            {
                if (byResult & 0x80)
                    byResult = (byResult << 1) ^ POLY;
                else
                    byResult <<= 1;
            }
            crcTable[i] = byResult;
        }
    }
    unsigned char CalcCrcTable(const void* buf, size_t len)
    {
        unsigned char* ptr = (unsigned char*)buf;
        unsigned char crc = 0;
        while (len-- != 0)
        {
            crc = crcTable[crc ^ *ptr++];
        }
        return crc;
    }

    unsigned char crcTable[256];
   // static CCrc8 s_Instance;
};

struct A{
    static int a;
};
struct A1:public A{
    int a1;
};

struct A2 :public A{
    int a2;
};
int A::a = 100;

#include <memory>
#include <functional>
using namespace std;

class CPlayer
{
    int id_;
public:
    void dosth()
    {
        cout << "do something";
    }
    void SetCreatureID(int id) { id_ = id; }
    int GetCreatureID() { return id_; }
};
struct SFmlyWarPlyPaticipate
{
    int rid;
    function<void()> hdlr;
    SFmlyWarPlyPaticipate(int RoleID_, function<void()> Hdlr_)
        :rid(RoleID_), hdlr(Hdlr_)
    {
    }
    ~SFmlyWarPlyPaticipate()
    {
        cout << "~SFmlyWarPlyPaticipate:" << rid << endl;
    }
    friend inline bool operator< (SFmlyWarPlyPaticipate const& l, SFmlyWarPlyPaticipate const& r)
    {
        return l.rid < r.rid;
    }
    friend inline bool operator== (SFmlyWarPlyPaticipate const& l, int rid)
    {
        return l.rid == rid;
    }
};

typedef unsigned int UINT32;
typedef vector<UINT32> TVecUINT32;
typedef enum { EEBATTLETYPE_NONE = 2, EBATTLETYPE_MAX = 10 } EBattleType;
struct SBitScene
{
    UINT32 bitscene[EBATTLETYPE_MAX];
    SBitScene()
    {
        for (auto i = 0; i < EBATTLETYPE_MAX; ++i)
            bitscene[i] = 0;
    }
    ~SBitScene()
    {
        cout << "dslfkjlsjdff" << endl;
    }
    bool any()
    {
        TVecUINT32 bs(bitscene, bitscene + EBATTLETYPE_MAX);
        return any_of(bs.begin(), bs.end(), [](UINT32 e){ return e > 0; });
    }
    bool test(EBattleType type)
    {
        return bitscene[type] != 0;
    }
    void set(EBattleType type, UINT32 id)
    {
        bitscene[type] = id;
    }
    void reset(EBattleType type)
    {
        bitscene[type] = 0;
    }
};
void test(int a)
{
    cout << "test:" << a << endl;
}
typedef unsigned char UINT8;
typedef int INT32;
INT32 CalcHurtByStar(INT32 hurt, UINT8 casterStar, UINT8 tarStar)
{
    if (casterStar == 0 || tarStar == 0 || casterStar == tarStar) return hurt;
    hurt += hurt * (casterStar - tarStar) * 1000 / 10000;
    return hurt;
}

//函数形式如 test（int&），怎样使 test（100）这样的调用编译过呢？（调用的地方太多，不想一个个改了）
namespace AA{
    void test(int& a)
    {
        a = 100;
        cout << "tst1..." << endl;
    }
    void test(const int& a)
    {
        cout << " tst22 ..." << endl;
        int b = a;
        test(b);
    }
    void main()
    {
        test(1);
        int a = 1;
        test(a);
        cout << "a is " << a;
    }
}

namespace {
    void main()
    {
        int b = 1;
        try{
            int l = 1 / b;
        }
        catch (const char* e)
        {
            cout << "catch string: " << e;
        }
        catch (std::exception& e)
        {
            cout << "catch std::exception: " << e.what();
        }
        catch (...)
        {
            cout << "catch unknown exception!";
        }
    }
}

namespace Test_shared{

    struct demo{
        int val;
        demo(int v): val(v) { 
            std::cout << "demo()," 
                << "0x" << std::hex << this 
                << "," << val 
                << std::endl; 
        }
        ~demo() {
            std::cout << "~demo(), " 
                << val 
                << std::endl; 
        }
    };
    void main() {
        shared_ptr<demo> v2 = make_shared<demo>(100);
        shared_ptr<demo> v3 = make_shared<demo>(200);

        shared_ptr<demo> v1(new demo(1));
    }
}

void other_main()
{
    Test_shared::main();

    main();
    AA::main();
    UINT8 a = 1, b = 2;
    int h = (a - b);
    b = (a = 3 * 5, a * 4), test(b);
    
    auto hurt = CalcHurtByStar(934, 2, 1);
    hurt = CalcHurtByStar(934, 2, 3);

    ///////////////////////////////////
    unsigned int i = unsigned long long(1000212) * 10000 / 10000 + 0;

    vector< shared_ptr<SFmlyWarPlyPaticipate> > v1, v2;
    {
        int maxcount = 1000000;
        int count = maxcount;
        auto p = new SFmlyWarPlyPaticipate(count, nullptr);
        {
            boost::progress_timer t;
            while (count--) v1.emplace_back(p);
        }
        count = maxcount;
        {
            boost::progress_timer t;
            while (count--) v2.push_back(shared_ptr<SFmlyWarPlyPaticipate>(p));
        }
    }

    //////////////////////////////////////////
    CPlayer ply;
    ply.SetCreatureID(100);
    CPlayer* player = &ply;
    set<SFmlyWarPlyPaticipate> plyset;

//     if (find(plyset.begin(), plyset.end(), player->GetCreatureID()) == plyset.end())
    plyset.emplace(
        player->GetCreatureID(),
        bind(&CPlayer::dosth, player));
    plyset.emplace(
        player->GetCreatureID(),
        bind(&CPlayer::dosth, player));
    plyset.emplace(
        player->GetCreatureID(),
        bind(&CPlayer::dosth, player));

    ///////////////////////////////////
    Work::main5();
    other::test();

    std::default_random_engine dre(23);
    std::uniform_int_distribution<int> id(10, 1000);

    int size = 5;
    vector< tuple<string, int> > vid(size);
    for (auto i = 0; i < size; ++i)
    {
        vid[i] = make_tuple("dksfjslf", id(dre));
    }

    sort(vid.begin(), vid.end(), [](tuple<string, int> const& left, tuple<string, int> const& right)->bool
    {
        return std::get<1>(left) < std::get<1>(right);
    });

    Recount::main();
    //Work::main1();
    cout << __DATE__ << endl;
    cout << __TIME__ << endl;
}


namespace{

    void test1(int x, int a, int b, const char* c, const char* d, const char* e)
    {

    }
    void test()
    {
        unsigned  a1 = 100;
        auto len = sizeof(a1);
        auto sa = std::to_string(a1);

        shared_ptr<int> sp;

        auto pp = sp.get();
        long day = 24 * 3600;
        long a = 1429647000 % day;
        long b = a % (3600);
        long c = b % 60;

        ///////////////////////////////////////////////////////////
        std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        int sum = std::accumulate(v.begin(), v.end(), 0);

        int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());

        std::string s = std::accumulate(v.begin() + 1, v.end(), std::to_string(v[0]),
            [](const std::string& a, int b) {
            return a + '-' + std::to_string(b);
        });
        std::cout << "sum: " << sum << '\n'
            << "product: " << product << '\n'
            << "dash-separated string: " << s << '\n';
    }
}