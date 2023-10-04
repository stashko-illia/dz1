#include <vector>
#include <queue>
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

class LabNumber {
public:
    unsigned int number;
    unsigned int steps;
    LabNumber() : number{0}, steps{0} {}
    LabNumber(unsigned int number) : number{number}, steps{0} {}
};

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
std::queue<LabNumber> arr;

void threadFunction(std::queue<LabNumber>& queue, std::atomic<unsigned int>& totalSteps) {
    while (true) {
        LabNumber labNumber;
        {
            std::lock_guard<std::mutex> lock(colatzMutex);

            if (queue.empty()) return;
            labNumber = queue.front();
            queue.pop();
        }

        int steps = getCollatz(labNumber.number);

        labNumber.steps = steps;
        totalSteps += steps;
    }
}

int main() {
    const int N = 100'000;
    const int THREAD_NUMBER = 8;

    std::vector<std::thread> threads;
    std::atomic<unsigned int> totalSteps(0);

    for (int i = 1; i < N; i++)
        arr.push(LabNumber(i));

    for (int i = 0; i < THREAD_NUMBER; i++) 
        threads.push_back(std::thread(threadFunction, std::ref(arr), std::ref(totalSteps)));

    for (std::thread& thread : threads) 
        thread.join();

    std::cout << "Total steps: " << totalSteps.load() << std::endl;
    std::cout << "Average steps per number: " << totalSteps.load()/N << std::endl;

    return 0;
}
