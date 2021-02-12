#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

/*
    Test for overloaded functions that modify the resources(resource is passed by reference)
    e.g Camera publish images constantly, cloning image is expensive(pass by reference required)
        mutex_test::foo() and mutex_test::foo(int) needs lock because both function does operations on resource(mutex)
*/


class mutex_test
{
private:
    std::string _className;
    std::mutex _mutex;

    // overloaded functions for test
    void foo();
    void foo(int);
public:
    mutex_test(std::string className);
    ~mutex_test();

    // overloaded functions for test
    void whileFoo();
    void whileFoo(int);
};


mutex_test::mutex_test(std::string className) : _className{className}
{
}


mutex_test::~mutex_test()
{
}


void mutex_test::foo()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << _className << "\n";
}


void mutex_test::foo(int)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << _className << "\n";
}


void mutex_test::whileFoo()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
        if(lock){
            foo();
        }
    }
}


void mutex_test::whileFoo(int)
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(_mutex, std::try_to_lock);
        if(lock){
            foo(1);
        }
    }
}


int main(int argc, char const *argv[])
{
    auto _startTime = std::chrono::system_clock::now();
    mutex_test a("class a");
    mutex_test b("class b");

    // pointer to function(points to different overloaded function)
    void (mutex_test::*fncPtrNoParam)() = &mutex_test::whileFoo;
    void (mutex_test::*fncPtrOther)(int) = &mutex_test::whileFoo;

    // threads calls overloaded functions
    std::thread c(fncPtrNoParam, &a);
    std::thread d(fncPtrOther, &b, 1);
    c.join();
    d.join();

    return 0;
}

