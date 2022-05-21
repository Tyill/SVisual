#include <atomic>
#include <thread>
#include <algorithm>


namespace SV_Aux {

class SpinLock{
public:
    SpinLock() = default;
    
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

    enum Action{ READ, WRITE };

    void lock(Action act){
        bool isBlock = false;
        while (!m_readLock.compare_exchange_weak(isBlock, true)){
            isBlock = false;
            std::this_thread::yield();
        }     

        isBlock = (act == WRITE) && (m_readCount.load() > 0);
        while (!m_writeLock.compare_exchange_weak(isBlock, act == WRITE ? true : false)){
            isBlock = (act == WRITE) && (m_readCount.load() > 0);
        }

        if (act == READ){
            ++m_readCount;
        }
        
        m_readLock.store(false);
    }

    void unlock(Action act){
        if (act == READ){
            --m_readCount;
        }else{
            m_writeLock.store(false);
        }
    }

private:
    std::atomic<int> m_readCount{0};
    std::atomic<bool> m_readLock{false};
    std::atomic<bool> m_writeLock{false};
};

class Locker {
public:
    Locker(SpinLock& lock, SpinLock::Action act) :
        m_lock(lock),
        m_act(act) {
        m_lock.lock(act);
    }
    ~Locker() {
        m_lock.unlock(m_act);
    }

private:
    SpinLock& m_lock;
    SpinLock::Action m_act;
};
}