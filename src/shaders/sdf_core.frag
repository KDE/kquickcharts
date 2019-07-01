// Copyright (c) 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
//
// This file is based on
// https://iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
//
// The MIT License
// Copyright © 2017 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions: The above copyright
// notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//include "desktop_header_core.glsl"

// A maximum point count to be used for sdf_polygon input arrays.
// Unfortunately even function inputs require a fixed size at declaration time
// for arrays, unless we were to use OpenGL 4.5.
// Since the polygon is most likely to be defined in a uniform, this should be
// at least less than MAX_FRAGMENT_UNIFORM_COMPONENTS / 2 (since we need vec2).
#define SDF_POLYGON_MAX_POINT_COUNT 400

/*********************************
    Shapes
*********************************/

// Distance field for a circle.
//
// \param point A point on the distance field.
// \param radius The radius of the circle.
//
// \return The signed distance from point to the circle. If negative, point is
//         inside the circle.
float sdf_circle(in vec2 point, in float radius)
{
    return length(point) - radius;
}

// Distance field for a triangle.
//
// \param point A point on the distance field.
// \param p0 The first vertex of the triangle.
// \param p0 The second vertex of the triangle.
// \param p0 The third vertex of the triangle.
//
// \note The ordering of the three vertices does not matter.
//
// \return The signed distance from point to triangle. If negative, point is
//         inside the triangle.
float sdf_triangle(in vec2 point, in vec2 p0, in vec2 p1, in vec2 p2)
{
    vec2 e0 = p1 - p0;
    vec2 e1 = p2 - p1;
    vec2 e2 = p0 - p2;

    vec2 v0 = point - p0;
    vec2 v1 = point - p1;
    vec2 v2 = point - p2;

    vec2 pq0 = v0 - e0 * clamp( dot(v0, e0) / dot(e0, e0), 0.0, 1.0 );
    vec2 pq1 = v1 - e1 * clamp( dot(v1, e1) / dot(e1, e1), 0.0, 1.0 );
    vec2 pq2 = v2 - e2 * clamp( dot(v2, e2) / dot(e2, e2), 0.0, 1.0 );

    float s = sign( e0.x*e2.y - e0.y*e2.x );
    vec2 d = min(min(vec2(dot(pq0,pq0), s*(v0.x*e0.y-v0.y*e0.x)),
                     vec2(dot(pq1,pq1), s*(v1.x*e1.y-v1.y*e1.x))),
                     vec2(dot(pq2,pq2), s*(v2.x*e2.y-v2.y*e2.x)));

    return -sqrt(d.x)*sign(d.y);
}

// Distance field for an arbitrary polygon.
//
// \param point A point on the distance field.
// \param vertices An array of points that make up the polygon.
// \param count The amount of points to use for the polygon.
//
// \note points should be an array of vec2 of size SDF_POLYGON_MAX_POINT_COUNT.
//       Use count to indicate how many items of that array should be used.
//
// \return The signed distance from point to triangle. If negative, point is
//         inside the triangle.

float sdf_polygon(in vec2 point, in vec2[SDF_POLYGON_MAX_POINT_COUNT] vertices, in int count)
{
    float d = dot(point - vertices[0], point - vertices[0]);
    float s = 1.0;
    for (int i = 0, j = count - 1; i < count && i < SDF_POLYGON_MAX_POINT_COUNT; j = i, i++)
    {
        vec2 e = vertices[j] - vertices[i];
        vec2 w = point - vertices[i];
        float h = clamp( dot(w, e) / dot(e, e), 0.0, 1.0 );
        vec2 b = w - e * h;
        d = min(d, dot(b, b));

        bvec3 c = bvec3(point.y >= vertices[i].y, point.y < vertices[j].y, e.x * w.y > e.y * w.x);
        if(all(c) || all(not(c))) s *= -1.0;
    }
    return s * sqrt(d);
}

/*********************
    Operators
*********************/

// Convert a distance field to an annular (hollow) distance field.
//
// \param sdf The result of an sdf shape to convert.
// \param thickness The thickness of the resulting shape.
//
// \return The value of sdf modified to an annular shape.
float sdf_annular(in float sdf, in float thickness)
{
    return abs(sdf) - thickness;
}

// Union two sdf shapes together.
//
// \param sdf1 The first sdf shape.
// \param sdf2 The second sdf shape.
//
// \return The union of sdf1 and sdf2, that is, the distance to both sdf1 and
//         sdf2.
float sdf_union(in float sdf1, in float sdf2)
{
    return min(sdf1, sdf2);
}

// Subtract two sdf shapes.
//
// \param sdf1 The first sdf shape.
// \param sdf2 The second sdf shape.
//
// \return sdf1 with sdf2 subtracted from it.
float sdf_subtract(in float sdf1, in float sdf2)
{
    return max(sdf1, -sdf2);
}

// Intersect two sdf shapes.
//
// \param sdf1 The first sdf shape.
// \param sdf2 The second sdf shape.
//
// \return The intersection between sdf1 and sdf2, that is, the area where both
//         sdf1 and sdf2 provide the same distance value.
float sdf_intersect(in float sdf1, in float sdf2)
{
    return max(sdf1, sdf2);
}

// Round an sdf shape.
//
// \param sdf The sdf shape to round.
// \param amount The amount of rounding to apply.
//
// \return The rounded shape of sdf.
//         Note that rounding happens by basically selecting an isoline of sdf,
//         therefore, the resulting shape may be larger than the input shape.
float sdf_round(in float sdf, in float amount)
{
    return sdf - amount;
}

// Convert an sdf shape to an outline of its shape.
//
// \param sdf The sdf shape to turn into an outline.
//
// \return The outline of sdf.
float sdf_outline(in float sdf)
{
    return abs(sdf);
}

/********************
    Convenience
********************/

// A constant to represent a "null" value of an sdf.
// Since 0 is a point exactly on the outline of an sdf shape, and negative
// values are inside the shape, this uses a very large positive constant to
// indicate a value that is really far away from the actual sdf shape.
const float sdf_null = 99999.0;

// Render an sdf shape.
//
// This will render the sdf shape on top of whatever source color is input,
// making sure to apply smoothing if desired.
//
// \param sdf The sdf shape to render.
// \param sourceColor The source color to render on top of.
// \param sdfColor The color to use for rendering the sdf shape.
// \param smoothing The amount of smoothing to apply to the sdf.
//
// \return sourceColor with the sdf shape rendered on top.
vec4 sdf_render(in float sdf, in vec4 sourceColor, in vec4 sdfColor, in float smoothing)
{
    float g = fwidth(sdf);
    return mix(sourceColor, sdfColor, sdfColor.a * (1.0 - smoothstep(smoothing - g, smoothing + g, sdf)));
}
