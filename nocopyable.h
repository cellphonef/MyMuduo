#ifndef MYMUDUO_NOCOPYABLE_H
#define MYMUDUO_NOCOPYABLE_H

//  不可拷贝基类
//  
//  实现和设计要点：
//    ①理论上让一个类不可拷贝最直接的方式有两种：
//      （1）C++11之前，我们可以将拷贝构造函数和拷贝赋值运算符定义为私有成员，这样外部就无法访问了，
//          自然就无法拷贝和赋值了。不过对于友元类仍然能够访问其私有成员，为此我们可以不定义这两个操作，
//          从而当友元类使用时将发生链接错误。
//      （2）C++11之后，我们可以将拷贝构造函数和拷贝赋值运算符直接delete，这样任何地方都无法访问。
//  
//    ②这两种方式对于可能有大量不可拷贝的类比较麻烦，因此引出了nocopyable类，当一个类不可拷贝时，可以
//      直接继承该类使其不可拷贝。继承该类的派生类应该能够正常构造和析构。派生类的构造函数会先调用其基类的构造函数，
//      析构函数会后调用其基类的析构函数，因此基类的构造函数和析构函数对于派生类应该是可访问的且不是删除的。
class noncopyable {
public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};


#endif  // MYMUDUO_NOCOPYABLE_H