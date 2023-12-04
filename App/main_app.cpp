#include "main.h"
#include "tim.h"
#include "iwdg.h"
#include "main_controller.hpp"
#include "IO/input_signal.hpp"
#include "app_config.hpp"

extern "C"
{
//    void HAL_IncTick() {
//        uwTick += uwTickFreq;
//        MainController::GetRef().SysTickTimersTickHandler();
//    }

    void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
    {
        if(htim->Instance == TIM1){
            HAL_IWDG_Refresh(&hiwdg);
            MainController::GetRef().BoardUpdate();
        }
        if(htim->Instance == TIM6){
            HAL_TIM_Base_Stop_IT(htim);
            MainController::GetRef().TimTaskHandler();
        }
        if(htim->Instance == TIM7){
            MainController::GetRef().UpdateSignals();
        }
        if(htim->Instance == TIM17){
            MainController::GetRef().UpdateConfig();
        }
    }

    void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
    {
        if(htim->Instance == TIM15){
            MotorController::GetRef().MotorRefresh();
        }
    }

    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {
    }

    void EXTI_clear_enable(){
        __HAL_GPIO_EXTI_CLEAR_IT(BTN_IN_Pin);
        NVIC_ClearPendingIRQ(EXTI2_IRQn);
        HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    }

    void TIM_IT_clear_(){
        __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE); // app tim
        __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE); // pwm gen1
        __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // pwm gen2
        __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE); // pww capture
        __HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE); // main controller task tim
        __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE); // button tim
        __HAL_TIM_CLEAR_IT(&htim15, TIM_IT_UPDATE); // motor step pwm
        __HAL_TIM_CLEAR_IT(&htim17, TIM_IT_UPDATE); // dip config listener
    }

    void AppInit(){
//        EXTI_clear_enable();
        TIM_IT_clear_();
        HAL_TIM_Base_Start_IT(&htim1); // app tim
        HAL_TIM_Base_Start_IT(&htim7); // signals & btns tim
        HAL_TIM_Base_Start_IT(&htim17); // dip config listener
        MainController::GetRef().BoardInit();
    }

    void AppLoop()
    {
        MainController::GetRef().ProcessTimTasks();
    }
}