#include <atomic>

class SpinLock{
public:
    SpinLock() = default;
    
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

    enum Action{ READ, WRITE }

    void lock(Action act){
        bool expected = false;
        while (!m_writerLock.compare_exchange_weak(expected, act == WRITE ? true : false)){
            expected = false;
            std::this_thread::yield();
        }  
        if (act == READ){
            ++m_readerCount;  
        }else{
            while (m_readerCount > 0);
        }
    }

    void unlock(Action act){
        if (act == READ){
            m_readerCount = std::max(0, --m_readerCount);
        }else{
            m_writerLock.exchange(false);
        }
    }

private:
    std::atomic_int m_readerCount = 0;
    std::atomic_bool m_writerLock = false;
}