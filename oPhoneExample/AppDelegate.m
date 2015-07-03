//
//  AppDelegate.m
//  oPhoneExample
//
//  Created by Brendan Regan on 7/2/15.
//  Copyright (c) 2015 Vapor Communications. All rights reserved.
//

#import "AppDelegate.h"
#import <oPhoneBluetoothSDK/OPBTPeripheral.h>

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@property (unsafe_unretained) IBOutlet NSTextView *log;
@property (weak) IBOutlet NSTableView *table;

@property (nonatomic, strong) NSArray *smellNames;
@property (nonatomic, strong) NSArray *smellCodes;

@end




@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    [self setupoPhone];
    [self appendToTextView:@"started"];

    [self.table reloadData];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {

}

- (void)appendToTextView: (NSString*) moreText {
    dispatch_async(dispatch_get_main_queue(), ^{
        self.log.string = [NSString stringWithFormat:@"%@%@\n",
                           self.log.string, moreText];
        [self.log scrollRangeToVisible:NSMakeRange(self.log.string.length-1, 1)];
    });
}

- (void)setupoPhone {
    self.opbtManager = [[OPBTCentralManager alloc] initWithDelegate:self];
    [self.opbtManager scanForPeripheralDevices];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(peripheralDidConnect:) name:OPBTPeripheralConnectedNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(peripheralDidDisconnect:) name:OPBTPeripheralDisconnectedNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(peripheralDidAcknowledgeData:) name:OPBTPeripheralDataQueueAcknowledgedNotification object:nil];


    
    // Use some example smells from the "Coffee" scent family
    self.smellNames = [NSArray arrayWithObjects:@"Passion Fruit",
                       @"Citrus",
                       @"Jasmine",
                       @"Green Vegetation",
                       @"Honey",
                       @"Exotic Wood",
                       @"Apricot",
                       @"Red Berries",
                       @"Cedar",
                       @"Licorice",
                       @"Butter",
                       @"Caramel",
                       @"Cocoa Bean",
                       @"Walnut",
                       @"Grilled Toast",
                       @"Cream", nil];
    
    self.smellCodes = [NSArray arrayWithObjects:@"M",
                       @"E",
                       @"H",
                       @"J",
                       @"M",
                       @"O",
                       @"R",
                       @"T",
                       @"c",
                       @"e",
                       @"h",
                       @"j",
                       @"m",
                       @"o",
                       @"r",
                       @"t", nil];


}

- (void)buttonPressed:(NSButton *)sender {
    
    // Send a smell to the device when its button is pressed
    NSInteger row = sender.tag;
    NSString *smellName = [self.smellNames objectAtIndex:row];
    NSString *smellCode = [self.smellCodes objectAtIndex:row];
    NSInteger smellDuration = 15;

    [self appendToTextView:[NSString stringWithFormat:@"Sending smell %@ (%@) for duration %ld seconds.", smellName, smellCode, smellDuration]];

    NSError *error = [self sendSmell:smellCode withDuration:smellDuration toPeripheral:[self.opbtManager.currentPeripherals anyObject]];
    if (error) {
        [self appendToTextView:[NSString stringWithFormat:@"Error sending smell: %@", error.localizedDescription]];
    }
}

-(NSError*)sendSmell:(NSString*)smellId withDuration:(Byte)duration toPeripheral:(OPBTPeripheral *)peripheral
{
    if(peripheral == nil) {
        return  [NSError errorWithDomain:OPBTPeripheralDomain code:-1 userInfo:@{NSLocalizedDescriptionKey:@"No peripherals connected."}];
    }
    
    if(peripheral.isReadyForWriting) {
        char bytes[] = "\x40\x05\x6D";
        bytes[1] = duration;
        
        const char *c = [smellId UTF8String];
        bytes[2] = c[0];
        
        size_t length = (sizeof bytes) - 1;
        NSData *data = [NSData dataWithBytes:bytes length:length];
        
        [peripheral enqueueData:data context:@"1"];
    } else {
        return  [NSError errorWithDomain:OPBTPeripheralDomain code:-1 userInfo:@{NSLocalizedDescriptionKey:@"Peripheral not ready for writing."}];
    }
    
    return nil;
}



-(void)peripheralDidConnect:(NSNotification*)note
{
    [self appendToTextView:@"peripheralDidConnect"];
    
    // We can stop scanning now that we found a peripheral.
    [self.opbtManager stopScan];
}

-(void)peripheralDidDisconnect:(NSNotification*)note
{
    [self appendToTextView:@"peripheralDidDisconnect"];
}

-(void)peripheralDidAcknowledgeData:(NSNotification*)note
{
    [self appendToTextView:@"peripheralDidAcknowledgeData"];
}

-(void) centralManager:(OPBTCentralManager *)manager didEncounterConnectionError:(NSError*)error
{
    NSString *str = [NSString stringWithFormat:@"didEncounterConnectionError %@", error.localizedDescription];
    [self appendToTextView:str];
}

-(void)centralManagerDidConnect:(OPBTCentralManager *)manager
{
    [self appendToTextView:@"centralManagerDidConnect"];
}

-(void)centralManagerDidDisconnect:(OPBTCentralManager *)manager
{
    [self appendToTextView:@"centralManagerDidDisconnect"];
}

-(void) centralManager:(OPBTCentralManager *)manager didRegisterValidPeripheral:(OPBTPeripheral *)peripheral
{
    [peripheral connectWithCompletionHandler:nil];
    [self appendToTextView:@"didRegisterValidPeripheral"];
}

-(void) centralManager:(OPBTCentralManager *)manager didDeregisterValidPeripheral:(OPBTPeripheral *)peripheral
{
    [self appendToTextView:@"didDeregisterValidPeripheral"];
}

#pragma mark NSTableViewDataSource
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
    return self.smellNames.count;
}


#pragma mark NSTableViewDelegate

- (NSView *)tableView:(NSTableView *)tableView
   viewForTableColumn:(NSTableColumn *)tableColumn
                  row:(NSInteger)row {
    
    NSButton *button = [tableView makeViewWithIdentifier:@"SmellTableRow" owner:self];
    
    if (button == nil) {
        button = [[NSButton alloc] initWithFrame:NSMakeRect(0,0,self.table.frame.size.width,self.table.frame.size.height)];
        [[self.window contentView] addSubview:button];
        button.title = [NSString stringWithFormat:@"%@ (%@)", [self.smellNames objectAtIndex:row], [self.smellCodes objectAtIndex:row]];
        button.tag = row;
        [button setTarget:self];
        [button setAction:@selector(buttonPressed:)];
    }

    return button;
}

@end
