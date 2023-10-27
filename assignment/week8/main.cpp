#include<iostream>

template<class T>
class Learn {
private:
    T attr;

public:
    Learn() : attr(0) {
        std::cout<<"Calling the default constructor"<<std::endl;
    }

    Learn(T x) : attr(x) {
        std::cout<<"Calling the constructor with ("<<attr<<")\n";
    }

    Learn(const Learn& rhs) : attr(rhs.attr) {
        std::cout<<"Calling the copy constructor"<<std::endl;
    }

    Learn& operator=(const Learn& rhs) {
        attr = rhs.attr;
        std::cout<<"Calling the assign operator"<<std::endl;
        return *this;
    }

    ~Learn() {
        std::cout<<"Calling the destructor [with "<<attr<<"]\n"<<std::endl;
    }

    void increase(T x) {
        attr += x;
    }
};

template<class T>
void foo(Learn<T> & a, Learn<T> b) {
    std::cout<<"Inside foo"<<std::endl;
    a.increase(10);
    b.increase(20);
}

int main (void) {
    printf("learn1:\n");
    Learn<int> learn1;
    printf("learn2:\n");
    Learn<int> learn2(4);
    printf("learn3:\n");
    Learn<int> learn3 = learn2;
    learn3.increase(5);
    printf("learn1 = learn2:\n");
    learn1 = learn2;
    printf("foo(learn1, learn2):\n");
    foo(learn1, learn2);
    printf("Back to main\n");
    return EXIT_SUCCESS ;
}