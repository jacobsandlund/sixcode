#include "Gpu/Renderer.h"

#include "Gpu/ShaderTypes.h"

#include <stdlib.h>
#include <string.h>

void GpuRendererInit(GpuRenderer *renderer,
                     GpuDevice *device,
                     GpuView *view,
                     GpuRendererConfig *config)
{
    renderer->viewport_size = config->viewport_size;
    renderer->vertex_function =
            gpu_function_create_with_name(device, "vertex_shader");
    renderer->fragment_function =
            gpu_function_create_with_name(device, "fragment_shader");

    GpuPipelineStateConfig pipeline_state_config = {
        .label = "Simple Pipeline",
        .vertex_function = renderer->vertex_function,
        .fragment_function = renderer->fragment_function,
        .pixel_format = config->pixel_format,
    };

    renderer->pipeline_state =
            GpuPipelineStateCreate(device, &pipeline_state_config);

    // Generate vertex buffer
    {
        const GpuVertex QuadVertices[] = {
            // Pixel positions, RGBA colors
            { { -20, 20 }, { 1, 0, 0, 1 } },  { { 20, 20 }, { 0, 0, 1, 1 } },
            { { -20, -20 }, { 0, 1, 0, 1 } },

            { { 20, -20 }, { 1, 0, 0, 1 } },  { { -20, -20 }, { 0, 1, 0, 1 } },
            { { 20, 20 }, { 0, 0, 1, 1 } },
        };
        const i64 NumRows = 15;
        const i64 NumColumns = 25;
        const i64 NumVerticesPerQuad = sizeof(QuadVertices) / sizeof(GpuVertex);
        const float QuadSpacing = 50.0;
        const i64 DataLength = sizeof(QuadVertices) * NumRows * NumColumns;

        renderer->vertex_buffer = GpuBufferCreateWithLength(
                device, DataLength, GpuBufferStorageModeShared);
        renderer->num_vertices = DataLength / sizeof(GpuVertex);

        GpuVertex *current_quad = GpuBufferContents(renderer->vertex_buffer);

        for (i32f row = 0; row < NumRows; row++) {
            for (i32f column = 0; column < NumColumns; column++) {
                float2 upper_left_position;
                upper_left_position.x =
                        ((-((float)NumColumns) / 2.0) + column) * QuadSpacing +
                        QuadSpacing / 2.0;
                upper_left_position.y =
                        ((-((float)NumRows) / 2.0) + row) * QuadSpacing +
                        QuadSpacing / 2.0;

                memcpy(current_quad, &QuadVertices, sizeof(QuadVertices));

                for (i32f vertex_in_quad = 0;
                     vertex_in_quad < NumVerticesPerQuad;
                     vertex_in_quad++) {
                    current_quad[vertex_in_quad].position +=
                            upper_left_position;
                }

                current_quad += NumVerticesPerQuad;
            }
        }
    }
}

void GpuRendererDestroy(GpuRenderer *renderer)
{
    GpuBufferDestroy(renderer->vertex_buffer);
    GpuPipelineStateDestroy(renderer->pipeline_state);
    gpu_function_destroy(renderer->fragment_function);
    gpu_function_destroy(renderer->vertex_function);
}

void GpuRendererDrawInView(GpuRenderer *renderer,
                           GpuView *view,
                           GpuCommandQueue *queue)
{
    GpuCommandBuffer *command_buffer = GpuCommandBufferCreate(queue);
    GpuRenderPassConfig *render_pass_config =
            GpuViewCurrentRenderPassConfig(view);

    if (render_pass_config) {
        GpuCommandEncoder *encoder;
        GpuCommandEncoderBeginRenderEncoding(
                &encoder, command_buffer, render_pass_config);
        GpuCommandEncoderLabel(encoder, "MyRenderEncoder");

        GpuCmdSetViewport(encoder, renderer->viewport_size);

        GpuCmdSetPipelineState(encoder, renderer->pipeline_state);

        GpuCmdBindVertexBuffer(
                encoder, renderer->vertex_buffer, 0, GpuVertexIndexVertices);

        GpuCmdBindVertexBytes(encoder,
                              &renderer->viewport_size,
                              sizeof(renderer->viewport_size),
                              GpuVertexIndexViewportSize);

        GpuCmdDrawPrimitives(
                encoder, GpuPrimitiveTypeTriangle, 0, renderer->num_vertices);

        GpuCommandEncoderEndEncoding(encoder);
        GpuCommandBufferPresentDrawable(command_buffer, view);
    }

    GpuCommandBufferCommit(command_buffer);
}

void GpuRendererSizeChanged(GpuRenderer *renderer,
                            GpuView *view,
                            float2 viewport_size)
{
    (void)view;
    renderer->viewport_size = viewport_size;
}
