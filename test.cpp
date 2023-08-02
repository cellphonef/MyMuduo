#include <iostream>


using namespace std;


class test {
public:

    static void pub_static_method() {
        cout << b << endl;
        cout << c << endl;
    }


private:
    test() {
        cout << "private ctor invoke" << endl;
    }
    ~test() {
        cout << "private dtor invoke" << endl;
    }

    static int initc() {
        return 40;
    }
    int a = 20;
    static int b;
    static test* t;
    static int c;

};

// test t2;
int test::b = 30;
test* test::t = new test();
int test::c = initc();

int main()  {
    test::pub_static_method();
    // cout << test::b << endl;
    
}