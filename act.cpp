namespace olymplib {

template <class InputIt, class Callable>
void CallStuff(InputIt begin, InputIt end, Callable callee, int threads = 1)
{
    while (begin != end) {
        callee(*begin++);
    }
}

} // namespace olymplib