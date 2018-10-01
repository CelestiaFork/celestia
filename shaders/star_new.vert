attribute float magnitude;

uniform vec2 viewportSize;
uniform float limitingMagnitude;
uniform float saturationMagnitude;
//uniform float glareBrightness;
//uniform float sigma2;
//uniform float glareSigma2;
//uniform float exposure;

varying vec2 pointCenter;
varying vec4 starColor;
varying float brightness;

const float glareBrightness = 0.003f;
const float sigma2 = 0.35f;
const float glareSigma2 = 0.35f; // fixme

const float thresholdBrightness = 1.0 / 512.0;

void main()
{
    vec4 projectedPosition = ftransform();

    // Perspective projection to get the star position in normalized device coordinates
    vec2 ndcPosition = projectedPosition.xy / projectedPosition.w;

    // Convert to viewport coordinates (the same coordinate system as the gl_FragCoord register)
    pointCenter = (ndcPosition * 0.5 + vec2(0.5, 0.5)) * viewportSize;

    // Get the star color from the gl_Color attribute
    starColor  = gl_Color;
    brightness = pow(2.512, (limitingMagnitude - saturationMagnitude) * (saturationMagnitude - magnitude));

    // Calculate the minimum size of a point large enough to contain both the glare function and PSF; these
    // functions are both infinite in extent, but we can clip them when they fall off to an indiscernable level
    float r2  = -log(thresholdBrightness / brightness) * 2.0 * sigma2;
    float rG2 = -log(thresholdBrightness / (glareBrightness * brightness)) * 2.0 * glareSigma2;
    gl_PointSize = 2.0 * sqrt(max(r2, rG2));

    gl_Position = projectedPosition;
}
