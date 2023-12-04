
#include "app_timer.hpp"

AppTimer::AppTimer(AppTimer::HandlerT handler, uint32_t msDelay, bool one_shot = false)
    :handler_(std::move(handler)),
     interval_(msDelay),
     one_shot_(one_shot)
{
    if(!one_shot_)
        StartTimer();


}

void AppTimer::UpdateState() {
    if(count_ >= interval_){
        count_ = 0;
        pending_ = true;
    }
}

void AppTimer::IsPending() const{
    return pending_;
}

void AppTimer::ProcessTask(){
    if(pending_) {
        pending_ = false;
        if(one_shot_) StopTimer();
        handler_();
    }
}

void AppTimer::TickHandle() {
    if(disabled_)
        return;
    count_ += KTick_freq_;
    UpdateState();
}

void AppTimer::SetMSDelay(uint32_t delay) {
    interval_ = delay;
}

void AppTimer::StopTimer() {
    disabled_ = true;
}

void AppTimer::StartTimer() {
    count_ = 0;
    disabled_ = false;
}

