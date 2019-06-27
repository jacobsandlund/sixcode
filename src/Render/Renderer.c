#include "Render/Renderer.h"

#include "Gpu/Cmd.h"
#include "Gpu/Primitive.h"
#include "Gpu/ShaderTypes.h"

#include <stdlib.h>
#include <string.h>

void RendererInit(Renderer *renderer, GpuDevice *device, RendererConfig *config)
{
    renderer->vertex_function =
            GpuFunctionCreateWithName(device, "vertex_shader");
    renderer->fragment_function =
            GpuFunctionCreateWithName(device, "fragment_shader");

    GpuPipelineStateConfig pipeline_state_config = {
        .label = "Simple Pipeline",
        .vertex_function = renderer->vertex_function,
        .fragment_function = renderer->fragment_function,
        .pixel_format = config->view.color_pixel_format,
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
                    GpuVertex *current_vertex = &current_quad[vertex_in_quad];
                    current_vertex->position.x += upper_left_position.x;
                    current_vertex->position.y += upper_left_position.y;
                }

                current_quad += NumVerticesPerQuad;
            }
        }
    }
}

void RendererDestroy(Renderer *renderer)
{
    GpuBufferDestroy(renderer->vertex_buffer);
    GpuPipelineStateDestroy(renderer->pipeline_state);
    GpuFunctionDestroy(renderer->fragment_function);
    GpuFunctionDestroy(renderer->vertex_function);
}

void RendererDrawInView(Renderer *renderer,
                        GpuView *view,
                        RenderViewport *viewport,
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

        GpuCmdSetViewport(encoder, viewport->size);

        GpuCmdSetPipelineState(encoder, renderer->pipeline_state);

        GpuCmdBindVertexBuffer(
                encoder, renderer->vertex_buffer, 0, GpuVertexIndexVertices);

        GpuCmdBindVertexBytes(encoder,
                              &viewport->size,
                              sizeof(viewport->size),
                              GpuVertexIndexViewportSize);

        GpuCmdDrawPrimitives(
                encoder, GpuPrimitiveTypeTriangle, 0, renderer->num_vertices);

        GpuCommandEncoderEndEncoding(encoder);
        GpuCommandBufferPresentDrawable(command_buffer, view);
        GpuRenderPassConfigDestroy(render_pass_config);
    }

    GpuCommandBufferCommit(command_buffer);
    GpuCommandBufferDestroy(command_buffer);
}
