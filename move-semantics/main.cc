#include <iostream>
#include <cstring>

/**
 * rule of three : dtor, copy ctor, copy assign operator.
 * copy and swap idiom (exception safety)
 *   make a copy
 *   swap the contents with the copy
 *   get rid of the copy by leaving the scope.
 */


class String {
public:
    String(const char* p){
        auto size = std::strlen(p) + 1;
        data = new char[size];
        std::memcpy(data, p, size);
    }

    String(const String& that){
        auto size = strlen(that.data) + 1;
        data = new char[size];
        memcpy(data, that.data, size);
    }

    //
    // valid state이 되게 보장만 하면,
    // 무슨 로직을 넣어도 된다.
    //
    String(String&& that){
    // rvalue가 들어오는 경우엔, deep copy 대신 shallow copy를 하고
    // that의 dtor에서 소멸 당하지 않게 that의 data에 nullptr 대입.
        data = that.data;
        that.data = nullptr;
    }

    //
    // &&를 사용하지 않았다.
    // that은 String object의 ctor 로직을 따라 초기화 될 것이다.
    // =의 operand가 lvalue면 copy ctor, rvalue면 move ctor
    //
    String& operator=(String that)
    {
        std::swap(data, that.data);
        return *this;
    }

    ~String(){
        if(data){
            delete[] data;
        }
    }
private:
    char* data;
};

int main() {

    // rvalue?
    //   이름이 붙여지지 않아 다시 참조할 수 없고,
    //   다음 expression 실행 시점으로 넘어갈 때 소멸되는 value를 말한다.
    //   An rvalue of class type is an expression
    //   whose evaluation creates a temporary object

    // String a(x);                                    // Line 1
    // String b(x + y);                                // Line 2
    // String c(some_function_returning_a_string());   // Line 3

    // Line 1과 같은 lvalue는 나중에 참조가 가능하므로, copy assign 동작에 쓰인 operand가 수정되어서는 안된다.
    // Line 2,3의 경우는 해당 expr을 벗어나면 임시 객체들에 접근할 방법 조차 없으니,
    // 임시 객체들은 딱히 보존할 필요가 없다.
    // 그래서 Line 2,3의 경우는 실질적으로 deep copy가 필요하지는 않다.
    // 그냥 임시 객체에 할당된 data를 shallow copy해서 가져다 사용함으로써 성능 향상을 얻을 수 있다.

    // C++11은 이런 경우를 구분해 동작하게 하기 위해서 &&(rvalue reference) 타입을 도입하고
    // overloading을 통해서 programming level에서 동작을 구분해 정의할 수 있게 하였다.


    // 아래 코드는 lvalues move는 잠재적 위험이 있고,
    // rvalues move는 괜찮다는 것을 시사한다.

    // auto_ptr<Shape> a(new Triangle);   // create triangle
    // auto_ptr<Shape> b(a);              // move a into b
    // double area = a->area();           // undefined behavior

    // 만일 lvalue arg와 rvalue arg를 구분할 수 있는 language support가
    // 있다면 lvalue move를 하는 실수를 방지할 수 있을 거다.
    // 그리하여 나온게 T&&
    // rvalue reference는 rvalue에만 binging된다.

    //            lvalue   const lvalue   rvalue   const rvalue
    // ---------------------------------------------------------
    // X&          yes
    // const X&    yes      yes            yes      yes
    // X&&                                 yes
    // const X&&                           yes      yes

    // rvalue move ctor만 존재하는 unique_ptr의 경우
    // unique_ptr<Shape> a(new Triangle);
    // unique_ptr<Shape> b(a);                 // error
    // unique_ptr<Shape> c(make_triangle());   // okay

    // 실수로 lvalue로부터의 move를 하지 못하게 하는 것이 우리가 원하는 것.

    // 가끔은 lvalue로 부터의 move가 쓰고 싶을 수도 있다.
    // 잠재적으로 위험이 있는 것을 인지하더라도 lvalue를 rvalue function으로
    // 넘기고 싶은 상황에 <utility>에 있는 std::move를 이용해 casting하여 사용한다.

    // std::move는 temporary object를 생성하지 않는다.
    // 이 때문에 표준위원회는 XValue라는 3번째 value category를 만들게 되었다.
    // 전통적 의미의 rvalue는 아니지만, rvalue reference에 bind 될 수 있는 것을 xvalue라 하고
    // 전통적 의미의 rvalue를 prvalue라고 부르기로 하였다.
    // (expiring value) (pure rvalues)
    //
    //          expressions
    //           /     \
    //          /       \
    //         /         \
    //     glvalues   rvalues
    //       /  \       /  \
    //      /    \     /    \
    //     /      \   /      \
    // lvalues   xvalues   prvalues

    //
    // Special rule : Never use std::move to move automatic objects out of functions.
    // automatic object? : static이 아닌 local variable
    //
    // NRVO(Named Return Value Optimization)
    //
    // unique_ptr<Shape> make_square()
    // {
    //     unique_ptr<Shape> result(new Square);
    //     return result;   // note the missing std::move
    // }
    //
    // unique_prt<Shape> c(make_square); // Implicit move 발생!
    //
    // 어차피 지역 변수도 리턴되고 나면 접근할 방법이 없으므로,
    // 이러한 최적화가 동작하도록 표준위원회에서 special rule을 제정했다.
    //
    // 아래와 같은 코드는 dangling reference를 유발한다..
    // move의 수행자체는 std::move에 의해서가 아니라 move ctor에 의해서이다.
    // unique_ptr<Shape>&& flawed_attempt()   // DO NOT DO THIS!
    // {
    //     unique_ptr<Shape> very_bad_idea(new Square);
    //     return std::move(very_bad_idea);   // WRONG!
    // }

    // class Foo
    // {
    //     unique_ptr<Shape> member;
    //
    // public:
    //
    //     Foo(unique_ptr<Shape>&& parameter)
    //     : member(parameter)   // error : A named rvalue reference is an lvalue, just like any other variable.
    //     {}
    // };

    return 0;
}
