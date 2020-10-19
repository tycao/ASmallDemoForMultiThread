#include <iostream>     // for std::cout
#include <string>   // for std::getline
#include <mutex>    // for std::mutex
#include <condition_variable>   // for std:: condition_variable
#include <thread>              // for std::thread
#include <queue>                // for std::queue
#include <chrono>

using namespace std;

std::queue<string> _queue;
std::mutex _mtx;
std::condition_variable _cond;
std::atomic_bool stop_flag = false;

int bak_main() {

    for (int i = 0; i < 5; ++i) {
        // 从队列获取元素
        std::thread([]() {
            while (true) {
                string msg;
                {
                    std::unique_lock<std::mutex> lck(_mtx);
                    if (_queue.empty()) {
                        _cond.wait(lck, []() {return stop_flag || !_queue.empty(); });
                    }
                    if (stop_flag && _queue.empty()) break;
                    msg = _queue.front();
                    _queue.pop();
                }

                cout << "[thread_id= " << this_thread::get_id() << "] your input:" << msg << " is now in queue." << endl;
            }
        }).detach();
    }

    


    // 给队列添加元素
    std::thread([]() {
        string str;
        while (true) {
            std::getline(cin, str);
            if (str == "stop") {
                stop_flag = true;
                _cond.notify_one();
                break;
            }
            if (str.size() > 0) {
                std::lock_guard<std::mutex> lck(_mtx);
                _queue.push(str);
                cout << "[thread_id= " << this_thread::get_id() << "] your input is: " << str << endl;
                _cond.notify_one();
            }
        }
    }).join();

    //std::vector<std::thread> thr;
    //for (int i = 0; i < 5; ++i) {
    //    thr.push_back(std::thread([]() {
    //        while (!stop_flag) {
    //            std::lock_guard<std::mutex> lck(_mtx);
    //            _queue.push("Ash");
    //            cout << "[thread_id= " << this_thread::get_id() << "]" << endl;
    //            _cond.notify_one();
    //        }
    //    }));
    //}
    //this_thread::sleep_for(chrono::seconds(10));
    //stop_flag = true;
    //_cond.notify_all();
    //for (auto& t : thr) {
    //    if (t.joinable()) t.join();
    //}
    return 0;
}
