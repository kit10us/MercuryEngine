
#pragma once

#include <me/buffer/BufferLock.h>
#include <unify/Result.h>

#include <optional>
#include <memory>


namespace me::buffer
{
   struct DecodeResults
    {
        size_t read;
        size_t written;
    };

    class IDecoder
    {
    public:
        using ptr = std::shared_ptr<IDecoder>;

        virtual ~IDecoder() = default;

        /// @brief Decoded buffer.
        /// @param in buffer to decode
        /// @param out output for data portion from decoded buffer.
        /// @return bytes used of out buffer
        virtual unify::Result<DecodeResults> Decode(const ReadOnlyBufferLock& in, BufferLock& out) = 0;

        virtual std::optional<size_t> GetPayloadSize() const 
        { 
            return {};
        }
    };
}