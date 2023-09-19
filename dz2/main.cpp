#include <iostream>
#include <vector>
#include <random> //distribution support
#include <thread>
#include <chrono> // time measure


std::default_random_engine generator;
std::uniform_real_distribution<double> dis(-1.0, 1.0);
double getRandomDouble(){
    return dis(generator);
}
class Point{
    public:
        double x;
        double y;
        Point(double x, double y) : x{x}, y{y}{}
        Point(){
            x = getRandomDouble();
            y = getRandomDouble();
        }
};

const size_t POINTS_NUMBER = 100'000'000;
const size_t THREADS_NUMBER = 16;

bool isInCircle(Point point, double radius = 1.0){
    return sqrt((point.x * point.x) + (point.y * point.y)) <= radius;
}

void computeInOneThread(){
    int inCircleCount = 0;
    for(int i =0 ; i < POINTS_NUMBER; i++){
        if(isInCircle(Point())) inCircleCount++;
    }
    const double res = inCircleCount / (POINTS_NUMBER * 1.0) * 4;
    std::cout<<"The result is " << res << '\n';
}

void threadFunction(int amount, int& totalInCircleCount){
    int localInCircle = 0;
    for(int i = 0; i < amount; i++){
        if(isInCircle(Point())) localInCircle++;
    }
    totalInCircleCount += localInCircle;
}

void computeInMultipleThreads(){
    std::vector<std::thread> threadsArr;
    int inCircleCount = 0;

    int pointsPerThread = POINTS_NUMBER / THREADS_NUMBER;
    for(size_t i = 0; i < THREADS_NUMBER; i++){
        threadsArr.push_back(std::thread(threadFunction, pointsPerThread, std::ref(inCircleCount)));
    }
    for( auto& thread: threadsArr){
        thread.join();
    }

    const double res = inCircleCount / (POINTS_NUMBER * 1.0) * 4;
    std::cout<<"The result is " << res << '\n';
}


int main(){
    using namespace std::chrono;
    auto start1 = high_resolution_clock::now();
    computeInOneThread();
    auto stop1 = high_resolution_clock::now();

    std::cout<<"Single thread: "<< (duration_cast<milliseconds>(stop1 - start1).count() * 1.0 )/1000 <<" seconds \n";

    auto start2 = high_resolution_clock::now();
    computeInMultipleThreads();
    auto stop2 = high_resolution_clock::now();

    std::cout<<"Multi thread("<< THREADS_NUMBER<<"): "<< (duration_cast<milliseconds>(stop2 - start2).count() * 1.0) /1000 <<" seconds \n";

    return 0;
}