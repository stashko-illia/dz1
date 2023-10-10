#include <vector>
#include <queue>
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

int getCollatz(int number) {
    if (number < 1) return 0;
    int count = 0;
    while (number != 1) {
        if (number % 2 == 0) number = number / 2;
        else number = 3 * number + 1;
        count++;
    }
    return count;
}

std::mutex colatzMutex;

void threadFunction(std::atomic<int>& value, std::atomic<unsigned int>& totalSteps) {
    while (true) {
        int temp;
        {
            std::lock_guard<std::mutex> lock(colatzMutex);

            if (value == 0) return;
            temp = value;
            value--;
        }
        //computing required steps
        totalSteps += getCollatz(temp);
    }
}

int main() {
    const int N = 100'000;
    const int THREAD_NUMBER = 8;

    std::vector<std::thread> threads;
    std::atomic<unsigned int> totalSteps(0);
    std::atomic<int> value(N);

    for (int i = 0; i < THREAD_NUMBER; i++) 
        threads.push_back(std::thread(threadFunction, std::ref(value), std::ref(totalSteps)));

    for (std::thread& thread : threads) 
        thread.join();

    std::cout << "Total steps: " << totalSteps.load() << std::endl;
    std::cout << "Average steps per number: " << totalSteps.load()/N << std::endl;

    return 0;
}