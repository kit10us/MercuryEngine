
#pragma once

#include <me/buffer/BufferLock.h>
#include <unify/Result.h>

#include <memory>
#include <optional>


namespace me::buffer
{
   struct EncodeResults
    {
        size_t read;
        size_t written;
    };

    class IEncoder
    {
    public:
        using ptr = std::shared_ptr<IEncoder>;

        virtual ~IEncoder() = default;

        /// @brief Emcpde a buffer
        /// @param in buffer to encode
        /// @param out formatter buffer
        /// @return bytes used of out buffer
        virtual unify::Result<EncodeResults> Encode(const ReadOnlyBufferLock& in, BufferLock& out) = 0;

        virtual std::optional<size_t> GetPayloadSize() const 
        { 
            return {};
        }
    };
}