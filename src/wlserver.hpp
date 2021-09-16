// Wayland stuff

#pragma once

#include <wayland-server-core.h>
#include <atomic>

#define WLSERVER_BUTTON_COUNT 4
#define WLSERVER_TOUCH_COUNT 11 // Ten fingers + nose ought to be enough for anyone

struct wlserver_t {
	struct wl_display *display;
	struct wl_event_loop *event_loop;
	char wl_display_name[32];

	struct {
		struct wlr_backend *multi_backend;
		struct wlr_backend *noop_backend;
		struct wlr_backend *libinput_backend;

		struct wlr_renderer *renderer;
		struct wlr_compositor *compositor;
		struct wlr_xwayland_server *xwayland_server;
		struct wlr_session *session;	
		struct wlr_seat *seat;
		struct wlr_output *output;

		// Used to simulate key events when nested
		struct wlr_input_device *virtual_keyboard_device;
	} wlr;
	
	struct wlr_surface *mouse_focus_surface;
	double mouse_surface_cursorx;
	double mouse_surface_cursory;
	
	bool button_held[ WLSERVER_BUTTON_COUNT ];
	bool touch_down[ WLSERVER_TOUCH_COUNT ];

	struct wl_listener session_active;
};

struct wlserver_keyboard {
	struct wlr_input_device *device;
	
	struct wl_listener modifiers;
	struct wl_listener key;
};

struct wlserver_pointer {
	struct wlr_input_device *device;
	
	struct wl_listener motion;
	struct wl_listener button;
	struct wl_listener axis;
	struct wl_listener frame;
};

struct wlserver_touch {
	struct wlr_input_device *device;
	
	struct wl_listener down;
	struct wl_listener up;
	struct wl_listener motion;
};

enum wlserver_touch_click_mode {
	WLSERVER_TOUCH_CLICK_HOVER = 0,
	WLSERVER_TOUCH_CLICK_LEFT = 1,
	WLSERVER_TOUCH_CLICK_RIGHT = 2,
	WLSERVER_TOUCH_CLICK_MIDDLE = 3,
	WLSERVER_TOUCH_CLICK_PASSTHROUGH = 4,
};

extern enum wlserver_touch_click_mode g_nTouchClickMode;

void xwayland_surface_role_commit(struct wlr_surface *wlr_surface);

bool wlsession_init( void );
int wlsession_open_kms( const char *device_name );

bool wlserver_init( void );

void wlserver_run(void);

void wlserver_lock(void);
void wlserver_unlock(void);

void wlserver_keyboardfocus( struct wlr_surface *surface );
void wlserver_key( uint32_t key, bool press, uint32_t time );

void wlserver_mousefocus( struct wlr_surface *wlrsurface, int x = 0, int y = 0 );
void wlserver_mousemotion( int x, int y, uint32_t time );
void wlserver_mousebutton( int button, bool press, uint32_t time );
void wlserver_mousewheel( int x, int y, uint32_t time );

void wlserver_send_frame_done( struct wlr_surface *surf, const struct timespec *when );

const char *wlserver_get_nested_display_name( void );
const char *wlserver_get_wl_display_name( void );

struct wlserver_surface
{
	std::atomic<struct wlr_surface *> wlr;

	// owned by wlserver
	long wl_id, x11_id;
	bool overridden;
	struct wl_list pending_link;
	struct wl_listener destroy;
};

void wlserver_surface_init( struct wlserver_surface *surf, long x11_id );
void wlserver_surface_set_wl_id( struct wlserver_surface *surf, long id );
void wlserver_surface_finish( struct wlserver_surface *surf );
