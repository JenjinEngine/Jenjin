# TODO
- [x] Basic scene and mesh rendering
- [x] Texture support for models
- [x] Scene saving/loading
- [x] Editor renderring game to texture for embedding in ImGui
- [x] Scene editing
- [x] Add Lua and Sol2 bindings
- [ ] Add type definitions for Teal (Lua type checker)
- [ ] Implement a basic code editor in ImGui
- [ ] Implement a basic console in ImGui
- [ ] Build with Teal inside the editor
- [ ] Add automatic reloading of Lua scripts
- [ ] Add automatic transpilation of Teal scripts
- [ ] Storing arbitrary data/data types on `GameObject`s
- [ ] Support loading scenes from serialized data
- [ ] Support having a global game state in Lua

## Optimisation
- [ ] Revise usages of `push_back`  vs `emplace_back`
- [ ] Resource manager (don't load the same resource twice)
- [ ] Use a single set of vertices/indices loaded into the VBO for quads (VRAM optimisation)
- [ ] Cache model matrices (CPU optimisation)
