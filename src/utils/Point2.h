#ifndef POINT2
#define POINT2





namespace ifs {

    class Point2 {
        public:
        float x, y;

        Point2(float x, float y): x(x), y(y){}

        const Point2 operator+ (const Point2& v) const 
        { 
            return Point2(this->x+v.x, this->y+v.y); 
        }

        const Point2 operator- (const Point2& v) const 
        { 
            return Point2(this->x-v.x, this->y-v.y); 
        }

        friend const Point2 operator* (const double w, const Point2& v)
        { return v*w; } 

        const Point2 operator* (const double v) const{ 
            return Point2(this->x*v, this->y*v); 
        }
        
        static Point2 Random2DPoint() {
            return Point2(random_double(), random_double());
        }

    };



}


#endif