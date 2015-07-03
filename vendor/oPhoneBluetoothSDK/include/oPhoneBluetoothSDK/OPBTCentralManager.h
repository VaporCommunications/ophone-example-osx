//
// Created by Glenn R. Martin on 9/17/14.
// Copyright (c) 2014 Vapor Communications. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol OPBTCentralManagerDelegate;

@class OPBTPeripheral;

/**
 * NSError Domain for all Central Manager Errors
 */
extern NSString* const OPBTCentralManagerDomain;

typedef enum {
    /**
     * The BT LE Transmitter on the device has been disabled globally by the user.
     */
    OPBTCentralManagerErrorCodeRadioDisabled = 1,
    /**
     * This device either does not support BT LE (Bluetooth 4.0 Spec),
     * or does not have a transmitter. (The Simulator will report this.)
     */
    OPBTCentralManagerErrorCodeRadioUnsupported,
    /**
     * The application has been denied rights to use the BT LE Transmitter directly.
     * To continue the user must manually enable it.
     */
    OPBTCentralManagerErrorCodeApplicationUnauthorized
} OPBTCentralManagerErrorCode;

@interface OPBTCentralManager : NSObject
/**
 * If the Central manager is currently connected.
 */
@property (assign, readonly, getter=isConnected) BOOL connected;

/**
 * An unordered set of currently valid peripherals.
 */
@property (copy, nonatomic, readonly) NSSet* currentPeripherals;

/**
 * Initialize a Central Manager with a delegate whilst specifying the GCD Queue to use.
 * @param delegate The delegate intended to handle callbacks
 * @param queue A GCD Queue in which to perform query actions.
 * @return OPBTCentralManager or nil upon error.
 */
-(instancetype) initWithDelegate:(id<OPBTCentralManagerDelegate>)delegate queue:(dispatch_queue_t)queue;

/**
 * Initialize a Central Manager with a delegate, telling it to use an initialize its own global queue.
 * @param delegate The delegate intended to handle callbacks
 * @return OPBTCentralManager or nil upon error.
 */
-(instancetype) initWithDelegate:(id<OPBTCentralManagerDelegate>)delegate;

/**
 * Start a scan for valid peripherals.
 * This call will first search for then inspect all peripherals found, connecting 
 * to them for a moment to verify they are compatible then adding to the set and 
 * calling the delegate if valid.
 *
 * If supported backgrond mode is enabled, this will also enable restoring of 
 * BT LE devices when returning from the background.
 */
-(void) scanForPeripheralDevices;

/**
 * Stop scanning for devices.
 * This terminates scans, allowing us to save battery.
 */
-(void) stopScan;
@end

@protocol OPBTCentralManagerDelegate <NSObject>
@required
/**
 * This delegate method is called when an error occurs within the central manager upon trying to connect to the radio
 * and start a BT LE peripheral scan.
 */
-(void) centralManager:(OPBTCentralManager *)manager didEncounterConnectionError:(NSError*)error;

@optional
/**
 * This delegate method is called when the central manager is able to connect to the BT LE radio and start a scan of
 * devices.
 */
-(void)centralManagerDidConnect:(OPBTCentralManager *)manager;
/**
 * This delegate method is called when the central manager is able to disconnect from the BT LE radio and has ended its
 * scan for devices.
 */
-(void)centralManagerDidDisconnect:(OPBTCentralManager *)manager;

/**
 * This delegate method is called when the manager finds and has verified a device as a valid oPhone device.
 */
-(void) centralManager:(OPBTCentralManager *)manager didRegisterValidPeripheral:(OPBTPeripheral *)peripheral;

/**
 * This delegate method is called when the manager has lost a previously valid oPhone device.
 * ***NOT YET IMPLEMENTED ***
 */
-(void) centralManager:(OPBTCentralManager *)manager didDeregisterValidPeripheral:(OPBTPeripheral *)peripheral; /* NYI */
@end