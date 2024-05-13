struct Camera 
{
    Projection : mat4x4<f32>,
    View : mat4x4<f32>,
    Position : vec3f
};

@group(0) @binding(0) var<uniform> camera : Camera;
@group(1) @binding(0) var<uniform> albedo : vec3f;
@group(2) @binding(0) var<uniform> model : mat4x4<f32>;

struct VertexOutput 
{
    @builtin(position) Position : vec4f,
};


@vertex
fn vs_main(@location(0) pos: vec3f) -> VertexOutput 
{
    var output : VertexOutput;
    output.Position = camera.Projection * camera.View * model * vec4f(pos, 1.0);
    return output;
}   



@fragment
fn fs_main() -> @location(0) vec4f 
{
    return vec4f(albedo, 1.0);
}

// @vertex
// fn vs_main(@location(0) in_vertex_position: vec2f) -> @builtin(position) vec4f {
//     return vec4f(in_vertex_position, 0.0, 1.0);
// }

// @fragment
// fn fs_main() -> @location(0) vec4f 
// {
//     return vec4f(0.8, 0.3, 0.1, 1.0);
// }