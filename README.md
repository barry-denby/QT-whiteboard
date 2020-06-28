# QT Whiteboard

A personal project to make a whiteboard for my own use when teaching. Was born out of a need for a decent whiteboard during zoom lectures/tutorials. Zoom's own whiteboard was very finnikey to use during zoom sessions and openboard was just too cumbersome. There weren't any other decent alternatives at the time.

## Features

- supports multiple whiteboards in the same session
- all whiteboards in a session are saved in a single file.
- export of whiteboards to PNG images inside a directory
- support for JPG, PNG, and SVG images to be rendered in a whiteboard.
- square, circle, and x points, freehand and straight lines.
- locking of a whiteboard to prevent accidential undo of previous draw operations
- undo and locking works across multiple sessions. i.e. if you save a file and come back to it at a later session both operations will still function.

## Keyboard shortcuts
- Arrow left: move to previous whiteboard
- Arrow right: move to next whiteboard
- Arrow up: increase tool size
- Arrow down: decrease tool size
- Ctrl+Q: quit QT Whiteboard
- Ctrl+S: save current whiteboard
- Ctrl+Z: undo
- E: next colour
- W: previous colour
- D: next tool
- S: previous tool
- C: increase text rotation by 45 degrees
- X: decrease text rotation by 45 degrees
- Ctrl+T: set keyboard focus on board title
- Ctrl+G: set keyboard focus on text for text tool


## Build Requirements
- Qt >= 5.14
- Meson build system (using ninja)

To build just run meson in the directory cd into the build directory and then run ninja.

## Known Limitations
- whiteboard and PNG exports are fixed to 1080p resolution
