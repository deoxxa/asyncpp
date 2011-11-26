#include <iostream>
#include <boost/bind.hpp>
#include <zmq.hpp>

#include "asyncpp.h"

class MyObject
{
  public:
    typedef AsyncPP<int*,int*> myAsync_t;

  private:
    int m_a;
    int m_b;
    myAsync_t* m_q;

  public:
    MyObject() : m_a(0),m_b(0)
    {
      m_q = new myAsync_t(boost::bind(&MyObject::whenDone, this, _1, _2, _3));
      m_q->addCallback(boost::bind(&MyObject::cbA, this, _1, _2, _3));
      m_q->addCallback(boost::bind(&MyObject::cbB, this, _1, _2, _3));
    }

    void run()
    {
      m_q->run(&m_a, &m_b);
    }

    void cbA(myAsync_t& q, int* a, int* b)
    {
      std::cout << "cbA: " << *a << std::endl;
      ++(*a);
      q.doNextStep(a, b);
    }

    void cbB(myAsync_t& q, int* a, int* b)
    {
      std::cout << "cbB: " << *b << std::endl;
      ++(*b);
      q.doNextStep(a, b);
    }

    void whenDone(myAsync_t& q, int* a, int* b)
    {
      std::cout << "whenDone: " << *a << "," << *b << std::endl;
    }
};

int main(int argc, char** argv)
{
  MyObject o;
  o.run();
  o.run();
  o.run();
  o.run();
  o.run();
}
