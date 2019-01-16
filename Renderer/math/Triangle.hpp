//
//  Triangle.h
//  Z-Renderer
//
//  Created by 贺鹏程 on 2019/1/15.
//  Copyright © 2019年 SATAN_Z. All rights reserved.
//

#ifndef Triangle_h
#define Triangle_h

class Triangle {
public:
    
    Triangle(const VertexOut &v1 , const VertexOut &v2 , const VertexOut &v3):
    v1(v1),
    v2(v2),
    v3(v3){
        
    }
    
    Triangle() {
        
    }
    
    VertexOut v1;
    VertexOut v2;
    VertexOut v3;
};


#endif /* Triangle_h */
