// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "core/CompileConfig.h"

#if OUZEL_SUPPORTS_METAL

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import "MetalView.h"

@implementation MetalView

-(id)initWithFrame:(NSRect)frameRect
{
    if (self = [super initWithFrame:frameRect])
    {
        [self setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
        [self setWantsLayer:YES];

        CAMetalLayer* metalLayer = [[[CAMetalLayer alloc] init] autorelease];

        CGFloat bgColor[] = { 0.0, 0.0, 0.0, 0.0 };
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGColorRef backgroundColor = CGColorCreate(colorSpace, bgColor);

        metalLayer.edgeAntialiasingMask = 0;
        metalLayer.masksToBounds = YES;
        metalLayer.backgroundColor = backgroundColor;
        metalLayer.presentsWithTransaction = NO;
        metalLayer.anchorPoint = CGPointMake(0.5, 0.5);
        metalLayer.frame = frameRect;
        metalLayer.magnificationFilter = kCAFilterNearest;
        metalLayer.minificationFilter = kCAFilterNearest;
        metalLayer.framebufferOnly = NO;
        metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;

        [self setLayer:metalLayer];

        CGColorRelease(backgroundColor);
        CGColorSpaceRelease(colorSpace);
    }

    return self;
}

@end

#endif
