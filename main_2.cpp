/*
 * author : tycao
 * date   : 2020/10/20
*/
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>


template<typename Signature> class MyTemp;

template<typename R, typename ...Args> class MyTemp<R(Args...)> {
    using Callback = typename std::function<R(Args...args)>;
public:
    MyTemp(Callback&& callback_) {
        callback = callback_;
    }

    void operator()(Args...args) const
    {
        callback(args...);
    }
private:
    Callback callback;
};

int Fun(int i, int j) {
    std::cout << "i=" << i << "j=" << j << std::endl;
    return i + j;
}

int main() {

    MyTemp<int(int, int)> task([](int i, int j) {
        std::cout << "i=" << i << "j=" << j << std::endl;
        return i + j;
    });
    task(1, 2);

    MyTemp<int(int, int)> task__(Fun);
    task__(1, 2);

    MyTemp<void(std::string str)> task_([](std::string str) {
        std::cout << str << std::endl;
    });
    task_("MyTemp");


    return 0;
}
