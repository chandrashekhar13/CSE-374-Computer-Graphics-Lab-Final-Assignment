#include<bits/stdc++.h>
#include<graphics.h>
#include<windows.h>
#include<math.h>
#define INF 10000
using namespace std;

const int WINDOW_W = 1280, WINDOW_H = 720;

struct PT
{
    double x, y;
    PT() {}
    PT(double a, double b)
    {
        x = a;
        y = b;
    }
    bool operator  < (const PT &p) const
    {
        return x < p.x;
    }
};
void drawAxis()
{
    for (int i = 0; i < WINDOW_H; i++)
        putpixel(WINDOW_W/2,i,WHITE);
    for (int i = 0; i < WINDOW_W; i++)
        putpixel(i,WINDOW_H/2,WHITE);
}

PT convertPixel(PT p)
{
    p.x  += WINDOW_W/2;
    p.y  = -p.y;
    p.y  += WINDOW_H/2;
    return p;
}
void drawPixel(PT p, int color)
{
    p = convertPixel(p);
    putpixel((int)(p.x+0.5),(int)(p.y+0.5),color);
}
void drawLine(PT a, PT b,int col)
{
    if(a.x > b.x) swap(a, b);



    if(a.y == b.y)
    {
        for(int x = a.x; x <= b.x; x++)
        {
            drawPixel(PT(x, a.y), col);
        }
    }



    else if(a.x == b.x)
    {
        if(a.y > b.y) swap(a, b);
        for(int y = a.y; y <= b.y; y++)
        {
            drawPixel(PT(a.x, y), col);
        }
    }

    else
    {
        double m = (b.y-a.y)/(b.x-a.x);
        double m_inv = 1.0/m;
        if(fabs(m) <= 1.0)
        {
            if(a.x>b.x) swap(a,b);

            while(a.x <= b.x)
            {


                drawPixel(a, col);
                a.y+=m;
                a.x += 1;
            }

        }
        else
        {
            if(a.y >b.y) swap(a,b);
            while(a.y <= b.y)
            {


                drawPixel(a, col);
                a.x+=m_inv;
                a.y += 1;
            }
        }
    }
}

void drawPolygon(vector<PT> points,int col)
{
    int n = points.size();
    for(int i=0; i<n; i++)
    {
        drawLine(points[i], points[(i+1)%n],col);
    }
}




vector<PT> trans(vector <PT> points,double x,double y)
{
    int n = points.size();
    for(int i=0; i<n; i++)
    {
        points[i].x +=x;
        points[i].y += y;
    }
    return points;
}
struct EDGE
{
    double y_min,y_max,x_with_y_min,m_inv;
    EDGE() {}
    EDGE (double a, double b, double c, double d)
    {
        y_min = a;
        y_max = b;
        x_with_y_min = c;
        m_inv = d;

    }
    bool operator < (const EDGE  &p) const
    {
        return y_min < p.y_min;
    }


};
void scanLine (vector <PT> points, int color)
{

    vector <EDGE> edges;
    double st = 1e15, en = -1e15;

    int n = points.size();
    unordered_map <double, int> mp;


    for (int i = 0; i < n; i++)
    {
        PT a = points[i];
        PT b = points[(i+1)%n];
        EDGE temp;
        temp.y_min = min(a.y,b.y);
        temp.y_max = max(a.y,b.y);
        temp.x_with_y_min = a.y <b.y ? a.x : b.x;
        temp.m_inv = (b.x- a.x) / (b.y - a.y);
        st = min(st,temp.y_min);
        en = max(en,temp.y_max);
        mp[temp.y_min] =1;
        edges.push_back(temp);
    }

    n = edges.size();
    sort(edges.begin(),edges.end());
    for (int i = 0; i < n; i++)
    {
        if(mp[edges[i].y_max])
            edges[i].y_max-- ;
    }

    for (double y =st; y <= en; y++)
    {
        vector<PT> int_points;
        for (int i =0; i <n ; i++)
        {
            if(y >= edges[i].y_min && y <= edges[i].y_max)
            {
                PT temp;
                temp.x = edges[i].x_with_y_min;
                temp.y = y;
                edges[i].x_with_y_min += edges[i].m_inv;
                int_points.push_back(temp);
            }
        }

        sort(int_points.begin(),int_points.end());
        for(int i =0; i <= int_points.size()-1; i+=2)
        {
            drawLine(int_points[i],int_points[i+1],color);

        }
    }

}
bool onSegment(PT p, PT q, PT r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
            q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;
    return false;
}
int orientation(PT p, PT q, PT r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;
    return (val > 0)? 1: 2;
}
bool doIntersect(PT p1, PT q1, PT p2, PT q2)
{

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);


    if (o1 != o2 && o3 != o4)
        return true;


    if (o1 == 0 && onSegment(p1, p2, q1)) return true;


    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    if (o3 == 0 && onSegment(p2, p1, q2)) return true;


    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}
    bool isInside(vector<PT> polygon, PT p)
    {
        int n = polygon.size();

        if (n < 3) return false;


        PT extreme = PT(INF, p.y);

        int count = 0, i = 0;
        do
        {
            int next = (i+1)%n;


            if (doIntersect(polygon[i], polygon[next], p, extreme))
            {

                if (orientation(polygon[i], p, polygon[next]) == 0)
                    return onSegment(polygon[i], p, polygon[next]);

                count++;
            }
            i = next;
        }
        while (i != 0);


        return count&1;
    }
    int main()
    {
        initwindow(WINDOW_W,WINDOW_H,"A Simple Graphics Project");


        int x=0,y=0;
        vector<PT> poly;

        poly.push_back(PT(0, 100));
        poly.push_back(PT(50, 70));
        poly.push_back(PT(45, -120));
        poly.push_back(PT(0, -110));
        poly.push_back(PT(-60,-70));
        poly.push_back(PT(-30,60));


        while(1)
        {

            cleardevice();
            drawPolygon(poly,WHITE);



            if(GetAsyncKeyState(VK_RIGHT))
                poly = trans(poly,10,0);
            else if (GetAsyncKeyState(VK_LEFT))
                poly = trans(poly,-10,0);
            else if (GetAsyncKeyState(VK_UP))
                poly = trans(poly,0,10);
            else if (GetAsyncKeyState(VK_DOWN))
                poly = trans(poly,0,-10);
            if(GetAsyncKeyState(VK_RETURN))
                break;

            getmouseclick(WM_LBUTTONDOWN, x, y);


            x -=WINDOW_W/2;
            y = WINDOW_H/2 - y;
            if(isInside(poly,PT(x,y)))
            {

                scanLine(poly,YELLOW);
                cout << "The mouse was clicked at: "<<x<<" "<<y<<endl;
                delay(24);

            }
            delay(24);

        }
        getchar();


        return 0;
    }


