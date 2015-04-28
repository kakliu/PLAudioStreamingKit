//
//  PLAudioStreamingSession.h
//  PLAudioStreamingKit
//
//  Created on 15/4/28.
//  Copyright (c) 2015年 Pili Engineering, Qiniu Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

#import "PLAudioStreamingConfiguration.h"
#import "PLMacroDefines.h"
#import "PLTypeDefines.h"

// post with userinfo @{@"state": @(state)}. always posted via MainQueue.
extern NSString *PLStreamStateDidChangeNotification;
extern NSString *PLMicrophoneAuthorizationStatusDidGetNotificaiton;

@class PLAudioStreamingSession;

/*!
 * @protocol PLAudioStreamingSessionDelegate
 *
 * @discussion delegate 对象可以实现对应的方法来获取流的状态及设备授权情况。
 */
@protocol PLAudioStreamingSessionDelegate <NSObject>

@optional
- (void)audioStreamingSession:(PLAudioStreamingSession *)session streamStateDidChange:(PLStreamState)state;
- (void)audioStreamingSession:(PLAudioStreamingSession *)session didGetMicrophoneAuthorizationStatus:(PLAuthorizationStatus)status;

@end

/*!
 * @class PLAudioStreamingSession
 *
 * @abstract 推流中的核心类。
 *
 * @discussion 一个 PLAudioStreamingSession 实例会包含了对视频源、音频源的控制，并且对流的操作及流状态的返回都是通过它来完成的。
 *
 * updated 2015-04-27
 */
@interface PLAudioStreamingSession : NSObject

/*!
 * @property configuration
 *
 * @abstract 音视频编码信息均包含其中。
 */
@property (nonatomic, PL_STRONG) PLAudioStreamingConfiguration *configuration;  // reset will not work until startWithPushURL: invoked.

/*!
 * @property delegate
 *
 * @abstract 代理对象
 */
@property (nonatomic, PL_WEAK) id<PLAudioStreamingSessionDelegate> delegate;

/*!
 * @property streamState
 *
 * @abstract 流的状态，只读属性
 */
@property (nonatomic, assign, readonly) PLStreamState               streamState;

/*!
 * @property isRunning
 *
 * @abstract 是否在推流，只读属性
 */
@property (nonatomic, assign, readonly) BOOL                        isRunning;

/*!
 * @property pushURL
 *
 * @abstract 推流 URL，只读属性
 */
@property (nonatomic, PL_STRONG, readonly) NSURL *pushURL;   // rtmp only now.

/*!
 * 初始化方法
 *
 * @param configuration 用于音视频编码的配置信息
 *
 * @return PLAudioStreamingSession 实例
 *
 * @discussion 初始化方法会优先使用后置摄像头，如果发现设备没有后置摄像头，会判断是否有前置摄像头，如果都没有，便会返回 nil。
 */
- (instancetype)initWithConfiguration:(PLAudioStreamingConfiguration *)configuration;

// RTMP Operations
/*!
 * 开始推流
 *
 * @param pushURL 推流地址
 *
 * @param handler 流连接的结果会通过该回调方法返回
 *
 * @discussion 当调用过一次并且开始推流时，如果再调用该方法会直接返回不会做任何操作，尽管如此，也不要在没有断开时重复调用该方法。
 */
- (void)startWithPushURL:(NSURL *)pushURL completed:(void (^)(BOOL success))handler;

/*!
 * 结束推流
 */
- (void)stop;

@end

#pragma mark - Category (MicrophoneSource)

/*!
 * @category PLAudioStreamingSession(MicrophoneSource)
 *
 * @discussion 与麦克风相关的接口
 */
@interface PLAudioStreamingSession (MicrophoneSource)

@property (nonatomic, assign, getter=isMuted)   BOOL    muted;                   // default as NO.

@end

#pragma mark - Categroy (Application)

/*!
 * @category PLAudioStreamingSession(Application)
 *
 * @discussion 与系统相关的接口
 */
@interface PLAudioStreamingSession (Application)

@property (nonatomic, assign, getter=isIdleTimerDisable) BOOL  idleTimerDisable;   // default as YES.

@end

#pragma mark - Category (Authorization)

/*!
 * @category PLAudioStreamingSession(Authorization)
 *
 * @discussion 与设备授权相关的接口
 */
@interface PLAudioStreamingSession (Authorization)

// Microphone
+ (PLAuthorizationStatus)microphoneAuthorizationStatus;
+ (void)requestMicrophoneAccessWithCompletionHandler:(void (^)(BOOL granted))handler;

@end
