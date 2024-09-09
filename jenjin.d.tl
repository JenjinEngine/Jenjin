global record vec2
	x: number
	y: number

	new:function(self:vec2, x:number, y:number): vec2
end

global record vec3
	x: number
	y: number
	z: number

	new:function(self:vec3, x:number, y:number, z:number): vec3
end

global record Transform
	position: vec2
	rotation: number
	scale: vec2
	z_index: number

	new:function(self:Transform, position:vec2, rotation:number, scale:number, z_index:number): Transform
end

global record GameObject
	transform: Transform
	meshdata: MeshData
	name: string
	color: vec3
	mesh_id: number
	id: number

	new:function(self:GameObject, mesh_data:MeshData, name:string): GameObject

	set_transform:function(self:GameObject, transform:Transform)
	set_position:function(self:GameObject, position:vec2)
	set_rotation:function(self:GameObject, rotation:number)
	set_scale:function(self:GameObject, scale:vec2)
	set_color:function(self:GameObject, color:vec3)
	set_mesh_id:function(self:GameObject, mesh_id:number)
	set_id:function(self:GameObject, id:number)
	set_name:function(self:GameObject, name:string)
	set_meshdata:function(self:GameObject, mesh_data:MeshData)

	translate:function(self:GameObject, translation:vec2)
	rotate:function(self:GameObject, rotation:number)
	scale:function(self:GameObject, scale:vec2)

	rotate_towards:function(self:GameObject, target:vec2)
	rotation_towards:function(self:GameObject, target:vec2): number
	distance_to:function(self:GameObject, target:vec2): number
end

global record Scene
	get_gameobject:function(self:Scene, name:string): GameObject
	add_gameobject:function(self:Scene, gameobject:GameObject)
	build:function(self:Scene)
end

global record state
	dt: number
	window: table
	engine: table
	scene: Scene
end

global record s
	dt: number
	window: table
	engine: table
	scene: Scene
end

global record MeshData
	vertices: table
	indices: table
end

global record Mesh
	base_vertex: number
	base_index: number
	vertex_count: number
	index_count: number
end

global is_key_pressed: function(key: number): boolean
global create_quad: function(width: number, height: number): MeshData
global nilgobj: function(): GameObject

-- global function print(_:string) end
global function info(_:string) end
global function warn(_:string) end
-- global function error(_:string) end
-- global function debug(_:string) end
global function trace(_:string) end

global function normalize(_:vec2): vec2 end
global function normalize_v2(_:vec2): vec2 end
global function normalize_v3(_:vec3): vec3 end

-- Keys
global KEY_0: number
global KEY_1: number
global KEY_2: number
global KEY_3: number
global KEY_4: number
global KEY_5: number
global KEY_6: number
global KEY_7: number
global KEY_8: number
global KEY_9: number
global KEY_A: number
global KEY_B: number
global KEY_C: number
global KEY_D: number
global KEY_E: number
global KEY_F: number
global KEY_G: number
global KEY_H: number
global KEY_I: number
global KEY_J: number
global KEY_K: number
global KEY_L: number
global KEY_M: number
global KEY_N: number
global KEY_O: number
global KEY_P: number
global KEY_Q: number
global KEY_R: number
global KEY_S: number
global KEY_T: number
global KEY_U: number
global KEY_V: number
global KEY_W: number
global KEY_X: number
global KEY_Y: number
global KEY_Z: number
