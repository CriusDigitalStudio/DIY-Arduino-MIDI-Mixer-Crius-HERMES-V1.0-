#include <Control_Surface.h>

USBMIDI_Interface midi;

CD74HC4051 mux1 {
  A2,       // Analog input pin
  {5, 8, 10} // Address pins S0, S1, S2
};

CD74HC4051 mux2 {
  A3,       // Analog input pin
  {4, 3, 2} // Address pins S0, S1, S2
};

Bank<2> bank1(4);
//   │       └───── number of tracks per bank
//   └───────────── number of banks

Bank<2> bank2(8);

// Instantiate a Bank selector to control which one of the four Banks is active.
IncrementDecrementSelector<2> selector1 {
  bank1,       // Bank to manage
  {7, 9},      // push button pins (increment, decrement)
  Wrap::Clamp, // Wrap around
};

IncrementDecrementSelector<2> selector2 {
  bank2,       // Bank to manage
  {7, 9},      // push button pins (increment, decrement)
  Wrap::Clamp, // Wrap around
};

Bankable::CCPotentiometer volumePotentiometers[] {
  { bank2, mux1.pin(3), { 1, CHANNEL_1 } },
  { bank2, mux1.pin(0), { 2, CHANNEL_1 } },
  { bank2, mux1.pin(1), { 3, CHANNEL_1 } },
  { bank2, mux1.pin(2), { 4, CHANNEL_1 } },
  { bank2, mux1.pin(4), { 5, CHANNEL_1 } },
  { bank2, mux1.pin(6), { 6, CHANNEL_1 } },
  { bank2, mux1.pin(7), { 7, CHANNEL_1 } },
  { bank2, mux1.pin(5), { 8, CHANNEL_1 } },
};

Bankable::CCButton buttons[] {
  { bank2, mux2.pin(6), { 50, CHANNEL_1 } },
  { bank2, mux2.pin(0), { 51, CHANNEL_1 } },
  { bank2, mux2.pin(4), { 52, CHANNEL_1 } },
  { bank2, mux2.pin(2), { 53, CHANNEL_1 } },
  { bank2, mux2.pin(1), { 54, CHANNEL_1 } },
  { bank2, mux2.pin(3), { 55, CHANNEL_1 } },
  { bank2, mux2.pin(7), { 56, CHANNEL_1 } },
  { bank2, mux2.pin(5), { 57, CHANNEL_1 } },
};

// Instantiate two daisy-chained shift register with the SPI slave select pin as
// latch pin, most significant bit first, and a total of 16 outputs.
SPIShiftRegisterOut<32> sreg {SPI, SS, MSBFIRST};


// Instantiate a VULEDs object with 12 LEDs.
MCU::Bankable::VULEDs < 2, 8 > vu1 {
  {bank1, BankType::CHANGE_ADDRESS},
  sreg.pins().slice<0, 7>(), // first 12 pins of the shift registers
  1,                          // track number [1, 8]
  MCU::VUDecay::Default,      // how long does it take for the meter to decay
};

MCU::Bankable::VULEDs < 2, 8 > vu2 {
  {bank1, BankType::CHANGE_ADDRESS},
  sreg.pins().slice<8, 15>(), // first 12 pins of the shift registers
  2,                          // track number [1, 8]
  MCU::VUDecay::Default,      // how long does it take for the meter to decay
};

MCU::Bankable::VULEDs < 2, 8 > vu3 {
  {bank1, BankType::CHANGE_ADDRESS},
  sreg.pins().slice<16, 23>(), // first 12 pins of the shift registers
  3,                          // track number [1, 8]
  MCU::VUDecay::Default,      // how long does it take for the meter to decay
};

MCU::Bankable::VULEDs < 2, 8 > vu4 {
  {bank1, BankType::CHANGE_ADDRESS},
  sreg.pins().slice<24, 31>(), // first 12 pins of the shift registers
  4,                          // track number [1, 8]
  MCU::VUDecay::Default,      // how long does it take for the meter to decay
};

// Push button to switch between dot and bar mode.
Button dotBarModeButton = 6;

void setup() {
  Control_Surface.begin();
  dotBarModeButton.begin();  
}

void loop() {
  Control_Surface.loop();
  if (dotBarModeButton.update() == Button::Falling) {   // when button is pressed
    vu1.toggleMode();
    vu2.toggleMode();
    vu3.toggleMode();
    vu4.toggleMode();                                   // toggle between dot and bar mode
    }
//delay(5);
}
