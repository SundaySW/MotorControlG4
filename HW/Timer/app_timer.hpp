#pragma once

#include "stm32g4xx_hal.h"
#include "functional"

    /**
     * @brief To use:
     * impl weak HAL func:
     * void HAL_IncTick()
     * {
            uwTick += uwTickFreq;
            SysTickTimHandler();
        }
        ***in SysTickTimHandler() call TickHandle() for all your impl timers in loop

        void SysTickTimHandler(){
            for(auto & timer : timers_)
                timer->TickHandle();
        }

        ***Impl timer with lambda or functor in ctor
        AppTimer tim1_ {[this](){SomeFunc();}};

        std::array<AppTimer*, 2> timers_{
            &Tim1,
            &Tim2
        };
     */

class AppTimer {
public:
    using HandlerT = std::function<void()>;

    AppTimer() = delete;
    explicit AppTimer(HandlerT, uint32_t msDelay, bool one_shot = false);
    void TickHandle();
    void SetMSDelay(uint32_t delay);
    void StopTimer();
    void StartTimer();
    void IsPending() const;
    void ProcessTask();

private:
    __IO uint32_t count_{0};
    __IO uint32_t interval_{0};
    const uint32_t KTick_freq_ = HAL_TICK_FREQ_DEFAULT;
    bool one_shot_ {false};
    bool disabled_ {true};
    bool pending_ {false};
    const HandlerT handler_;
    void UpdateState();
};