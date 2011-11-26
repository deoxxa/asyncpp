#include <deque>
#include <boost/function.hpp>

template <typename ...Args>
class AsyncPP
{
  public:
    typedef boost::function< void (AsyncPP< Args... >&, Args...) > callback_t;
    typedef std::deque< callback_t > callbackQueue_t;

  private:
    callbackQueue_t m_callbackQueue;
    typename callbackQueue_t::iterator m_currentStep;

  public:
    AsyncPP(callback_t whenDone)
    {
      m_callbackQueue.push_back(whenDone);
    }

    void addCallback(callback_t callback)
    {
      m_callbackQueue.insert((m_callbackQueue.end())-1, callback);
    }

    void run(Args... args)
    {
      m_currentStep = m_callbackQueue.begin();
      doNextStep(args...);
    }

    void doNextStep(Args... args)
    {
      (*m_currentStep++)(*this, args...);
    }
};
