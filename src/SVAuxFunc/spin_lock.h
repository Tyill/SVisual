#include <atomic>
#include <thread>
#include <algorithm>

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

        isBlock = (m_readCount.load() > 0) && (act == WRITE);
        while (!m_writeLock.compare_exchange_weak(isBlock, act == WRITE ? true : false)){
            isBlock = (m_readCount.load() > 0) && (act == WRITE);
            std::this_thread::yield();
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
    std::atomic<int> m_readCount = 0;
    std::atomic<bool> m_readLock = false;
    std::atomic<bool> m_writeLock = false;
};