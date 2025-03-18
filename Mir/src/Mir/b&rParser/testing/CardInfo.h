##pragma once
#include <string>
#include <iostream>
#include "Mirpch.h"

namespace brTypes
{
    enum class CardType {
        NONE = 0,
        DIGITAL_INPUT,
        DIGITAL_OUTPUT,
        ANALOG_INPUT,
        ANALOG_OUTPUT
    };

    enum class CardNameType{
        NONE = 0,
        
        SHORT_DEFAULT,
        SHORT_UPPER_CASE,
        SHORT_LOWER_CASE,

        MEDIUM_DEFAULT,
        MEDIUM_UPPER_CASE,
        MEDIUM_LOWER_CASE,

        LONG_DEFAULT,
        LONG_UPPER_CASE,
        LONG_LOWER_CASE,
        
        MEMORY_ADRESS_DEFAULT,
        MEMORY_ADRESS_LOWER_CASE
    };
}


namespace Mir {

    class IOCardInfo{
    public:
    private:
        brTypes::CardType m_CardType = brTypes::CardType::NONE;
        brTypes::CardNameType m_NameType = brTypes::CardNameType::NONE;
    
    public:
        IOCardInfo(brTypes::CardType cardType);

        void setCardName(brTypes::CardNameType NameType) { m_NameType = NameType; }
        constexpr std::string_view GetCardName();
        constexpr std::string_view GetCardDataTypeString();

        ~IOCardInfo();
    private:

    };

    IOCardInfo::IOCardInfo(brTypes::CardType cardType){ 
        m_CardType = cardType;
    }




    IOCardInfo::~IOCardInfo()
    {
    }

    constexpr std::string_view IOCardInfo::GetCardName(){
        
        if (type == brTypes::CardType::NONE){ MIR_ASSERT(false, "shoudlnt be possible"); }
        switch (m_NameType){
            case brTypes::CardNameType::LONG_DEFAULT:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "DigitalInput"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "DigitalOutput"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "AnalogInput"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "AnalogOutput"; }
            break;
            case brTypes::CardNameType::LONG_LOWER_CASE:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "digitalinput"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "digitaloutput"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "analoginput"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "analogoutput"; }
            break;
            case brTypes::CardNameType::LONG_UPPER_CASE:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "DIGITALINPUT"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "DIGITALOUTPUT"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "ANALOGINPUT"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "ANALOGOUTPUT"; }
            break;
            case brTypes::CardNameType::MEDIUM_DEFAULT:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "DigIn"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "DigOut"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "AnIn"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "AnOut"; }
            break;
            case brTypes::CardNameType::MEDIUM_LOWER_CASE:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "digin"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "digout"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "anin"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "anout"; }
            break;
            case brTypes::CardNameType::MEDIUM_UPPER_CASE:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "DIGIN"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "DIGOUT"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "ANIN"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "ANOUT"; }
            break;
            case brTypes::CardNameType::SHORT_DEFAULT:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "Di"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "Do"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "Ai"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "Ao"; }
            break;
            case brTypes::CardNameType::SHORT_LOWER_CASE:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "di"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "do"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "ai"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "ao"; }
            break;
            case brTypes::CardNameType::SHORT_UPPER_CASE:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "DI"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "DO"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "AI"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "AO"; }
            break;
            case brTypes::CardNameType::MEMORY_ADRESS_DEFAULT:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "IX"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "QX"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "IW"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "QW"; }
            break;
            case brTypes::CardNameType::MEMORY_ADRESS_LOWER_CASE:
            if (m_CardType == brTypes::CardType::DIGITAL_INPUT)   { return "ix"; }
            if (m_CardType == brTypes::CardType::DIGITAL_OUTPUT)  { return "qx"; }
            if (m_CardType == brTypes::CardType::ANALOG_INPUT)    { return "iw"; }
            if (m_CardType == brTypes::CardType::ANALOG_OUTPUT)   { return "qw"; }
            break;
            default:
                MIR_ASSERT(false, "shoudlnt be possible");
            break;
        }    
    }
    constexpr std::string_view IOCardInfo::GetCardDataTypeString()
    {
        switch (m_CardType){
        case brTypes::CardType::DIGITAL_INPUT: { return "BOOL" }
        case brTypes::CardType::DIGITAL_OUTPUT:{ return "BOOL" }
        case brTypes::CardType::ANALOG_INPUT:  { return "INT" }
        case brTypes::CardType::ANALOG_OUTPUT: { return "INT" }
            break;
        
        default:
            MIR_ASSERT(false, "shoudlnt be possible");
            break;
        }
    }
}

