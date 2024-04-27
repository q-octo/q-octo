#include <iostream>
#include <cstdint>
#include <cassert>

class TransferCRC
{
    std::uint16_t value_;

public:
    TransferCRC()
        : value_(0xFFFFU)
    {
    }

    void add(std::uint8_t byte)
    {
        value_ ^= static_cast<std::uint16_t>(byte) << 8;
        for (std::uint8_t bit = 8; bit > 0; --bit)
        {
            if (value_ & 0x8000U)
            {
                value_ = (value_ << 1) ^ 0x1021U;
            }
            else
            {
                value_ = (value_ << 1);
            }
        }
    }

    void add(const std::uint8_t *bytes, unsigned len)
    {
        assert(bytes);
        while (len--)
        {
            add(*bytes++);
        }
    }

    std::uint16_t get() const { return value_; }
};

void crcAddSignature(TransferCRC &transferCrc, uint16_t crc_val, uint64_t data_type_signature)
{
    for (uint16_t shift_val = 0; shift_val < 64; shift_val = (uint16_t)(shift_val + 8U))
    {
        transferCrc.add((uint8_t)(data_type_signature >> shift_val));
        // crc_val = crcAddByte(crc_val, (uint8_t)(data_type_signature >> shift_val));
    }
    // return crc_val;
}

void crcAdd(TransferCRC &transferCrc, uint16_t crc_val, const uint8_t *bytes, size_t len)
{
    while (len--)
    {
        transferCrc.add(*bytes++);
    }
    // return crc_val;
}

int main()
{
    TransferCRC crc;
    // 00 00 43 4E EC 00 00 00 1F C0 00 01 42 61 74 74 4D 6F 6E 20 2D 31
    // const uint8_t data[] = {0x00, 0x00, 0x43, 0x4E, 0xEC,
    //                         0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x01,
    //                         0x42, 0x61, 0x74, 0x74, 0x4D, 0x6F, 0x6E,
    //                         0x20, 0x2D, 0x31};
    // const uint8_t data[] = {0x00, 0x00, 0x43, 0x4E, 0xEC, 0x42, 0x61, 0x74, 0x74, 0x4D, 0x6F, 0x6E, 0x20, 0x2D, 0x31};
    // const uint8_t data[] = {
    //     0x20, 0x2D, 0x31,
    //     0x42, 0x61, 0x74, 0x74, 0x4D, 0x6F, 0x6E,
    //     // 0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x01,
    //     0x00, 0x00, 0x43, 0x4E, 0xEC};
    // const uint8_t data[] = { 0xF0, 0x43, 0x00, 0x29, 0x00, 0x08, 0x50, 0x48, 0x39, 0x48, 0x33, 0x38, 0x20};

    // Expected CRC = 0x27C3
    // 00 00 00 00 EC
    // 00 00 00 1F C0 00 01
    // 42 61 74 74 4D 6F 6E
    // 20 2D 31
    // const uint8_t data[] = {
    //     0x00, 0x00, 0x00, 0x00, 0xEC,
    //     0x00, 0x00, 0x00, 0x1F, 0xC0, 0x00, 0x01,
    //     0x42, 0x61, 0x74, 0x74, 0x4D, 0x6F, 0x6E,
    //     0x20, 0x2D, 0x31
    // };

    // Expected CRC = 0xE938
    // 38 E9 00 00 00 00 00 80
    // 00 00 00 00 00 00 00 20
    // 00 00 00 00 00 00 00 00
    // 00 00 00 00 60
    const uint8_t data[] = {
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00};
    long signatureForDNA = 0xB2A812620A11D40ULL;
    long signatureForBatt = (0x249C26548A711966ULL);
    crcAddSignature(crc, 0, signatureForBatt);
    crcAdd(crc, 0, data, sizeof(data));
    // crc.add(data, sizeof(data));
    // crc.add(reinterpret_cast<const std::uint8_t*>("123456789"), 9);
    std::cout << std::hex << "0x" << crc.get() << std::endl;
}