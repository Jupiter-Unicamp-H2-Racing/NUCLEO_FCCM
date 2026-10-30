#include "state.h"
#include "context.h"
#include "contactor.h"
#include "valve.h"
#include "auto_control.h"
#include "uart.h"
#include "button.h"
#include "main.h"

/* --- VARIÁVEIS EXTERNAS DE FORÇA MANUAL (declaradas em main.c) --- */
extern uint8_t  ForceSupplyActive;
extern uint8_t  ForcePurgeActive;
extern uint32_t ForceSupplyOpenTime;
extern uint32_t ForcePurgeOpenTime;
extern int      Start;
extern int      Delay_Restart;

/* --- BOTÃO ECM: nível alto liga, nível baixo desliga --- */
void StartEndECU(void)
{
    GPIO_PinState ecm = HAL_GPIO_ReadPin(ECM_GPIO_Port, ECM_Pin);

    // Rising: partida do sistema
    if (ecm == GPIO_PIN_SET && Delay_Restart && !Start && FcActualState != FC_ALARM)
    {
        FcActualState = FC_STARTUP_FANSPOOLUP;
        Start         = 1;
        Entry         = 1;
    }

    // Falling: desligamento do sistema
    if (ecm == GPIO_PIN_RESET && Start && FcActualState != FC_ALARM)
    {
        Entry         = 1;
        FcActualState = FC_SHUTDOWN;
        Start         = 0;
        Delay_Restart = 0;
    }
}

/* --- FORÇA SUPRIMENTO (abre ao apertar, fecha após 500 ms) --- */
void ForceSupply(void)
{
    if (HAL_GPIO_ReadPin(FORCE_SUPPLY_GPIO_Port, FORCE_SUPPLY_Pin) == GPIO_PIN_SET && !ForceSupplyActive)
    {
        SupplyValve_CMD(OPEN);
        ForceSupplyActive   = 1;
        ForceSupplyOpenTime = Time;
    }

    if (ForceSupplyActive && (Time - ForceSupplyOpenTime >= 500))
    {
        SupplyValve_CMD(CLOSED);
        ForceSupplyActive = 0;
        println("SUPPLY SUPPLY SUPPLY SUPPLY SUPPLY SUPPLY SUPPLY SUPPLY SUPPLY");
    }
}

/* --- FORÇA PURGA (abre ao apertar, fecha após 500 ms) --- */
void ForcePurge(void)
{
    if (HAL_GPIO_ReadPin(FORCE_PURGE_GPIO_Port, FORCE_PURGE_Pin) == GPIO_PIN_SET && !ForcePurgeActive)
    {
        PurgeValve_CMD(OPEN);
        ForcePurgeActive   = 1;
        ForcePurgeOpenTime = Time;
    }

    if (ForcePurgeActive && (Time - ForcePurgeOpenTime >= 500))
    {
        PurgeValve_CMD(CLOSED);
        ForcePurgeActive = 0;
        println("PURGE PURGE PURGE PURGE PURGE PURGE PURGE PURGE PURGE ");
    }
}

/* --- MODO AIR STARVE (desliga fan, abre suprimento, fecha purga) --- */
void Air_Starve(void)
{
    Fan_CMD(FAN_MIN_CMD);
    SupplyValve_CMD(OPEN);
    PurgeValve_CMD(CLOSED);
    MainContactor_CMD(OPEN);
    ResistorContactor_CMD(CLOSED);
    HAL_Delay(10000);
}
