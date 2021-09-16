#pragma once

#include <pipewire/pipewire.h>
#include <spa/param/video/format-utils.h>

struct pipewire_state {
	struct pw_loop *loop;
	struct pw_context *context;
	struct pw_core *core;
	bool running;

	struct pw_stream *stream;
	uint32_t stream_node_id;
	bool streaming;
	bool needs_buffer;
	struct spa_video_info_raw video_info;
	int stride;
	uint64_t seq;
};

/**
 * PipeWire buffers are allocated by the PipeWire thread, and are temporarily
 * shared with the steamcompmgr thread (via dequeue_pipewire_buffer and
 * push_pipewire_buffer) for copying.
 */
struct pipewire_buffer {
	struct spa_video_info_raw video_info;
	int stride;
	uint8_t *data;
	int fd;

	// The following fields are not thread-safe

	// The PipeWire buffer, or nullptr if it's been destroyed.
	struct pw_buffer *buffer;
	// We pass the buffer to the steamcompmgr thread for copying. This is set
	// to true if the buffer is currently owned by the steamcompmgr thread.
	bool copying;
};

bool init_pipewire(void);
uint32_t get_pipewire_stream_node_id(void);
struct pipewire_buffer *dequeue_pipewire_buffer(void);
void push_pipewire_buffer(struct pipewire_buffer *buffer);
void nudge_pipewire(void);
