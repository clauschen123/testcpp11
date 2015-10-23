#include "stdafx.h"
#include <string>  
#include <map>  
#include <iostream>
using namespace std;

// #define DEFINE_DYN_CREATE(class_name)              \
// static CBaseClass * CreateClass## class_name();
// 
// #define IMPLIMENT_DYN_CREATE(class_name)           \
// static CBaseClass * CreateClass## class_name(){    \
//         return new class_name;                     \
// };


#define DEFINE_CLASS(class_name)                        \
char NameArray[] = #class_name;                         \
class class_name : public Register<class_name, NameArray>

#define DEFINE_CLASS_EX(class_name,father_class)        \
char NameArray[] = #class_name;                         \
class class_name : public Register<class_name, NameArray>, public father_class

namespace reflect
{
    typedef void* (*CreateFuntion)(void);

    class ClassFactory
    {
    public:
        static void* GetClassByName(std::string name)
        {
            std::map<std::string, CreateFuntion>::const_iterator find;
            find = m_clsMap.find(name);
            if (find == m_clsMap.end())
            {
                return nullptr;
            }
            else
            {
                return find->second();
            }
        }

        static void RegistClass(std::string name, CreateFuntion method)
        {
            m_clsMap.insert(std::make_pair(name, method));
        }

    private:
        static std::map<std::string, CreateFuntion> m_clsMap;
    };

    std::map<std::string, CreateFuntion> ClassFactory::m_clsMap;

    class RegistyClass
    {
    public:
        RegistyClass(std::string name, CreateFuntion method)
        {
            ClassFactory::RegistClass(name, method);
        }
    };

    template<class T, const char name[]>
    class Register
    {
    public:
        Register()
        {
            const RegistyClass tmp = rc;
        }

        static void* CreateInstance()
        {
            return new T;
        }

    public:
        static const RegistyClass rc;//这个为什么没有生成
    };

    template<class T, const char name[]>
    const RegistyClass Register<T, name>::rc(name, Register<T, name>::CreateInstance);

    DEFINE_CLASS(CG)
    {
//         int a_;
    public:
        void Display()
        {
//             cout << a_ << endl;
            printf("I am Here\n");
        }
    };

    int main1()
    {
        CG* tmp = (CG*)ClassFactory::GetClassByName("CG");
        tmp->Display();
        return 0;
    }
}

void reflect_main()
{
    reflect::main1();
}