#include "TivaQEI.h"

Encoder::Encoder() {
}

void Encoder::init_encoder(uint32_t ui32Base) {

    this->ui32Base = ui32Base;

    /*
    //Unlock GPIOD7 - Like PF0 its used for NMI - Without this step it doesn't work
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY; //In Tiva include this is the same as "_DD" in older versions (0x4C4F434B)
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
    */

    // setup gpio pins for qei
    if(QEI0_BASE == ui32Base) {

      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);

      GPIOPinConfigure(GPIO_PD6_PHA0);
      GPIOPinConfigure(GPIO_PD7_PHB0);

      GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    } else if(QEI1_BASE == ui32Base) {

      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);

      GPIOPinConfigure(GPIO_PC5_PHA1);
      GPIOPinConfigure(GPIO_PC6_PHB1);

      GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);

    }

    // disable before config
    QEIDisable(ui32Base);
    QEIIntDisable(ui32Base, QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);

    // quadrature encoder config
    if(QEI0_BASE == ui32Base) {
      QEIConfigure(ui32Base, (QEI_CONFIG_CAPTURE_A | QEI_CONFIG_NO_RESET | QEI_CONFIG_CLOCK_DIR | QEI_CONFIG_NO_SWAP), 4294967295);      
    } else if(QEI1_BASE == ui32Base) {
      QEIConfigure(ui32Base, (QEI_CONFIG_CAPTURE_A | QEI_CONFIG_NO_RESET | QEI_CONFIG_CLOCK_DIR | QEI_CONFIG_NO_SWAP), 4294967295);     
    }        
    
    // enable encoder
    QEIEnable(ui32Base);

    // disable before config
    QEIVelocityDisable(ui32Base);

    // velocity config
    QEIVelocityConfigure(ui32Base, QEI_VELDIV_1, period);

    // enable velocity
    QEIVelocityEnable(ui32Base);

    // set initial position
    QEIPositionSet(ui32Base, 0);


}

void Encoder::set_encoder_position(uint16_t position) {
  QEIPositionSet(ui32Base, position);
}

uint32_t Encoder::get_velocity() {
  return QEIVelocityGet(ui32Base);
}

uint32_t Encoder::get_position() {
  return QEIPositionGet(ui32Base);
}

int32_t Encoder::get_direction() {
  return QEIDirectionGet(ui32Base);
}
