//
//  FOXAchieveStageEvent.h
//  FOXExtension
//
//  Created by Wuwei on 2016/07/22.
//  Copyright © 2016年 CyberZ. All rights reserved.
//

#import "FOXExtensionBaseEvent.h"

@interface FOXAchieveStageEvent : FOXExtensionBaseEvent

/**
 * Create event for certain stage achieved with default eventName and specified ltvId.
 * @param ltvId         LTV identifier given by FOX
 */
-(instancetype _Nullable) initWithLtvId:(NSUInteger) ltvId;

/**
 * Add specified level for event.
 * @param stage         Stage
 */
-(void) setStage:(NSString* _Nonnull) stage;

@end