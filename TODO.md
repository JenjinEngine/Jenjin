# TODO
- [x] Basic scene and mesh rendering
- [x] Texture support for models
- [x] Scene saving/loading
- [x] Editor renderring game to texture for embedding in ImGui
- [x] Scene editing
- [x] Add Lua and Sol2 bindings
- [x] Add reloading of Lua scripts
- [x] Support loading scenes from serialized data
- [x] Implement a basic code editor in ImGui
- [x] Implement proper syntax highlighting for Lua
- [ ] Implement a basic console in ImGui
- [ ] Add type definitions for Teal (Lua type checker)
- [ ] Build with Teal inside the editor
- [ ] Add automatic reloading of Lua scripts
- [ ] Storing arbitrary data/data types on `GameObject`s

## Optimisation
- [ ] Revise usages of `push_back`  vs `emplace_back`
- [ ] Resource manager (don't load the same resource twice)
- [ ] Use a single set of vertices/indices loaded into the VBO for quads (VRAM optimisation)
- [ ] Cache model matrices (CPU optimisation)
