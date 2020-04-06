// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <kit/ILogger.h>

namespace me
{
    namespace debug
    {
        class BlockPopper
        {
            kit::ILogger* m_logger;
        public:
            BlockPopper( kit::ILogger* logger )
                : m_logger{ logger }
            {
            }

            void operator()( kit::IBlock* block )
            {
                m_logger->PopBlock( block );
            }
        };
    }
}