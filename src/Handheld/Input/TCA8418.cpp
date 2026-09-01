#include "Handheld/Input/TCA8418.h"
#include "Handheld/Pins.h"

#include <Wire.h>

// ============================================================
// TCA8418 registers
// ============================================================

constexpr uint8_t REG_CONFIG           = 0x01;
constexpr uint8_t REG_INT_STAT         = 0x02;
constexpr uint8_t REG_KEY_LCK_EC       = 0x03;
constexpr uint8_t REG_KEY_EVENT_A      = 0x04;

constexpr uint8_t REG_GPIO_INT_EN1     = 0x1A;
constexpr uint8_t REG_GPIO_INT_EN2     = 0x1B;
constexpr uint8_t REG_GPIO_INT_EN3     = 0x1C;

constexpr uint8_t REG_KP_GPIO1         = 0x1D;
constexpr uint8_t REG_KP_GPIO2         = 0x1E;
constexpr uint8_t REG_KP_GPIO3         = 0x1F;

constexpr uint8_t REG_GPI_EM1          = 0x20;
constexpr uint8_t REG_GPI_EM2          = 0x21;
constexpr uint8_t REG_GPI_EM3          = 0x22;

constexpr uint8_t REG_GPIO_DIR1        = 0x23;
constexpr uint8_t REG_GPIO_DIR2        = 0x24;
constexpr uint8_t REG_GPIO_DIR3        = 0x25;

constexpr uint8_t REG_DEBOUNCE_DIS1    = 0x29;
constexpr uint8_t REG_DEBOUNCE_DIS2    = 0x2A;
constexpr uint8_t REG_DEBOUNCE_DIS3    = 0x2B;


// ============================================================
// CONFIG bits
// ============================================================

constexpr uint8_t CONFIG_KE_IEN        = 0x01;
constexpr uint8_t CONFIG_OVR_FLOW_IEN  = 0x08;


// ============================================================
// INT_STAT bits
// ============================================================

constexpr uint8_t INT_STAT_K_INT       = 0x01;


// ============================================================
// Object
// ============================================================

TCA8418 tca8418;


// ============================================================
// Event callback
// ============================================================

void TCA8418::setEventCallback(EventCallback callback)
{
    eventCallback = callback;
}


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
    // --------------------------------------------------------
    // R0-R7 + C0-C3 = keypad matrix
    //
    // R7/C3 = 2ndF
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
    // GPIO inputs
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
    // C5-C9 will later be used for battery/IP5306 inputs.
    // --------------------------------------------------------

    if (!writeRegister(REG_GPIO_INT_EN1, 0x00))
        return false;

    if (!writeRegister(REG_GPIO_INT_EN2, 0x00))
        return false;

    if (!writeRegister(REG_GPIO_INT_EN3, 0x00))
        return false;


    // --------------------------------------------------------
    // C4-C9 are not part of the keypad event FIFO.
    // --------------------------------------------------------

    if (!writeRegister(REG_GPI_EM1, 0x00))
        return false;

    if (!writeRegister(REG_GPI_EM2, 0x00))
        return false;

    if (!writeRegister(REG_GPI_EM3, 0x00))
        return false;


    // --------------------------------------------------------
    // Debounce enabled
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
    // Clear pending interrupt
    // --------------------------------------------------------

    if (!writeRegister(REG_INT_STAT, 0xFF))
        return false;


    // --------------------------------------------------------
    // Enable keypad events and FIFO overflow interrupt
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
    // Number of events in FIFO
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


        // ----------------------------------------------------
        // Bit 7:
        //
        // 1 = key press
        // 0 = key release
        // ----------------------------------------------------

        bool pressed = (event & 0x80) != 0;

        uint8_t keyNumber = event & 0x7F;


        if (keyNumber == 0)
        {
            break;
        }


        // ----------------------------------------------------
        // Convert physical KEY number to logical key
        // ----------------------------------------------------

        Key key = matrixToKey(keyNumber);


        // ----------------------------------------------------
        // Send event to application
        // ----------------------------------------------------

        if (key != Key::NONE && eventCallback != nullptr)
        {
            KeyEvent keyEvent;

            keyEvent.key = key;

            keyEvent.type =
                pressed
                ? EventType::DOWN
                : EventType::UP;

            eventCallback(keyEvent);
        }

        eventCount--;
    }


    // --------------------------------------------------------
    // Clear keypad interrupt
    // --------------------------------------------------------

    writeRegister(REG_INT_STAT, INT_STAT_K_INT);
}


// ============================================================
// KEY NUMBER -> logical key
// ============================================================
//
// This is the verified working keypad mapping.
//
// Key numbers are the actual TCA8418 event numbers.
// ============================================================

TCA8418::Key TCA8418::matrixToKey(uint8_t keyNumber)
{
    switch (keyNumber)
    {
        case 63:
            return Key::SKL;

        case 61:
            return Key::SKR;

        case 74:
            return Key::SECOND_F;

        case 62:
            return Key::UP;

        case 52:
            return Key::OK;    

        case 42:
            return Key::DOWN;

        case 53:
            return Key::LEFT;

        case 51:
            return Key::RIGHT;
            

        case 43:
            return Key::ESC;

        case 41:
            return Key::C;


        // ----------------------------------------------------
        // Numbers
        // ----------------------------------------------------

        case 23:
            return Key::NUM_1;

        case 22:
            return Key::NUM_2;

        case 21:
            return Key::NUM_3;

        case 3:
            return Key::NUM_4;

        case 2:
            return Key::NUM_5;

        case 1:
            return Key::NUM_6;

        case 13:
            return Key::NUM_7;

        case 12:
            return Key::NUM_8;

        case 11:
            return Key::NUM_9;

        case 32:
            return Key::NUM_0;


        // ----------------------------------------------------
        // Operators
        // ----------------------------------------------------

        case 33:
            return Key::PLUS;

        case 31:
            return Key::MINUS;


        // ----------------------------------------------------
        // Unknown / unused
        // ----------------------------------------------------

        default:
            return Key::NONE;
    }
}

// ============================================================
// Logical key -> name
// ============================================================

const char* TCA8418::keyName(Key key)
{
    switch (key)
    {
        case Key::NUM_0:     return "0";
        case Key::NUM_1:     return "1";
        case Key::NUM_2:     return "2";
        case Key::NUM_3:     return "3";
        case Key::NUM_4:     return "4";
        case Key::NUM_5:     return "5";
        case Key::NUM_6:     return "6";
        case Key::NUM_7:     return "7";
        case Key::NUM_8:     return "8";
        case Key::NUM_9:     return "9";

        case Key::PLUS:      return "+";
        case Key::MINUS:     return "-";

        case Key::C:         return "C";
        case Key::ESC:       return "ESC";
        case Key::OK:        return "OK";

        case Key::UP:        return "UP";
        case Key::DOWN:      return "DOWN";
        case Key::LEFT:      return "LEFT";
        case Key::RIGHT:     return "RIGHT";

        case Key::SKL:       return "SKL";
        case Key::SKR:       return "SKR";

        case Key::SECOND_F:  return "2ndF";

        case Key::NONE:
        default:
            return "UNKNOWN";
    }
}