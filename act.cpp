#include <thread>

namespace olymplib {

template <class InputIter, class Callable>
void CallStuff(InputIter begin, InputIter end, Callable callee)
{
    while (begin != end) {
        callee(*begin++);
    }
}

template <class RandomAccessIter, class Callable>
void CallStuff(RandomAccessIter begin, RandomAccessIter end, Callable callee, int threads) {
    std::thread* roflmao = new std::thread[threads];
    if (threads == 2) {
        RandomAccessIter mid = begin + (end - begin) / 2;
        roflmao[0] = std::thread([](RandomAccessIter begin, RandomAccessIter end, Callable callee) { CallStuff(begin, end, callee); }, begin, mid, callee);        
        roflmao[1] = std::thread([](RandomAccessIter begin, RandomAccessIter end, Callable callee) { CallStuff(begin, end, callee); }, mid, end, callee);        
    } else {
//    	for (int i = 0; i < threads; ++i) {
//           roflmao[i] = std::thread(CallStuff<RandomAccessIter, Callable>, (begin * (threads - i) + end * i) / threads, (begin * (threads - i - 1) + end * (i + 1)) / threads, callee);        
//	}
    }
    for (int i = 0; i < threads; ++i) {
        roflmao[i].join();
    }
    delete[] roflmao;
}

} // namespace olymplib
