#version 120 

uniform float LineWidth;
varying vec2 Vert0, Vert1, Vert2;

// Compute distance from a point to a line
float point_line_dist(vec2 p, vec2 v1, vec2 v2) {
    float s = (v2.x - v1.x) * (v1.y - p.y) - (v1.x - p.x) * (v2.y - v1.y);
    float t = length(v2 - v1);
    return abs(s) / t;
}

void main() {
    
    float d0 = point_line_dist(gl_FragCoord.xy, Vert0, Vert1);
    float d1 = point_line_dist(gl_FragCoord.xy, Vert1, Vert2);
    float d2 = point_line_dist(gl_FragCoord.xy, Vert2, Vert0);
    float m = min(d0, min(d1, d2));
    gl_FragColor = gl_Color * smoothstep(0.0, LineWidth, 1.0-m); // invert m if don't need faces, and use the mesh face color as wireframe
    //if(m < 1.0) gl_FragColor.a = 1.0-gl_FragColor.a;
    
    // lines render as transparent... need to set this color from a uniform
    // 1.0-m = lines rendered as normal
    //vec4 lineClr = vec4(1.0,0.0,0.0,1.0) * smoothstep(0.0, LineWidth, m);
    
    /*vec4 lineClr = vec4(0.0,0.0,0.0,1.0);
    vec4 faceColor = gl_Color;
    float alpha = smoothstep(0.0, LineWidth, m); // < 1 = wireframe
    if(alpha < 0.5) {
        // wireframe
        gl_FragColor = lineClr;// * (1.0-alpha);
    } else {
        // face
        gl_FragColor = faceColor * alpha;
    }*/
    //float invAlpha = 1.0-alpha;
    //gl_FragColor = faceColor * alpha;
    //gl_FragColor.a = 1.0;
}