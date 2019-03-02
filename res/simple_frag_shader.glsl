#ifdef GL_ES
precision mediump float;
varying highp vec2 vTexCoord;
#else
varying vec2 vTexCoord;
#endif

uniform bool useTex;
uniform sampler2D sampler;
uniform vec4 color;

void main() {
    if(useTex) {
        gl_FragColor = texture2D(sampler, vTexCoord);
    } else {
        gl_FragColor = color;
    }
}
