#include <stdio.h>
#include <assert.h>
#include "ringbuffer.hpp"

/* A simple map function, which adds an element by 5 */
int add_five(int x){
    return x + 5;
}

/* The main function entry */
int main(){
    /* Get a new ring buffer */
    std::vector<int> a{};
    std::vector<int> b{};
    for (int i=0; i<1023; i++) a.push_back(i);
    RingBuffer<int> rb;
    rb.write_multi(a.size(),a);
    for(int i=0; i<1023; i++)
    {
        std::cout<<a[i]<<" ";
    }
    std::cout<<std::endl;
    rb.read_multi(511,b);
    std::cout<<std::endl;
    rb.write_multi(a.size(),a);
    b.clear();
    rb.read_multi(1023,b);
    std::cout<<std::endl;

    std::cout<<"###################Below are iterator test:################"<<std::endl;
    __detail::__iterator<int> x = rb.begin();
    __detail::__iterator<int> y = x;
    /*std::cout<<"operator = for __iterator; "<<"x.para: "<<x._I_capacity<<" "<<x._I_content<<" "<<x._I_nowpos<<" ;y.paramater: "<<y._I_capacity<<" "<<y._I_content<<" "<<y._I_nowpos<<std::endl;*/
    if(x == y) std::cout<<"operator == for __iterator: CORRECT"<<std::endl;
    if(!(x != y)) std::cout<<"operator != for __iterator: CORRECT"<<std::endl;
    if(*x == 511) std::cout<<"operator * for __iterator: CORRECT"<<std::endl;
    if(*(x++) == 511) std::cout<<"operator ++(postfix) for __iterator: CORRECT"<<std::endl;
    if(*(++x) == 513) std::cout<<"operator ++(prefix) for __iterator CORRECT"<<std::endl;
    if(*(x--) == 513) std::cout<<"operator --(postfix) for __iterator CORRECT"<<std::endl;
    if(*(--x) == 511) std::cout<<"operator --(prefix) for __iterator CORRECT"<<std::endl;
    x+=1;
    if(*x == 512) std::cout<<"operator += for __iterator CORRECT"<<std::endl;
    y = x+8;
    if(*y == 520) std::cout<<"operator + for __iterator CORRECT"<<std::endl;
    x = y-8;
    if(*x == 512) std::cout<<"operator - for __iterator CORRECT"<<std::endl;
    x-=1;
    if(*x == 511) std::cout<<"operator -= for __iterator CORRECT"<<std::endl;

    std::cout<<"###################Below are const iterator test:################"<<std::endl;
    __detail::__const_iterator<int> m(x);
    __detail::__const_iterator<int> n(m);
    /*std::cout<<"operator = for __iterator; "<<"m.para: "<<m._I_capacity<<" "<<m._I_content<<" "<<m._I_nowpos<<" ;n.paramater: "<<n._I_capacity<<" "<<n._I_content<<" "<<n._I_nowpos<<std::endl;*/
    if(m == m) std::cout<<"operator == for __const_iterator: CORRECT"<<std::endl;
    if(!(m != n)) std::cout<<"operator != for __const_iterator: CORRECT"<<std::endl;
    if(*m == 511) std::cout<<"operator * for __const_iterator: CORRECT"<<std::endl;
    if(*(m++) == 511) std::cout<<"operator ++(postfix) for __const_iterator: CORRECT"<<std::endl;
    if(*(++m) == 513) std::cout<<"operator ++(prefix) for __const_iterator CORRECT"<<std::endl;
    if(*(m--) == 513) std::cout<<"operator --(postfix) for __const_iterator CORRECT"<<std::endl;
    if(*(--m) == 511) std::cout<<"operator --(prefix) for __const_iterator CORRECT"<<std::endl;
    m+=1;
    if(*m == 512) std::cout<<"operator += for __const_iterator CORRECT"<<std::endl;
    n = m+8;
    if(*n == 520) std::cout<<"operator + for __const_iterator CORRECT"<<std::endl;
    m = n-8;
    if(*m == 512) std::cout<<"operator - for __const_iterator CORRECT"<<std::endl;
    m-=1;
    if(*m == 511) std::cout<<"operator -= for __const_iterator CORRECT"<<std::endl;


    RingBuffer<int> rs;
    std::vector<int> rbuf{};
    std::vector<int> wbuf{};
    for(int i=0; i<20; i++)
    {
        wbuf.push_back(i+1);
    }
    assert(rs.write_multi(wbuf.size(), wbuf));
    assert(!rs.is_empty());
    assert(rs.read_multi(10, rbuf));
    for(int i=0; i<10;i++)
    {
        assert(rbuf[i] == i+1);
    }
    rs.map(add_five);
    for(int i=10;i<20;i++)
    {
        int x;
        assert(rs.read(x));
        assert(x == i + 6);
    }
    assert(rs.is_empty());
    std::cout<<"CORRECT";


    RingBuffer<int> ra;
    int data;
    for(int i = 0; i<1; i++)
    {
        int size = 100;
        for(int j = 0; j<size; j++)
        {
            ra.write(j+5);
        }
        for(int j = 0; j<size; j++)
        {
            if(!ra.read(data)) std::cout<<"Error 1";
            if(data!=j+5) std::cout<<"Error 2: Suppose: "<<j+5<<";Data: "<<data<<std::endl;
        }
    }
    return 0;
}