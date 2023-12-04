#include <iostream>
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

int main(){
    const int N = 100'000;
    int totalSteps = 0;
    for(int i = 1; i < N; i++){
        totalSteps += getCollatz(i);
    }

    std::cout<<"Total steps:"<<totalSteps<<'\n';
    std::cout << "Average steps per number: " << totalSteps/N << std::endl;

}