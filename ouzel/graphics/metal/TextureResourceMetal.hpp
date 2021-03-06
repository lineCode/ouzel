// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "core/CompileConfig.h"

#if OUZEL_SUPPORTS_METAL

#if defined(__OBJC__)
#import <Metal/Metal.h>
typedef id<MTLSamplerState> MTLSamplerStatePtr;
typedef id<MTLTexture> MTLTexturePtr;
typedef MTLRenderPassDescriptor* MTLRenderPassDescriptorPtr;
#else
#include <objc/objc.h>
typedef id MTLSamplerStatePtr;
typedef id MTLTexturePtr;
typedef id MTLRenderPassDescriptorPtr;
typedef NSUInteger MTLPixelFormat;
#define MTLPixelFormatInvalid 0
typedef NSUInteger MTLLoadAction;
#define MTLLoadActionDontCare 0
#endif

#include "graphics/TextureResource.hpp"

namespace ouzel
{
    namespace graphics
    {
        class RenderDeviceMetal;

        class TextureResourceMetal: public TextureResource
        {
        public:
            TextureResourceMetal(RenderDeviceMetal* aRenderDeviceMetal);
            virtual ~TextureResourceMetal();

            virtual bool init(const Size2& newSize,
                              uint32_t newFlags = 0,
                              uint32_t newMipmaps = 0,
                              uint32_t newSampleCount = 1,
                              PixelFormat newPixelFormat = PixelFormat::RGBA8_UNORM) override;
            virtual bool init(const std::vector<uint8_t>& newData,
                              const Size2& newSize,
                              uint32_t newFlags = 0,
                              uint32_t newMipmaps = 0,
                              PixelFormat newPixelFormat = PixelFormat::RGBA8_UNORM) override;

            virtual bool setSize(const Size2& newSize) override;
            virtual bool setData(const std::vector<uint8_t>& newData, const Size2& newSize) override;
            virtual bool setFilter(Texture::Filter newFilter) override;
            virtual bool setAddressX(Texture::Address newAddressX) override;
            virtual bool setAddressY(Texture::Address newAddressY) override;
            virtual bool setMaxAnisotropy(uint32_t newMaxAnisotropy) override;
            virtual bool setClearColorBuffer(bool clear) override;
            virtual bool setClearDepthBuffer(bool clear) override;
            virtual bool setClearColor(Color color) override;
            virtual bool setClearDepth(float clear) override;

            MTLTexturePtr getTexture() const { return texture; }
            MTLPixelFormat getColorFormat() const { return colorFormat; }
            MTLPixelFormat getDepthFormat() const { return depthFormat; }

            MTLSamplerStatePtr getSamplerState() const { return samplerState; }
            MTLRenderPassDescriptorPtr getRenderPassDescriptor() const { return renderPassDescriptor; }

            NSUInteger getWidth() const { return width; }
            NSUInteger getHeight() const { return height; }

            MTLLoadAction getColorBufferLoadAction() const { return colorBufferLoadAction; }
            MTLLoadAction getDepthBufferLoadAction() const { return depthBufferLoadAction; }

        protected:
            bool createTexture();
            bool updateSamplerState();

            RenderDeviceMetal* renderDeviceMetal;

            MTLTexturePtr texture = nil;

            NSUInteger width = 0;
            NSUInteger height = 0;

            MTLSamplerStatePtr samplerState = nil;
            MTLRenderPassDescriptorPtr renderPassDescriptor = nil;
            MTLTexturePtr msaaTexture = nil;
            MTLTexturePtr depthTexture = nil;

            MTLPixelFormat colorFormat = MTLPixelFormatInvalid;
            MTLPixelFormat depthFormat = MTLPixelFormatInvalid;

            MTLLoadAction colorBufferLoadAction = MTLLoadActionDontCare;
            MTLLoadAction depthBufferLoadAction = MTLLoadActionDontCare;
        };
    } // namespace graphics
} // namespace ouzel

#endif
