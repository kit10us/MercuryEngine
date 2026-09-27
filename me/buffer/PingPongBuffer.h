
#pragma once

#include <me/buffer/IBuffer.h>

#include <memory>
#include <stdexcept>
#include <mutex>
#include <condition_variable>


namespace me::buffer
{
    enum class Side
    {
        Left,
        Right
    };

    const size_t NumSides = 2;

    const const char* ToString(std::string& out, Side side)
    {
         out = (side == Side::Left ? "Left" : "Right");
         return out.c_str();
    }

    template<typename T>
    class PingPongBuffer : public IBuffer<T>
    {
    public:
        typedef std::shared_ptr<PingPongBuffer> ptr;
        typedef std::weak_ptr<PingPongBuffer> weak_ptr;

        PingPongBuffer();
        PingPongBuffer(size_t size);

        virtual ~PingPongBuffer();

        virtual bool IsValid() const override;

        virtual size_t Size() const override;

        virtual size_t Alignment() const override;

        /// @brief 
        /// Returns the data available for reading.
        /// We always read from the bottom of the buffer.
        /// @param side 
        /// @return available data
        size_t AvailableRead() const;

        /// @brief 
        /// Returns the space available for writing.
        /// We always write to the top of the buffer.
        /// @param side 
        /// @return available space
        size_t AvailableWrite() const;

        /// @brief
        /// Returns the side we are currently reading from.
        /// @return side for reading
        Side SideReading() const;

        /// @brief 
        /// Returns the side we are currently writing from.
        /// @return side for writing
        Side SideWriting() const;

        /// @brief 
        /// Returns true if the memory was allocated by us, and thus we are managing it.
        /// @return true if managed
        bool IsManaged() const;

        /// @brief
        /// Allocates the buffers.
        /// Fails if already allocated.
        /// @param size size of the buffers
        /// @return true if allocated
        bool Allocate(size_t size);

        /// @brief
        /// Assign non-managed memory to the buffer.
        /// Fails if the buffer is already allocated or assigned.
        /// @param buffer 
        /// @param size 
        /// @return 
        bool Assign(T* left_buffer, T* right_buffer, size_t size);

        virtual void Destroy() override;

        /// @brief
        /// Appends memory to the top of the write buffer.
        /// This is non-destructive.
        /// Fails if there is not enough memory left in the target buffer.
        /// @param in_buffer memory to append
        /// @param size side of memory to append
        /// @return true if appended
        bool Append(T* in_buffer, size_t size);

        /// @brief
        /// Reads up to max_size data from the bottom of the read buffer.
        /// Fails if not size specified exceeds amount written.
        /// @param size the amount of data to consume
        /// @return amount of data read
        size_t Read(T* out_buffer, size_t max_size) const;

        /// @brief
        /// Consume up to max_size data from the bottom of the read buffer.
        /// This is a destructive action.
        /// @param size the amount of data to consume
        /// @return amount of data consumed
        size_t Consume(T* out_buffer, size_t max_size);

        /// @brief 
        /// Flip the side we are reading from and writting to.
        /// Flipping is a destructive action. In invalidates the new write buffer.
        /// @return true on success
        bool Flip();

        std::weak_ptr< BufferLock<T> > LockBuffer();

        void UnlockBuffer();

    private:
        /// @brief
        /// Returns the top offset index of the buffer.
        /// @param side buffer to get top of
        /// @return top of the buffer
        size_t Top(Side side) const;

        /// @brief
        /// Returns the bottom offset index of the buffer.
        /// @param side buffer to get bottom of
        /// @return bottom of the buffer
        size_t Bottom(Side side) const;

        size_t m_size;
        size_t m_top[NumSides];
        size_t m_bottom[NumSides];
        Side m_write;
        bool m_managed;
        T* m_buffer[NumSides];
        std::mutex m_lock_mutex;
        std::condition_variable m_locked;
        std::shared_ptr< BufferLock<T> > m_lock;
    };
}


namespace utils
{
    template<typename T>
    PingPongBuffer<T>::PingPongBuffer()
    : m_size {}
    , m_top {}
    , m_bottom {}
    , m_write {Side::Left}
    , m_managed {true}
    , m_buffer {}
    {
    }

    template<typename T>
    PingPongBuffer<T>::PingPongBuffer(size_t size)
        : PingPongBuffer()
    {
        if (!Allocate(size))
        {
            throw std::logic_error("Failed to allocate buffer!");
        }
    }

    template<typename T>
    PingPongBuffer<T>::~PingPongBuffer()
    {
        Destroy();
    }

    template<typename T>
    bool PingPongBuffer<T>::IsValid() const
    {
        return m_size ? true : false;
    }

    template<typename T>
    size_t PingPongBuffer<T>::Size() const
    {
        return m_size;
    }

    template<typename T>
    size_t PingPongBuffer<T>::Alignment() const
    {
        return sizeof(T);
    }

    template<typename T>
    size_t PingPongBuffer<T>::AvailableRead() const
    {
        auto side = (size_t)SideReading();
        return m_top[side] - m_bottom[side];
    }

    template<typename T>
    size_t PingPongBuffer<T>::AvailableWrite() const
    {
        auto side = (size_t)SideWriting();
        return Size() - m_top[side];
    }

    template<typename T>
    Side PingPongBuffer<T>::SideReading() const
    {
        return m_write == Side::Left ? Side::Right : Side::Left;
    }

    template<typename T>
    Side PingPongBuffer<T>::SideWriting() const
    {
        return m_write;
    }

    template<typename T>
    bool PingPongBuffer<T>::IsManaged() const
    {
        return m_managed;
    }

    template<typename T>
    bool PingPongBuffer<T>::Allocate(size_t size)
    {
        if (IsValid() || size == 0)
        {
            return false;
        }

        Destroy(); // Called anyways to reset the values of the buffer.

        m_buffer[(size_t)Side::Left] = (T*)malloc(Alignment() * size);
        m_buffer[(size_t)Side::Right] = (T*)malloc(Alignment() * size);
        m_managed = true;
        m_size = size;
        bool allocated = m_buffer[(size_t)Side::Left] && m_buffer[(size_t)Side::Right];
        return allocated;
    }

    template<typename T>
    bool PingPongBuffer<T>::Assign(T* left_buffer, T* right_buffer, size_t size)
    {
        if (IsValid())
        {
            return false;
        }

        Destroy(); // Called anyways to reset the values of the buffer.

        m_buffer[(size_t)Side::Left] = left_buffer;
        m_buffer[(size_t)Side::Right] = right_buffer;
        m_managed = false;
        m_size = size;
    }

    template<typename T>
    void PingPongBuffer<T>::Destroy()
    {
        auto side_left = (size_t)Side::Left;
        auto side_right = (size_t)Side::Right;
        if (m_managed)
        {
            free(m_buffer[side_left]);
            m_buffer[side_left] = nullptr;

            free(m_buffer[side_right]);
            m_buffer[side_right] = nullptr;            
        }


        m_size = {};
        m_top[side_left] = {};
        m_top[side_right] = {};
        m_bottom[side_left] = {};
        m_bottom[side_right] = {};
        m_write = Side::Left;
        m_managed = true;
    }

    template<typename T>
    bool PingPongBuffer<T>::Append(T* buffer, size_t size)
    {
        auto side = (size_t)SideWriting();
        size_t available = AvailableWrite();
        if (size > available)
        {
            return false;
        }
        memcpy(m_buffer[side] + m_top[side], buffer, size);
        m_top[side] += size;
        return true;
    }

    template<typename T>
    size_t PingPongBuffer<T>::Read(T* buffer_out, size_t max_size) const
    {
        if (!IsValid())
        {
            return false;
        }

        if (!AvailableRead())
        {
            return false;
        }

        size_t size = std::min<size_t>(AvailableRead(), max_size);

        auto side = (size_t)SideReading();

        memcpy(buffer_out, m_buffer[side] + m_bottom[side], size);
        
        return size;
    }

    template<typename T>
    size_t PingPongBuffer<T>::Consume(T* buffer_out, size_t max_size)
    {
        if (!IsValid())
        {
            return false;
        }

        auto read = Read(buffer_out, max_size);
        auto side = (size_t)SideReading();

        m_bottom[side] += read;
        
        if (m_top[side] == m_bottom[side])
        {
            m_bottom[side] = 0;
            m_top[side] = 0;
        }

        return read;
    }

    template<typename T>
    bool PingPongBuffer<T>::Flip()
    {
        if (!IsValid())
        {
            return false;
        }

        m_write = m_write == Side::Left ? Side::Right : Side::Left;
        auto side = (size_t)SideWriting();
        m_top[side] = 0;
        m_bottom[side] = 0;
        return true;
    }

    template<typename T>
    std::weak_ptr< BufferLock<T> > PingPongBuffer<T>::LockBuffer()
    {
        return {};
    }

    template<typename T>
    void PingPongBuffer<T>::UnlockBuffer()
    {
    }

    template<typename T>
    size_t PingPongBuffer<T>::Top(Side side) const
    {
        return m_top[(size_t)side];
    }

    template<typename T>
    size_t PingPongBuffer<T>::Bottom(Side side) const
    {
        return m_bottom[(size_t)side];
    }
}
