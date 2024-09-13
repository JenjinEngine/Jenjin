# TODO
- [x] Basic scene and mesh rendering
- [x] Texture support for models
- [x] Scene saving/loading
- [ ] Add Lua and Sol2 bindings
- [ ] Add type definitions for Teal (Lua type checker)
- [ ] Add automatic reloading of Lua scripts
- [ ] Add automatic transpilation of Teal scripts
- [ ] Storing arbitrary data/data types on `GameObject`s
- [ ] Support loading scenes from serialized data
- [ ] Support having a global game state in Lua

## Optimisation
- [ ] Resource manager (don't load the same resource twice)
- [ ] Use a single set of vertices/indices loaded into the VBO for quads (VRAM optimisation)
- [ ] Cache model matrices (CPU optimisation)
