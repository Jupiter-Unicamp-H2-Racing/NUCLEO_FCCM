#include "print.h"
#include "context.h"
#include "uart.h"
#include "valve.h"
#include "main.h"
#include "auto_control.h"
#include "can.h"

/* --- HANDLES EXTERNOS (ADC / CAN) --- */
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern FDCAN_HandleTypeDef   hfdcan2;
extern FDCAN_TxHeaderTypeDef ERROR_CAN;

/* --- VARIÁVEIS LOCAIS --- */
int P_inteiro;
int P_decimal;
int V_inteiro;
int V_decimal;
int T_inteiro;
int T_decimal;
int C_inteiro;
int C_decimal;

/* --- IMPRESSÃO DOS SENSORES VIA UART --- */
void print_sensors(int print_sensor, int print_state)
{
    P_inteiro = (int)Pressure;
    P_decimal = (int)((Pressure - P_inteiro) * 100);
    if (P_decimal < 0)
    {
        P_decimal = 0;
    }
    V_inteiro = (int)Voltage;
    V_decimal = (int)((Voltage - V_inteiro) * 100);
    T_inteiro = (int)(Temp);
    T_decimal = (int)((Temp - T_inteiro) * 100);
    C_inteiro = (int)Current;
    C_decimal = (int)((Current - C_inteiro) * 100);

    print("Temp: %d.%02dC; ", T_inteiro, T_decimal);
    print("Current: %d.%02dA; ", C_inteiro, C_decimal);
    print("Voltage: %d.%02dV; ", V_inteiro, V_decimal);
    print("Pressure: %d.%02d; ", P_inteiro, P_decimal);
    print("FAN: %d%%; ", Fan_power);
    print("H2: %dg; ", (int)H2_consumed);
    print("A*s: %d; ", (int)amp_segundo);
    print("Purgas: %d; ", purgas);
    println("Estado: %d", (int)FcActualState);
}
