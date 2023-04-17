#version 400
// The MIT License
// Copyright © 2013 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    

// A list of useful distance function to simple primitives, and an example on how to 
// do some interesting boolean operations, repetition and displacement.
//
// More info here: http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform int       iFrame;                // shader playback frame

in vec2 UV;

out vec4 fragColor;

//------------------------------------------------------------------

float sdPlane( vec3 p )
{
	return p.y;
}

float sdSphere( vec3 p, float s )
{
    return length(p)-s;
}

float sdBox( vec3 p, vec3 b )
{
    vec3 d = abs(p) - b;
    return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float sdRoundBox( in vec3 p, in vec3 b, in float r ) 
{
    vec3 q = abs(p) - b;
    return min(max(q.x,max(q.y,q.z)),0.0) + length(max(q,0.0)) - r;
}

float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
	vec3 pa = p-a, ba = b-a;
	float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
	return length( pa - ba*h ) - r;
}

float sdEquilateralTriangle(  in vec2 p )
{
    const float k = 1.73205;//sqrt(3.0);
    p.x = abs(p.x) - 1.0;
    p.y = p.y + 1.0/k;
    if( p.x + k*p.y > 0.0 ) p = vec2( p.x - k*p.y, -k*p.x - p.y )/2.0;
    p.x += 2.0 - 2.0*clamp( (p.x+2.0)/2.0, 0.0, 1.0 );
    return -length(p)*sign(p.y);
}

// vertical
float sdCylinder( vec3 p, vec2 h )
{
    vec2 d = abs(vec2(length(p.xz),p.y)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float sdCone( in vec3 p, in vec3 c )
{
    vec2 q = vec2( length(p.xz), p.y );
    float d1 = -q.y-c.z;
    float d2 = max( dot(q,c.xy), q.y);
    return length(max(vec2(d1,d2),0.0)) + min(max(d1,d2), 0.);
}

//------------------------------------------------------------------

vec2 opAND( vec2 d1, vec2 d2 )
{
    return d2.x < d1.x ? d1 : d2;
}

vec2 opNOT( vec2 d1, vec2 d2 )
{
    return -d2.x < d1.x ? d1 : vec2(-d2.x, d2.y);
}

vec2 opOR( vec2 d1, vec2 d2 )
{
	return (d1.x < d2.x) ? d1 : d2;
}

vec3 opRep( vec3 p, vec3 c )
{
    return mod(p,c)-0.5*c;
}

// polynomial smooth min
vec2 smin( float a, float b, float k )
{
    float h = max( k-abs(a-b), 0.0 )/k;
    float m = h*h*0.5;
    float s = m*k*0.5;
    return (a<b) ? vec2(a-s,m) : vec2(b-s,1.0-m);
}

vec2 opORSmooth( vec2 d1, vec2 d2, float k)
{
    vec2 t = smin(d1.x, d2.x, k);
	return vec2(t.x, mix(d1.y, d2.y, t.y));
}

//------------------------------------------------------------------

//------------------------------------------------------------------

vec2 map( in vec3 pos )
{
    vec2 res = vec2( 1e10, 0.0 );    
    
    //OR: additive result, both shapes are merged into one, 
    //get the smallest time and the material
    //AND: intersection area, closest time of the overlapping times, 
    //take greatest time of intersections, only works for concave eclosed shapes
    //NOT: substracting,not commutative(order matters)
    

    // TODO: Insert the SDF usage here
    res = opNOT(vec2(sdBox(pos + vec3(0.5, 0, 0), vec3(0.6)), 30.0), 
                vec2(sdSphere(pos, 0.8), 255.0));
    //res = vec2(sdPlane(pos, 0.8), 30.0);
    //res = vec2(sdSphere(pos, 0.8), 30.0);
    //res = vec2(sdBox(pos, 0.8), 30.0);
    //res = vec2(sdRoundBox(pos, 0.8), 30.0);
    //res = vec2(sdCapsule(pos, 0.8), 30.0);
    //res = vec2(sdEquilateralTriangle(pos, 0.8), 30.0);
    //res = vec2(sdCylinder(pos, 0.8), 30.0);
    //res = vec2(sdCone(pos, 0.8), 30.0);
    return res;
}


vec2 castRay( in vec3 ro, in vec3 rd )
{
    vec2 res = vec2(-1.0,-1.0);

    float tmin = 1.0;
    float tmax = 20.0;
    
    // raymarch primitives       
    float t = tmin;
    for( int i=0; i<200 && t<tmax; i++ )
    {
        vec2 h = map( ro+rd*t );//gets the closest  thing on scene, x value is distance and y is material

        if( abs(h.x)<(0.0001*t) )//epsilon to check if intersected
        { 
            res = vec2(t,h.y); //if founded intersection point, break
            break;
        }
        t += h.x;//move in direction of ray
    }    
    
    return res;
}

// http://iquilezles.org/www/articles/normalsSDF/normalsSDF.htm
vec3 calcNormal( in vec3 pos )
{
    vec2 e = vec2(1.0,-1.0);
    return normalize( e.xyy*map( pos + e.xyy ).x + 
					  e.yyx*map( pos + e.yyx ).x + 
					  e.yxy*map( pos + e.yxy ).x + 
					  e.xxx*map( pos + e.xxx ).x );
}


vec3 render( in vec3 ro, in vec3 rd )
{ 
    vec3 col = vec3(0.0);
    vec2 res = castRay(ro,rd);
    float t = res.x;
	float matColor = res.y;
    if( matColor > 0 )
    {
        //lighting computations
        vec3 pos = ro + t*rd;
        vec3 N = calcNormal( pos );
		vec3 L = normalize( vec3(-0.4, 0.7, -0.6) );

        float dif = clamp( dot( N, L ), 0.3, 1.0 );

        // material        
		col = 0.45 + 0.35*sin( vec3(0.05,0.08,0.10)*(matColor-1.0)); 
        col *= dif;
    }
    else
    {
        //background gradient
        col =  vec3(0.7, 0.9, 1.0) +rd.y*0.7;
    }

	return vec3( clamp(col,0.0,1.0) );
}

mat3 setCamera( in vec3 ro, in vec3 ta, float cr )
{
	vec3 cw = normalize(ta-ro);
	vec3 cp = vec3(sin(cr), cos(cr),0.0);
	vec3 cu = normalize( cross(cw,cp) );
	vec3 cv = cross(cu,cw);
    return mat3(cu, cv, cw);
}

void main()
{
    vec2 fragCoord = gl_FragCoord.xy;
	float time = 15.0 + iTime * 5;

    // camera	
    float radius = 2.0;
    vec3 ro = vec3( radius*cos(0.1*time), 1.0, radius*sin(0.1*time) );
    //ro =  vec3(0.0, 1.0, -2.0);
    vec3 ta = vec3( -0.5, -0.4, 0.5 );

    // camera-to-world transformation
    mat3 ca = setCamera( ro, vec3(0.0), 0.0 );

    vec3 tot = vec3(0.0);
    
    vec2 p = (-iResolution.xy + 2.0*fragCoord)/iResolution.y;

    // ray direction
    vec3 rd = ca * normalize( vec3(p.xy,1.0) );

    // render	
    vec3 col = render( ro, rd );
        
    fragColor = vec4( col, 1.0 );
}