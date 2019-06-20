#include "gpu/renderer.h"
#include "gpu/shader-types.h"
#include <stdlib.h>
#include <string.h>

void gpu_renderer_init(GpuRenderer *renderer, GpuDevice *device, GpuView *view, float2 viewport_size)
{
    renderer->viewport_size = viewport_size;
    renderer->vertex_function = gpu_function_create_with_name(device, "vertex_shader");
    renderer->fragment_function = gpu_function_create_with_name(device, "fragment_shader");

    GpuPipelineStateConfig pipeline_state_config = {
        .label = "Simple Pipeline",
        .vertex_function = renderer->vertex_function,
        .fragment_function = renderer->fragment_function,
        .pixel_format = gpu_view_color_pixel_format(view),
    };

    renderer->pipeline_state = gpu_pipeline_state_create(device, &pipeline_state_config);

    // Generate vertex buffer
    {
        const GpuVertex QuadVertices[] = {
            // Pixel positions, RGBA colors
            {{-20, 20}, {1, 0, 0, 1}},
            {{20, 20}, {0, 0, 1, 1}},
            {{-20, -20}, {0, 1, 0, 1}},

            {{20, -20}, {1, 0, 0, 1}},
            {{-20, -20}, {0, 1, 0, 1}},
            {{20, 20}, {0, 0, 1, 1}},
        };
        const i64 NumRows = 15;
        const i64 NumColumns = 25;
        const i64 NumVerticesPerQuad = sizeof(QuadVertices) / sizeof(GpuVertex);
        const float QuadSpacing = 50.0;
        const i64 DataLength = sizeof(QuadVertices) * NumRows * NumColumns;

        renderer->vertex_buffer = gpu_buffer_create_with_length(device,
                DataLength, GpuBufferStorageModeTypeShared);
        renderer->num_vertices = DataLength / sizeof(GpuVertex);

        GpuVertex *current_quad = gpu_buffer_contents(renderer->vertex_buffer);

        for (i32f row = 0; row < NumRows; row++) {
            for (i32f column = 0; column < NumColumns; column++) {
                float2 upper_left_position;
                upper_left_position.x = ((-((float)NumColumns) / 2.0) + column) * QuadSpacing + QuadSpacing/2.0;
                upper_left_position.y = ((-((float)NumRows) / 2.0) + row) * QuadSpacing + QuadSpacing/2.0;

                memcpy(current_quad, &QuadVertices, sizeof(QuadVertices));

                for (i32f vertex_in_quad = 0; vertex_in_quad < NumVerticesPerQuad; vertex_in_quad++)
                {
                    current_quad[vertex_in_quad].position += upper_left_position;
                }

                current_quad += NumVerticesPerQuad;
            }
        }
    }
}

void gpu_renderer_destroy(GpuRenderer *renderer)
{
    gpu_buffer_destroy(renderer->vertex_buffer);
    gpu_pipeline_state_destroy(renderer->pipeline_state);
    gpu_function_destroy(renderer->fragment_function);
    gpu_function_destroy(renderer->vertex_function);
}

void gpu_renderer_draw_in_view(GpuRenderer *renderer, GpuView *view, GpuCommandQueue *queue)
{
    GpuCommandBuffer *command_buffer = gpu_command_buffer_create(queue);
    GpuRenderPassConfig *render_pass_config = gpu_view_current_render_pass_config(view);

    if (render_pass_config) {
        GpuCommandEncoder *encoder = gpu_command_encoder_create(command_buffer, render_pass_config);
        gpu_command_encoder_label(encoder, "MyRenderEncoder");

        gpu_command_encoder_set_viewport(encoder, renderer->viewport_size);

        gpu_command_encoder_set_pipeline_state(encoder, renderer->pipeline_state);

        gpu_command_encoder_set_vertex_buffer(encoder, renderer->vertex_buffer,
                0, GpuVertexIndexVertices);

        gpu_command_encoder_set_vertex_bytes(encoder, &renderer->viewport_size,
                sizeof(renderer->viewport_size), GpuVertexIndexViewportSize);

        gpu_command_encoder_draw_primitives(encoder,
                GpuPrimitiveTypeTriangle, 0, renderer->num_vertices);

        gpu_command_encoder_end_encoding(encoder);
        gpu_command_buffer_present_drawable(command_buffer, view);
    }

    gpu_command_buffer_commit(command_buffer);
}

void gpu_renderer_size_changed(GpuRenderer *renderer, GpuView *view, float2 viewport_size)
{
    (void) view;
    renderer->viewport_size = viewport_size;
}
