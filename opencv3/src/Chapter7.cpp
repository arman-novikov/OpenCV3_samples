#include "include/Chapter7.h"
#include <atomic>
#include <thread>
#include <condition_variable>
#include <array>
#include <typeinfo>
#include <functional>

using namespace std::chrono;

/*
1.Using the cv::RNG random number generator:
    a.Generate and print three floating-point numbers, each drawn from a uniform
        distribution from 0.0 to 1.0.
    b.Generate  and  print  three  double-precision  numbers,  each  drawn  from  a
        Gaussian distribution centered at 0.0 and with a standard deviation of 1.0.
    c.Generate  and  print  three  unsigned  bytes,  each  drawn
        from  a  uniform  distribution from 0 to 255.
*/
int Chapter7::exer_1()
{
    static constexpr size_t HOW_MANY = 3U;
    const time_point<system_clock> now = system_clock::now();
    const long count_of_mls = now.time_since_epoch().count();
    cv::RNG rng(static_cast<unsigned long>(count_of_mls));


    for (size_t i = 0; i < HOW_MANY; ++i)
        std::cout << rng.uniform(0.0F, 1.0F) << "\t";
    std::cout << std::endl;

    for (size_t i = 0; i < HOW_MANY; ++i)
        std::cout << rng.gaussian(1.0) << "\t";
    std::cout << std::endl;

    for (size_t i = 0; i < HOW_MANY; ++i)
        std::cout << rng.uniform(0, 255) << "\t";
    std::cout << std::endl;

    return 0;
}

int Chapter7::exer_1_uni_maker()
{
    const time_point<system_clock> now = system_clock::now();
    const long count_of_mls = now.time_since_epoch().count();
    cv::RNG rng(static_cast<unsigned long>(count_of_mls));

    auto uni_maker = [&rng] (auto &container) {
        const std::type_info &container_type(typeid(container));

        if (container_type == typeid(float_container_t))
            for (auto &i: container)
                i = rng.uniform(0.0, 1.0);
        else if (container_type == typeid(double_container_t))
            for (auto &i: container)
                i = rng.gaussian(1.0);
        else if (container_type == typeid(uint8_container_t))
            for (auto &i: container)
                i = static_cast<uint8_t>(rng.uniform(0, 255));
    };
    uni_maker(this->uint8_ones);
    this->printer(this->uint8_ones);
    return 0;
}

int Chapter7::exer_1_cond_maker()
{
    using namespace std;
    using namespace std::chrono_literals;
    static constexpr int MAX_SLEEP_TIME_MS = 3'000;
    static constexpr auto WAIT_TIME = 10ms;
    static constexpr size_t TASKS_COUNT = 3;
    enum task_t {FLOATS_TASK, DOUBLES_TASK, UINT8S_TASK};
    struct IPC_data_t {
        struct {
            condition_variable cv;
            mutex mx;
        } sync_data[TASKS_COUNT];
        atomic<size_t> ready_semaphore;
    } IPC_data;
    IPC_data.ready_semaphore.store(memory_order::memory_order_relaxed);

    auto floats_maker = []
        (float_container_t &container, IPC_data_t &ipc_data, const task_t task_type)
    {
        const time_point<system_clock> now = system_clock::now();
        const long count_of_mls = now.time_since_epoch().count();
        cv::RNG rng(static_cast<unsigned long>(count_of_mls));

        unique_lock<mutex> mu_lock(ipc_data.sync_data[task_type].mx);
        for (auto &i: container)
            i = rng.uniform(0.0F, 1.0F);

        this_thread::sleep_for(milliseconds(rng.uniform(0, MAX_SLEEP_TIME_MS))); // heavy job imitation
        ipc_data.ready_semaphore.fetch_add(1, memory_order::memory_order_release);
        cout << "floater done\n";
        ipc_data.sync_data[task_type].cv.notify_one();
    };
    auto doubles_maker = []
        (double_container_t &container, IPC_data_t &ipc_data, const task_t task_type)
    {
        const time_point<system_clock> now = system_clock::now();
        const long count_of_mls = now.time_since_epoch().count();
        cv::RNG rng(static_cast<unsigned long>(count_of_mls));

        unique_lock<mutex> mu_lock(ipc_data.sync_data[task_type].mx);
        for (auto &i: container)
            i = rng.gaussian(1.0);

        this_thread::sleep_for(milliseconds(rng.uniform(0, MAX_SLEEP_TIME_MS))); // heavy job imitation
        ipc_data.ready_semaphore.fetch_add(1, memory_order::memory_order_release);
        cout << "doubler done\n";
        ipc_data.sync_data[task_type].cv.notify_one();
    };
    auto uint8s_maker = []
        (uint8_container_t &container, IPC_data_t &ipc_data, const task_t task_type)
    {
        const time_point<system_clock> now = system_clock::now();
        const long count_of_mls = now.time_since_epoch().count();
        cv::RNG rng(static_cast<unsigned long>(count_of_mls));

        unique_lock<mutex> mu_lock(ipc_data.sync_data[task_type].mx);
        for (auto &i: container)
            i = static_cast<uint8_t>(rng.uniform(0, 255));

        this_thread::sleep_for(milliseconds(rng.uniform(0, MAX_SLEEP_TIME_MS))); // heavy job imitation
        ipc_data.ready_semaphore.fetch_add(1, memory_order::memory_order_release);
        cout << "uint8er done\n";
        ipc_data.sync_data[task_type].cv.notify_one();
    };

    thread f_th(floats_maker, ref(this->float_ones), ref(IPC_data), FLOATS_TASK);
    thread d_th(doubles_maker, ref(this->double_ones), ref(IPC_data), DOUBLES_TASK);
    thread u_th(uint8s_maker, ref(this->uint8_ones), ref(IPC_data), UINT8S_TASK);
    unique_lock<mutex> f_ulock(IPC_data.sync_data[FLOATS_TASK].mx);
    unique_lock<mutex> d_ulock(IPC_data.sync_data[DOUBLES_TASK].mx);
    unique_lock<mutex> u_ulock(IPC_data.sync_data[UINT8S_TASK].mx);
    do {
        IPC_data.sync_data[FLOATS_TASK].cv.wait_for(f_ulock, WAIT_TIME);
        IPC_data.sync_data[FLOATS_TASK].cv.wait_for(d_ulock, WAIT_TIME);
        IPC_data.sync_data[FLOATS_TASK].cv.wait_for(u_ulock, WAIT_TIME);
    } while (IPC_data.ready_semaphore.load(memory_order_acquire) != TASKS_COUNT);
    f_th.join();
    d_th.join();
    u_th.join();

    //this->printer(this->uint8_ones);
    return 0;
}

template <typename T>
void Chapter7::printer(const T &container)
{
    for (auto i : container)
        std::cout<<static_cast<double>(i)<<' ';
    std::cout<<std::endl;
}
