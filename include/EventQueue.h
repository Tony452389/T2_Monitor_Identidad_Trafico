#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "Evento.h"

class EventQueue
{
private:
    std::queue<Evento> cola;
    std::mutex mtx;
    std::condition_variable cv;

public:

    void push(const Evento& e)
    {
        std::lock_guard<std::mutex> lock(mtx);
        cola.push(e);
        cv.notify_one();
    }

    Evento pop()
    {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this] { return !cola.empty(); });

        Evento e = cola.front();
        cola.pop();

        return e;
    }
};