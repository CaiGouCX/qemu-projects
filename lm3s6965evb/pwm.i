# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
# 32 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/inc/hw_ints.h" 1
# 33 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c" 2
# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/inc/hw_memmap.h" 1
# 34 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c" 2
# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/inc/hw_pwm.h" 1
# 35 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c" 2
# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/inc/hw_sysctl.h" 1
# 36 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c" 2
# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/inc/hw_types.h" 1
# 33 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/inc/hw_types.h"
typedef unsigned char tBoolean;
# 37 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c" 2
# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/debug.h" 1
# 34 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/debug.h"
extern void __error__(char *pcFilename, unsigned long ulLine);
# 38 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c" 2
# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/interrupt.h" 1
# 52 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/interrupt.h"
extern tBoolean IntMasterEnable(void);
extern tBoolean IntMasterDisable(void);
extern void IntRegister(unsigned long ulInterrupt, void (*pfnHandler)(void));
extern void IntUnregister(unsigned long ulInterrupt);
extern void IntPriorityGroupingSet(unsigned long ulBits);
extern unsigned long IntPriorityGroupingGet(void);
extern void IntPrioritySet(unsigned long ulInterrupt,
                           unsigned char ucPriority);
extern long IntPriorityGet(unsigned long ulInterrupt);
extern void IntEnable(unsigned long ulInterrupt);
extern void IntDisable(unsigned long ulInterrupt);
extern void IntPendSet(unsigned long ulInterrupt);
extern void IntPendClear(unsigned long ulInterrupt);
extern void IntPriorityMaskSet(unsigned long ulPriorityMask);
extern unsigned long IntPriorityMaskGet(void);
# 39 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c" 2
# 1 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.h" 1
# 212 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.h"
extern void PWMGenConfigure(unsigned long ulBase, unsigned long ulGen,
                            unsigned long ulConfig);
extern void PWMGenPeriodSet(unsigned long ulBase, unsigned long ulGen,
                            unsigned long ulPeriod);
extern unsigned long PWMGenPeriodGet(unsigned long ulBase,
                                     unsigned long ulGen);
extern void PWMGenEnable(unsigned long ulBase, unsigned long ulGen);
extern void PWMGenDisable(unsigned long ulBase, unsigned long ulGen);
extern void PWMPulseWidthSet(unsigned long ulBase, unsigned long ulPWMOut,
                             unsigned long ulWidth);
extern unsigned long PWMPulseWidthGet(unsigned long ulBase,
                                      unsigned long ulPWMOut);
extern void PWMDeadBandEnable(unsigned long ulBase, unsigned long ulGen,
                              unsigned short usRise, unsigned short usFall);
extern void PWMDeadBandDisable(unsigned long ulBase, unsigned long ulGen);
extern void PWMSyncUpdate(unsigned long ulBase, unsigned long ulGenBits);
extern void PWMSyncTimeBase(unsigned long ulBase, unsigned long ulGenBits);
extern void PWMOutputState(unsigned long ulBase, unsigned long ulPWMOutBits,
                           tBoolean bEnable);
extern void PWMOutputInvert(unsigned long ulBase, unsigned long ulPWMOutBits,
                            tBoolean bInvert);
extern void PWMOutputFaultLevel(unsigned long ulBase,
                                unsigned long ulPWMOutBits,
                                tBoolean bDriveHigh);
extern void PWMOutputFault(unsigned long ulBase, unsigned long ulPWMOutBits,
                           tBoolean bFaultSuppress);
extern void PWMGenIntRegister(unsigned long ulBase, unsigned long ulGen,
                              void (*pfnIntHandler)(void));
extern void PWMGenIntUnregister(unsigned long ulBase, unsigned long ulGen);
extern void PWMFaultIntRegister(unsigned long ulBase,
                                void (*pfnIntHandler)(void));
extern void PWMFaultIntUnregister(unsigned long ulBase);
extern void PWMGenIntTrigEnable(unsigned long ulBase, unsigned long ulGen,
                                unsigned long ulIntTrig);
extern void PWMGenIntTrigDisable(unsigned long ulBase, unsigned long ulGen,
                                 unsigned long ulIntTrig);
extern unsigned long PWMGenIntStatus(unsigned long ulBase, unsigned long ulGen,
                                     tBoolean bMasked);
extern void PWMGenIntClear(unsigned long ulBase, unsigned long ulGen,
                           unsigned long ulInts);
extern void PWMIntEnable(unsigned long ulBase, unsigned long ulGenFault);
extern void PWMIntDisable(unsigned long ulBase, unsigned long ulGenFault);
extern void PWMFaultIntClear(unsigned long ulBase);
extern unsigned long PWMIntStatus(unsigned long ulBase, tBoolean bMasked);
extern void PWMFaultIntClearExt(unsigned long ulBase,
                                unsigned long ulFaultInts);
extern void PWMGenFaultConfigure(unsigned long ulBase, unsigned long ulGen,
                                 unsigned long ulMinFaultPeriod,
                                 unsigned long ulFaultSenses);
extern void PWMGenFaultTriggerSet(unsigned long ulBase, unsigned long ulGen,
                                  unsigned long ulGroup,
                                  unsigned long ulFaultTriggers);
extern unsigned long PWMGenFaultTriggerGet(unsigned long ulBase,
                                           unsigned long ulGen,
                                           unsigned long ulGroup);
extern unsigned long PWMGenFaultStatus(unsigned long ulBase,
                                       unsigned long ulGen,
                                       unsigned long ulGroup);
extern void PWMGenFaultClear(unsigned long ulBase, unsigned long ulGen,
                             unsigned long ulGroup,
                             unsigned long ulFaultTriggers);
# 40 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c" 2
# 182 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenConfigure(unsigned long ulBase, unsigned long ulGen,
                unsigned long ulConfig)
{



    ;
    ;




    ulGen = ((ulBase) + (ulGen));




    (*((volatile unsigned long *)(ulGen + 0x00000000))) = (((*((volatile unsigned long *)(ulGen + 0x00000000))) &
                                   ~(0x00000002 | 0x00000004 |
                                     0x00040000 | 0x00020000 |
                                     0x00010000 | 0x0000C000 |
                                     0x00003000 |
                                     0x00000C00 |
                                     0x00000300 |
                                     0x000000C0 |
                                     0x00000008 | 0x00000010 |
                                     0x00000020)) | ulConfig);




    if(ulConfig & 0x00000002)
    {





        (*((volatile unsigned long *)(ulGen + 0x00000020))) = (0x00000030 |
                                       0x00000080);
        (*((volatile unsigned long *)(ulGen + 0x00000024))) = (0x00000300 |
                                       0x00000800);
    }
    else
    {




        (*((volatile unsigned long *)(ulGen + 0x00000020))) = (0x0000000C |
                                       0x00000080);
        (*((volatile unsigned long *)(ulGen + 0x00000024))) = (0x0000000C |
                                       0x00000800);
    }
}
# 259 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenPeriodSet(unsigned long ulBase, unsigned long ulGen,
                unsigned long ulPeriod)
{



    ;
    ;




    ulGen = ((ulBase) + (ulGen));




    if((*((volatile unsigned long *)(ulGen + 0x00000000))) & 0x00000002)
    {




        ;
        (*((volatile unsigned long *)(ulGen + 0x00000010))) = ulPeriod / 2;
    }
    else
    {




        ;
        (*((volatile unsigned long *)(ulGen + 0x00000010))) = ulPeriod - 1;
    }
}
# 317 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
unsigned long
PWMGenPeriodGet(unsigned long ulBase, unsigned long ulGen)
{



    ;
    ;




    ulGen = ((ulBase) + (ulGen));




    if((*((volatile unsigned long *)(ulGen + 0x00000000))) & 0x00000002)
    {



        return((*((volatile unsigned long *)(ulGen + 0x00000010))) * 2);
    }
    else
    {



        return((*((volatile unsigned long *)(ulGen + 0x00000010))) + 1);
    }
}
# 364 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenEnable(unsigned long ulBase, unsigned long ulGen)
{



    ;
    ;




    (*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x00000000))) |= 0x00000001;
}
# 393 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenDisable(unsigned long ulBase, unsigned long ulGen)
{



    ;
    ;




    (*((volatile unsigned long *)(((ulBase) + (+ ulGen)) + 0x00000000))) &= ~(0x00000001);
}
# 427 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMPulseWidthSet(unsigned long ulBase, unsigned long ulPWMOut,
                 unsigned long ulWidth)
{
    unsigned long ulGenBase, ulReg;




    ;
    ;




    ulGenBase = ((ulBase) + ((ulPWMOut) & 0xFFFFFFC0));




    if((*((volatile unsigned long *)(ulGenBase + 0x00000000))) & 0x00000002)
    {
        ulWidth /= 2;
    }




    ulReg = (*((volatile unsigned long *)(ulGenBase + 0x00000010)));




    ;




    ulReg = ulReg - ulWidth;




    if(((ulPWMOut) & 0x00000001))
    {
        (*((volatile unsigned long *)(ulGenBase + 0x0000001C))) = ulReg;
    }
    else
    {
        (*((volatile unsigned long *)(ulGenBase + 0x00000018))) = ulReg;
    }
}
# 498 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
unsigned long
PWMPulseWidthGet(unsigned long ulBase, unsigned long ulPWMOut)
{
    unsigned long ulGenBase, ulReg, ulLoad;




    ;
    ;




    ulGenBase = ((ulBase) + ((ulPWMOut) & 0xFFFFFFC0));





    ulLoad = (*((volatile unsigned long *)(ulGenBase + 0x00000010)));
    if(((ulPWMOut) & 0x00000001))
    {
        ulReg = (*((volatile unsigned long *)(ulGenBase + 0x0000001C)));
    }
    else
    {
        ulReg = (*((volatile unsigned long *)(ulGenBase + 0x00000018)));
    }
    ulReg = ulLoad - ulReg;




    if((*((volatile unsigned long *)(ulGenBase + 0x00000000))) & 0x00000002)
    {
        ulReg = ulReg * 2;
    }




    return(ulReg);
}
# 561 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMDeadBandEnable(unsigned long ulBase, unsigned long ulGen,
                  unsigned short usRise, unsigned short usFall)
{



    ;
    ;
    ;
    ;




    ulGen = ((ulBase) + (ulGen));




    (*((volatile unsigned long *)(ulGen + 0x0000002C))) = usRise;
    (*((volatile unsigned long *)(ulGen + 0x00000030))) = usFall;




    (*((volatile unsigned long *)(ulGen + 0x00000028))) |= 0x00000001;
}
# 604 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMDeadBandDisable(unsigned long ulBase, unsigned long ulGen)
{



    ;
    ;




    (*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x00000028))) &=
        ~(0x00000001);
}
# 636 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMSyncUpdate(unsigned long ulBase, unsigned long ulGenBits)
{



    ;
   
                                           ;




    (*((volatile unsigned long *)(ulBase + 0x00000000))) = ulGenBits;
}
# 668 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMSyncTimeBase(unsigned long ulBase, unsigned long ulGenBits)
{



    ;
   
                                           ;





    (*((volatile unsigned long *)(ulBase + 0x00000004))) = ulGenBits;
}
# 706 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMOutputState(unsigned long ulBase, unsigned long ulPWMOutBits,
               tBoolean bEnable)
{



    ;
   

                                                              ;





    if(bEnable == 1)
    {
        (*((volatile unsigned long *)(ulBase + 0x00000008))) |= ulPWMOutBits;
    }
    else
    {
        (*((volatile unsigned long *)(ulBase + 0x00000008))) &= ~(ulPWMOutBits);
    }
}
# 754 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMOutputInvert(unsigned long ulBase, unsigned long ulPWMOutBits,
                tBoolean bInvert)
{



    ;
   

                                                              ;





    if(bInvert == 1)
    {
        (*((volatile unsigned long *)(ulBase + 0x0000000C))) |= ulPWMOutBits;
    }
    else
    {
        (*((volatile unsigned long *)(ulBase + 0x0000000C))) &= ~(ulPWMOutBits);
    }
}
# 810 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMOutputFaultLevel(unsigned long ulBase, unsigned long ulPWMOutBits,
                    tBoolean bDriveHigh)
{



    ;
   

                                                              ;





    if(bDriveHigh == 1)
    {
        (*((volatile unsigned long *)(ulBase + 0x00000024))) |= ulPWMOutBits;
    }
    else
    {
        (*((volatile unsigned long *)(ulBase + 0x00000024))) &= ~(ulPWMOutBits);
    }
}
# 863 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMOutputFault(unsigned long ulBase, unsigned long ulPWMOutBits,
               tBoolean bFaultSuppress)
{



    ;
   

                                                              ;





    if(bFaultSuppress == 1)
    {
        (*((volatile unsigned long *)(ulBase + 0x00000010))) |= ulPWMOutBits;
    }
    else
    {
        (*((volatile unsigned long *)(ulBase + 0x00000010))) &= ~(ulPWMOutBits);
    }
}
# 903 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
static unsigned long
PWMGenIntGet(unsigned long ulBase, unsigned long ulGen)
{



    switch(ulBase + ulGen)
    {



        case 0x40028000 + 0x00000040:
        {
            return(26);
        }




        case 0x40028000 + 0x00000080:
        {
            return(27);
        }




        case 0x40028000 + 0x000000C0:
        {
            return(28);
        }




        case 0x40028000 + 0x00000100:
        {
            return(61);
        }




        case 0x40029000 + 0x00000040:
        {
            return(150);
        }




        case 0x40029000 + 0x00000080:
        {
            return(151);
        }




        case 0x40029000 + 0x000000C0:
        {
            return(152);
        }




        case 0x40029000 + 0x00000100:
        {
            return(153);
        }




        default:
        {
            return(0);
        }
    }
}
# 1008 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenIntRegister(unsigned long ulBase, unsigned long ulGen,
                  void (*pfnIntHandler)(void))
{
    unsigned long ulInt;




    ;
    ;




    ulInt = PWMGenIntGet(ulBase, ulGen);




    IntRegister(ulInt, pfnIntHandler);




    IntEnable(ulInt);
}
# 1056 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenIntUnregister(unsigned long ulBase, unsigned long ulGen)
{
    unsigned long ulInt;




    ;
    ;




    ulInt = PWMGenIntGet(ulBase, ulGen);




    IntDisable(ulInt);




    IntUnregister(ulInt);
}
# 1095 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
static unsigned long
PWMFaultIntGet(unsigned long ulBase)
{



    return((ulBase == 0x40028000) ? 25 : 154);
}
# 1125 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMFaultIntRegister(unsigned long ulBase, void (*pfnIntHandler)(void))
{
    unsigned long ulInt;




    ;




    ulInt = PWMFaultIntGet(ulBase);




    IntRegister(ulInt, pfnIntHandler);




    IntEnable(ulInt);
}
# 1168 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMFaultIntUnregister(unsigned long ulBase)
{
    unsigned long ulInt;




    ;




    ulInt = PWMFaultIntGet(ulBase);




    IntDisable(ulInt);




    IntUnregister(ulInt);
}
# 1215 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenIntTrigEnable(unsigned long ulBase, unsigned long ulGen,
                    unsigned long ulIntTrig)
{



    ;
    ;
   



                                               ;




    (*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x00000004))) |= ulIntTrig;
}
# 1257 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenIntTrigDisable(unsigned long ulBase, unsigned long ulGen,
                     unsigned long ulIntTrig)
{



    ;
    ;
   



                                               ;




    (*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x00000004))) &= ~(ulIntTrig);
}
# 1296 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
unsigned long
PWMGenIntStatus(unsigned long ulBase, unsigned long ulGen, tBoolean bMasked)
{



    ;
    ;




    ulGen = ((ulBase) + (ulGen));





    if(bMasked == 1)
    {
        return((*((volatile unsigned long *)(ulGen + 0x0000000C))));
    }
    else
    {
        return((*((volatile unsigned long *)(ulGen + 0x00000008))));
    }
}
# 1351 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenIntClear(unsigned long ulBase, unsigned long ulGen, unsigned long ulInts)
{



    ;
    ;
   

             ;





    (*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x0000000C))) = ulInts;
}
# 1386 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMIntEnable(unsigned long ulBase, unsigned long ulGenFault)
{



    ;
   

                                                                  ;





    (*((volatile unsigned long *)(ulBase + 0x00000014))) |= ulGenFault;
}
# 1420 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMIntDisable(unsigned long ulBase, unsigned long ulGenFault)
{



    ;
   

                                                                  ;





    (*((volatile unsigned long *)(ulBase + 0x00000014))) &= ~(ulGenFault);
}
# 1464 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMFaultIntClear(unsigned long ulBase)
{



    ;




    (*((volatile unsigned long *)(ulBase + 0x0000001C))) = 0x00010000;
}
# 1495 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
unsigned long
PWMIntStatus(unsigned long ulBase, tBoolean bMasked)
{



    ;




    if(bMasked == 1)
    {
        return((*((volatile unsigned long *)(ulBase + 0x0000001C))));
    }
    else
    {
        return((*((volatile unsigned long *)(ulBase + 0x00000018))));
    }
}
# 1548 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMFaultIntClearExt(unsigned long ulBase, unsigned long ulFaultInts)
{



    ;
   
                                                                   ;




    (*((volatile unsigned long *)(ulBase + 0x0000001C))) = ulFaultInts;
}
# 1593 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenFaultConfigure(unsigned long ulBase, unsigned long ulGen,
                     unsigned long ulMinFaultPeriod,
                     unsigned long ulFaultSenses)
{



    ;
    ;
    ;
   



             ;




    (*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x0000003C))) = ulMinFaultPeriod;




    (*((volatile unsigned long *)(((ulBase) + 0x00000800 + ((ulGen) - 0x00000040) * 2) + 0x00000000))) = ulFaultSenses;
}
# 1659 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenFaultTriggerSet(unsigned long ulBase, unsigned long ulGen,
                      unsigned long ulGroup, unsigned long ulFaultTriggers)
{



    ;
    ;
    ;
   

                                                                             ;
   



                                                                           ;




    if(ulGroup == 0)
    {
        (*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x00000034))) =
            ulFaultTriggers;
    }
    else
    {
        (*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x00000038))) =
            ulFaultTriggers;
    }
}
# 1720 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
unsigned long
PWMGenFaultTriggerGet(unsigned long ulBase, unsigned long ulGen,
                      unsigned long ulGroup)
{



    ;
    ;
    ;




    if(ulGroup == 0)
    {
        return((*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x00000034))));
    }
    else
    {
        return((*((volatile unsigned long *)(((ulBase) + (ulGen)) + 0x00000038))));
    }
}
# 1778 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
unsigned long
PWMGenFaultStatus(unsigned long ulBase, unsigned long ulGen,
                  unsigned long ulGroup)
{



    ;
    ;
    ;




    if(ulGroup == 0)
    {
        return((*((volatile unsigned long *)(((ulBase) + 0x00000800 + ((ulGen) - 0x00000040) * 2) + 0x00000004))));
    }
    else
    {
        return((*((volatile unsigned long *)(((ulBase) + 0x00000800 + ((ulGen) - 0x00000040) * 2) + 0x00000008))));
    }
}
# 1826 "/home/chengxin/qemu-projects/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib/pwm.c"
void
PWMGenFaultClear(unsigned long ulBase, unsigned long ulGen,
                 unsigned long ulGroup, unsigned long ulFaultTriggers)
{



    ;
    ;
    ;
   

                                                                             ;
   



                                                                           ;




    if(ulGroup == 0)
    {
        (*((volatile unsigned long *)(((ulBase) + 0x00000800 + ((ulGen) - 0x00000040) * 2) + 0x00000004))) =
            ulFaultTriggers;
    }
    else
    {
        (*((volatile unsigned long *)(((ulBase) + 0x00000800 + ((ulGen) - 0x00000040) * 2) + 0x00000008))) =
            ulFaultTriggers;
    }
}
