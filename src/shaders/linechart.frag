#version 120

#define MAX_POINT_COUNT 400

uniform lowp float opacity; // inherited opacity of this item
uniform lowp vec4 lineColor;
uniform lowp vec4 fillColor;
uniform lowp float lineWidth;

uniform highp vec2 points[MAX_POINT_COUNT];
uniform int pointCount;

varying highp vec2 uv;

float sdPolygon( in vec2[MAX_POINT_COUNT] v, in int count, in vec2 p )
{
    float d = dot(p-v[0],p-v[0]);
    float s = 1.0;
    for( int i=0, j = count; i < count; j=i, i++ )
    {
        vec2 e = v[j] - v[i];
        vec2 w =    p - v[i];
        float h = clamp( dot(w,e)/dot(e,e), 0.0, 1.0 );
        vec2 b = w - e * h;
        d = min( d, dot(b,b) );

        bvec3 c = bvec3(p.y>=v[i].y,p.y<v[j].y,e.x*w.y>e.y*w.x);
        if( all(c) || all(not(c)) ) s*=-1.0;
    }
    return s*sqrt(d);
}

void main()
{
    vec2 point = uv;

    vec4 col = vec4(0.0, 0.0, 0.0, 0.0);

    float d = sdPolygon(points, pointCount, point);
    float g = fwidth(d);

    col = mix(col, fillColor, fillColor.a * (1.0 - smoothstep(0.001 - g, 0.001 + g, d)));

    float halfLW = lineWidth / 2.0;
    col = mix(col, lineColor, lineColor.a * (1.0 - smoothstep(halfLW - g, halfLW + g, abs(d))));

    gl_FragColor = col * opacity;
}
