//
//  AppDelegate.h
//  oPhoneExample
//
//  Created by Brendan Regan on 7/2/15.
//  Copyright (c) 2015 Vapor Communications. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <oPhoneBluetoothSDK/OPBTCentralManager.h>

@interface AppDelegate : NSObject <NSApplicationDelegate, OPBTCentralManagerDelegate>

@property(nonatomic,strong) OPBTCentralManager* opbtManager;
@end

