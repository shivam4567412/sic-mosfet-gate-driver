#include "F28x_Project.h"

#define TBPRD_VAL        4999
#define PULSE1_WIDTH     2500
#define PULSE2_WIDTH     120

typedef enum {
    DPT_IDLE = 0,
    DPT_PULSE1_ON,
    DPT_PULSE1_OFF,
    DPT_PULSE2_ON,
    DPT_DONE
} DPT_State;

volatile DPT_State dptState = DPT_IDLE;
volatile Uint32 tickCount = 0;

void InitEPWM1_DPT(void);
void GPIO_Init(void);
interrupt void epwm1_isr(void);

void main(void)
{
    InitSysCtrl();

    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    GPIO_Init();
    InitEPWM1_DPT();

    EALLOW;
    PieVectTable.EPWM1_INT = &epwm1_isr;
    EDIS;

    IER |= M_INT3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

    EINT;
    ERTM;

    EALLOW;
    EPwm1Regs.CMPA.bit.CMPA = PULSE1_WIDTH;
    EDIS;

    dptState = DPT_PULSE1_ON;
    tickCount = 0;

    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;

    while(1)
    {
        __asm(" NOP");
    }
}

interrupt void epwm1_isr(void)
{
    tickCount++;

    switch(dptState)
    {
        case DPT_PULSE1_ON:
            EALLOW;
            EPwm1Regs.CMPA.bit.CMPA = 0;
            EDIS;
            dptState = DPT_PULSE1_OFF;
            tickCount = 0;
            break;

        case DPT_PULSE1_OFF:
            if(tickCount >= 1)
            {
                EALLOW;
                EPwm1Regs.CMPA.bit.CMPA = PULSE2_WIDTH;
                EDIS;
                dptState = DPT_PULSE2_ON;
                tickCount = 0;
            }
            break;

        case DPT_PULSE2_ON:
            EALLOW;
            EPwm1Regs.CMPA.bit.CMPA = 0;
            EPwm1Regs.AQCTLA.bit.CAU = AQ_NO_ACTION;
            EPwm1Regs.AQCTLA.bit.ZRO = AQ_CLEAR;
            EPwm1Regs.AQSFRC.bit.ACTSFA = AQ_CLEAR;
            EPwm1Regs.AQSFRC.bit.OTSFA = 1;
            EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
            EDIS;
            dptState = DPT_DONE;
            break;

        case DPT_DONE:
        default:
            break;
    }

    EPwm1Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

void InitEPWM1_DPT(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 1;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    EPwm1Regs.TBCTL.bit.CTRMODE = TB_FREEZE;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
    EPwm1Regs.TBPRD = TBPRD_VAL;
    EPwm1Regs.TBCTR = 0;

    EPwm1Regs.CMPA.bit.CMPA = 0;
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;

    EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;

    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
    EPwm1Regs.ETSEL.bit.INTEN = 1;
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

void GPIO_Init(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    EDIS;
}
