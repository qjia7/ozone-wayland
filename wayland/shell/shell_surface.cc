// Copyright 2013 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone/wayland/shell/shell_surface.h"

#include "ozone/ui/events/event_factory_ozone_wayland.h"
#include "ozone/wayland/display.h"
#include "ozone/wayland/input_device.h"

namespace ozonewayland {

WaylandShellSurface::WaylandShellSurface()
    : surface_(NULL) {
  WaylandDisplay* display = WaylandDisplay::GetInstance();
  surface_ = wl_compositor_create_surface(display->GetCompositor());
}

WaylandShellSurface::~WaylandShellSurface() {
  DCHECK(surface_);
  wl_surface_destroy(surface_);
  FlushDisplay();
}

struct wl_surface* WaylandShellSurface::GetWLSurface() const {
    return surface_;
}

void WaylandShellSurface::FlushDisplay() const {
  WaylandDisplay* display = WaylandDisplay::GetInstance();
  DCHECK(display);
  display->FlushDisplay();
}

void WaylandShellSurface::PopupDone() {
  WaylandInputDevice* input = WaylandDisplay::GetInstance()->PrimaryInput();
  ui::EventConverterOzoneWayland* dispatcher =
      ui::EventFactoryOzoneWayland::GetInstance()->EventConverter();

  if (!input->GetGrabWindowHandle())
    return;
  dispatcher->CloseWidget(input->GetGrabWindowHandle());
  input->SetGrabWindowHandle(0, 0);
}

void WaylandShellSurface::WindowResized(void* data,
                                 unsigned width,
                                 unsigned height) {
  WaylandWindow *window = static_cast<WaylandWindow*>(data);
  ui::EventConverterOzoneWayland* dispatcher =
      ui::EventFactoryOzoneWayland::GetInstance()->EventConverter();
  dispatcher->WindowResized(window->Handle(), width, height);
}

}  // namespace ozonewayland
