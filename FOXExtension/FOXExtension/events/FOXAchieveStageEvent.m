//
//  FOXAchieveStageEvent.m
//  FOXExtension
//
//  Created by Wuwei on 2016/07/22.
//  Copyright © 2016年 CyberZ. All rights reserved.
//

#import "FOXAchieveStageEvent.h"

@implementation FOXAchieveStageEvent

-(instancetype) initWithLtvId:(NSUInteger) ltvId {
    return [super initWithEventName:@"fox_achieved_stage" andLtvId:ltvId];
}

-(void) setStage:(NSString *) stage {
    if (stage && stage.length > 0) {
        [self addExtraValue:stage forKey:@"stage"];
        self.eventInfo = @{@"stage" : stage};
    }
}

@end