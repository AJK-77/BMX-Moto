#include "Handheld/Input/TCA8418.h"
#include "Handheld/Pins.h"

#include <Wire.h>


// ============================================================
// TCA8418 registers
// ============================================================

constexpr uint8_t REG_CONFIG       = 0x01;
constexpr uint8_t REG_INT_STAT     = 0x02;
constexpr uint8_t REG_KEY_LCK_EC   = 0x03;
constexpr uint8_t REG_KEY_EVENT_A  = 0x04;

constexpr uint8_t REG_GPIO_INT_EN1 = 0x1A;
constexpr uint8_t REG_GPIO_INT_EN2 = 0x1B;
constexpr uint8_t REG_GPIO_INT_EN3 = 0x1C;

constexpr uint8_t REG_KP_GPIO1     = 0x1D;
constexpr uint8_t REG_KP_GPIO2     = 0x1E;
constexpr uint8_t REG_KP_GPIO3     = 0x1F;

constexpr uint8_t REG_GPI_EM1      = 0x20;
constexpr uint8_t REG_GPI_EM2      = 0x21;
constexpr uint8_t REG_GPI_EM3      = 0x22;

constexpr uint8_t REG_GPIO_DIR1    = 0x23;
constexpr uint8_t REG_GPIO_DIR2    = 0x24;
constexpr uint8_t REG_GPIO_DIR3    = 0x25;

constexpr uint8_t REG_DEBOUNCE_DIS1 = 0x29;
constexpr uint8_t REG_DEBOUNCE_DIS2 = 0x2A;
constexpr uint8_t REG_DEBOUNCE_DIS3 = 0x2B;


// ============================================================
// CONFIG bits
// ============================================================

constexpr uint8_t CONFIG_KE_IEN       = 0x01;
constexpr uint8_t CONFIG_OVR_FLOW_IEN = 0x08;


// ============================================================
// INT_STAT bits
// ============================================================

constexpr uint8_t INT_STAT_K_INT = 0x01;


// ============================================================
// Object
// ============================================================

TCA8418 tca8418;


// ============================================================
// I2C register access
// ============================================================

bool TCA8418::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(TCA8418_ADDR);

    Wire.write(reg);
    Wire.write(value);

    return Wire.endTransmission() == 0;
}


bool TCA8418::readRegister(uint8_t reg, uint8_t &value)
{
    Wire.beginTransmission(TCA8418_ADDR);

    Wire.write(reg);

    if (Wire.endTransmission(false) != 0)
    {
        return false;
    }

    if (Wire.requestFrom(
            static_cast<uint8_t>(TCA8418_ADDR),
            static_cast<uint8_t>(1)) != 1)
    {
        return false;
    }

    value = Wire.read();

    return true;
}


// ============================================================
// Begin
// ============================================================

bool TCA8418::begin()
{
    Wire.begin(I2C_SDA, I2C_SCL);

    pinMode(TCA8418_INT, INPUT_PULLUP);

    delay(10);

    if (!configure())
    {
        Serial.println("TCA8418: initialization FAILED");
        return false;
    }

    Serial.println("TCA8418: initialized");

    return true;
}


// ============================================================
// Configure
// ============================================================

bool TCA8418::configure()
{
    writeRegister(REG_KP_GPIO1, 0xFF);
    writeRegister(REG_KP_GPIO2, 0x0F);
    writeRegister(REG_KP_GPIO3, 0x00);
    
    // --------------------------------------------------------
    // R0-R7 = keypad rows
    // C0-C3 = keypad columns
    //
    // R7/C3 = separate 1x1 2ndF matrix position
    //
    // KP_GPIO:
    //   1 = keypad mode
    //   0 = GPIO mode
    // --------------------------------------------------------

    if (!writeRegister(REG_KP_GPIO1, 0xFF))
        return false;

    if (!writeRegister(REG_KP_GPIO2, 0x0F))
        return false;

    if (!writeRegister(REG_KP_GPIO3, 0x00))
        return false;


    // --------------------------------------------------------
    // C4-C9 are GPIO inputs.
    //
    // GPIO_DIR:
    //   0 = input
    //   1 = output
    //
    // All GPIOs are inputs for now.
    // --------------------------------------------------------

    if (!writeRegister(REG_GPIO_DIR1, 0x00))
        return false;

    if (!writeRegister(REG_GPIO_DIR2, 0x00))
        return false;

    if (!writeRegister(REG_GPIO_DIR3, 0x00))
        return false;


    // --------------------------------------------------------
    // GPIO interrupts disabled for now.
    //
    // C5-C9 will later be read as battery/IP5306 inputs.
    // --------------------------------------------------------

    if (!writeRegister(REG_GPIO_INT_EN1, 0x00))
        return false;

    if (!writeRegister(REG_GPIO_INT_EN2, 0x00))
        return false;

    if (!writeRegister(REG_GPIO_INT_EN3, 0x00))
        return false;


    // --------------------------------------------------------
    // C4-C9 are NOT part of the event FIFO.
    //
    // Only the keypad matrix generates key events for now.
    // --------------------------------------------------------

    if (!writeRegister(REG_GPI_EM1, 0x00))
        return false;

    if (!writeRegister(REG_GPI_EM2, 0x00))
        return false;

    if (!writeRegister(REG_GPI_EM3, 0x00))
        return false;


    // --------------------------------------------------------
    // Enable debounce for the matrix columns.
    //
    // 0 = debounce enabled
    // --------------------------------------------------------

    if (!writeRegister(REG_DEBOUNCE_DIS1, 0x00))
        return false;

    if (!writeRegister(REG_DEBOUNCE_DIS2, 0x00))
        return false;

    if (!writeRegister(REG_DEBOUNCE_DIS3, 0x00))
        return false;


    // --------------------------------------------------------
    // Clear any pending interrupt.
    // --------------------------------------------------------

    writeRegister(REG_INT_STAT, 0xFF);


    // --------------------------------------------------------
    // Enable keypad events and FIFO overflow interrupt.
    // --------------------------------------------------------

    if (!writeRegister(
            REG_CONFIG,
            CONFIG_KE_IEN | CONFIG_OVR_FLOW_IEN))
    {
        return false;
    }

    return true;
}


// ============================================================
// Update
// ============================================================

void TCA8418::update()
{
    readKeyEvents();
}


// ============================================================
// Read key events
// ============================================================

void TCA8418::readKeyEvents()
{
    uint8_t intStatus;

    if (!readRegister(REG_INT_STAT, intStatus))
    {
        return;
    }

    if ((intStatus & INT_STAT_K_INT) == 0)
    {
        return;
    }


    // --------------------------------------------------------
    // Read number of events in FIFO
    // --------------------------------------------------------

    uint8_t keyLockEventCount;

    if (!readRegister(REG_KEY_LCK_EC, keyLockEventCount))
    {
        return;
    }

    uint8_t eventCount = keyLockEventCount & 0x0F;


    // --------------------------------------------------------
    // Read all pending events
    // --------------------------------------------------------

    while (eventCount > 0)
    {
        uint8_t event;

        if (!readRegister(REG_KEY_EVENT_A, event))
        {
            return;
        }

        if (event == 0)
        {
            break;
        }


        // Bit 7:
        //   1 = key press
        //   0 = key release

        bool pressed = (event & 0x80) != 0;

        uint8_t keyNumber = event & 0x7F;


        // ----------------------------------------------------
        // TCA8418 key numbering:
        //
        // key 1 = R0/C0
        //
        // With 8 rows:
        //
        // row = (key - 1) % 8
        // col = (key - 1) / 8
        // ----------------------------------------------------

        if (keyNumber == 0)
        {
            break;
        }

        uint8_t row = keyNumber / 10;
        uint8_t col = (keyNumber % 10) - 1;


        Serial.printf(
            "TCA8418: %s  R%u C%u\n",
            pressed ? "DOWN" : "UP",
            row,
            col
        );

        eventCount--;
    }


    // --------------------------------------------------------
    // Clear keypad interrupt
    // --------------------------------------------------------

    writeRegister(REG_INT_STAT, INT_STAT_K_INT);
}