varying vec2 pointCenter;
varying vec4 starColor;
varying float brightness;

uniform float sigma2;
uniform float glareSigma2;
uniform float glareBrightness;

void main()
{
    // gl_FragCoord contains the position of the pixel center in viewport coordinates. Compute
    // the distance to the center of the star.
    vec2 offset = gl_FragCoord.xy - pointCenter;
    float r2 = dot(offset, offset);
    float b = exp(-r2 / (2.0 * sigma2));
    b += glareBrightness * exp(-r2 / (2.0 * glareSigma2));
//    gl_FragColor = vec4(linearToSRGB(b * starColor.rgb * brightness), 1.0);
     gl_FragColor = vec4(b * starColor.rgb * brightness, 1.0);
}
