//
// Created by Glenn R. Martin on 9/17/14.
// Copyright (c) 2014 Vapor Communications. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CoreBluetooth/CoreBluetooth.h>

@class OPBTPeripheral;

@protocol OPBTPeripheralOperation <NSObject>
/**
 * Unique identifier for this operation.
 */
-(NSString*) identifier;

/**
 * The user supplied context when the operation was created.
 */
-(id<NSCoding>) context;
@end

/**
 * Global Notification of peripheral connection.
 */
extern NSString* const OPBTPeripheralConnectedNotification;

/**
 * Global Notification of peripheral disconnection.
 */
extern NSString* const OPBTPeripheralDisconnectedNotification;

/**
 * Global Notification of data being sent to the peripheral.
 */
extern NSString* const OPBTPeripheralDataQueueSendingNotification;

/**
 * Global notification of Operation Canceling due to bad connection/connection loss on peripheral.
 */
extern NSString* const OPBTPeripheralDataQueueConnectionDiedNotification;

/**
 * Global Notification of data being acknowledged via response from the peripheral.
 */
extern NSString* const OPBTPeripheralDataQueueAcknowledgedNotification;

/**
 * Notification UserInfo key, used in connection and disconnection notifications to let the user know whether the device is suspect.
 * It is a CFBoolean (NSNumber)
 */
extern NSString* const OPBTPeripheralWasSuspectKey;

/**
 * Key to get the surrogate for the operation started/completed from a notification.
 * object will implement the OPBTPeripheralOperation protocol.
 */
extern NSString* const OPBTPeripheralDataQueueOperationKey;

/**
 * NSError Domain for all Peripheral Errors
 */
extern NSString* const OPBTPeripheralDomain;

typedef void(^OPBTPeripheralCompletionHandler)(OPBTPeripheral* peripheral, NSError* error);

typedef enum {
    /** The peripheral is connected and we are determing its validity. */
    OPBTPeripheralStateUndetermined = 0,
    /** The peripheral is connected and we are determing its validity. */
    OPBTPeripheralStateDiscovering,
    /** Attempting to connect to peripheral to determine if it is an oPhone device */
    OPBTPeripheralStateConnecting,
    /** The peripheral is connected presently */
    OPBTPeripheralStateConnected,
    /** Connection to the peripheral has failed. */
    OPBTPeripheralStateConnectionError,
    /** The peripheral has disconnected. */
    OPBTPeripheralStateDisconnected
} OPBTPeripheralState;

typedef enum {
    /** The peripheral has been determined to not be a valid oPhone device */
    OPBTPeripheralInvalid = -1,
    /** The peripheral determination has not been done or is incomplete, this may or may not be a valid device. */
    OPBTPeripheralSuspect = 0,
    /** The peripheral has been determined to be a valid oPhone device */
    OPBTPeripheralValid = 1
} OPBTPeripheralValidity;

@interface OPBTPeripheral : NSObject
/**
 * The current state of the detected BT LE Device.
 */
@property (readonly, assign) OPBTPeripheralState state;

/**
 * The current validity of the detected BT LE Device. (is it one of ours?)
 */
@property (readonly, assign) OPBTPeripheralValidity validity;

/**
 * The CoreBluetooth Peripheral reference.
 */
@property (readonly, strong, nonatomic) CBPeripheral* corePeripheral;

/**
 * Whether the device is ready for writing.
 */
@property (readonly, nonatomic, assign, getter=isReadyForWriting) BOOL readyForWriting;

/**
 * The currently executing operation.
 */
@property(nonatomic, strong) id currentWriteOp;
@end

@interface OPBTPeripheral(OPBTConnectionHandling)
/**
 * Connect to the current peripheral.
 * @param handler A completion callback block.
 */
-(void) connectWithCompletionHandler:(OPBTPeripheralCompletionHandler)handler;

/**
 * Disconnect the current peripheral.
 * @param handler A completion callback block.
 */
-(void) disconnectWithCompletionHandler:(OPBTPeripheralCompletionHandler)handler;
@end

@interface OPBTPeripheral (OPBTRawDataWriting)
/**
 * Enqueue data to be sent to the device.
 * @param data The raw data to be sent.
 * @param context a user created context to be attached to the operation.
 */
-(void) enqueueData:(NSData*)data context:(id<NSCoding>)context;

/**
 * Clear the current write operation queue.
 * NOTE: Does not clear currentWriteOp until the Operation returns.
 */
-(void) clearQueue;
@end

typedef enum {
    OPBTPeripheralDomainErrorCodeBadState = 1000
} OPBTPeripheralDomainErrorCode;