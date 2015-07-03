//
//  CBUUID+OPBTStringRepresentation.h
//  oPhoneBluetoothSDK
//
//  Created by Glenn R. Martin on 10/9/14.
//  Copyright (c) 2014 Vapor Communications. All rights reserved.
//

#import <CoreBluetooth/CoreBluetooth.h>

@interface CBUUID (OPBTStringRepresentation)
/**
 * Made publically available because pre 7.1 OS's dont support UUIDString.
 */
-(NSString*) opbtStringRepresentation;
@end
